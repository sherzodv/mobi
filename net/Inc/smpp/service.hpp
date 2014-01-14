#ifndef smpp_service_hpp
#define smpp_service_hpp

#include <stack>
#include <vector>
#include <functional>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <smpp/proto.hpp>
#include <toolbox/toolbox.hpp>
#include <toolbox/service.hpp>

namespace mobi { namespace net { namespace smpp {

namespace ba = boost::asio;
namespace bs = boost::system;

template <class ProtoT, class AllocatorT, class LogT>
class service
	: private parser<LogT>
	, private writer<LogT>
	, private toolbox::service<ProtoT, AllocatorT, LogT, bin::u32_t> {

	typedef service<ProtoT, AllocatorT, LogT>	service_t;

	protected:

	typedef toolbox::service<ProtoT, AllocatorT, LogT, bin::u32_t>
						service_base;

	typedef typename service_base::log_t			log_t;
	typedef typename service_base::sock_t			sock_t;
	typedef typename service_base::acpt_t			acpt_t;
	typedef typename service_base::proto_t			proto_t;
	typedef typename service_base::channel_t		channel_t;
	typedef typename service_base::allocator_t		allocator_t;
	typedef typename service_base::endpoint_t		endpoint_t;

	typedef parser<LogT> parser_base;
	typedef writer<LogT> writer_base;

	friend channel_t;

	using service_base::L;
	using service_base::A;

	public:
		service(const endpoint_t & ep, allocator_t & a, log_t l)
			: parser_base(l)
			, writer_base(l)
			, service_base(ep, a, l)
		{}

		virtual ~service() {
		}

		using service_base::start;
		using service_base::stop;
		using service_base::close_channel;

	protected:
		template <typename MsgT>
		bin::sz_t send(bin::sz_t channel_id, MsgT & msg) {
			bin::buffer buf;
			buf.len = msg.raw_size();
			buf.data = bin::asbuf(A.alloc(buf.len));
			/* Set message overall length before serializing it to buffer */
			msg.command.len = msg.raw_size();
			writer_base::write(buf.data, buf.data + buf.len, msg);
			return service_base::send(channel_id, buf);
		}

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
		virtual void on_submit_multi_sm(bin::sz_t channel_id, const submit_multi_sm & msg) = 0;
		virtual void on_submit_multi_r(bin::sz_t channel_id, const submit_multi_r & msg) = 0;
		virtual void on_parse_error(bin::sz_t channel_id) = 0;

	private:
		typedef typename parser_base::action action;

		bin::sz_t m_channel_id;

		void on_recv(bin::sz_t channel_id, bin::buffer buf) {
			m_channel_id = channel_id;
			parser_base::parse(buf.data, buf.data + buf.len);
		}

		action on_parse_error(const bin::u8_t * buf, const bin::u8_t * bend) {
			(void)(buf);
			(void)(bend);
			on_parse_error(m_channel_id);
			return parser_base::stop;
		}

		action on_bind_transmitter(const bind_transmitter & msg) {
			on_bind_transmitter(m_channel_id, msg);
			return parser_base::resume;
		}

		action on_bind_transmitter_r(const bind_transmitter_r & msg) {
			on_bind_transmitter_r(m_channel_id, msg);
			return parser_base::resume;
		}

		action on_bind_receiver(const bind_receiver & msg) {
			on_bind_receiver(m_channel_id, msg);
			return parser_base::resume;
		}

		action on_bind_receiver_r(const bind_receiver_r & msg) {
			on_bind_receiver_r(m_channel_id, msg);
			return parser_base::resume;
		}

		action on_bind_transceiver(const bind_transceiver & msg) {
			on_bind_transceiver(m_channel_id, msg);
			return parser_base::resume;
		}

		action on_bind_transceiver_r(const bind_transceiver_r & msg) {
			on_bind_transceiver_r(m_channel_id, msg);
			return parser_base::resume;
		}

		action on_unbind(const unbind & msg) {
			on_unbind(m_channel_id, msg);
			return parser_base::resume;
		}

		action on_unbind_r(const unbind_r & msg) {
			on_unbind_r(m_channel_id, msg);
			return parser_base::resume;
		}

		action on_outbind(const outbind & msg) {
			on_outbind(m_channel_id, msg);
			return parser_base::resume;
		}

		action on_generic_nack(const generic_nack & msg) {
			on_generic_nack(m_channel_id, msg);
			return parser_base::resume;
		}

		action on_submit_sm(const submit_sm & msg) {
			on_submit_sm(m_channel_id, msg);
			return parser_base::resume;
		}

		action on_submit_sm_r(const submit_sm_r & msg) {
			on_submit_sm_r(m_channel_id, msg);
			return parser_base::resume;
		}

		action on_submit_multi_sm(const submit_multi_sm & msg) {
			on_submit_multi_sm(m_channel_id, msg);
			return parser_base::resume;
		}

		action on_submit_multi_r(const submit_multi_r & msg) {
			on_submit_multi_r(m_channel_id, msg);
			return parser_base::resume;
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
