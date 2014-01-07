#ifndef smpp_channel_hpp
#define smpp_channel_hpp

#include <queue>
#include <vector>
#include <stdexcept>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <smpp/proto.hpp>
#include <toolbox/bin.hpp>

namespace mobi { namespace net { namespace smpp {

using boost::bind;
namespace ba = boost::asio;
namespace bs = boost::system;

template <class ServiceT>
class channel {

	typedef ServiceT service_t;
	typedef typename ServiceT::sock_t sock_t;

	public:
		channel(bin::sz_t index, sock_t & sock, ServiceT & service)
			: S(service)
			, m_idx(index)
			, m_sock(std::move(sock))
		{
			in.ready = true;
			in.total_bytes = 0;

			out.ready = true;
			out.total_bytes = 0;

			ltrace(S.L) << "channel created: idx: " << m_idx;
		}

		virtual ~channel() {
			ltrace(S.L) << "channel destroyed: idx: " << m_idx;
		}

		bin::sz_t id() const {
			return m_idx;
		}

		bool ready_to_send() const {
			return out.ready;
		}

		bool ready_to_recv() const {
			return in.ready;
		}

		void flush() {
			if (!out.ready || out.que.empty())
				return;
			bin::buffer buf = out.que.front();
			out.que.pop();
			send(buf);
		}

		virtual void send(bin::buffer buf) {
			if (out.ready) {
				using namespace bin;
				/* Caller is responsible for destroying message. */
				out.ready = false;
				out.buf = buf;
				ltrace(S.L) << "channel#" << m_idx << "::send: bytes: " << buf.len;
				ba::async_write(m_sock
					, ba::buffer(out.buf.data, out.buf.len)
					, bind(&channel::on_out_bytes, this
						, &channel::on_send
						, ba::placeholders::error
						, ba::placeholders::bytes_transferred));
			} else {
				out.que.push(buf);
			}
		}

		virtual void recv() {
			recv_len();
		}

	private:
		typedef void (channel::* cb_t)(const bs::error_code &);

		service_t & S;
		bin::sz_t m_idx;
		sock_t m_sock;

		struct inbuf {
			bool ready;
			bin::buffer buf;
			bin::u32_t msglen;
			bin::sz_t total_bytes;
		} in;

		struct outbuf {
			bool ready;
			bin::buffer buf;
			std::queue<bin::buffer> que;
			bin::sz_t total_bytes;
		} out;

		void on_in_bytes(cb_t cb, const bs::error_code & ec, bin::sz_t bytes) {
			in.total_bytes += bytes;
			ltrace(S.L) << "channel#" << m_idx << "::on_in_bytes:" << bytes;
			/* !!! It's crucial to call callback at the very end of this
			 * member function, since callback may delete this */
			(this->*cb)(ec);
		}

		void on_out_bytes(cb_t cb, const bs::error_code & ec, bin::sz_t bytes) {
			out.total_bytes += bytes;
			ltrace(S.L) << "channel #" << m_idx << "on_out_bytes:" << bytes;
			/* !!! It's crucial to call callback at the very end of this
			 * member function, since callback may delete this */
			(this->*cb)(ec);
		}

		void recv_len() {
			using namespace bin;
			if (!in.ready) {
				lerror(S.L) << "channel::recv_header: in buffer is busy";
				return;
			}
			in.ready = false;
			m_sock.async_receive(ba::buffer(asbuf(in.msglen)
				, sizeof(in.msglen))
				, bind(&channel::on_in_bytes, this, &channel::on_recv_len
					, ba::placeholders::error
					, ba::placeholders::bytes_transferred));
		}

		void on_recv_len(const bs::error_code & ec) {
			if (!ec) {
				in.msglen = bin::bo::to_host(in.msglen);
				ltrace(S.L) << "channel::on_recv_len: len=" << in.msglen;
				if (in.msglen < sizeof(pdu)) {
					lerror(S.L) << "channel::on_recv_len: wrong len: "
						<< in.msglen;
					/* !!! callback may delete this channel */
					S.on_recv_error(this);
					return;
				} else {
					in.buf.len = in.msglen;
					in.buf.data = static_cast<bin::u8_t *>(S.A.alloc(in.msglen));
					/* Parser expects network byte order in buffer */
					bin::w::cp_u32(in.buf.data, bin::ascbuf(in.msglen));
					recv_body();
				}
			} else {
				lerror(S.L) << "channel::on_recv_len: " << ec.message();
				in.ready = true;
				/* !!! callback may delete this channel */
				S.on_recv_error(this);
				return;
			}
		}

		void recv_body() {
			/* Read the remaining body of a messsage, beyond msg len */
			m_sock.async_receive(
				ba::buffer(bin::asbuf(in.buf.data) + sizeof(in.msglen)
					, in.msglen - sizeof(in.msglen))
					, bind(&channel::on_in_bytes, this, &channel::on_recv_body
						, ba::placeholders::error
						, ba::placeholders::bytes_transferred));
		}

		void on_recv_body(const bs::error_code & ec) {
			in.ready = true;
			if (!ec) {
				ltrace(S.L) << "channel::on_recv_body: ok";
				/* Let the service process the received message */
				S.on_recv(this, in.buf);
			} else {
				lerror(S.L) << "channel::on_recv_body: " << ec.message();
				S.A.dealloc(in.buf.data);
				/* !!! callback may delete this channel */
				S.on_recv_error(this);
			}
		}

		void on_send(const bs::error_code & ec) {
			out.ready = true;
			if (!ec) {
				ltrace(S.L) << "channel::on_send: ok";
				S.on_send(this, out.buf);
				flush();
			} else {
				/* Send cycle stop here. Call to flush is needed
				 * in order to let messages in queue to be sent */
				lerror(S.L) << "channel::on_send: " << ec.message();
				/* !!! It's crucial to call callback at the very end of this
				 * member function, since callback may delete this */
				S.on_send_error(this, out.buf);
			}
		}
};

} } }

#endif
