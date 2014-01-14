
#include <toolbox/toolbox.hpp>
#include <smpp/service.hpp>
#include <vision/log.hpp>

#include "pool.hpp"
#include <sstream>

namespace local {

	using namespace mobi::net;
	using namespace mobi::net::toolbox;

	namespace ba = boost::asio;
	namespace bs = boost::system;

	typedef vision::log::source log_t;
	typedef smpp::tcp_service<smpp::malloc_allocator, log_t> smpp_service;

	class service: public smpp_service {

		public:
			service(const ba::ip::tcp::endpoint & endpoint
					, smpp::malloc_allocator & a
					, log_t l)
				: smpp_service(endpoint, a, std::move(l))
			{
			}

			virtual ~service()
			{
			}

		protected:
			using smpp_service::L;

			bin::sz_t msg_id;

			void on_recv_error(bin::sz_t channel_id) {
				ltrace(L) << "channel #" << channel_id << " recv error";
			}

			void on_send(bin::sz_t channel_id, bin::sz_t msg_id) {
				ltrace(L) << "channel #" << channel_id << " msg sent: " << msg_id;
			}

			void on_send_error(bin::sz_t channel_id, bin::sz_t msg_id) {
				ltrace(L) << "channel #" << channel_id << " msg not sent: " << msg_id;
			}

			void on_parse_error(bin::sz_t channel_id) {
				lerror(L) << "channel #" << channel_id << " parse error";
			}

			void on_bind_transmitter(bin::sz_t channel_id, const smpp::bind_transmitter & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_bind_receiver(bin::sz_t channel_id, const smpp::bind_receiver & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_bind_transceiver(bin::sz_t channel_id, const smpp::bind_transceiver & msg) {
				std::string sys_id(reinterpret_cast<const char *>(msg.sys_id), msg.password_len);
				std::string password(reinterpret_cast<const char *>(msg.password), msg.password_len);
				smpp::bind_transceiver_r r;
				std::memcpy(r.sys_id, msg.sys_id, msg.sys_id_len);
				r.sys_id_len = msg.sys_id_len;
				r.sc_interface_version.set(msg.interface_version);
				r.command.seqno = msg.command.seqno;
				smpp_service::send(channel_id, r);
			}

			void on_unbind(bin::sz_t channel_id, const smpp::unbind & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_outbind(bin::sz_t channel_id, const smpp::outbind & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_generic_nack(bin::sz_t channel_id, const smpp::generic_nack & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_submit_sm(bin::sz_t channel_id, const smpp::submit_sm & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_submit_multi_sm(bin::sz_t channel_id, const smpp::submit_multi_sm & msg) {
				(void)(msg);
				ltrace(L) << "channel #" << channel_id << " got: ";
				/*
				ltrace(L) << "channel #" << channel_id << " got: ";
				std::stringstream idstr;
				msg_id++;
				strid << msg_id;
				std::string id = std::move(idstr.str());
				smpp::submit_sm_r r;
				std::memcpy(r.message_id, id.c_str(), id.size());
				r.command.seqno = msg.command.seqno;
				smpp_service::send(channel_id, r);
				*/
			}

			/* Messages which are not expected at the server side */
			void on_bind_transmitter_r(bin::sz_t channel_id, const smpp::bind_transmitter_r & msg) {
				lerror(L) << "channel #" << channel_id << " unexpected: " << msg;
			}
			void on_bind_receiver_r(bin::sz_t channel_id, const smpp::bind_receiver_r & msg) {
				lerror(L) << "channel #" << channel_id << " unexpected: " << msg;
			}
			void on_bind_transceiver_r(bin::sz_t channel_id, const smpp::bind_transceiver_r & msg) {
				lerror(L) << "channel #" << channel_id << " unexpected: " << msg;
			}
			void on_submit_sm_r(bin::sz_t channel_id, const smpp::submit_sm_r & msg) {
				(void)(msg);
				lerror(L) << "channel #" << channel_id << " unexpected";
			}
			void on_submit_multi_r(bin::sz_t channel_id, const smpp::submit_multi_r & msg) {
				(void)(msg);
				lerror(L) << "channel #" << channel_id << " unexpected";
			}
			void on_unbind_r(bin::sz_t channel_id, const smpp::unbind_r & msg) {
				lerror(L) << "channel #" << channel_id << " unexpected: " << msg;
			}
	};

}

int main()
{
	using namespace mobi::net;
	namespace ba = boost::asio;
	namespace bs = boost::system;

	vision::log::file::add("smppd%5N.log", false);
	vision::log::console::add();

	static auto L = vision::log::channel("main");

	linfo(L) << "Log initialized";
	linfo(L) << "sizeof(pdu): " << sizeof(smpp::pdu);
	linfo(L) << "sizeof(submit_sm): " << sizeof(smpp::submit_sm);

	try {
		smpp::malloc_allocator allocator;
		ba::ip::tcp::endpoint endpoint(ba::ip::tcp::v4(), 5555);
		local::service service(endpoint, allocator, vision::log::channel("srv"));
		toolbox::set_signal_handler(toolbox::stopper<local::service>(service));
		service.start();
	} catch (const std::exception & e) {
		lcritical(L) << e.what();
		return 1;
	}

	return 0;
}
