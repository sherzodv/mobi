#ifndef itcp_proto_hpp
#define itcp_proto_hpp

#include <memory>
#include "proto.hpp"

namespace fastmq { namespace proto {

class message_pool {
	public:
		u8_t * allocate(size_t sz) {
			return new (std::nothrow) u8_t[sz];
		}
		void dealloc(u8_t * const ptr) {
			delete [] ptr;
		}
};

} }

#endif
