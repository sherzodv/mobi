#ifndef fastmq_toolbox_hpp
#define fastmq_toolbox_hpp

#include <boost/asio.hpp>

namespace fastmq {

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

}

#endif
