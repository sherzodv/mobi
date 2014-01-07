
#include <toolbox/toolbox.hpp>
#include <vision/log.hpp>

#include "pool.hpp"
#include "server.hpp"
#include <sstream>

namespace local {

	using namespace mobi::net;
	using namespace mobi::net::toolbox;

	namespace ba = boost::asio;
	namespace bs = boost::system;

	typedef vision::log::source log_t;
	typedef smpp::tcp_server<smpp::malloc_allocator, log_t> server_base;

	class server: public server_base {

		public:
			server(const ba::ip::tcp::endpoint & endpoint
					, smpp::malloc_allocator & a
					, log_t l)
				: server_base(endpoint, a, std::move(l))
			{
			}

			virtual ~server()
			{
			}

		protected:
			bool authenticate(const std::string & sys_id, const std::string & password) {
				(void)(password);
				ltrace(L) << sys_id << " authenticated";
				return true;
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
		local::server server(endpoint, allocator, vision::log::channel("srv"));
		toolbox::set_signal_handler(toolbox::stopper<local::server>(server));
		server.start();
	} catch (const std::exception & e) {
		lcritical(L) << e.what();
		return 1;
	}

	return 0;
}
