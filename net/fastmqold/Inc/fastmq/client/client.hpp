#ifndef fastmq_client_service_hpp
#define fastmq_client_service_hpp

#include <vector>
#include <functional>
#include <fastmq/proto/proto.hpp>
#include <fastmq/logger/logger.hpp>

namespace fastmq { namespace client {

using boost::bind;
using namespace ba;
using namespace proto;

template <class ProtoT>
class service {

	typedef ba::basic_stream_socket<ProtoT> sock_t;
	typedef std::function<void (const proto::msgu *)> on_msg_F;

	public:

		template <class EndpointIteratorT>
		service(ba::io_service & io
				, const EndpointIteratorT & begin
				, proto::u16_t id, proto::u16_t type
				, const std::string& logchan)
			: m_id(id), m_type(type), m_io(io), m_sock(m_io)
		{
			L = std::move(log::file::mklogger(logchan));
			ba::async_connect(m_sock, begin
				, bind(&service::on_connect
					, this, ba::placeholders::error));
		}

		~service() {
		}

		void set_msg_handler(const func::msg_handler_F &handler) {
			m_handle_message = handler;
		}

	private:

		proto::u16_t m_id;
		proto::u16_t m_type;
		ba::io_service & m_io;
		sock_t m_sock;
		std::vector<proto::u8_t> m_buf;
		func::msg_handler_F m_handle_message;
		log::file::source L;

		void on_connect(const bs::error_code & ec) {
			if (!ec) {
				send_identity();
			} else {
				lerror(L) << "service::on_connect: " << ec.message();
				/* TODO: handle error */
			}
		}

		void send_identity() {

			using std::bind;
			using namespace ba;
			using namespace proto;
			using namespace std::placeholders;

			msgu id = { 0, m_id, m_type, {} };

			m_sock.async_send(buffer(as_cvp(&id), sizeof(msgu))
				, bind(&service::on_send_identity, this
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void recv_greeting() {

			using std::bind;
			using namespace ba;
			using namespace proto;
			using namespace std::placeholders;

			if (m_buf.size() < sizeof(msgu))
				m_buf.resize(sizeof(msgu));

			m_sock.async_receive(buffer(&m_buf[0], sizeof(msgu))
				, bind(&service::on_recv_greeting, this
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_send_identity(const bs::error_code & ec, size_t ) {
			if (!ec) {
				recv_greeting();
			} else {
				lerror(L) << "service::on_send_identity: " << ec.message();
				/* TODO: handle error */
			}
		}

		void on_recv_greeting(const bs::error_code & ec) {
			if (!ec) {
			} else {
				lerror(L) << "service::on_recv_greeting: " << ec.message();
				/* TODO: handle error */
			}
		}
};

} }

#endif
