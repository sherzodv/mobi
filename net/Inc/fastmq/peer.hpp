#ifndef fastmq_router_peer_hpp
#define fastmq_router_peer_hpp

#include <queue>
#include <vector>
#include <stdexcept>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <fastmq/log.hpp>
#include <fastmq/proto.hpp>
#include <fastmq/terminal.hpp>

namespace fastmq {

using boost::bind;
namespace ba = boost::asio;
namespace bs = boost::system;

/* ServiceT requirements:
 * class ServiceT {
 * public:
 * };
 * */

template <class ServiceT>
class peer: terminal_base {

	friend ServiceT;

	typedef typename ServiceT::proto_t proto_t;
	typedef ba::basic_stream_socket<proto_t> sock_t;

	public:
		peer(std::uint16_t index, sock_t & sock, ServiceT & service)
			: S(service)
			, m_idx(index)
			, m_sock(std::move(sock))
		{
			in.ready = true;
			in.total_bytes = 0;

			out.ready = true;
			out.total_bytes = 0;
		}

		peer(std::size_t index, u16_t id, u16_t type, sock_t & sock
				, ServiceT & service)
			: terminal_base(id, type)
			, S(service)
			, m_idx(index)
			, m_sock(std::move(sock))
		{
			in.ready = true;
			in.total_bytes = 0;

			out.ready = true;
			out.total_bytes = 0;
		}

		std::size_t index() { return m_idx; }

		void flush()
		{
			if (!out.ready || out.que.empty())
				return;
			msgu * msg = out.que.front();
			out.que.pop();
			send_message(msg);
		}

		void send(msgu * msg)
		{
			if (out.ready) {
				send_message(msg);
			} else {
				out.que.push(msg);
			}
		}

		void send_greeting()
		{
			if (!out.ready) {
				lerror(S.L) << "peer::recv_identity: out buffer is busy";
				return;
			}
			out.ready = false;
			out.msg = S.P.create_message();
			out.msg->len = 0;
			out.msg->id = m_id;
			out.msg->type = m_type;
			out.msg->data[0] = 0;
			ltrace(S.L) << "peer::send_greeting: bytes: " << sizeof(msgu);
			ba::async_write(m_sock, ba::buffer(as_u8p(out.msg), sizeof(msgu))
				, bind(&peer::on_out_bytes, this, &peer::on_send_greeting
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void send_identity()
		{
			if (!out.ready) {
				lerror(S.L) << "peer::send_identity: out buffer is busy";
				return;
			}
			out.ready = false;
			out.msg = S.P.create_message();
			out.msg->len = 0;
			out.msg->id = m_id;
			out.msg->type = m_type;
			out.msg->data[0] = 0;
			ltrace(S.L) << "peer::send_identity: bytes: " << sizeof(msgu);
			ba::async_write(m_sock, ba::buffer(as_u8p(out.msg), sizeof(msgu))
				, bind(&peer::on_out_bytes, this, &peer::on_send_identity
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void recv_identity()
		{
			if (!in.ready) {
				lerror(S.L) << "peer::recv_identity: in buffer is busy";
				return;
			}
			in.ready = false;
			in.msg = S.P.create_message();
			ltrace(S.L) << "peer::recv_identity: bytes: " << sizeof(msgu);
			m_sock.async_receive(ba::buffer(as_u8p(in.msg), sizeof(msgu))
				, bind(&peer::on_in_bytes, this, &peer::on_recv_identity
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void recv_greeting()
		{
			if (!in.ready) {
				lerror(S.L) << "peer::recv_greeting: in buffer is busy";
				return;
			}
			in.ready = false;
			in.msg = S.P.create_message();
			ltrace(S.L) << "peer::recv_greeting: bytes: " << sizeof(msgu);
			m_sock.async_receive(ba::buffer(as_u8p(in.msg), sizeof(msgu))
				, bind(&peer::on_in_bytes, this, &peer::on_recv_greeting
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		virtual void consume_message(msgu * msg) {
			send(msg);
		}

		virtual void produce_message() {
			recv_header();
		}

	private:
		typedef void (peer::* cb_t)(const bs::error_code &);

		ServiceT & S;
		std::size_t m_idx;
		sock_t m_sock;

		struct inbuf {
			msgu *msg;
			bool ready;
			std::size_t total_bytes;
		} in;

		struct outbuf {
			msgu *msg;
			bool ready;
			std::queue<msgu *> que;
			std::size_t total_bytes;
		} out;

		void on_in_bytes(cb_t cb, const bs::error_code & ec,
				std::size_t bytes)
		{
			in.total_bytes += bytes;
			ltrace(S.L) << "peer: bytes in: " << bytes;
			/* !!! It's crucial to call callback at the very end of this
			 * member function, since callback may delete this */
			(this->*cb)(ec);
		}

		void on_out_bytes(cb_t cb, const bs::error_code & ec,
				std::size_t bytes)
		{
			out.total_bytes += bytes;
			ltrace(S.L) << "peer: bytes out: " << bytes;
			/* !!! It's crucial to call callback at the very end of this
			 * member function, since callback may delete this */
			(this->*cb)(ec);
		}

		void send_message(msgu * msg)
		{
			std::size_t len;
			/* Caller is responsible for destroying message. */
			out.msg = msg;
			len = out.msg->len;
			if (len <= sizeof(msgu))
				len = sizeof(msgu);
			out.ready = false;
			ltrace(S.L) << "peer::send_message: bytes: " << len;
			ba::async_write(m_sock, ba::buffer(as_cu8p(out.msg), len)
				, bind(&peer::on_out_bytes, this, &peer::on_send_message
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_send_message(const bs::error_code & ec)
		{
			out.ready = true;
			if (!ec) {
				ltrace(S.L) << "peer::on_send_message: ok";
				S.on_send(this, out.msg);
				if (out.que.empty()) {
					return;
				}
				msgu * msg = out.que.front();
				out.que.pop();
				send_message(msg);
			} else {
				/* Send cycle stop here. Call to flush is needed
				 * in order to let messages in queue to be sent */
				lerror(S.L) << "peer::on_send_message: " << ec.message();
				/* !!! It's crucial to call callback at the very end of this
				 * member function, since callback may delete this */
				S.on_send_error(this, in.msg);
			}
		}

		void recv_header()
		{
			if (!in.ready) {
				lerror(S.L) << "peer::recv_header: in buffer is busy";
				return;
			}
			in.ready = false;
			in.msg = S.P.create_message();
			ltrace(S.L) << "peer::recv_header: bytes: " << sizeof(msgu);
			m_sock.async_receive(ba::buffer(as_u8p(in.msg), sizeof(msgu))
				, bind(&peer::on_in_bytes, this, &peer::on_recv_header
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_recv_header(const bs::error_code & ec)
		{
			if (!ec) {
				ltrace(S.L) << "peer::on_recv_header: msg->len: "
					<< in.msg->len;
				if (in.msg->len <= sizeof(msgu)) {
					/* Service message */
					in.ready = true;
					S.on_recv(this, in.msg);
				} else {
					/* Data message */
					/* Previously we have created msg of len = sizeof(msgu)
					 * only to recv header in it. Now we've found out that
					 * message has body. Copy header, destroy message
					 * and create new one with sufficient len */
					msgu * msg = S.P.create_message(in.msg->len);
					std::memcpy(msg, in.msg, sizeof(msgu));
					S.P.destroy_message(in.msg);
					in.msg = msg;
					recv_body();
				}
			} else {
				lerror(S.L) << "peer::on_recv_header: " << ec.message();
				in.ready = true;
				S.P.destroy_message(in.msg);
				/* !!! It's crucial to call callback at the very end of this
				 * member function, since callback may delete this */
				S.on_recv_error(this);
			}
		}

		void recv_body()
		{
			ltrace(S.L) << "peer::recv_body: bytes: "
				<< in.msg->len - sizeof(msgu);
			/* Read the remaining body of a messsage, beyond msg->data[0] */
			m_sock.async_receive(
				ba::buffer(as_u8p(in.msg) + sizeof(msgu)
					, in.msg->len-sizeof(msgu))
				, bind(&peer::on_in_bytes, this, &peer::on_recv_body
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_recv_body(const bs::error_code & ec)
		{
			in.ready = true;
			if (!ec) {
				ltrace(S.L) << "peer::on_recv_body: ok";
				/* Let the service process received message */
				S.on_recv(this, in.msg);
			} else {
				lerror(S.L) << "peer::on_recv_body: " << ec.message();
				S.P.destroy_message(in.msg);
				/* !!! It's crucial to call callback at the very end of this
				 * member function, since callback may delete this */
				S.on_recv_error(this);
			}
		}

		void on_recv_identity(const bs::error_code & ec)
		{
			in.ready = true;
			if (!ec) {
				ltrace(S.L) << "peer::on_recv_identity: ok";
				if (in.msg->len <= sizeof(msgu)) {
					m_id = in.msg->id;
					m_type = in.msg->type;
					S.P.destroy_message(in.msg);
					S.on_recv_identity(this);
					return;
				}
				lerror(S.L) << "peer::on_recv_identity: malformed message";
			} else {
				lerror(S.L) << "peer::on_recv_identity: " << ec.message();
			}
			S.P.destroy_message(in.msg);
			/* !!! It's crucial to call callback at the very end of this
			 * member function, since callback may delete this */
			S.on_recv_error(this);
		}

		void on_recv_greeting(const bs::error_code & ec)
		{
			in.ready = true;
			if (!ec) {
				ltrace(S.L) << "peer::on_recv_greeting: ok";
				if (in.msg->len <= sizeof(msgu) && in.msg->id == m_id
						&& in.msg->type == m_type) {
					S.P.destroy_message(in.msg);
					S.on_recv_greeting(this);
					return;
				}
				lerror(S.L) << "peer::on_recv_greeting: malformed message";
			} else {
				lerror(S.L) << "peer::on_recv_greeting: " << ec.message();
			}
			S.P.destroy_message(in.msg);
			/* !!! It's crucial to call callback at the very end of this
			 * member function, since callback may delete this */
			S.on_recv_error(this);
		}

		void on_send_identity(const bs::error_code & ec)
		{
			out.ready = true;
			S.P.destroy_message(out.msg);
			if (!ec) {
				ltrace(S.L) << "peer::on_send_identity: ok";
				S.on_send_identity(this);
			} else {
				lerror(S.L) << "peer::on_send_identity: " << ec.message();
				/* !!! It's crucial to call callback at the very end of this
				 * member function, since callback may delete this */
				S.on_send_identity_error(this);
			}
		}

		void on_send_greeting(const bs::error_code & ec)
		{
			out.ready = true;
			S.P.destroy_message(out.msg);
			if (!ec) {
				ltrace(S.L) << "peer::on_send_greeting: ok";
				S.on_send_greeting(this);
			} else {
				lerror(S.L) << "peer::on_send_greeting: " << ec.message();
				/* !!! It's crucial to call callback at the very end of this
				 * member function, since callback may delete this */
				S.on_send_greeting_error(this);
			}
		}
};

}

#endif
