#ifndef fastmq_talker_hpp
#define fastmq_talker_hpp

#include <cstddef>
#include "proto.hpp"

namespace fastmq {

	struct msgu;
	class router_base;

	class subscriber_base {

		friend class router_base;

		public:
			subscriber_base()
				: m_id(0), m_type(0), m_router(nullptr) {}

			virtual ~subscriber_base() {}
			virtual void process_message(const msgu * msg) = 0;

			u16_t id() const { return m_id; }
			u16_t type() const { return m_type; }

		protected:
			u16_t m_id;
			u16_t m_type;
			router_base * m_router;
	};

}

#endif
