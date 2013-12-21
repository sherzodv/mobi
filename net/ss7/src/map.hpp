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

	const std::size_t max_address_string_len = 20;
	const std::size_t max_isdn_address_string_len = 9;

	typedef proto::u8_t address_string[max_address_string_len];
	typedef proto::u8_t isdn_address_string[max_isdn_address_string_len];

	void bcd_decode_z(char * dst, const proto::u8_t * src
			, std::size_t len, bool odd = false) {
		while (len) {
			*dst++ = (*src & 0x0F) + 0x30;
			if (odd && len == 1)
				break;
			*dst++ = ((*src & 0xF0) >> 4) + 0x30;
			src++;
			len--;
		}
		*dst = 0;
	}

	struct ms_isdn {
		proto::u8_t data[20];
		std::size_t len;
	};

	enum sm_rp_mti {
		sms_deliver			= 0x00,
		sms_status_report	= 0x01
	};

	namespace operation {

		const proto::u32_t send_routing_info_for_sm	= 0x0000002D;

		struct routing_info_for_sm_arg {
			ms_isdn		msisdn;
			bool		sm_rp_pri;
			ms_isdn		sc_address;
		};

		struct empty {
		};

	}

	template <class LogT>
	class parser: public tcap::parser<LogT> {
		public:
			parser(LogT & l): tcap::parser<LogT>(l), L(l) {}
			virtual ~parser() {}

		protected:
			LogT & L;

			using typename tcap::parser<LogT>::action;
			using tcap::parser<LogT>::stop;
			using tcap::parser<LogT>::resume;
			using tcap::parser<LogT>::skip;

			virtual action on_message(const operation::empty & msg) = 0;
			virtual action send_routing_info_for_sm(const operation::routing_info_for_sm_arg & msg) = 0;

		private:

			action on_uni(const tcap::element::uni & el) {
				(void)(el);
				return stop;
			}

			action on_begin(const tcap::element::begin & el) {
				using tcap::operator<<;
				L << std::hex << el << std::endl;
				return resume;
			}

			action on_end(const tcap::element::end & el) {
				using tcap::operator<<;
				L << el << std::endl;
				return resume;
			}

			action on_resume(const tcap::element::resume & el) {
				(void)(el);
				return resume;
			}

			action on_abort(const tcap::element::abort & el) {
				(void)(el);
				return resume;
			}

			action on_invoke(const tcap::element::invoke & inv) {
				using namespace asn::ber;
				using tcap::operator<<;

				const proto::u8_t * buf, * bend;

				L << inv << std::endl;

				buf = inv.data;
				bend = inv.dend;

				switch (inv.op_code) {
					case operation::send_routing_info_for_sm:
						buf = parse_routing_info_for_sm_arg(buf, bend);
						if (buf == nullptr) {
							return stop;
						}
						return resume;
					default: return stop; /* not supported */
				}

				/* Skip payload of an invoke, as we parsed it already */
				return skip;
			}

			action on_return_result_last(const tcap::element::return_result & el) {
				using tcap::operator<<;
				L << el << std::endl;
				return resume;
			}

			action on_primitive(asn::ber::tag el, const tcap::proto::u8_t * data) {
				(void)(data);
				L << el << std::endl;
				return resume;
			}

			action on_constructor_start(asn::ber::tag el, const tcap::proto::u8_t * data) {
				(void)(data);
				L << el << std::endl;
				return resume;
			}

			action on_constructor_end() {
				return resume;
			}

			const proto::u8_t * parse_routing_info_for_sm_arg(const proto::u8_t * buf
					, const proto::u8_t * bend) {
				using namespace asn::ber;

				/* RoutingInfoForSM-Arg SEQUENCE {
					msisdn						[0] IMPLICIT OCTET STRING ( SIZE( 1 .. 20 ) ) ( SIZE( 1 .. 9 ) )
					, sm-RP-PRI					[1] IMPLICIT BOOLEAN
					, serviceCentreAddress		[2] IMPLICIT OCTET STRING ( SIZE( 1 .. 20 ) )
					, extensionContainer		[6] IMPLICIT SEQUENCE {
						privateExtensionList	[0] IMPLICIT SEQUENCE  ( SIZE( 1 .. 10 ) ) OF SEQUENCE {
							extId		MAP-EXTENSION .&extensionId  ( { , ... } )
							, extType	MAP-EXTENSION .&ExtensionType  ( { , ... } { @extId }  ) OPTIONAL
						} OPTIONAL
						, pcs-Extensions		[1] IMPLICIT SEQUENCE { ... } OPTIONAL
						, ...
					} OPTIONAL
					, ...
					, gprsSupportIndicator		[7] IMPLICIT NULL OPTIONAL
					, sm-RP-MTI					[8] IMPLICIT INTEGER ( 0 .. 10 ) OPTIONAL
					, sm-RP-SMEA				[9] IMPLICIT OCTET STRING ( SIZE( 1 .. 12 ) ) OPTIONAL
				}*/

				tag t;
				operation::routing_info_for_sm_arg ri;

				/* Parse SEQUENCE tag */
				buf = parse_tag(t, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr || t != type::sequence);

				/* Parse msisdn */
				buf = parse_el_octstring(ri.msisdn, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);

				/* Parse sm-RP-PRI tag */
				buf = parse_el_boolean(ri.sm_rp_pri, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);

				/* Parse serviceCentreAddress */
				buf = parse_el_octstring(ri.sc_address, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);

				switch (send_routing_info_for_sm(ri)) {
					case resume: return buf;
					default: return nullptr;
				}
			}
	};

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const ms_isdn & n) {
		char num[50];
		bcd_decode_z(num, n.data, n.len);
		L << "[msisdn:" << num << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const operation::routing_info_for_sm_arg & op) {
		L << "[routing info for sm arg:"
			<< op.msisdn
		<< "]";
		return L;
	}

} }

#endif
