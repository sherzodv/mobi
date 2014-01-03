#ifndef smpp_message_pool_hpp
#define smpp_message_pool_hpp

#include <cstring>
#include <cstdlib>

#include <smpp/proto.hpp>

namespace mobi { namespace net { namespace smpp {

	class message_pool_base {
		public:
			message_pool_base() {}
			~message_pool_base() {}

			virtual pdu * create_message(std::size_t len = 0) = 0;
			virtual void destroy_message(pdu * msg) = 0;
			virtual pdu * copy_message(pdu * msg) = 0;
	};

	class malloc_message_pool: public message_pool_base {
		public:
			malloc_message_pool() {
			}

			~ malloc_message_pool() {
			}

			virtual pdu * create_message(std::size_t len = 0) {
				if (len <= sizeof(pdu)) {
					return static_cast<pdu *>(std::malloc(sizeof(pdu)));
				} else {
					return static_cast<pdu *>(std::malloc(len));
				}
			}

			virtual void destroy_message(pdu * msg) {
				std::free(static_cast<void *>(msg));
			}

			virtual pdu * copy_message(pdu * msg) {
				pdu * nmsg = create_message(msg->len);
				if (msg->len <= sizeof(pdu)) {
					std::memcpy(nmsg, msg, sizeof(pdu));
				} else {
					std::memcpy(nmsg, msg, msg->len);
				}
				return nmsg;
			}
	};
} } }

#endif
