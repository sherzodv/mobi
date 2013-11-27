#ifndef fastmq_toolbox_hpp
#define fastmq_toolbox_hpp

#include <signal.h>
#include <boost/asio.hpp>

namespace toolbox {

	namespace io {

		class stop {
			boost::asio::io_service & m_io;
			public:
				stop(boost::asio::io_service &io): m_io(io) {}
				stop(boost::asio::io_service *io): m_io(*io) {}
				void operator()() {
					m_io.stop();
				}
				void operator()(const boost::system::error_code & ec) {
					(void)(ec);
					m_io.stop();
				}
		};

		inline void stop_on_signal(boost::asio::io_service & io) {
			static struct sigaction m_exitAction;
			static boost::asio::io_service * m_io = &io;
			class exit_delegate {
				public:
					static void call(int /* sid */) {
						m_io->post(stop(m_io));
					}
			};
			m_exitAction.sa_handler = &exit_delegate::call;
			sigemptyset(&m_exitAction.sa_mask);
			m_exitAction.sa_flags = 0;
			sigaction(SIGINT, &m_exitAction, nullptr);
		}

		inline void stop_after(boost::asio::io_service & io
				, const boost::posix_time::time_duration & d) {
			static boost::asio::deadline_timer stop_timer(io);
			stop_timer.expires_from_now(d);
			stop_timer.async_wait(toolbox::io::stop(io));
		}

	}

}

#endif
