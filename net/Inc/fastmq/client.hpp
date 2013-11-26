#ifndef fastmq_client_base_hpp
#define fastmq_client_base_hpp

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
class client_base: node_base {

	typedef LogT log_t;
	typedef ProtoT proto_t;
	typedef ba::basic_stream_socket<ProtoT> sock_t;
	typedef peer<client_base<proto_t, log_t>> peer_t;

	friend peer_t;

	public:
		client_base(ba::io_service & io
				, message_pool_base & pool, log_t l)
			: m_tmp_id(0), m_tmp_type(0), m_sock(io), ready(true)
			, L(std::move(l)), P(pool)
		{
		}

		virtual ~client_base() {
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

		template <typename EndpointT>
		void connect(const EndpointT & ep, u16_t id, u16_t type) {
			if (!ready) {
				lerror(L) << "client::base::connect: buffer is busy";
				return;
			}
			ltrace(L) << "client::base::connect: connecting";
			m_tmp_id = id;
			m_tmp_type = type;
			ready = false;
			m_sock.async_connect(ep
				, bind(&client_base::on_connect
					, this, ba::placeholders::error));
		}

	protected:

		u16_t m_tmp_id;
		u16_t m_tmp_type;
		sock_t m_sock;
		bool ready;
		log_t L;
		message_pool_base & P;

		void on_connect(const bs::error_code & ec) {
			ready = true;
			if (!ec) {
				linfo(L) << "client_base::on_connect: New outgoing connection";
				peer_t * p = create(m_tmp_id, m_tmp_type, m_sock, *this);
				p->send_identity();
			} else {
				lerror(L) << ec.message();
				on_connect_error();
			}
		}

		void on_recv_error(terminal_base * p) {
			unregister_terminal(p);
			destroy(static_cast<peer_t *>(p));
		}

		void on_send_error(terminal_base * p, msgu * msg) {
			(void)(msg);
			unregister_terminal(p);
			destroy(static_cast<peer_t *>(p));
		}

		void on_send_identity(peer_t * p) {
			p->recv_greeting();
		}

		void on_recv_greeting(peer_t * p) {
			if(register_terminal(p)) {
				ltrace(L)
					<< "client_base::on_recv_identity: terminal registered: "
					<< "(" << p->id() << ":" << p->type() << ")";
				p->produce_message();
			} else {
				lerror(L)
					<< "client_base::on_recv_identity: terminal rejected: "
					<< "(" << p->id() << ":" << p->type() << ")";
				destroy(p);
			}
		}

		void on_send_identity_error(peer_t * p) {
			/* TODO: notify implementation */
			destroy(p);
		}

		void on_recv_greeting_error(peer_t * p) {
			/* TODO: notify implementation */
			destroy(p);
		}

		/* Used only in server_base */
		void on_recv_identity(peer_t * p) {
			(void)(p);
			lerror(L) << "Call to on_recv_identity in client";
		}
		void on_send_greeting(peer_t * p) {
			(void)(p);
			lerror(L) << "Call to on_send_greeting in client";
		}
		void on_recv_dentity_error(peer_t * p) {
			(void)(p);
			lerror(L) << "Call to on_recv_identity_error in client";
		}
		void on_send_greeting_error(peer_t * p) {
			(void)(p);
			lerror(L) << "Call to on_send_greeting_error in client";
		}

	private:
		std::stack<std::size_t> m_hole;
		std::vector<peer_t *> m_book;

		template<typename ... Args>
		peer_t * create(Args & ... args) {
			std::size_t idx = 0;
			peer_t * p;
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
using unix_domain_client_base
	= client_base<boost::asio::local::stream_protocol, LogT>;

template <class LogT>
using tcp_client_base
	= client_base<boost::asio::ip::tcp, LogT>;

}

#endif
