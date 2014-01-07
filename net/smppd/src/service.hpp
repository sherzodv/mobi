#ifndef smpp_service_hpp
#define smpp_service_hpp

#include <stack>
#include <vector>
#include <functional>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <smpp/proto.hpp>
#include <toolbox/toolbox.hpp>
#include "channel.hpp"

namespace mobi { namespace net { namespace smpp {

namespace ba = boost::asio;
namespace bs = boost::system;

template <class ProtoT, class AllocatorT, class LogT>
class service: private parser<LogT>, private writer<LogT>  {

	typedef service<ProtoT, AllocatorT, LogT> service_t;

	protected:

	typedef LogT								log_t;
	typedef ProtoT								proto_t;
	typedef AllocatorT							allocator_t;
	typedef ba::basic_stream_socket<proto_t>	sock_t;
	typedef ba::basic_socket_acceptor<proto_t>	acpt_t;
	typedef channel<service<proto_t, allocator_t, log_t>>	channel_t;

	typedef parser<LogT> parser_base;
	typedef writer<LogT> writer_base;

	friend channel_t;

	log_t L;

	public:
		typedef typename proto_t::endpoint endpoint_t;

		service(const endpoint_t & ep, allocator_t & a, log_t l)
			: parser<LogT>(l)
			, writer<LogT>(l)
			, L(std::move(l))
			, m_io()
			, m_sock(m_io)
			, m_acpt(m_io, ep)
			, A(a)
		{}

		virtual ~service() {}

		void start() {
			m_acpt.async_accept(m_sock
				, boost::bind(&service::on_accept
					, this, ba::placeholders::error));
			m_io.run();
		}

		void stop() {
			m_io.post(io::stop(m_io));
		}

	protected:
		virtual void on_bind_transmitter(bin::sz_t channel_id, const bind_transmitter & msg) = 0;
		virtual void on_bind_transmitter_r(bin::sz_t channel_id, const bind_transmitter_r & msg) = 0;
		virtual void on_bind_receiver(bin::sz_t channel_id, const bind_receiver & msg) = 0;
		virtual void on_bind_receiver_r(bin::sz_t channel_id, const bind_receiver_r & msg) = 0;
		virtual void on_bind_transceiver(bin::sz_t channel_id, const bind_transceiver & msg) = 0;
		virtual void on_bind_transceiver_r(bin::sz_t channel_id, const bind_transceiver_r & msg) = 0;
		virtual void on_unbind(bin::sz_t channel_id, const unbind & msg) = 0;
		virtual void on_unbind_r(bin::sz_t channel_id, const unbind_r & msg) = 0;
		virtual void on_outbind(bin::sz_t channel_id, const outbind & msg) = 0;
		virtual void on_generic_nack(bin::sz_t channel_id, const generic_nack & msg) = 0;
		virtual void on_submit_sm(bin::sz_t channel_id, const submit_sm & msg) = 0;
		virtual void on_submit_sm_r(bin::sz_t channel_id, const submit_sm_r & msg) = 0;

		virtual void on_send_error(bin::sz_t channel_id) = 0;

		void send_bind_transceiver_r(bin::sz_t channel_id, bind_transceiver_r & msg) {
			channel_t * ch = get_channel(channel_id);
			if (ch == nullptr) {
				lerror(L) << "wrong channel id: " << channel_id;
				on_send_error(channel_id);
			}
			bin::buffer buf;
			buf.len = msg.raw_size();
			buf.data = bin::asbuf(A.alloc(buf.len));
			msg.command.len = msg.raw_size();
			writer_base::write(buf.data, buf.data + buf.len, msg);
			ch->send(buf);
		}

		void close(std::size_t session_id) {
		}

	private:
		ba::io_service m_io;
		sock_t m_sock;
		acpt_t m_acpt;
		allocator_t & A;

		std::stack<bin::sz_t> m_hole;
		std::vector<channel_t *> m_book;

		bin::sz_t m_channel_id;

		typedef typename parser_base::action action;

		using parser_base::stop;
		using parser_base::resume;
		using parser_base::skip;

		action on_bind_transmitter(const bind_transmitter & msg) {
			on_bind_transmitter(m_channel_id, msg);
			return resume;
		}

		action on_bind_transmitter_r(const bind_transmitter_r & msg) {
			on_bind_transmitter_r(m_channel_id, msg);
			return resume;
		}

		action on_bind_receiver(const bind_receiver & msg) {
			on_bind_receiver(m_channel_id, msg);
			return resume;
		}

		action on_bind_receiver_r(const bind_receiver_r & msg) {
			on_bind_receiver_r(m_channel_id, msg);
			return resume;
		}

		action on_bind_transceiver(const bind_transceiver & msg) {
			on_bind_transceiver(m_channel_id, msg);
			return resume;
		}

		action on_bind_transceiver_r(const bind_transceiver_r & msg) {
			on_bind_transceiver_r(m_channel_id, msg);
			return resume;
		}

		action on_unbind(const unbind & msg) {
			on_unbind(m_channel_id, msg);
			return resume;
		}

		action on_unbind_r(const unbind_r & msg) {
			on_unbind_r(m_channel_id, msg);
			return resume;
		}

		action on_outbind(const outbind & msg) {
			on_outbind(m_channel_id, msg);
			return resume;
		}

		action on_generic_nack(const generic_nack & msg) {
			on_generic_nack(m_channel_id, msg);
			return resume;
		}

		action on_submit_sm(const submit_sm & msg) {
			on_submit_sm(m_channel_id, msg);
			return resume;
		}

		action on_submit_sm_r(const submit_sm_r & msg) {
			on_submit_sm_r(m_channel_id, msg);
			return resume;
		}

		action on_submit_multi_sm(const submit_multi_sm & msg) {
			ltrace(L) << "on_submit_multi_sm";
			(void)(msg);
			return resume;
		}

		action on_parse_error(const bin::u8_t * buf, const bin::u8_t * bend) {
			ltrace(L) << "on_parse_error";
			(void)(buf);
			(void)(bend);
			return resume;
		}

		void on_accept(const bs::error_code & ec) {
			if (!ec) {
				channel_t * ch = create(m_sock, *this);
				ch->recv();
				start();
			} else {
				lerror(L) << ec.message();
			}
		}

		void on_recv_error(channel_t * ch) {
			(void)(ch);
		}

		void on_send_error(channel_t * ch, bin::buffer buf) {
			(void)(ch);
			A.dealloc(buf.data);
		}

		void on_recv(channel_t * ch, bin::buffer buf) {
			m_channel_id = ch->id();
			parser_base::parse(buf.data, buf.data + buf.len);
			ltrace(L) << bin::hex_str_ref(buf) << std::endl;
			A.dealloc(buf.data);
			ch->recv();
		}

		void on_send(channel_t * ch, bin::buffer buf) {
			ltrace(L) << "channel" << ch->id() << "::on_send:";
			ltrace(L) << bin::hex_str_ref(buf);
			A.dealloc(buf.data);
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
			return ch;
		}

		void destroy(channel_t * ch) {
			m_book.at(ch->id()) = nullptr;
			m_hole.push(ch->id());
			delete ch;
		}

		channel_t * get_channel(bin::sz_t id) {
			if (id >= m_book.size()) {
				return nullptr;
			}
			return m_book[id];
		}
};

template <class AllocatorT, class LogT>
using local_service
	= service<boost::asio::local::stream_protocol, AllocatorT, LogT>;

template <class AllocatorT, class LogT>
using tcp_service
	= service<boost::asio::ip::tcp, AllocatorT, LogT>;

} } }

#endif
