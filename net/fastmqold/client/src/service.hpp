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

template <class ProtoT, class LoggerT, class ImplT>
class service {

	typedef ba::basic_stream_socket<ProtoT> sock_t;

	public:
		typedef LoggerT	log_t;
		typedef ProtoT proto_t;

		template <class EndpointIteratorT>
		service(ba::io_service & io
				, const EndpointIteratorT & begin
				, proto::u16_t id, proto::u16_t type
				, LoggerT & logger)
			: m_id(id), m_type(type), m_io(io), m_sock(m_io), L(logger)
		{
			m_this = static_cast<ImplT *>(this);
			m_sock.async_connect(begin
				, bind(&service::on_connect
					, this, ba::placeholders::error));
			in.total_bytes = 0;
			out.total_bytes = 0;
		}

		void recv_message() {
			start_recv_msg();
		}

		void send_message(const msgu * msg) {
			send(&service::on_send_message, msg);
		}

		void log_io_stats() {
			linfo(L)
				<< "bytes in: " << in.total_bytes
				<< ", bytes out: " << out.total_bytes
			;
		}

		/*
		 * Implement these member functions:
		 *
		 * static_virtual void on_register() = 0;
		 * static_virtual void on_message(peer_t *p, msgu * msg) = 0;
		 * static_virtual void on_send_message(const bs::error_code & ec
		 *		, size_t bytes) = 0;
		 *
		 */

	protected:

		proto::u16_t m_id;
		proto::u16_t m_type;

		ba::io_service & m_io;
		sock_t m_sock;

		LoggerT & L;
		ImplT * m_this;

		inbuf in;
		outbuf out;

		void on_in_bytes(void (service::* cb)(const bs::error_code & ec
				, std::size_t bytes)
				, const bs::error_code & ec, std::size_t bytes)
		{
			(this->*cb)(ec, bytes);
			in.total_bytes += bytes;
		}

		void on_out_bytes(void (service::* cb)(const bs::error_code & ec
				, std::size_t bytes)
				, const bs::error_code & ec, std::size_t bytes)
		{
			(this->*cb)(ec, bytes);
			out.total_bytes += bytes;
		}

		void send(void (service::* cb)(const bs::error_code & ec
				, std::size_t bytes), const msgu *msg)
		{
			size_t len = 0;

			if (msg->len <= min_data_msgu_length) {
				/* Service message length in octets */
				len = sizeof(msgu);
			} else {
				/* Data message length in octets */
				len = msg->len;
			}

			if (out.buf.size() < len) {
				out.buf.resize(len);
			}

			std::copy(as_cu8p(msg), as_cu8p(msg) + len, &out.buf[0]);

			m_sock.async_send(buffer(&out.buf[0], len)
				, bind(&service::on_out_bytes, this, cb
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void recv(void (service::* cb)(const bs::error_code & ec
				, std::size_t bytes), std::size_t len, std::size_t offset = 0)
		{
			if (in.buf.size() < len)
				in.buf.resize(len);
			m_sock.async_receive(buffer(&in.buf[0] + offset, len)
				, bind(&service::on_in_bytes, this, cb
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_send_message(const bs::error_code & ec, std::size_t bytes) {
			m_this->on_send_message(ec, bytes);
		}

		void on_connect(const bs::error_code & ec) {
			if (!ec) {
				send_identity();
			} else {
				lerror(L) << "service::on_connect: " << ec.message();
				/* TODO: handle error */
			}
		}

		void send_identity() {
			msgu id = { 0, m_id, m_type, {} };
			send(&service::on_send_identity, &id);
		}

		void on_send_identity(const bs::error_code & ec, size_t ) {
			if (!ec) {
				ltrace(L) << "identity sent";
				recv_greeting();
			} else {
				lerror(L) << "service::on_send_identity: " << ec.message();
				/* TODO: handle error */
			}
		}

		void recv_greeting() {
			recv(&service::on_recv_greeting, sizeof(msgu));
		}

		void on_recv_greeting(const bs::error_code & ec, size_t bytes) {
			if (!ec) {
				/* TODO: check if greeting is ok */
				ltrace(L) << "greeting received: " << bytes << " bytes.";
				m_this->on_register();
			} else {
				lerror(L) << "service::on_recv_greeting: " << ec.message();
				/* TODO: handle error */
			}
		}

		void start_recv_msg() {
			/* Try to receive message header */
			recv(&service::on_start_recv_msg, sizeof(msgu));
		}

		void on_start_recv_msg(const bs::error_code & ec, std::size_t bytes) {
			/* Message header received */
			if (!ec) {
				ltrace(L) << "msg header received: " << bytes << " bytes.";
				in.msg = as_cmsgup(&in.buf[0]);
				if (in.msg->len <= min_data_msgu_length) {
					/* Service message received. Handle it. */
					m_this->on_message(in.msg);
				} else {
					/* Data message received. Read the remainging part. */
					end_recv_msg();
				}
			} else {
				lerror(L) << "peer::on_start_recv_msg:" << ec.message();
				/* TODO: handle recv error */
			}
		}

		void end_recv_msg() {
			recv(&service::on_end_recv_msg
				, in.msg->len - sizeof(msgu), sizeof(msgu));
		}

		void on_end_recv_msg(const bs::error_code & ec, std::size_t bytes) {
			if (!ec) {
				ltrace(L) << "msg data received: " << bytes << " bytes.";
				m_this->on_message(in.msg);
			} else {
				lerror(L) << "peer::on_end_recv_msg:" << ec.message();
				/* TODO: handle recv error */
			}
		}
};

} }

#endif
