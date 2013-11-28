#ifndef smpp_server_base_hpp
#define smpp_server_base_hpp

#include <stack>
#include <vector>
#include <functional>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <smpp/pool.hpp>
#include <smpp/proto.hpp>
#include <smpp/channel.hpp>

namespace smpp {

namespace ba = boost::asio;
namespace bs = boost::system;

template <class ProtoT, class LogT>
class server_base {

	protected:

	typedef LogT log_t;
	typedef ProtoT proto_t;
	typedef ba::basic_stream_socket<proto_t> sock_t;
	typedef ba::basic_socket_acceptor<proto_t> acpt_t;
	typedef channel<server_base<proto_t, log_t>> channel_t;

	friend channel_t;

	public:
		typedef typename proto_t::endpoint endpoint_t;

		server_base(ba::io_service & io
				, message_pool_base & pool
				, const endpoint_t & ep
				, log_t l)
			: L(std::move(l)), P(pool)
			, m_sock(io), m_acpt(io, ep)
		{
				m_book.reserve(30);
		}

		virtual ~server_base() {
				for (channel_t * ch: m_book) {
					/* No need to call unregister_channel here
					 * since implementation is already
					 * destructed by this moment, and actually
					 * did all cleanup itself.
					 *
					 * Morover since it'ch a virtual member function
					 * it is impossible to call it here. */
					if (ch)
						destroy(ch);
				}
		}

		void listen() {
			ltrace(L) << "server_base::listen: Listening...";
			m_acpt.async_accept(m_sock, boost::bind(&server_base::on_accept
				, this, ba::placeholders::error));
		}

	protected:

		log_t L;
		message_pool_base & P;

		virtual bool open(session * s) = 0;
		virtual void close(session * s) = 0;

		void on_accept(const bs::error_code & ec) {
			if (!ec) {
				linfo(L) << "server_base::on_accept: New incoming connection";
				channel_t * ch = create(m_sock, *this);
				ch->recv_bind();
				listen();
			} else {
				lerror(L) << ec.message();
			}
		}

		void on_recv(channel_t * ch, pdu * msg) {
			(void)(ch);
			(void)(msg);
			ltrace(L) << "server_base::on_recv: new pdu: "
				<< " len: " << msg->len;
		}

		void on_send(channel_t * ch, pdu * msg) {
			(void)(ch);
			(void)(msg);
		}

		virtual void on_recv_error(session * ch) {
			/* Default implementation:
			 * unregister and destroy */
			destroy(dynamic_cast<channel_t *>(ch));
		}

		virtual void on_send_error(session * ch, pdu * msg) {
			(void)(msg);
			/* Default implementation:
			 * unregister and destroy */
			destroy(dynamic_cast<channel_t *>(ch));
		}

		void on_recv_bind(channel_t * ch, pdu * msg) {
			using namespace utl;
			switch (msg->id) {
				case command::bind_receiver: {
					bind_receiver r;
					parse_bind_receiver(r, w::asbuf(msg), L);
					ch->bind_pdu(r);
					break;
				}
				case command::bind_transmitter: {
					bind_transmitter r;
					parse_bind_transmitter(r, w::asbuf(msg), L);
					ch->bind_pdu(r);
					break;
				}
				case command::bind_transceiver: {
					bind_transceiver r;
					parse_bind_transceiver(r, w::asbuf(msg), L);
					ch->bind_pdu(r);
					break;
				}
				default: {
					lerror(L) << "server_base::on_recv_bind:"
						"wrong bind id: " << msg->id;
					P.destroy_message(msg);
					destroy(ch);
					return;
				}
			}

			if (open(ch)) {
				ch->recv();
			} else {
				/* TODO: send error bind_response_r */
				destroy(ch);
			}
		}

		void on_recv_bind_error(channel_t * ch) {
			(void)(ch);
		}

	private:
		sock_t m_sock;
		acpt_t m_acpt;

		std::stack<std::size_t> m_hole;
		std::vector<channel_t *> m_book;

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
			return ch;
		}

		void destroy(channel_t * ch) {
			m_book.at(ch->index()) = nullptr;
			m_hole.push(ch->index());
			delete ch;
		}
};

template <class LogT>
using unix_domain_server_base
	= server_base<boost::asio::local::stream_protocol, LogT>;

template <class LogT>
using tcp_server_base
	= server_base<boost::asio::ip::tcp, LogT>;

}

#endif
