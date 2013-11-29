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
		/* Return msg_id. */
		virtual std::string on_submit_sm(session * s, submit_sm * msg) = 0;

		void send_submit_sm_r(channel_t * ch, pdu * msg
				, const std::string & msg_id) {
			submit_sm_r r;
			r.command.id = the<submit_sm>::r_id;
			r.command.status = msg->status;
			r.command.seqno = msg->seqno;
			std::strcpy((char *)r.msg_id, msg_id.c_str());
			r.msg_id_len = msg_id.size() + 1;
			r.command.len = the<submit_sm>::r_size(r);
			msg = P.create_message(r.command.len);
			write(utl::asbuf(msg), r, L);
			ch->send(msg);
		}

		template <class BindT>
		void send_bind_r(channel_t * ch, pdu * omsg) {
			/* TODO: Set correct sys_id and sc_interface_version */
			pdu * msg;
			typename the<BindT>::r_type r;

			r.command.id = the<BindT>::r_id;
			r.command.status = omsg->status;
			r.command.seqno = omsg->seqno;
			std::strcpy((char *)r.sys_id, "smppd");
			r.sys_id_len = sizeof("smppd");
			r.command.len = the<BindT>::r_size(r);

			ltrace(L) << "server_base::send_bind_r: bytes: " << r.command.len;

			msg = P.create_message(r.command.len);
			write_bind_r(utl::asbuf(msg), r, L);
			ch->send_bind_r(msg);
		}

		template <class BindT>
		void bind_session(channel_t * ch, pdu * msg) {
			BindT b;
			parse_bind(b, utl::ascbuf(msg), L);
			ch->set_properties(b);
			if (open(ch)) {
				send_bind_r<BindT>(ch, msg);
			} else {
				/* TODO: send error bind_response_r */
				destroy(ch);
			}
		}

		void on_recv_bind(channel_t * ch, pdu * msg) {
			switch (msg->id) {
				case command::bind_receiver:
					bind_session<bind_receiver>(ch, msg);
					break;
				case command::bind_transmitter:
					bind_session<bind_transmitter>(ch, msg);
					break;
				case command::bind_transceiver: {
					bind_session<bind_transceiver>(ch, msg);
					break;
				}
				default: {
					/* TODO: send generic_nack and then destroy channel */
					lerror(L) << "server_base::on_recv_bind:"
						"wrong bind id: " << msg->id;
					break;
				}
			}
			P.destroy_message(msg);
		}

		void on_recv_bind_error(channel_t * ch) {
			(void)(ch);
		}

		void on_send_bind_r(channel_t * ch, pdu * msg) {
			P.destroy_message(msg);
			ch->recv();
		}

		void on_send_bind_r_error(channel_t * ch, pdu * msg) {
			P.destroy_message(msg);
			destroy(ch);
		}

		void on_recv(channel_t * ch, pdu * msg) {
			ch->recv();
			switch (msg->id) {
				case command::submit_sm: {
					ltrace(L) << "server_base::on_recv: submit_sm:"
						" msg->len: " << msg->len;
					std::string msg_id = std::move(on_submit_sm(ch, nullptr));
					send_submit_sm_r(ch, msg, msg_id);
					break;
				}
				default: {
					ltrace(L) << "server_base::on_recv: unknown msg id: "
						<< msg->id << " len: " << msg->len;
					return;
				}
			}
			P.destroy_message(msg);
		}

		void on_send(channel_t * ch, pdu * msg) {
			/* TODO: Find out if need to notify implementaion */
			(void)(ch);
			P.destroy_message(msg);
		}

		void on_recv_error(session * ch) {
			/* TODO: Find out if need to notify implementaion */
			/* Default implementation:
			 * unregister and destroy */
			destroy(dynamic_cast<channel_t *>(ch));
		}

		void on_send_error(session * ch, pdu * msg) {
			/* TODO: Find out if need to notify implementaion */
			/* Default implementation:
			 * unregister and destroy */
			P.destroy_message(msg);
			destroy(dynamic_cast<channel_t *>(ch));
		}

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
