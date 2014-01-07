#ifndef fastmq_toolbox_hpp
#define fastmq_toolbox_hpp

#include <signal.h>
#include <boost/asio.hpp>

namespace mobi { namespace net { namespace toolbox {

	template <typename T>
	void set_signal_handler(T h) {
		static struct sigaction m_action;
		static T handler(h);
		class delegate {
			public:
				static void call(int sid) {
					handler(sid);
				}
		};
		m_action.sa_handler = &delegate::call;
		sigemptyset(&m_action.sa_mask);
		m_action.sa_flags = 0;
		sigaction(SIGINT, &m_action, nullptr);
	}

	template <typename ServiceT>
	class stopper {
		ServiceT & S;
		bool m_stop_on_destroy;
		public:
			stopper(ServiceT & s, bool sd = false)
				: S(s), m_stop_on_destroy(sd)
			{}

			stopper(const stopper & other)
				: S(other.S)
				, m_stop_on_destroy(other.m_stop_on_destroy){}

			stopper(stopper && other)
				: S(other.S)
				, m_stop_on_destroy(other.m_stop_on_destroy){}

			~stopper() {
				if (m_stop_on_destroy) {
					S.stop();
				}
			}

			void operator()() {
				S.stop();
			}

			/* As a signal handler */
			void operator()(int) {
				S.stop();
			}
	};

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

		void stop_on_signal(boost::asio::io_service & io) {
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

		void stop_after(boost::asio::io_service & io
				, const boost::posix_time::time_duration & d) {
			static boost::asio::deadline_timer stop_timer(io);
			stop_timer.expires_from_now(d);
			stop_timer.async_wait(toolbox::io::stop(io));
		}

	}

} } }

#endif
