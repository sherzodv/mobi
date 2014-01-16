#ifndef eastwind_ewgate_service_hpp
#define eastwind_ewgate_service_hpp

#include <stack>
#include <vector>
#include <functional>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <ewgate/proto.hpp>
#include <toolbox/toolbox.hpp>
#include <toolbox/service.hpp>

namespace eastwind { namespace ewgate {

namespace ba = boost::asio;
namespace bs = boost::system;

template <class ProtoT, class AllocatorT, class LogT>
class service
	: private parser<LogT>
	, private writer<LogT>
	, private mobi::net::toolbox::service<ProtoT, AllocatorT, LogT, pdu> {

	typedef service<ProtoT, AllocatorT, LogT>	service_t;

	protected:

	typedef mobi::net::toolbox::service<ProtoT, AllocatorT, LogT, pdu>
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
		using service_base::close;

	protected:
		template <typename MsgT>
		bin::sz_t send(bin::sz_t channel_id, MsgT & msg) {
			bin::buffer buf;
			buf.len = msg.raw_size();
			buf.data = bin::asbuf(A.alloc(buf.len));
			/* Set message overall length before serializing it to buffer */
			msg.hdr.len = msg.raw_size();
			writer_base::write(buf.data, buf.data + buf.len, msg);
			return service_base::send(channel_id, buf);
		}

		virtual void on_heartbeat(bin::sz_t channel_id, const heartbeat & msg) = 0;
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

		action on_heartbeat(const heartbeat & msg) {
			on_heartbeat(m_channel_id, msg);
			return parser_base::resume;
		}

};

template <class AllocatorT, class LogT>
using local_service
	= service<boost::asio::local::stream_protocol, AllocatorT, LogT>;

template <class AllocatorT, class LogT>
using tcp_service
	= service<boost::asio::ip::tcp, AllocatorT, LogT>;

} }

#endif
