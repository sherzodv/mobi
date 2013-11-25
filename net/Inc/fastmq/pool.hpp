#ifndef fastmq_message_pool_hpp
#define fastmq_message_pool_hpp

#include <fastmq/proto.hpp>
#include <cstring>
#include <cstdlib>

namespace fastmq {

	class message_pool_base {
		public:
			message_pool_base() {}
			~message_pool_base() {}

			virtual msgu * create_message(std::size_t len = 0) = 0;
			virtual void destroy_message(msgu * msg) = 0;
			virtual msgu * copy_message(msgu * msg) = 0;
	};

	class malloc_message_pool: public message_pool_base {
		public:
			malloc_message_pool() {
			}

			~ malloc_message_pool() {
			}

			virtual msgu * create_message(std::size_t len = 0) {
				if (len <= sizeof(msgu)) {
					return static_cast<msgu *>(std::malloc(sizeof(msgu)));
				} else {
					return static_cast<msgu *>(std::malloc(len));
				}
			}

			virtual void destroy_message(msgu * msg) {
				std::free(static_cast<void *>(msg));
			}

			virtual msgu * copy_message(msgu * msg) {
				msgu * nmsg = create_message(msg->len);
				if (msg->len <= sizeof(msgu)) {
					std::memcpy(nmsg, msg, sizeof(msgu));
				} else {
					std::memcpy(nmsg, msg, msg->len);
				}
				return nmsg;
			}
	};
}

#endif
