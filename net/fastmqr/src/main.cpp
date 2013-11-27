
#include <toolbox/toolbox.hpp>
#include <vision/log.hpp>
#include <fastmq/server.hpp>
#include <fastmq/client.hpp>
#include <fastmq/routers.hpp>
#include <fastmq/toolbox.hpp>

namespace local {

	namespace ba = boost::asio;
	namespace bs = boost::system;

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

	vision::log::file::add("fastmqr%5N.log", true);

	static auto L = std::move(vision::log::channel("main"));
	static const char opt_sock_path[] = "/tmp/fastmqr.sock";

	linfo(L) << "Log initialized";

	::unlink(opt_sock_path);

	ba::io_service io;
	ba::local::stream_protocol::endpoint endpoint(opt_sock_path);

	toolbox::io::stop_on_signal(io);
	toolbox::io::stop_after(io, boost::posix_time::seconds(10));

	try {
		fastmq::malloc_message_pool pool;
		fastmq::router router(pool);
		fastmq::unx_router_server server(io, pool, router, endpoint
				, vision::log::channel("S"));

		local::unx_client clientA(io, pool, 2, 2, vision::log::channel("A"));
		local::unx_client clientB(io, pool, 1, 1, vision::log::channel("B"));

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
