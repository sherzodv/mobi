
#include <fastmq/log.hpp>
#include <fastmq/peer.hpp>
#include <fastmq/proto.hpp>
#include <fastmq/server.hpp>
#include <fastmq/client.hpp>
#include <fastmq/routers.hpp>
#include <fastmq/terminal.hpp>

namespace local {

	namespace ba = boost::asio;
	namespace bs = boost::system;

	class stop {
		ba::io_service & m_io;
		public:
			stop(ba::io_service &io): m_io(io) {}
			void operator()() {
				m_io.stop();
			}
			void operator()(const bs::error_code & ec) {
				(void)(ec);
				m_io.stop();
			}
	};

	class on_signal {
		struct sigaction m_exitAction;
		public:
			on_signal(ba::io_service & io) {
				static ba::io_service * m_io = &io;
				class exit_delegate {
					public:
						static void call(int /* sid */) {
							m_io->post(stop(*m_io));
						}
				};
				m_exitAction.sa_handler = &exit_delegate::call;
				sigemptyset(&m_exitAction.sa_mask);
				m_exitAction.sa_flags = 0;
				sigaction(SIGINT, &m_exitAction, nullptr);
			}
	};

	typedef fastmq::flog_t flog_t;
	typedef fastmq::unix_domain_client_base<fastmq::flog_t> client_base;

	class unx_client: public client_base {
			public:
				unx_client(boost::asio::io_service & io
						, fastmq::message_pool_base & p
						, fastmq::u16_t remote_id, fastmq::u16_t remote_type
						, flog_t l)
					: client_base(io, p, std::move(l))
					, m_remote_id(remote_id)
					, m_remote_type(remote_type)
				{
				}

				virtual ~unx_client() {
				}

			protected:

				fastmq::terminal_base * m_terminal;

				fastmq::u16_t m_remote_id;
				fastmq::u16_t m_remote_type;

				void send_hello(fastmq::terminal_base *t) {
					fastmq::msgu * msg = P.create_message(sizeof(fastmq::msgu)
							+ sizeof("hello")-1);
					msg->id = m_remote_id;
					msg->type = m_remote_type;
					msg->len = sizeof(fastmq::msgu) + sizeof("hello")-1;
					std::memcpy(msg->data, "hello", sizeof("hello"));
					t->consume_message(msg);
				}

				bool register_terminal(fastmq::terminal_base *t) {
					m_terminal = t;
					send_hello(t);
					return true;
				}

				void unregister_terminal(fastmq::terminal_base *t) {
					m_terminal = nullptr;
					(void)(t);
				}

				void on_connect_error() {
					lerror(L) << "unx_client::on_connect_error error!";
				}

				void on_recv(fastmq::terminal_base * t, fastmq::msgu * msg) {
					if (msg->len <= sizeof(fastmq::msgu)) {
						ltrace(L) << "Service message received";
					} else {
						ltrace(L) << "Message received: "
							<< std::string(reinterpret_cast<char *>(msg->data)
									, msg->len - sizeof(fastmq::msgu) + 1);
					}
					t->produce_message();
				}

				void on_send(fastmq::terminal_base * t, fastmq::msgu * msg) {
					send_hello(t);
					(void)(t);
					(void)(msg);
				}

				void on_send_error(fastmq::terminal_base * t, fastmq::msgu *) {
					lerror(L) << "unx_client::on_send_error error!";
					client_base::on_recv_error(t);
				}

				void on_recv_error(fastmq::terminal_base * t) {
					lerror(L) << "unx_client::on_recv_error error!";
					client_base::on_recv_error(t);
				}
	};
}

int main()
{
	namespace ba = boost::asio;
	namespace bs = boost::system;

	fastmq::log::file::add("fastmqr%5N.log", true);

	static auto L = std::move(fastmq::log::channel("main"));
	static const char opt_sock_path[] = "/tmp/fastmqr.sock";

	linfo(L) << "Log initialized";

	::unlink(opt_sock_path);

	ba::io_service io;
	ba::local::stream_protocol::endpoint endpoint(opt_sock_path);
	local::on_signal quit(io);

	ba::deadline_timer stop_timer(io);
	stop_timer.expires_from_now(boost::posix_time::seconds(10));
	stop_timer.async_wait(local::stop(io));

	try {
		fastmq::malloc_message_pool pool;
		fastmq::router router(pool);
		fastmq::unx_router_server server(io, pool, router, endpoint
				, fastmq::log::channel("S"));

		local::unx_client clientA(io, pool, 2, 2, fastmq::log::channel("A"));
		local::unx_client clientB(io, pool, 1, 1, fastmq::log::channel("B"));

		server.listen();

		clientA.connect(endpoint, 1, 1);
		clientB.connect(endpoint, 2, 2);

		linfo(L) << "Starting io service";
		io.run();
		linfo(L) << "Bye!";
	} catch (const std::exception & e) {
		::unlink(opt_sock_path);
		lcritical(L) << e.what();
		return 1;
	}

	::unlink(opt_sock_path);
	return 0;
}
