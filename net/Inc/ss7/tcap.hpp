#ifndef mobi_net_ss7_tcap_hpp
#define mobi_net_ss7_tcap_hpp

#include <bitset>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <ostream>
#include <arpa/inet.h>

namespace ss7 { namespace tcap {

	namespace proto {
		typedef std::uint8_t	u8_t;
		typedef std::uint16_t	u16_t;
		typedef std::uint32_t	u32_t;
		typedef std::uint32_t	u64_t;

		typedef std::int8_t		s8_t;
		typedef std::int16_t	s16_t;
		typedef std::int32_t	s32_t;
		typedef std::int32_t	s64_t;
	}

	enum message_type {
		mtype_uni		= 0x61,
		mtype_begin		= 0x62,
		mtype_end		= 0x64,
		mtype_continue	= 0x65,
		mtype_abort		= 0x66,
	};

	enum component_tag {
		component_invoke = 0x6C
	};

	namespace message_code {
		const proto::u8_t mcode_uni				= 0x01;
		const proto::u8_t mcode_begin			= 0x02;
		const proto::u8_t mcode_end				= 0x04;
		const proto::u8_t mcode_continue		= 0x05;
		const proto::u8_t mcode_abort			= 0x06;

		const proto::u8_t mcode_torig_id		= 0x00;
		const proto::u8_t mcode_tdest_id		= 0x01;
		const proto::u8_t mcode_pabort_cause	= 0x0A;
		const proto::u8_t mcode_dialog			= 0x0B;
		const proto::u8_t mcode_component		= 0x0C;

		const proto::u8_t mcode_invoke			= 0x01;
		const proto::u8_t mcode_rresult_last	= 0x02;
		const proto::u8_t mcode_return_error	= 0x03;
		const proto::u8_t mcode_reject			= 0x04;
		const proto::u8_t mcode_return_result	= 0x05;
	}

	namespace element {

		struct invoke {
			proto::s8_t id;
			proto::s8_t linked_id;
			proto::u32_t opCode;
		};

		struct begin {
			proto::u64_t otid;
		};

	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, element::begin b) {
		L << "[begin:"
			<< "[otid:" << b.otid << "]"
		<< "]";
		return L;
	}

} }

#endif
