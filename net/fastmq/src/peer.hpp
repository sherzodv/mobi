#ifndef fastmq_router_peer_hpp
#define fastmq_router_peer_hpp

#include <queue>
#include <vector>
#include <stdexcept>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "proto.hpp"
#include "logger.hpp"
#include "subscriber.hpp"

namespace fastmq {

using boost::bind;
namespace ba = boost::asio;
namespace bs = boost::system;

template <class ServiceT>
class peer: subscriber_base {

	friend ServiceT;

	public:

		typedef typename ServiceT::log_t log_t;
		typedef typename ServiceT::proto_t proto_t;
		typedef ba::basic_stream_socket<proto_t> sock_t;

		peer(std::uint16_t index, sock_t & sock, ServiceT & service)
			: S(service)
			, m_idx(index)
			, m_sock(std::move(sock))
		{
			in.total_bytes = 0;
			out.total_bytes = 0;
			out.ready = true;
		}

		void start() {
			recv_identity();
		}

	private:
		typedef void (peer::* cb_t)(const bs::error_code &, std::size_t);

		ServiceT & S;
		std::size_t m_idx;
		sock_t m_sock;

		struct inbuf {
			msgu *msg;
			msgu msghdr[1];
			std::size_t total_bytes;
		} in;

		struct outbuf {
			msgu *msg;
			bool ready;
			std::queue<const msgu *> que;
			std::size_t total_bytes;
		} out;

		void on_in_bytes(cb_t cb, const bs::error_code & ec,
				std::size_t bytes)
		{
			(this->*cb)(ec, bytes);
			in.total_bytes += bytes;
		}

		void on_out_bytes(cb_t cb, const bs::error_code & ec,
				std::size_t bytes)
		{
			(this->*cb)(ec, bytes);
			out.total_bytes += bytes;
		}

		virtual void process_message(const msgu * msg)
		{
			if (out.ready) {
				send_message(msg);
			} else {
				out.que.push(msg);
			}
		}

		void send_message(const msgu * msg)
		{
			std::size_t len = msg->len;

			if (len <= sizeof(msgu))
				len = sizeof(msgu);

			out.ready = false;

			ba::async_write(m_sock, ba::buffer(as_cu8p(msg), len)
				, bind(&peer::on_out_bytes, this, &peer::on_send_message
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_send_message(const bs::error_code & ec, std::size_t )
		{
			if (!ec) {
				ltrace(S.L) << "greeting sent";
				if (out.que.empty()) {
					out.ready = true;
					return;
				}
				const msgu * msg = out.que.front();
				out.que.pop();
				send_message(msg);
			} else {
				S.R.route_clear(this);
				S.destroy(this);
				lerror(S.L) << "peer::on_send_greeting:" << ec.message();
			}
		}

		void recv_identity()
		{
			m_sock.async_receive(ba::buffer(as_u8p(in.msghdr), sizeof(msgu))
				, bind(&peer::on_in_bytes, this, &peer::on_recv_identity
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_recv_identity(const bs::error_code & ec, std::size_t bytes)
		{
			ltrace(S.L) << "identity received";
			m_id = in.msghdr->id;
			m_type = in.msghdr->type;
			if (!ec) {
				if (in.msghdr->len != 0) {
					lerror(S.L) << "Invalid identity message: " << bytes;
					/* TODO: handle error */
					return;
				}
				if (S.R.route_add(this) == m_id) {
					/* Client id:type accepted. Greet it. */
					send_greeting();
				} else {
					/* TODO: handle wrong identity */
					lcritical(S.L) << "client id rejected";
				}
			} else {
				lerror(S.L) << "peer::on_recv_identity:" << ec.message();
				S.destroy(this);
			}
		}

		void send_greeting()
		{
			out.ready = false;
			out.msg = S.R.create_message();
			out.msg->len = 0;
			out.msg->id = m_id;
			out.msg->type = m_type;
			out.msg->data[0] = 0;
			ba::async_write(m_sock, ba::buffer(as_u8p(out.msg), sizeof(msgu))
				, bind(&peer::on_out_bytes, this, &peer::on_send_greeting
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_send_greeting(const bs::error_code & ec, std::size_t )
		{
			out.ready = true;
			S.R.destroy_message(out.msg);
			if (!ec) {
				ltrace(S.L) << "greeting sent";
			} else {
				S.R.route_clear(this);
				S.destroy(this);
				lerror(S.L) << "peer::on_send_greeting:" << ec.message();
			}
		}

		void recv_header()
		{
			m_sock.async_receive(ba::buffer(as_u8p(in.msghdr), sizeof(msgu))
				, bind(&peer::on_in_bytes, this, &peer::on_recv_header
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_recv_header(const bs::error_code & ec, std::size_t bytes)
		{
			ltrace(S.L) << "header received: " << bytes;
			if (!ec) {
				if (in.msghdr->len <= sizeof(msgu)) {
					/* Service message */
					msgu * msg = S.R.create_message();
					std::memcpy(msg, &in.msghdr, sizeof(msgu));
					S.R.transfer_message(msg);
				} else {
					in.msg = S.R.create_message();
					std::memcpy(in.msg, &in.msghdr, sizeof(msgu));
					/* Data message */
					recv_body();
				}
			} else {
				lerror(S.L) << "peer::on_recv_header:" << ec.message();
				S.destroy(this);
			}
		}

		void recv_body()
		{
			/* Read the remaining body of a messsage, beyond msg->data[0] */
			m_sock.async_receive(
				ba::buffer(as_u8p(in.msg) + sizeof(msgu)
					, in.msg->len-sizeof(msgu))
				, bind(&peer::on_in_bytes, this, &peer::on_recv_body
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_recv_body(const bs::error_code & ec, std::size_t bytes)
		{
			ltrace(S.L) << "body received: " << bytes;
			if (!ec) {
				/* Route message */
				S.R.transfer_message(in.msg);
				/* Receive next */
				recv_header();
			} else {
				lerror(S.L) << "peer::on_recv_body:" << ec.message();
				S.destroy(this);
			}
		}

};

}

#endif
