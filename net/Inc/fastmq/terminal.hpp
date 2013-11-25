#ifndef fastmq_terminal_hpp
#define fastmq_terminal_hpp

#include <cstddef>
#include "proto.hpp"

namespace fastmq {

	struct msgu;
	class router_base;

	/* terminal_base is abstract base class for all
	 * message handling classes. Router routes message to
	 * terminal_base via process_message virtual call.
	 */

	class terminal_base {

		friend class router_base;

		public:
			terminal_base()
				: m_id(0), m_type(0), m_router(nullptr) {}
			terminal_base(u16_t id, u16_t type)
				: m_id(id), m_type(type), m_router(nullptr) {}

			virtual ~terminal_base() {}
			virtual void process_message(msgu * msg) = 0;

			u16_t id() const { return m_id; }
			u16_t type() const { return m_type; }

		protected:
			u16_t m_id;
			u16_t m_type;
			router_base * m_router;
	};

}

#endif
