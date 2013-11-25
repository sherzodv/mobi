#ifndef fastmq_routers_hpp
#define fastmq_routers_hpp

#include "logger.hpp"
#include "server.hpp"
#include "client.hpp"
#include "router.hpp"

#include <boost/asio.hpp>

namespace fastmq {

	typedef log::file::source flog_t;

	class unx_router_server: public unix_domain_server_base<flog_t> {
			public:
				unx_router_server(boost::asio::io_service & io
						, message_pool_base & p
						, router & r
						, const endpoint_t & endpoint
						, flog_t & l)
					: unix_domain_server_base<flog_t>(io, p, endpoint, l), R(r)
				{
				}

				virtual ~unx_router_server() {
				}

			protected:

				router & R;

				msgu * create_message(std::size_t len = 0) {
					std::size_t sz;
					sz = (len <= sizeof(msgu))
						? sizeof(msgu) : len;
					return static_cast<msgu *>(std::malloc(sz));
				}

				void destroy_message(msgu * msg) {
					std::free(msg);
				}

				bool register_terminal(terminal_base *t) {
					return R.route_add(route::on_idtype, t);
				}

				void unregister_terminal(terminal_base *t) {
					R.route_del(route::on_idtype, t);
				}

				void on_recv(terminal_base * t, msgu * msg) {
					(void)(t);
					R.route_message(msg);
				}

				void on_send(terminal_base * t, msgu * msg) {
					(void)(t);
					(void)(msg);
				}

				void on_recv_error(terminal_base * t)
				{
					lerror(L) << "unx_server::on_recv_error error!";
					unix_domain_server_base<flog_t>::on_recv_error(t);
				}
	};

	class unx_router_client: public unix_domain_client_base<flog_t> {
			public:
				unx_router_client(boost::asio::io_service & io
						, message_pool_base & p
						, router & r
						, flog_t & l)
					: unix_domain_client_base<flog_t>(io, p, l), R(r)
				{
				}

				virtual ~unx_router_client() {
				}

			protected:

				router & R;

				fastmq::msgu * create_message(std::size_t len = 0) {
					std::size_t sz;
					sz = (len <= sizeof(fastmq::msgu))
						? sizeof(fastmq::msgu) : len;
					return static_cast<fastmq::msgu *>(std::malloc(sz));
				}

				void destroy_message(fastmq::msgu * msg) {
					std::free(msg);
				}

				bool register_terminal(fastmq::terminal_base *t) {
					return R.route_add(route::on_idtype, t);
				}

				void unregister_terminal(fastmq::terminal_base *t) {
					R.route_del(route::on_idtype, t);
				}

				void on_connect_error() {
					lerror(L) << "unx_client::on_connect_error error!";
				}

				void on_recv(fastmq::terminal_base * t, fastmq::msgu * msg) {
					(void)(t);
					R.route_message(msg);
				}

				void on_send(fastmq::terminal_base * t, fastmq::msgu * msg) {
					(void)(t);
					(void)(msg);
				}

				void on_recv_error(fastmq::terminal_base * t) {
					lerror(L) << "unx_client::on_recv_error error!";
					unix_domain_client_base<flog_t>::on_recv_error(t);
				}
	};
}

#endif
