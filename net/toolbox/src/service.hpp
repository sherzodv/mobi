#ifndef mobi_net_toolbox_service_hpp
#define mobi_net_toolbox_service_hpp

#include <stack>
#include <vector>
#include <thread>
#include <functional>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <toolbox/bin.hpp>
#include <toolbox/toolbox.hpp>
#include <toolbox/channel.hpp>

namespace mobi { namespace net { namespace toolbox {

namespace ba = boost::asio;
namespace bs = boost::system;

template <class ProtoT, class AllocatorT, class LogT, typename HdrT>
class service {

	protected:

	typedef service<ProtoT, AllocatorT, LogT, HdrT>	service_t;
	typedef LogT								log_t;
	typedef ProtoT								proto_t;
	typedef AllocatorT							allocator_t;
	typedef ba::basic_stream_socket<proto_t>	sock_t;
	typedef ba::basic_socket_acceptor<proto_t>	acpt_t;
	typedef channel<service_t, HdrT>			channel_t;

	friend channel_t;

	log_t L;

	public:
		typedef typename proto_t::endpoint endpoint_t;

		service(const endpoint_t & ep, allocator_t & a, log_t l)
			: L(std::move(l))
			, m_io()
			, m_sock(m_io)
			, m_acpt(m_io, ep)
			, A(a)
		{
			m_channel_count = 0;
		}

		virtual ~service() {
			if (m_pm_thread.joinable()) {
				m_pm_thread.join();
			}
			if (m_io_thread.joinable()) {
				m_io_thread.join();
			}
		}

		void start() {
			m_pm_thread = std::thread([this] {
				process_messages();
			});
			m_io_thread = std::thread([this] {
				m_acpt.async_accept(m_sock
					, boost::bind(&service::on_accept
						, this, ba::placeholders::error));
				m_io.run();
			});
		}

		void stop() {
			ltrace(L) << "stopping service";
			{
				std::lock_guard<std::mutex> lock(in.mtx);
				in.que.push(inmsg(inmsg::stop));
				in.cond.notify_one();
			}
			if (m_pm_thread.joinable()) {
				m_pm_thread.join();
			}
			if (m_io_thread.joinable()) {
				m_io_thread.join();
			}
		}

	protected:
		virtual void on_send(bin::sz_t channel_id, bin::sz_t msg_id) = 0;
		virtual void on_send_error(bin::sz_t channel_id, bin::sz_t msg_id) = 0;
		virtual void on_recv(bin::sz_t channel_id, bin::buffer buf) = 0;
		virtual void on_recv_error(bin::sz_t channel_id) = 0;

		void close(bin::sz_t channel_id) {
			channel_t * ch = get_channel(channel_id);
			if (ch == nullptr) {
				lerror(L) << "service::close: wrong channel id: " << channel_id;
			} else {
				ch->close();
			}
		}

		bin::sz_t send(bin::sz_t channel_id, bin::buffer buf) {
			channel_t * ch = get_channel(channel_id);
			if (ch == nullptr) {
				lerror(L) << "service::send: wrong channel id: " << channel_id;
				A.dealloc(buf.data);
				return 0;
			}
			return ch->send(buf);
		}

	protected:
		ba::io_service m_io;
		sock_t m_sock;
		acpt_t m_acpt;
		allocator_t & A;

		std::stack<bin::sz_t> m_hole;
		std::vector<channel_t *> m_book;

		bin::sz_t m_channel_count;

		std::thread m_io_thread;
		std::thread m_pm_thread;

		struct inmsg {
			enum type_t { unknown, recv, recv_error, send, send_error, destroy, stop } type;
			bin::sz_t ch_id;
			bin::sz_t msg_id;
			bin::buffer buf;
			inmsg()
				: type(unknown), ch_id(0), msg_id(0), buf(){}

			inmsg(type_t t)
				: type(t), ch_id(0), msg_id(0), buf() {}

			inmsg(type_t t, bin::sz_t chid)
				: type(t), ch_id(chid), msg_id(0), buf() {}

			inmsg(type_t t, bin::sz_t chid, bin::buffer b)
				: type(t), ch_id(chid), msg_id(0), buf(b) {}

			inmsg(type_t t, bin::sz_t chid, bin::sz_t mid)
				: type(t), ch_id(chid), msg_id(mid), buf() {}

			inmsg(type_t t, bin::sz_t chid, bin::sz_t mid, bin::buffer b)
				: type(t), ch_id(chid), msg_id(mid), buf(b) {}
		};

		struct inque {
			std::mutex mtx;
			std::queue<inmsg> que;
			std::condition_variable cond;
		} in;

		void on_accept(const bs::error_code & ec) {
			if (!ec) {
				channel_t * ch = create(m_sock, *this);
				ch->recv();
				m_acpt.async_accept(m_sock
					, boost::bind(&service::on_accept
						, this, ba::placeholders::error));
			} else {
				lerror(L) << ec.message();
			}
		}

		void cancel_all() {
			for (channel_t * ch: m_book) {
				if (ch != nullptr) {
					ch->close();
				}
			}
			m_io.post([this] {
				m_acpt.close();
			});
		}

		void process_messages() {
			bool stop = false;
			inmsg msg;
			while (true) {
				try {
					std::unique_lock<std::mutex> lock(in.mtx);
					in.cond.wait(lock, [this, stop] { return !in.que.empty() || (!m_channel_count && stop); });
					if (in.que.empty() && !m_channel_count && stop) {
						ltrace(L) << "service::process_messages: end of processing loop";
						return;
					}
					msg = in.que.front();
					in.que.pop();
				} catch (...) {
					lcritical(L) << "service::process_messages: exception";
					return;
				}
				switch (msg.type) {
					case inmsg::recv:
						on_recv(msg.ch_id, msg.buf);
						A.dealloc(msg.buf.data);
						break;
					case inmsg::recv_error:
						on_recv_error(msg.ch_id);
						break;
					case inmsg::send:
						on_send(msg.ch_id, msg.msg_id);
						break;
					case inmsg::send_error:
						on_send_error(msg.ch_id, msg.msg_id);
						break;
					case inmsg::destroy: {
						channel_t * ch = get_channel(msg.ch_id);
						if (ch != nullptr) {
							destroy(ch);
						} else {
							lerror(L)
								<< "service::process_messages"
								<< " on_close with wrong channel id";
						}
						break;
					}
					case inmsg::stop: {
						cancel_all();
						stop = true;
						break;
					}
					case inmsg::unknown:
					default:
						lerror(L)
							<< "service::process_messages: wrong message type";
						break;
				}
			}
		}

		void on_recv(channel_t * ch, bin::buffer buf) {
			/* called from io thread */
			std::lock_guard<std::mutex> lock(in.mtx);
			in.que.push(inmsg(inmsg::recv, ch->id(), buf));
			in.cond.notify_one();
			ltrace(L) << "channel #" << ch->id()
				<< " msg in: " << buf.len << " bytes";
		}

		void on_recv_error(channel_t * ch) {
			std::lock_guard<std::mutex> lock(in.mtx);
			in.que.push(inmsg(inmsg::recv_error, ch->id()));
			in.cond.notify_one();
			lerror(L) << "channel #" << ch->id() << " recv error";
		}

		void on_send(channel_t * ch, bin::sz_t msg_id, bin::buffer buf) {
			std::lock_guard<std::mutex> lock(in.mtx);
			A.dealloc(buf.data);
			in.que.push(inmsg(inmsg::send, ch->id(), msg_id));
			in.cond.notify_one();
			ltrace(L) << "channel #" << ch->id()
				<< " msg out #" << msg_id << ": " << buf.len << " bytes";
		}

		void on_send_error(channel_t * ch, bin::sz_t msg_id, bin::buffer buf) {
			std::lock_guard<std::mutex> lock(in.mtx);
			A.dealloc(buf.data);
			in.que.push(inmsg(inmsg::send_error, ch->id(), msg_id));
			in.cond.notify_one();
			ltrace(L) << "channel #" << ch->id()
				<< " error msg out # " << msg_id << ": " << buf.len << " bytes";
		}

		void on_close(channel_t * ch) {
			std::lock_guard<std::mutex> lock(in.mtx);
			in.que.push(inmsg(inmsg::destroy, ch->id()));
			in.cond.notify_one();
			ltrace(L) << "service::on_close: " << ch->id();
		}

		template<typename ... Args>
		channel_t * create(Args & ... args) {
			channel_t * ch;
			std::size_t idx;
			if (!m_hole.empty()) {
				idx = m_hole.top();
				ch = new channel_t(idx, args ...);
				m_hole.pop();
				m_book[idx] = ch;
			} else {
			idx = m_book.size();
				ch = new channel_t(idx, args ...);
				m_book.push_back(ch);
			}
			m_channel_count++;
			return ch;
		}

		void destroy(channel_t * ch) {
			m_book.at(ch->id()) = nullptr;
			m_hole.push(ch->id());
			m_channel_count--;
			delete ch;
		}

		channel_t * get_channel(bin::sz_t id) {
			if (id >= m_book.size()) {
				return nullptr;
			}
			return m_book[id];
		}
};

} } }

#endif
