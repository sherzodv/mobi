
#include "subscriber.hpp"
#include "peer.hpp"
#include "pool.hpp"
#include "proto.hpp"
#include "queue.hpp"
#include "router.hpp"
#include "server.hpp"
#include "logger.hpp"

static void exit_gracefully(int /* sid */) {
	std::cout
		<< "notify: Got SIGINT, doing nothing :P."
		<< std::endl;
}

static void setup_signal_handlers() {
	struct sigaction exitAction;
	exitAction.sa_handler = exit_gracefully;
	sigemptyset(&exitAction.sa_mask);
	exitAction.sa_flags = 0;
	sigaction(SIGINT, &exitAction, nullptr);
}

namespace local {

	namespace ba = boost::asio;
	namespace bs = boost::system;

	typedef fastmq::unix_domain_server<fastmq::log::file::source> server;
	typedef fastmq::router router;

}

int main()
{
	namespace ba = boost::asio;
	namespace bs = boost::system;

	static auto L = std::move(fastmq::log::file::mklogger("main"));
	static const char opt_sock_path[] = "/tmp/fastmqr.sock";

	fastmq::log::file::add("main.log");
	linfo(L) << "Log initialized";

	setup_signal_handlers();
	linfo(L) << "Signal handlers set";

	::unlink(opt_sock_path);

	ba::io_service io;
	ba::local::stream_protocol::endpoint endpoint(opt_sock_path);
	ba::deadline_timer timer(io);

	class stop {
		ba::io_service & m_io;
		public:
			stop(ba::io_service &io): m_io(io) {}
			void operator()(const bs::error_code &) {
				m_io.stop();
			}
	};

	try {
		linfo(L) << "Starting io service";

		local::router router;
		local::server server(io, endpoint, router, L);
		server.start();

		//timer.expires_from_now(boost::posix_time::milliseconds(4000));
		//timer.async_wait(stop(io));

		io.run();

		linfo(L) << "Bye!";
		lflush(L);
	} catch (const std::exception & e) {
		::unlink(opt_sock_path);
		lcritical(L) << e.what();
		return 1;
	}

	::unlink(opt_sock_path);
	return 0;
}
