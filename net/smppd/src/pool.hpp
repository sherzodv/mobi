#ifndef smpp_message_pool_hpp
#define smpp_message_pool_hpp

#include <cstring>
#include <cstdlib>

#include <smpp/proto.hpp>

namespace mobi { namespace net { namespace smpp {

	class malloc_allocator {
		public:
			malloc_allocator() {
			}

			~malloc_allocator() {
			}

			void * alloc(std::size_t len) {
				return static_cast<pdu *>(std::malloc(len));
			}

			template <typename T>
			void dealloc(T * ptr) {
				std::free(static_cast<void*>(ptr));
			}
	};

} } }

#endif
