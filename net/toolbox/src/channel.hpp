#ifndef mobi_net_toolbox_channel_hpp
#define mobi_net_toolbox_channel_hpp

#include <stdexcept>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <toolbox/bin.hpp>
#include <toolbox/queue.hpp>
#include <vision/log.hpp>

namespace mobi { namespace net { namespace toolbox {

using boost::bind;
namespace ba = boost::asio;
namespace bs = boost::system;

template <class ServiceT, typename LenT>
class channel {

	typedef ServiceT service_t;
	typedef typename ServiceT::sock_t sock_t;

	public:
		channel(const channel &) = delete;
		channel(const channel &&) = delete;
		channel & operator=(const channel &) = delete;

		channel(bin::sz_t id, sock_t & sock, ServiceT & service)
			: S(service)
			, m_id(id)
			, m_sock(std::move(sock))
		{
			in.ready = true;
			in.total_bytes = 0;
			out.ready = true;
			out.total_bytes = 0;
			out.last_seqno = 0;
			ltrace(S.L) << "channel #" << m_id << " created";
		}

		~channel() {
			ltrace(S.L) << "channel #" << m_id << " destroyed";
		}

		void close() {
			m_sock.get_io_service().post([this] {
				ltrace(S.L) << "closing channel #" << m_id;
				m_sock.close();
				ltrace(S.L) << "canceling pending send messages for channel #" << m_id;
				cancel_all();
				/* on_close will delete this */
				S.on_close(this);
			});
		}

		bin::sz_t id() const {
			return m_id;
		}

		bin::sz_t send(bin::buffer buf) {
			using namespace bin;
			std::lock_guard<std::mutex> lock(out.mtx);
			out.last_seqno++;
			if (out.ready) {
				out.msg = outmsg(out.last_seqno, buf);
				m_sock.get_io_service().post([this] () {
					ba::async_write(m_sock
						, ba::buffer(out.msg.buf.data, out.msg.buf.len)
						, bind(&channel::on_out_bytes, this
							, &channel::on_send
							, ba::placeholders::error
							, ba::placeholders::bytes_transferred));
				});
			} else {
				out.que.push(outmsg(out.last_seqno, buf));
			}
			return out.last_seqno;
		}

		void recv() {
			m_sock.get_io_service().post([this] () {
				recv_len();
			});
		}

		void flush() {
			using namespace bin;
			std::lock_guard<std::mutex> lock(out.mtx);
			if (out.que.empty() || !out.ready) {
				return;
			}
			out.msg = out.que.front();
			out.que.pop();
			m_sock.get_io_service().post([this] () {
				ba::async_write(m_sock
					, ba::buffer(out.msg.buf.data, out.msg.buf.len)
					, bind(&channel::on_out_bytes, this
						, &channel::on_send
						, ba::placeholders::error
						, ba::placeholders::bytes_transferred));
			});
		}

	private:
		typedef void (channel::* cb_t)(const bs::error_code &);

		/* User class instance holding callbacks */
		service_t & S;
		/* User assigned value for bookeeping channels */
		bin::sz_t m_id;
		/* Socket to perform operations on */
		sock_t m_sock;

		struct outmsg {
			bin::sz_t seqno;
			bin::buffer buf;
			outmsg(): seqno(0), buf() {}
			outmsg(bin::sz_t n, bin::buffer b): seqno(n), buf(b) {}
		};

		struct inbuf {
			bool ready;
			bin::buffer buf;
			LenT msglen;
			/* Total bytes received */
			bin::sz_t total_bytes;
		} in;

		struct outbuf {
			/* Indicates if any outgoing operation is active at the moment */
			bool ready;
			/* Current message being sent */
			outmsg msg;
			/* Sequence numbering of outgoing messages
			 * Each outgoing message is tracked by it's outgoing seqno */
			bin::sz_t last_seqno;
			bin::sz_t total_bytes;
			std::mutex mtx;
			std::queue<outmsg> que;
		} out;

		void cancel_all() {
			/* io thread */
			using namespace bin;
			std::lock_guard<std::mutex> lock(out.mtx);
			while (!out.que.empty()) {
				out.msg = out.que.front();
				out.que.pop();
				S.on_send_error(this, out.msg.seqno, out.msg.buf);
			}
		}

		void on_in_bytes(cb_t cb, const bs::error_code & ec, bin::sz_t bytes) {
			in.total_bytes += bytes;
			ltrace(S.L) << "channel #" << m_id
				<< " in: " << bytes << " bytes";
			/* !!! It's crucial to call callback at the very end of this
			 * member function, since callback may delete this */
			(this->*cb)(ec);
		}

		void on_out_bytes(cb_t cb, const bs::error_code & ec, bin::sz_t bytes) {
			out.total_bytes += bytes;
			ltrace(S.L) << "channel #" << m_id
				<< " out: " << bytes << " bytes";
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
				in.buf.len = in.msglen;
				in.buf.data = static_cast<bin::u8_t *>(S.A.alloc(in.msglen));
				/* Parser expects network byte order in buffer */
				bin::w::cp_u32(in.buf.data, bin::ascbuf(in.msglen));
				recv_body();
			} else {
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
				/* Let the service process the received message */
				S.on_recv(this, in.buf);
				recv_len();
			} else {
				S.A.dealloc(in.buf.data);
				/* !!! callback may delete this channel */
				S.on_recv_error(this);
			}
		}

		void on_send(const bs::error_code & ec) {
			outmsg msg;
			{
				std::lock_guard<std::mutex> lock(out.mtx);
				out.ready = true;
				msg = out.msg;
				/* One should release lock before any call to callbacks
				 * as they may call send of flush leading to deadlock */
			}
			if (!ec) {
				S.on_send(this, msg.seqno, msg.buf);
				flush();
			} else {
				/* Send cycle stop here. Call to flush is needed
				 * in order to let messages in queue to be sent */
				lerror(S.L) << "channel::on_send: " << ec.message();
				/* !!! It's crucial to call callback at the very end of this
				 * member function, since callback may delete this */
				S.on_send_error(this, msg.seqno, msg.buf);
			}
		}
};

} } }

#endif
