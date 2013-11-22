#ifndef fastmq_server_base_hpp
#define fastmq_server_base_hpp

#include <stack>
#include <vector>
#include <functional>

#include "peer.hpp"

namespace fastmq {

using boost::bind;
namespace ba = boost::asio;
namespace bs = boost::system;

template <class ProtoT, class LoggerT>
class server_base {

	typedef peer<server_base<ProtoT, LoggerT>> peer_t;
	typedef ba::basic_socket_acceptor<ProtoT> acpt_t;

	friend peer_t;

	public:
		typedef ProtoT proto_t;
		typedef LoggerT	log_t;
		typedef ba::basic_stream_socket<ProtoT> sock_t;

		server_base(ba::io_service & io
				, const typename proto_t::endpoint & endpoint
				, LoggerT & logger)
			: m_sock(io), m_acpt(io, endpoint)
			, L(logger)
		{
		}

		virtual ~server_base() {
		}

		void start() {
			listen();
		}

	protected:

		sock_t m_sock;
		acpt_t m_acpt;
		LoggerT & L;
		router_base & R;

		std::vector<peer_t *> m_book;
		std::stack<std::size_t> m_hole;

		void listen() {
			m_acpt.async_accept(m_sock, bind(&server_base::on_accept
				, this, ba::placeholders::error));
		}

		void on_accept(const bs::error_code & ec) {
			if (!ec) {
				peer_t * p = create(m_sock, *this);
				p->start();
			} else {
				lerror(L) << ec.message();
			}
		}

		template<typename ... Args>
		peer_t * create(Args & ... args) {
			std::size_t idx = 0;
			peer_t * p = new peer_t(idx, args ...);
			if (!m_hole.empty()) {
				idx = m_hole.top();
				m_hole.pop();
				m_book[idx] = p;
			} else {
				idx = m_book.size();
				m_book.push_back(p);
			}
			return p;
		}

		void destroy(peer_t * p) {
			m_book.at(p->m_idx) = nullptr;
			m_hole.push(p->m_idx);
			delete p;
		}
};

template <class LoggerT>
using unix_domain_server_base
	= server_base<boost::asio::local::stream_protocol, LoggerT>;

template <class LoggerT>
using tcp_server_base
	= server_base<boost::asio::ip::tcp, LoggerT>;

}

#endif
