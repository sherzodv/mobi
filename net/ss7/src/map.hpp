#ifndef mobi_net_ss7_map_hpp
#define mobi_net_ss7_map_hpp

#include <bitset>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <ostream>
#include <arpa/inet.h>

#include <ss7/sccp.hpp>
#include <ss7/tcap.hpp>

namespace ss7 { namespace map {

	namespace proto {
		typedef std::uint8_t	u8_t;
		typedef std::uint16_t	u16_t;
		typedef std::uint32_t	u32_t;
		typedef std::uint32_t	u64_t;
	}

	using sccp::numbering_plan;
	using sccp::nature_of_address;

	const std::size_t max_address_string_len = 20;
	const std::size_t max_isdn_address_string_len = 9;

	typedef proto::u8_t address_string[max_address_string_len];
	typedef proto::u8_t isdn_address_string[max_isdn_address_string_len];

	struct ms_isdn {
		proto::u8_t cc[5];
		proto::u8_t ndc[10];
		proto::u8_t sn[10];
	};

	enum sm_rp_mti {
		sms_deliver			= 0x00,
		sms_status_report	= 0x01
	};

	namespace operation {

		struct routing_info_for_sm_arg {
			ms_isdn		msisdn;
			bool		sm_rp_pri;
			ms_isdn		sc_address;
		};

	}

} }

#endif
