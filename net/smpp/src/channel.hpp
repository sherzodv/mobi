#ifndef smpp_channel_hpp
#define smpp_channel_hpp

#include <queue>
#include <vector>
#include <stdexcept>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <smpp/proto.hpp>
#include <smpp/session.hpp>

namespace smpp {

void fix_pdu_bo(pdu * p) {
	using namespace utl::bo;
	p->len		= tohost(p->len);
	p->id		= tohost(p->id);
	p->status	= tohost(p->status);
	p->seqno	= tohost(p->seqno);
}

using boost::bind;
namespace ba = boost::asio;
namespace bs = boost::system;

template <class ServiceT>
class channel: public session {

	friend ServiceT;

	typedef typename ServiceT::proto_t proto_t;
	typedef ba::basic_stream_socket<proto_t> sock_t;

	public:
		channel(std::uint16_t index, sock_t & sock, ServiceT & service)
			: S(service)
			, m_idx(index)
			, m_sock(std::move(sock))
		{
			in.ready = true;
			in.total_bytes = 0;

			out.ready = true;
			out.total_bytes = 0;

			ltrace(S.L) << "channel::channel: idx: " << m_idx;
		}

		virtual ~channel() {
			ltrace(S.L) << "channel::~channel: idx: " << m_idx;
		}

		std::size_t index() const { return m_idx; }

		virtual bool ready_to_send() const {
			return out.ready;
		}

		virtual bool ready_to_recv() const {
			return in.ready;
		}

		virtual void flush() {
			if (!out.ready || out.que.empty())
				return;
			pdu * msg = out.que.front();
			out.que.pop();
			send_message(msg, &channel::on_send_message);
		}

		virtual void send(pdu * msg) {
			if (out.ready) {
				using namespace utl;
				/* Caller is responsible for destroying message. */
				out.msg = msg;
				out.ready = false;
				ltrace(S.L) << "channel::send_message: bytes: "
					<< bo::tohost(msg->len);
				/* Message to send is always comes with network byte order
				 * , so call bo::tohost(msg->len) is needed */
				ba::async_write(m_sock
					, ba::buffer(ascbuf(out.msg), bo::tohost(msg->len))
					, bind(&channel::on_out_bytes, this
						, &channel::on_send_message
						, ba::placeholders::error
						, ba::placeholders::bytes_transferred));
			} else {
				out.que.push(msg);
			}
		}

		virtual void recv() {
			recv_header();
		}

		void recv_bind() {
			recv_bind_header();
		}

		void send_bind_r(pdu * msg) {
			if (out.ready) {
				send_message(msg, &channel::on_send_bind_r);
			} else {
				/* out buffer should always be ready at this point */
				lerror(S.L) << "Out buf is not ready to send bind_r";
			}
		}

	private:
		typedef void (channel::* cb_t)(const bs::error_code &);

		ServiceT & S;
		std::size_t m_idx;
		sock_t m_sock;

		struct inbuf {
			pdu * msg;
			bool ready;
			std::size_t total_bytes;
		} in;

		struct outbuf {
			pdu * msg;
			bool ready;
			std::queue<pdu *> que;
			std::size_t total_bytes;
		} out;

		void on_in_bytes(cb_t cb, const bs::error_code & ec,
				std::size_t bytes)
		{
			in.total_bytes += bytes;
			ltrace(S.L) << "channel: bytes in: " << bytes;
			/* !!! It's crucial to call callback at the very end of this
			 * member function, since callback may delete this */
			(this->*cb)(ec);
		}

		void on_out_bytes(cb_t cb, const bs::error_code & ec,
				std::size_t bytes)
		{
			out.total_bytes += bytes;
			ltrace(S.L) << "channel: bytes out: " << bytes;
			/* !!! It's crucial to call callback at the very end of this
			 * member function, since callback may delete this */
			(this->*cb)(ec);
		}

		void send_message(pdu * msg, cb_t cb)
		{
			using namespace utl;
			/* Caller is responsible for destroying message. */
			out.msg = msg;
			out.ready = false;
			ltrace(S.L) << "channel::send_message: bytes: "
				<< bo::tohost(msg->len);
			/* Message to send is always comes with network byte order
			 * , so call bo::tohost(msg->len) is needed */
			ba::async_write(m_sock
				, ba::buffer(ascbuf(out.msg), bo::tohost(msg->len))
				, bind(&channel::on_out_bytes, this, cb
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_send_message(const bs::error_code & ec)
		{
			out.ready = true;
			if (!ec) {
				ltrace(S.L) << "channel::on_send_message: ok";
				S.on_send(this, out.msg);
				if (out.que.empty()) {
					return;
				}
				pdu * msg = out.que.front();
				out.que.pop();
				send_message(msg, &channel::on_send_message);
			} else {
				/* Send cycle stop here. Call to flush is needed
				 * in order to let messages in queue to be sent */
				lerror(S.L) << "channel::on_send_message: " << ec.message();
				/* !!! It's crucial to call callback at the very end of this
				 * member function, since callback may delete this */
				S.on_send_error(this, in.msg);
			}
		}

		void recv_header()
		{
			using namespace utl;
			if (!in.ready) {
				lerror(S.L) << "channel::recv_header: in buffer is busy";
				return;
			}
			in.ready = false;
			in.msg = S.P.create_message();
			ltrace(S.L) << "channel::recv_header: bytes: " << sizeof(pdu);
			m_sock.async_receive(ba::buffer(asbuf(in.msg), sizeof(pdu))
				, bind(&channel::on_in_bytes, this, &channel::on_recv_header
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_recv_header(const bs::error_code & ec)
		{
			fix_pdu_bo(in.msg);
			if (!ec) {
				ltrace(S.L) << "channel::on_recv_header: msg->len: "
					<< in.msg->len;
				if (in.msg->len < sizeof(pdu)) {
					lerror(S.L) << "channel::on_recv_header: wrong msg->len: "
						<< in.msg->len;
					S.P.destroy_message(in.msg);
					/* !!! It's crucial to call callback at the very
					 * end of this member function, since callback
					 * may delete this */
					S.on_recv_error(this);
					return;
				}
				if (in.msg->len == sizeof(pdu)) {
					/* Service message */
					in.ready = true;
					S.on_recv(this, in.msg);
				} else {
					/* Data message */
					/* Previously we have created msg of len = sizeof(pdu)
					 * only to recv header in it. Now we've found out that
					 * message has body. Copy header, destroy message
					 * and create new one with sufficient len */
					pdu * msg = S.P.create_message(in.msg->len);
					std::memcpy(msg, in.msg, sizeof(pdu));
					S.P.destroy_message(in.msg);
					in.msg = msg;
					recv_body();
				}
			} else {
				lerror(S.L) << "channel::on_recv_header: " << ec.message();
				in.ready = true;
				S.P.destroy_message(in.msg);
				/* !!! It's crucial to call callback at the very end of this
				 * member function, since callback may delete this */
				S.on_recv_error(this);
			}
		}

		void recv_body()
		{
			using namespace utl;
			ltrace(S.L) << "channel::recv_body: bytes: "
				<< in.msg->len - sizeof(pdu);
			/* Read the remaining body of a messsage, beyond msg header */
			m_sock.async_receive(
				ba::buffer(asbuf(in.msg) + sizeof(pdu)
					, in.msg->len-sizeof(pdu))
				, bind(&channel::on_in_bytes, this, &channel::on_recv_body
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_recv_body(const bs::error_code & ec)
		{
			in.ready = true;
			if (!ec) {
				ltrace(S.L) << "channel::on_recv_body: ok";
				/* Let the service process received message */
				S.on_recv(this, in.msg);
			} else {
				lerror(S.L) << "channel::on_recv_body: " << ec.message();
				S.P.destroy_message(in.msg);
				/* !!! It's crucial to call callback at the very end of this
				 * member function, since callback may delete this */
				S.on_recv_error(this);
			}
		}

		void recv_bind_header()
		{
			using namespace utl;
			if (!in.ready) {
				lerror(S.L) << "channel::recv_header: in buffer is busy";
				return;
			}
			in.ready = false;
			in.msg = S.P.create_message();
			ltrace(S.L) << "channel::recv_bind_header: bytes: " << sizeof(pdu);
			m_sock.async_receive(ba::buffer(asbuf(in.msg), sizeof(pdu))
				, bind(&channel::on_in_bytes, this
					, &channel::on_recv_bind_header
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_recv_bind_header(const bs::error_code & ec)
		{
			fix_pdu_bo(in.msg);
			if (!ec) {
				ltrace(S.L) << "channel::on_recv_bind_header: msg->len: "
					<< in.msg->len;
				if (in.msg->len < bind_pdu_min_len) {
					lerror(S.L) << "channel::on_recv_bind_header:"
						"wrong len for bind: " << in.msg->len;
					S.P.destroy_message(in.msg);
					/* !!! It's crucial to call callback at the very
					 * end of this member function, since callback
					 * may delete this */
					S.on_recv_bind_error(this);
					return;
				}
				if (in.msg->id != command::bind_receiver
						&& in.msg->id != command::bind_transmitter
						&& in.msg->id != command::bind_transceiver) {
					lerror(S.L) << "channel::on_recv_bind_header:"
						"wrong bind id: "
						<< in.msg->id;
					S.P.destroy_message(in.msg);
					/* !!! It's crucial to call callback at the very
					 * end of this member function, since callback
					 * may delete this */
					S.on_recv_bind_error(this);
					return;
				}
				/* Previously we have created msg of len = sizeof(pdu)
				 * only to recv header in it. Now we've found out that
				 * message has body. Copy header, destroy message
				 * and create new one with sufficient len */
				pdu * msg = S.P.create_message(in.msg->len);
				std::memcpy(msg, in.msg, sizeof(pdu));
				S.P.destroy_message(in.msg);
				in.msg = msg;
				recv_bind_body();
			} else {
				lerror(S.L) << "channel::on_recv_bind_header: "
					<< ec.message();
				in.ready = true;
				S.P.destroy_message(in.msg);
				/* !!! It's crucial to call callback at the very end of this
				 * member function, since callback may delete this */
				S.on_recv_bind_error(this);
			}
		}

		void recv_bind_body()
		{
			using namespace utl;
			ltrace(S.L) << "channel::recv_bind_body: bytes: "
				<< in.msg->len - sizeof(pdu);
			/* Read the remaining body of a messsage, beyond msg header */
			m_sock.async_receive(
				ba::buffer(asbuf(in.msg) + sizeof(pdu)
					, in.msg->len-sizeof(pdu))
				, bind(&channel::on_in_bytes, this
					, &channel::on_recv_bind_body
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_recv_bind_body(const bs::error_code & ec)
		{
			in.ready = true;
			if (!ec) {
				ltrace(S.L) << "channel::on_recv_bind_body: ok";
				/* Let the service process received message */
				S.on_recv_bind(this, in.msg);
			} else {
				lerror(S.L) << "channel::on_recv_bind_body: " << ec.message();
				S.P.destroy_message(in.msg);
				/* !!! It's crucial to call callback at the very end of this
				 * member function, since callback may delete this */
				S.on_recv_bind_error(this);
			}
		}

		void on_send_bind_r(const bs::error_code & ec)
		{
			out.ready = true;
			if (!ec) {
				ltrace(S.L) << "channel::on_send_bind_r: ok";
				S.on_send_bind_r(this, out.msg);
				if (out.que.empty()) {
					return;
				}
				pdu * msg = out.que.front();
				out.que.pop();
				send_message(msg, &channel::on_send_message);
			} else {
				/* Send cycle stop here. Call to flush is needed
				 * in order to let messages in queue to be sent */
				lerror(S.L) << "channel::on_send_bind_r: " << ec.message();
				/* !!! It's crucial to call callback at the very end of this
				 * member function, since callback may delete this */
				S.on_send_error(this, in.msg);
			}
		}

};

}

#endif
