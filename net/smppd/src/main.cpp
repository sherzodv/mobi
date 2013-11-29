
#include <toolbox/toolbox.hpp>
#include <vision/log.hpp>
#include <smpp/server.hpp>

namespace local {

	namespace ba = boost::asio;
	namespace bs = boost::system;

	typedef vision::log::source log_t;
	typedef smpp::tcp_server_base<log_t> server_base;

	class server: public server_base {
			public:
				server(boost::asio::io_service & io
						, smpp::message_pool_base & p
						, const ba::ip::tcp::endpoint & endpoint
						, log_t l)
					: server_base(io, p, endpoint, std::move(l))
				{
				}

				virtual ~server() {
				}

				virtual bool open(smpp::session * s) {
					ltrace(L) << "server::open: new incoming session:"
						<< " password: " << s->password
						<< " system_id: " << s->sys_id
						<< " system_type: " << s->sys_type;
					s->password.clear();
					return true;
				}

				virtual void close(smpp::session * s) {
					(void)(s);
				}

				virtual void on_recv_error(smpp::session * s) {
					server_base::on_recv_error(s);
				}

				virtual void on_send_error(smpp::session * s
						, smpp::pdu * msg) {
					server_base::on_send_error(s, msg);
				}
	};

}

int main()
{
	namespace ba = boost::asio;
	namespace bs = boost::system;

	vision::log::file::add("fastmqr%5N.log", false);
	//vision::log::console::add();

	static auto L = vision::log::channel("main");
	static const char opt_sock_path[] = "/tmp/fastmqr.sock";

	linfo(L) << "Log initialized";

	::unlink(opt_sock_path);

	ba::io_service io;

	toolbox::io::stop_on_signal(io);
	//toolbox::io::stop_after(io, boost::posix_time::seconds(10));

	linfo(L) << "sizeof(pdu): " << sizeof(smpp::pdu);

	try {
		smpp::malloc_message_pool pool;
		ba::ip::tcp::endpoint endpoint(ba::ip::tcp::v4(), 5555);
		local::server server(io, pool, endpoint, vision::log::channel("srv"));
		server.listen();
		linfo(L) << "Starting io service";
		io.run();
		linfo(L) << "Bye!";
	} catch (const std::exception & e) {
		::unlink(opt_sock_path);
		lcritical(L) << e.what();
		return 1;
	}

	::unlink(opt_sock_path);
	return 0;
}
