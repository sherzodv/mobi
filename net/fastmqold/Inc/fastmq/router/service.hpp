#ifndef fastmq_server_service_hpp
#define fastmq_server_service_hpp

#include <vector>
#include <functional>

#include <fastmq/proto/proto.hpp>
#include <fastmq/logger/logger.hpp>

#include "book.hpp"
#include "peer.hpp"

namespace fastmq { namespace router {

using boost::bind;
using namespace ba;
using namespace proto;

template <class ProtoT, class LoggerT, class MessagePoolT, class ImplT>
class service: public book<peer<ImplT>> {

	typedef book<peer<ImplT>> book_base;
	typedef ba::basic_socket_acceptor<ProtoT> acpt_t;

	protected:
		typedef peer<ImplT> peer_t;

	public:

		typedef LoggerT	log_t;
		typedef ProtoT proto_t;
		typedef MessagePoolT message_pool_t;
		typedef ba::basic_stream_socket<ProtoT> sock_t;

		template <class EndpointT>
		service(ba::io_service & io
				, const EndpointT & endpoint
				, LoggerT & logger)
			: m_sock(io), m_acpt(io, endpoint)
			, L(logger)
		{
			m_this = static_cast<ImplT *>(this);
		}

		~service() {
		}

		void start() {
			listen();
		}

		log_t & get_logger() { return L; }
		message_pool_t & get_message_pool() { return m_pool; }

		void log_io_stats() {
			book_base::for_each_call(&peer_t::log_io_stats);
		}

		/*
		 * Implement these member functions:
		 *
		 * static_virtual std::uint16_t register_peer(std::uint16_t id
		 * 		, std::uint16_t type);
		 *
		 * static_virtual void on_message(peer_t *p, msgu * msg) = 0;
		 *
		 */

	protected:

		sock_t m_sock;
		acpt_t m_acpt;
		log_t & L;
		ImplT * m_this;
		message_pool_t m_pool;

		void listen() {
			m_acpt.async_accept(m_sock, bind(&service::on_accept
				, this, ba::placeholders::error));
		}

		void on_accept(const bs::error_code & ec) {
			if (!ec) {
				peer_t * p = this->create(*m_this, m_sock);
				p->start();
			} else {
				lerror(L) << ec.message();
			}
		}
};

} }

#endif
