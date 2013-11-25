
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
	class on_signal {
		struct sigaction m_exitAction;
		public:
			on_signal(ba::io_service & io) {
				static ba::io_service * m_io = &io;
				class stop {
					ba::io_service & m_io;
					public:
						stop(ba::io_service &io): m_io(io) {}
						void operator()() {
							m_io.stop();
						}
				};
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
}

int main()
{
	namespace ba = boost::asio;
	namespace bs = boost::system;

	fastmq::log::file::add("fastmqr%5N.log", true);

	static auto L = std::move(fastmq::log::file::mklogger("main"));
	static const char opt_sock_path[] = "/tmp/fastmqr.sock";

	linfo(L) << "Log initialized";

	::unlink(opt_sock_path);

	ba::io_service io;
	ba::local::stream_protocol::endpoint endpoint(opt_sock_path);
	//local::on_signal quit(io);

	try {
		fastmq::malloc_message_pool pool;
		fastmq::router router(pool);
		fastmq::unx_router_server server(io, pool, router, endpoint, L);
		fastmq::unx_router_client client(io, pool, router, L);

		server.listen();
		client.connect(endpoint, 1, 1);

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
