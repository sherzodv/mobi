
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <unordered_map>

#include <fastmq/logger/logger.hpp>
#include <fastmq/router/service.hpp>
#include <fastmq/client/service.hpp>

namespace ba = boost::asio;
namespace bs = boost::system;

static auto L = std::move(fastmq::log::file::mklogger("main"));

static void exit_gracefully(int /* sid */) {
	std::cout
		<< "notify: Got SIGINT, doing nothing :P."
		<< std::endl;
}

static void setup_signal_handlers() {
	struct sigaction exitAction;
	exitAction.sa_handler = exit_gracefully;
	sigemptyset(&exitAction.sa_mask);
	exitAction.sa_flags = 0;
	sigaction(SIGINT, &exitAction, nullptr);
}

namespace local {

	class router;
	class client;

	typedef fastmq::router::service<ba::local::stream_protocol
		, fastmq::log::file::source, router> router_base;
	typedef fastmq::client::service<ba::local::stream_protocol
		, fastmq::log::file::source, client> client_base;

	const char ping[] = "ping";
	const char pong[] = "pong";

	class router: public router_base
	{
		typedef std::unordered_map<fastmq::proto::u16_t, peer_t *> map16_t;
		typedef std::unordered_map<fastmq::proto::u32_t, peer_t *> map32_t;

		map16_t m_byid;
		map16_t m_bytype;
		map32_t m_byidtype;

		fastmq::proto::u32_t make_key(fastmq::proto::u16_t id
				, fastmq::proto::u16_t type)
		{
			return ( id << 16 ) | type;
		}

		public:
			template <typename EndpointT>
			router(ba::io_service & io, const EndpointT & endpoint
				, router_base::log_t & logger)
				: router_base(io, endpoint, logger)
			{
			}

			std::uint16_t register_peer(std::uint16_t id, std::uint16_t type) {
				if (m_byid.find(id) != m_byid.end()) {
					return 0;
				}
				if (m_byidtype.find(make_key(id, type)) != m_byidtype.end()) {
					return 0;
				}
				return id;
			}

			void on_register_peer(peer_t * p) {
				m_byid.insert(std::make_pair(p->id(), p));
				m_bytype.insert(std::make_pair(p->type(), p));
				m_byidtype.insert(std::make_pair(
					make_key(p->id(), p->type()), p));
				p->recv_message();
			}

			void on_message(peer_t * p, const fastmq::proto::msgu * msg) {
				map32_t::const_iterator i
					= m_byidtype.find(make_key(p->id(), p->type()));
				if (i == m_byidtype.end()) {
					/* TODO: Handle error: no such destination */
					ltrace(L) << "message rejected";
				} else {
					/* Copy message from in.buf to out.buf */
					i->second->send_message(msg);
					/* in.buf ready to receive a new message */
					p->recv_message();
				}
 			}

			void on_send_message(peer_t * p, const bs::error_code & ec
				, std::size_t bytes)
			{
				(void)(p);
				(void)(ec);
				(void)(bytes);
				ltrace(L) << "message transferred to ("
					<< p->id() << ", " << p->type() << ")";
			}
	};

	class client: public client_base
	{
		std::size_t m_msg_count;
		fastmq::proto::msgu *msg;
		fastmq::proto::u8_t
			buf[sizeof(fastmq::proto::msgu) + sizeof(ping) - 2];

		void send_ping() {
				msg->len = sizeof(buf);
				msg->id = 0;
				msg->type = 0;
				std::copy(ping, ping + sizeof(ping)-1, msg->data);
				send_message(msg);
		}

		void send_pong() {
				msg->len = sizeof(buf);
				msg->id = 0;
				msg->type = 0;
				std::copy(pong, pong + sizeof(pong)-1, msg->data);
				send_message(msg);
		}

		public:
			template <class EndpointIteratorT>
			client(ba::io_service & io
					, const EndpointIteratorT & begin
					, fastmq::proto::u16_t id, fastmq::proto::u16_t type
					, client_base::log_t & logger)
				: client_base(io, begin, id, type, logger)
				, m_msg_count(0)
				, msg(fastmq::proto::as_msgup(buf))
			{
			}

			void on_register() {
				send_ping();
			}

			void on_message(const fastmq::proto::msgu *msg) {
				std::string data(reinterpret_cast<const char *>(msg->data)
					, msg->len - sizeof(fastmq::proto::msgu) + 1);
				ltrace(L) << "got message: "
					<< "id[" << msg->id << "]"
					<< " type[" << msg->type << "]"
					<< " data[" << data << "]";
				if (m_msg_count < 20000) {
					send_ping();
				} else {
					m_io.stop();
				}
			}

			void on_send_message(const bs::error_code & ec
				, std::size_t bytes)
			{
				(void)(ec);
				(void)(bytes);
				++m_msg_count;
				ltrace(L)
					<< "message send: " << ping
					<< ", total: " << m_msg_count;
				recv_message();
			}
	};
}

int main() {

	static const char opt_sock_path[] = "/tmp/fastmqr.sock";

	fastmq::log::file::add("main.log");
	linfo(L) << "Log initialized";

	setup_signal_handlers();
	linfo(L) << "Signal handlers set";

	::unlink(opt_sock_path);

	ba::io_service io;
	ba::local::stream_protocol::endpoint endpoint(opt_sock_path);
	ba::deadline_timer timer(io);

	class stop {
		ba::io_service & m_io;
		public:
			stop(ba::io_service &io): m_io(io) {}
			void operator()(const bs::error_code &) {
				m_io.stop();
			}
	};

	try {
		linfo(L) << "Starting io service";

		local::router srv(io, endpoint, L);
		local::client clt(io, endpoint, 2, 3, L);

		srv.start();

		//timer.expires_from_now(boost::posix_time::milliseconds(4000));
		//timer.async_wait(stop(io));

		io.run();

		srv.log_io_stats();
		clt.log_io_stats();

		linfo(L) << "Bye!";
		lflush(L);
	} catch (const std::exception & e) {
		::unlink(opt_sock_path);
		lcritical(L) << e.what();
		return 1;
	}

	::unlink(opt_sock_path);

	return 0;
}
