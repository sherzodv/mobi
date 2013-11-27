
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
	};

}

int main()
{
	namespace ba = boost::asio;
	namespace bs = boost::system;

	vision::log::file::add("fastmqr%5N.log", true);

	static auto L = std::move(vision::log::channel("main"));
	static const char opt_sock_path[] = "/tmp/fastmqr.sock";

	linfo(L) << "Log initialized";

	::unlink(opt_sock_path);

	ba::io_service io;

	toolbox::io::stop_on_signal(io);
	toolbox::io::stop_after(io, boost::posix_time::seconds(10));

	try {
		smpp::malloc_message_pool pool;
		ba::ip::tcp::endpoint endpoint(ba::ip::tcp::v4(), 5555);
		local::server server(io, pool, endpoint, L);

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
