#ifndef fastmq_router_peer_hpp
#define fastmq_router_peer_hpp

#include <stdexcept>
#include <fastmq/proto/proto.hpp>
#include <fastmq/logger/logger.hpp>

namespace fastmq { namespace router {

using boost::bind;
using namespace ba;
using namespace proto;

template <class ServiceT>
class peer {

	public:

		typedef typename ServiceT::log_t log_t;
		typedef typename ServiceT::proto_t proto_t;
		typedef ba::basic_stream_socket<proto_t> sock_t;

		peer(std::uint16_t index, sock_t & sock, ServiceT & service)
			: m_index(index)
			, m_sock(std::move(sock))
			, m_srv(service)
			, L(service.get_logger())
		{
			in.total_bytes = 0;
			out.total_bytes = 0;
			out.ready = true;
		}

		void index() const noexcept {
			return m_index;
		}

		void start() {
			recv_identity();
		}

		void recv_message() {
			start_recv_msg();
		}

		void send_message(const msgu * msg) {
			send(&peer::on_send_message, msg);
		}

		void log_io_stats() {
			linfo(L)
				<< "bytes in: " << in.total_bytes
				<< ", bytes out: " << out.total_bytes
			;
		}

		u16_t id() const { return m_id; }
		u16_t type() const { return m_type; }

	private:
		u16_t m_id;
		u16_t m_type;

		std::uint16_t m_index;
		sock_t m_sock;
		ServiceT & m_srv;
		log_t & L;

		inbuf in;
		outbuf out;

		std::size_t write_buf(const msgu * msg, buf_t & buf) {
			size_t len = 0;
			if (msg->len <= min_data_msgu_length) {
				/* Service message length in octets */
				len = sizeof(msgu);
			} else {
				/* Data message length in octets */
				len = msg->len;
			}
			if (buf.size() < len)
				buf.resize(len);
			std::copy(as_cu8p(msg), as_cu8p(msg) + len, &buf[0]);
			return len;
		}

		void enque(const msgu *msg) {
			if (out.ready) {
				send(&peer::on_send_message, msg);
			} else {
				buf_t buf;
				write_buf(msg, buf);
				out.que.push(std::move(buf));
			}
		}

		void on_in_bytes(void (peer::* cb)(const bs::error_code & ec
				, std::size_t bytes)
				, const bs::error_code & ec, std::size_t bytes)
		{
			(this->*cb)(ec, bytes);
			in.total_bytes += bytes;
		}

		void on_out_bytes(void (peer::* cb)(const bs::error_code & ec
				, std::size_t bytes)
				, const bs::error_code & ec, std::size_t bytes)
		{
			(this->*cb)(ec, bytes);
			out.total_bytes += bytes;
		}

		void send(void (peer::* cb)(const bs::error_code & ec
				, std::size_t bytes), const msgu *msg)
		{
			std::size_t len = write_buf(msg, out.buf);
			ba::async_write(m_sock, buffer(&out.buf[0], len)
				, bind(&peer::on_out_bytes, this, cb
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void recv(void (peer::* cb)(const bs::error_code & ec
				, std::size_t bytes), std::size_t len, std::size_t offset = 0)
		{
			if (in.buf.size() < len)
				in.buf.resize(len);
			m_sock.async_receive(buffer(&in.buf[0] + offset, len)
				, bind(&peer::on_in_bytes, this, cb
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_send_message(const bs::error_code & ec, std::size_t bytes) {
			m_srv.on_send_message(this, ec, bytes);
		}

		void recv_identity() {
			recv(&peer::on_recv_identity, sizeof(msgu));
		}

		void on_recv_identity(const bs::error_code & ec, std::size_t ) {
			ltrace(L) << "identity received";
			if (!ec) {
				in.msg = as_cmsgup(&in.buf[0]);
				if (in.msg->len != 0) {
					lerror(L) << "Invalid identity message";
					/* TODO: handle error */
					return;
				}
				try {
					u16_t res = m_srv.register_peer(in.msg->id, in.msg->type);
					if (res == in.msg->id) {
						m_id = in.msg->id;
						m_type = in.msg->type;
						/* Peer id accepted. Greet it. */
						send_greeting(m_id, m_type);
					} else {
						/* TODO: handle wrong identity */
						lcritical(L) << "peer rejected, code: " << res;
					}
				} catch (const std::bad_function_call & e) {
					lcritical(L) << "peer registrator not set";
					/* TODO: handle error */
				}
			} else {
				lerror(L) << "peer::on_recv_identity:" << ec.message();
				/* TODO: handle error */
			}
		}

		void send_greeting(u16_t id, u16_t type) {
			msgu grt = { 0, id, type, {} };
			send(&peer::on_send_greeting, &grt);
		}

		void on_send_greeting(const bs::error_code & ec, std::size_t ) {
			if (!ec) {
				ltrace(L) << "greeting sent";
				m_srv.on_register_peer(this);
			} else {
				lerror(L) << "peer::on_send_greeting:" << ec.message();
				/* TODO: handle send error */
			}
		}

		void start_recv_msg() {
			/* Try to receive message header */
			recv(&peer::on_start_recv_msg, sizeof(msgu));
		}

		void on_start_recv_msg(const bs::error_code & ec, std::size_t bytes) {
			/* Message header received */
			if (!ec) {
				ltrace(L) << "msg header received: " << bytes << " bytes.";
				in.msg = as_cmsgup(&in.buf[0]);
				if (in.msg->len <= min_data_msgu_length) {
					/* Service message received. Handle it. */
					m_srv.on_message(this, in.msg);
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
			recv(&peer::on_end_recv_msg
				, in.msg->len - sizeof(msgu), sizeof(msgu));
		}

		void on_end_recv_msg(const bs::error_code & ec, std::size_t bytes) {
			if (!ec) {
				ltrace(L) << "msg data received: " << bytes << " bytes.";
				m_srv.on_message(this, in.msg);
			} else {
				lerror(L) << "peer::on_end_recv_msg:" << ec.message();
				/* TODO: handle recv error */
			}
		}
};

} }

#endif
