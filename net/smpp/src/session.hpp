#ifndef smpp_session_hpp
#define smpp_session_hpp

#include <string>
#include <cstdint>
#include <smpp/proto.hpp>

namespace mobi { namespace net { namespace smpp {

struct session {

		proto::u32_t type;

		proto::u8_t interface_version;
		proto::u8_t addr_ton;
		proto::u8_t addr_npi;

		std::string sys_id;
		std::string sys_type;
		std::string addr_range;

		std::string password;

		session() {}
		session(const session &) = delete;
		session & operator=(const session &) = delete;

		virtual ~session() {}

		virtual void send(pdu * msg) = 0;
		virtual void recv() = 0;

		/* flush does not neccesseraly sends all messages in
		 * out queue. It may stop sending in case of error. Call
		 * it repeatedly and handle errors until ready_to_send() is true */

		virtual void flush() = 0;

		virtual bool ready_to_send() const = 0;
		virtual bool ready_to_recv() const = 0;

		template <class BindT>
		void set_properties(const BindT & bind) {
			type = bind.command.id;
			interface_version = bind.interface_version;
			addr_ton = bind.addr_ton;
			addr_npi = bind.addr_npi;
			addr_range.assign(reinterpret_cast<const char *>(bind.addr_range));
			sys_id.assign(reinterpret_cast<const char *>(bind.sys_id));
			sys_type.assign(reinterpret_cast<const char *>(bind.sys_type));
			password.assign(reinterpret_cast<const char *>(bind.password));
		}
};

} } }

#endif
