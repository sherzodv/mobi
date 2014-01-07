
#include <stack>
#include <vector>
#include <functional>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <smpp/proto.hpp>
#include <toolbox/toolbox.hpp>
#include "service.hpp"

namespace mobi { namespace net { namespace smpp {

namespace ba = boost::asio;
namespace bs = boost::system;

template <class ProtoT, class AllocatorT, class LogT>
class server: public service<ProtoT, AllocatorT, LogT> {

	typedef service<ProtoT, AllocatorT, LogT> service_base;

	public:
		server(const ba::ip::tcp::endpoint & endpoint
				, malloc_allocator & a
				, LogT l)
			: service_base(endpoint, a, std::move(l))
		{
		}

		virtual ~server()
		{
		}

	protected:
		using service_base::L;

		virtual bool authenticate(const std::string & sys_id, const std::string & password) = 0;

		void on_bind_transmitter(bin::sz_t channel_id, const bind_transmitter & msg) {
			ltrace(L) << "channel#" << channel_id << " got: " << msg << std::endl;
		}

		void on_bind_receiver(bin::sz_t channel_id, const bind_receiver & msg) {
			ltrace(L) << "channel#" << channel_id << " got: " << msg << std::endl;
		}

		void on_bind_transceiver(bin::sz_t channel_id, const bind_transceiver & msg) {
			ltrace(L) << "channel#" << channel_id << " got: " << msg << std::endl;
			std::string sys_id(reinterpret_cast<const char *>(msg.sys_id), msg.password_len);
			std::string password(reinterpret_cast<const char *>(msg.password), msg.password_len);
			if (authenticate(sys_id, password)) {
				bind_transceiver_r r;
				std::memcpy(r.sys_id, msg.sys_id, msg.sys_id_len);
				r.sys_id_len = msg.sys_id_len;
				r.sc_interface_version.set(msg.interface_version);
				r.command.seqno = msg.command.seqno;
				service_base::send_bind_transceiver_r(channel_id, r);
			}
		}

		/* Bind responses are not expected at server side */
		void on_bind_transmitter_r(bin::sz_t channel_id, const bind_transmitter_r & msg) {
			lerror(L) << channel_id << "got bind_transmitter_r" << std::endl;
			lerror(L) << channel_id << msg << std::endl;
		}
		void on_bind_receiver_r(bin::sz_t channel_id, const bind_receiver_r & msg) {
			lerror(L) << channel_id << "got bind_receiver_r" << std::endl;
			ltrace(L) << channel_id << msg << std::endl;
		}
		void on_bind_transceiver_r(bin::sz_t channel_id, const bind_transceiver_r & msg) {
			lerror(L) << channel_id << "got bind_transceiver_r" << std::endl;
			ltrace(L) << channel_id << msg << std::endl;
		}

		void on_unbind(bin::sz_t channel_id, const unbind & msg) {
			(void)(channel_id);(void)(msg);
		}

		void on_unbind_r(bin::sz_t channel_id, const unbind_r & msg) {
			(void)(channel_id);(void)(msg);
		}

		void on_outbind(bin::sz_t channel_id, const outbind & msg) {
			(void)(channel_id);(void)(msg);
		}

		void on_generic_nack(bin::sz_t channel_id, const generic_nack & msg) {
			(void)(channel_id);(void)(msg);
		}

		void on_submit_sm(bin::sz_t channel_id, const submit_sm & msg) {
			ltrace(L) << "channel#" << channel_id << " got: " << msg << std::endl;
		}

		void on_submit_sm_r(bin::sz_t channel_id, const submit_sm_r & msg) {
			(void)(channel_id);(void)(msg);
		}

		void on_send_error(bin::sz_t channel_id) {
			(void)(channel_id);
		}
};

template <class AllocatorT, class LogT>
using local_server
	= server<boost::asio::local::stream_protocol, AllocatorT, LogT>;

template <class AllocatorT, class LogT>
using tcp_server
	= server<boost::asio::ip::tcp, AllocatorT, LogT>;

} } }
