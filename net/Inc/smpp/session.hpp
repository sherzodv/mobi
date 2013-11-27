#ifndef smpp_session_hpp
#define smpp_session_hpp

#include <string>
#include <cstdint>
#include <smpp/proto.hpp>

namespace smpp {

struct session {

		const proto::u8_t interface_version;
		const proto::u8_t addr_ton;
		const proto::u8_t addr_npi;

		const std::string sys_id;
		const std::string sys_type;
		const std::string addr_range;

		session() = delete;
		session(const session &) = delete;
		session & operator=(const session &) = delete;

		session(proto::u8_t iv, proto::u8_t at, proto::u8_t an
				, const char * sid, const char *st, const char *ar)
			: interface_version(iv)
			, addr_ton(at)
			, addr_npi(an)
			, sys_id(sid)
			, sys_type(st)
			, addr_range(ar)
		{
		}

		virtual ~session() {}

		virtual void send(pdu * msg);
		virtual void recv();

		/* flush does not neccesseraly sends all messages in
		 * out queue. It may stop sending in case of error. Call
		 * it repeatedly and handle errors until ready_to_send() is true */
		virtual void flush();

		virtual bool ready_to_send() const;
		virtual bool ready_to_recv() const;
};

}

#endif
