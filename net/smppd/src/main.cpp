
#include <sstream>
#include <chrono>

#include <vision/log.hpp>
#include <smpp/service.hpp>
#include <toolbox/toolbox.hpp>

#include <boost/program_options.hpp>

#include "pool.hpp"

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
				msg_id = 0;
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
				(void)(channel_id);
				(void)(msg_id);
			}

			void on_send_error(bin::sz_t channel_id, bin::sz_t msg_id) {
				ltrace(L) << "channel #" << channel_id << " msg #" << msg_id << "not sent";
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
				std::string id(std::to_string(msg_id++));
				smpp::submit_sm_r r;
				std::memcpy(r.msg_id, id.c_str(), id.size() + 1);
				r.msg_id_len = id.size() + 1;
				r.command.seqno = msg.command.seqno;
				smpp_service::send(channel_id, r);
			}

			void on_submit_multi_sm(bin::sz_t channel_id, const smpp::submit_multi_sm & msg) {
				(void)(msg);
				ltrace(L) << "channel #" << channel_id << " got: ";
			}

			void on_deliver_sm(bin::sz_t channel_id, const smpp::deliver_sm & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_deliver_sm_r(bin::sz_t channel_id, const smpp::deliver_sm_r & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_data_sm(bin::sz_t channel_id, const smpp::data_sm & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_data_sm_r(bin::sz_t channel_id, const smpp::data_sm_r & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_query_sm(bin::sz_t channel_id, const smpp::query_sm & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_query_sm_r(bin::sz_t channel_id, const smpp::query_sm_r & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_cancel_sm(bin::sz_t channel_id, const smpp::cancel_sm & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_cancel_sm_r(bin::sz_t channel_id, const smpp::cancel_sm_r & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_replace_sm(bin::sz_t channel_id, const smpp::replace_sm & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_replace_sm_r(bin::sz_t channel_id, const smpp::replace_sm_r & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_enquire_link(bin::sz_t channel_id, const smpp::enquire_link & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_enquire_link_r(bin::sz_t channel_id, const smpp::enquire_link_r & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
			}

			void on_alert_notification(bin::sz_t channel_id, const smpp::alert_notification & msg) {
				ltrace(L) << "channel #" << channel_id << " got: " << msg;
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

int main(int argc, char ** argv)
{
	using namespace mobi::net;
	namespace ba = boost::asio;
	namespace bs = boost::system;
	namespace po = boost::program_options;

	po::options_description options("Options");
	options.add_options()
		("help", "Produce help messages")
		("console", "Log to console as well")
		("log-file", "Log file name template")
	;

	po::variables_map opts;
	po::store(po::parse_command_line(argc, argv, options), opts);

	if (opts.count("help")) {
		std::cout << options << std::endl;
		return 1;
	}

	std::string log_name_tmpl = "smppd%5N.log";

	if (opts.count("log-file")) {
		log_name_tmpl = opts["log-file"].as<std::string>();
	}

	vision::log::file::add("smppd%5N.log", true);

	if (opts.count("console")) {
		vision::log::console::add();
	}

	static auto L = vision::log::channel("main");

	linfo(L) << "Log initialized";
	linfo(L) << "sizeof(pdu): " << sizeof(smpp::pdu);
	linfo(L) << "sizeof(submit_sm): " << sizeof(smpp::submit_sm);

	std::string cmd;

	try {
		smpp::malloc_allocator allocator;
		ba::ip::tcp::endpoint endpoint(ba::ip::tcp::v4(), 5555);
		local::service service(endpoint, allocator, vision::log::channel("srv"));
		toolbox::set_signal_handler(toolbox::stopper<local::service>(service));
		service.start();
		std::getline(std::cin, cmd);
		service.stop();
		linfo(L) << "bye!";
	} catch (const std::exception & e) {
		lcritical(L) << e.what();
		return 1;
	}

	return 0;
}
