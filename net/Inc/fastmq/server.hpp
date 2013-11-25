#ifndef fastmq_server_base_hpp
#define fastmq_server_base_hpp

#include <stack>
#include <vector>
#include <functional>

#include <fastmq/peer.hpp>
#include <fastmq/node.hpp>
#include <fastmq/pool.hpp>

namespace fastmq {

namespace ba = boost::asio;
namespace bs = boost::system;

template <class ProtoT, class LogT>
class server_base: node_base {

	protected:

	typedef LogT log_t;
	typedef ProtoT proto_t;
	typedef ba::basic_stream_socket<proto_t> sock_t;
	typedef peer<server_base<proto_t, log_t>> peer_t;
	typedef ba::basic_socket_acceptor<proto_t> acpt_t;

	friend peer_t;

	public:
		typedef typename proto_t::endpoint endpoint_t;

		server_base(ba::io_service & io
				, message_pool_base & pool
				, const endpoint_t & ep
				, log_t & l)
			: m_sock(io), m_acpt(io, ep)
			, L(l), P(pool)
		{
				m_book.reserve(30);
		}

		virtual ~server_base() {
				for (peer_t * p: m_book) {
					/* No need to call unregister_terminal here
					 * since node_base implementation is already
					 * destructed by this moment, and actually
					 * did all cleanup itself.
					 *
					 * Morover since it's a virtual member function
					 * it is impossible to call it here. */
					if (p)
						destroy(p);
				}
		}

		void listen() {
			m_acpt.async_accept(m_sock, boost::bind(&server_base::on_accept
				, this, ba::placeholders::error));
		}

	protected:

		sock_t m_sock;
		acpt_t m_acpt;
		LogT & L;
		message_pool_base & P;

		void on_accept(const bs::error_code & ec) {
			if (!ec) {
				linfo(L) << "Incoming connection";
				peer_t * p = create(m_sock, *this);
				p->recv_identity();
			} else {
				lerror(L) << ec.message();
			}
		}

		void on_recv_error(terminal_base * p) {
			/* Default implementation:
			 * unregister and destroy */
			unregister_terminal(p);
			destroy(static_cast<peer_t *>(p));
		}

		void on_send_error(terminal_base * p, msgu * msg) {
			(void)(msg);
			/* Default implementation:
			 * unregister and destroy */
			unregister_terminal(p);
			destroy(static_cast<peer_t *>(p));
		}

		void on_recv_identity(peer_t * p) {
			if(register_terminal(p)) {
				p->send_greeting();
			} else {
				destroy(p);
			}
		}

		void on_send_greeting(peer_t * p) {
			p->recv();
		}

		void on_recv_identity_error(peer_t * p) {
			/* TODO: notify implementation */
			destroy(p);
		}

		void on_send_greeting_error(peer_t * p) {
			/* TODO: notify implementation */
			unregister_terminal(p);
			destroy(p);
		}

		/* Used only in client_base */
		void on_recv_greeting(peer_t * p) {
			(void)(p);
			lerror(L) << "Call to on_recv_greeting in server";
		}
		void on_send_identity(peer_t * p) {
			(void)(p);
			lerror(L) << "Call to on_send_identity in server";
		}
		void on_send_identity_error(peer_t * p) {
			(void)(p);
			lerror(L) << "Call to on_send_identity_error in server";
		}
		void on_recv_greeting_error(peer_t * p) {
			(void)(p);
			lerror(L) << "Call to on_recv_greeting_error in server";
		}

		void on_connect_error() {
			lerror(L) << "Call to on_connect_error in server";
		}

	private:
		std::stack<std::size_t> m_hole;
		std::vector<peer_t *> m_book;

		template<typename ... Args>
		peer_t * create(Args & ... args) {
			peer_t * p;
			std::size_t idx;
			if (!m_hole.empty()) {
				idx = m_hole.top();
				p = new peer_t(idx, args ...);
				m_hole.pop();
				m_book[idx] = p;
			} else {
				idx = m_book.size();
				p = new peer_t(idx, args ...);
				m_book.push_back(p);
			}
			return p;
		}

		void destroy(peer_t * p) {
			m_book.at(p->index()) = nullptr;
			m_hole.push(p->index());
			delete p;
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
