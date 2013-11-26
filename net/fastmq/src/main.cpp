
#include <fastmq/log.hpp>
#include <fastmq/peer.hpp>
#include <fastmq/proto.hpp>
#include <fastmq/server.hpp>
#include <fastmq/client.hpp>
#include <fastmq/routers.hpp>
#include <fastmq/terminal.hpp>

static const int VERSION = 0;

static void test() {

	namespace ba = boost::asio;
	namespace bs = boost::system;

	ba::io_service io;
	ba::local::stream_protocol::endpoint endpoint("/tmp/test.sock");

	static auto L = std::move(fastmq::log::channel("__libfastmqtest__"));

	fastmq::malloc_message_pool pool;
	fastmq::router router(pool);
	fastmq::unx_router_server server(io, pool, router, endpoint, L);
}

auto p = test;
