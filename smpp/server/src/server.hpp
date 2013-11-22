#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "cmanager.hpp"
#include "connection.hpp"

namespace smpp { namespace server {

namespace ba = boost::asio;
namespace bs = boost::system;

typedef boost::asio::ip::tcp btcp;

class service: public connection_manager<connection::shared_pointer> {
	public:
		service(ba::io_service & io)
			: m_io(io)
			, m_acceptor(io, btcp::endpoint(boost::asio::ip::tcp::v4(), port))
		{
		}

	private:
		ba::io_service & m_io;
		btcp::acceptor m_acceptor;

		void listen() {
			connection::shared_pointer newC = connection::create(m_io, *this);
			m_acceptor.async_accept(newC->socket()
				, boost::bind(&service::on_accept, this, newC, ba::placeholders::error));
		}

		void on_accept(connection::shared_pointer newC, const bs::error_code & errorCode) {
			if (errorCode) {
				if (errorCode == bs::errc::operation_canceled) {
				} else {
				}
			} else {
				listen();
				newC->process();
			}
		}

		void add(connection::shared_pointer c) {
		}

		void remove(connection::shared_pointer c) {
		}
};

} }

