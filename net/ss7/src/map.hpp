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

namespace mobi { namespace net { namespace ss7 { namespace map {

	using namespace toolbox;

	template <typename IntT, typename RetT = bin::u8_t>
	inline RetT bcd_encode(IntT v) {
		switch (v) {
			case '0': return 0x00;
			case '1': return 0x01;
			case '2': return 0x02;
			case '3': return 0x03;
			case '4': return 0x04;
			case '5': return 0x05;
			case '6': return 0x06;
			case '7': return 0x07;
			case '8': return 0x08;
			case '9': return 0x09;
			case '*': return 0x0A;
			case '#': return 0x0B;
			case 'a': return 0x0C;
			case 'b': return 0x0D;
			case 'c': return 0x0E;
			case 'd': return 0x0F;
			default: return 0x0F;
		}
	}

	template <typename IntT, typename RetT = bin::u8_t>
	inline RetT bcd_decode(IntT v) {
		switch (v) {
			case 0x00: return '0';
			case 0x01: return '1';
			case 0x02: return '2';
			case 0x03: return '3';
			case 0x04: return '4';
			case 0x05: return '5';
			case 0x06: return '6';
			case 0x07: return '7';
			case 0x08: return '8';
			case 0x09: return '9';
			case 0x0A: return '*';
			case 0x0B: return '#';
			case 0x0C: return 'a';
			case 0x0D: return 'b';
			case 0x0E: return 'c';
			case 0x0F: return 'd';
			default: return '\0';
		}
	}

	template <typename DU8T, typename SU8T>
	bin::sz_t bcd_encode(DU8T * dst, const SU8T * src
			, bin::sz_t len) {
		bin::sz_t rlen = 0;
		if (len == 0)
			return rlen;
		while (len--) {
			*dst = bcd_encode(*src++);
			++rlen;
			if (len) {
				*dst |= bcd_encode(*src++) << 4;
				--len;
			} else {
				*dst |= 0xF0;
			}
			++dst;
		}
		return rlen;
	}

	template <typename DU8T, typename SU8T>
	bin::sz_t bcd_decode(DU8T * dst, const SU8T * src
			, bin::sz_t len, bool odd = false) {
		bin::sz_t rlen = 0;
		if (len == 0)
			return rlen;
		while (len--) {
			*dst = bcd_decode(*src & 0x0F);
			if (*dst != 'd') {
				++dst;
				++rlen;
			}
			if (odd && len == 0)
				break;
			*dst = bcd_decode((*src++ & 0xF0) >> 4);
			if (*dst != 'd') {
				++dst;
				++rlen;
			}
		}
		return rlen;
	}

	template <class StringT>
	void bcd_decode(StringT & dst, const StringT & src, bool odd = false) {
		dst.len = bcd_decode(dst.data, src.data, src.len, odd);
	}

	template <class StringT>
	void bcd_encode(StringT & dst, const StringT & src) {
		dst.len = bcd_encode(dst.data, src.data, src.len);
	}

	template <class SU8T>
	std::string bcd_decode(const SU8T * src, bin::sz_t len, bool odd = false) {
		SU8T buf[1024];
		len = bcd_decode(buf, src, len, odd);
		return std::string(reinterpret_cast<char *>(buf), len);
	}

	namespace {

		enum nature_of_address {
			na_unknown				= 0x0,
			na_international		= 0x1,
			na_national_significant	= 0x2,
			na_network_specific		= 0x3,
			na_subscriber			= 0x4,
			na_reserved				= 0x5,
			na_abbreviated			= 0x6,
			na_reserved_for_ext		= 0x7
		};

		enum numbering_plan {
			np_unknown			= 0x0,
			np_isdn_telephony	= 0x1,
			np_spare_1			= 0x2,
			np_data				= 0x3,
			np_telex			= 0x4,
			np_spare_2			= 0x5,
			np_land_mobile		= 0x6,
			np_spare_3			= 0x7,
			np_national			= 0x8,
			np_private			= 0x9,
			np_reserved_1		= 0xA,
			np_reserved_2		= 0xB,
			np_reserved_3		= 0xC,
			np_reserved_4		= 0xD,
			np_reserved_5		= 0xE,
			np_reserved_for_ext	= 0xF
		};

		enum sm_rp_mti_t {
			sms_deliver			= 0x00,
			sms_status_report	= 0x01
		};

		template <bin::sz_t MaxLen>
		struct string_tt {
			bin::sz_t len;
			bin::u8_t data[MaxLen];

			string_tt(): len(0), data{0} {}

			constexpr bin::sz_t maxlen() {
				return MaxLen;
			}

			bin::sz_t length() const {
				return len;
			}

			bin::sz_t size(bin::sz_t code = 0) const {
				return asn::ber::element_size(code, length());
			}
		};

		template <bin::sz_t MaxLen>
		struct address_string_tt {

			/* The following fields are packed and aligned correctly
			 * do not reorder them until you know what you are doing */

			numbering_plan		np: 4;
			nature_of_address	na: 3;
			bin::u8_t			ext: 1;
			bin::u8_t			data[MaxLen - 1];

			bin::sz_t len;

			address_string_tt(): np(np_unknown), na(na_unknown), ext(1)
								 , data{0}, len(0) {}

			constexpr bin::sz_t maxlen() {
				return MaxLen;
			}

			bin::sz_t length() const {
				return len + 1;
			}

			bin::sz_t size(bin::sz_t code = 0) const {
				return asn::ber::element_size(code, length());
			}

			std::string digits() const {
				return bcd_decode(data, len, false);
			}

			void set_digits(const std::string & d) {
				len = bcd_encode(data, d.data(), d.size());
			}
		};

		template <bin::sz_t MaxLen>
		struct tbcd_string_tt {
			bin::sz_t len;
			bin::u8_t data[MaxLen];

			tbcd_string_tt(): len(0), data{0} {}

			std::string digits() const {
				return bcd_decode(data, len, false);
			}

			void set_digits(const std::string & d) {
				len = bcd_encode(data, d.data(), d.size());
			}

			constexpr bin::sz_t maxlen() { return MaxLen; }

			bin::sz_t length() const {
				return len;
			}

			bin::sz_t size(bin::sz_t code = 0) const {
				return asn::ber::element_size(code, length());
			}
		};

		/* TS 129 002, 17.7.8: Common data types:
		 * See definition in parser */
		typedef address_string_tt<20> address_string_t;
		typedef address_string_tt<9> isdn_address_string_t;

		/* TS 129 002, 17.7.8: Common data types
		 * IMSI ::= TBCD-STRING (SIZE (3..8))
		 * TMSI ::= TBCD-STRING (SIZE (1..4))
		 * LMSI ::= TBCD-STRING (SIZE (4))
		 */
		typedef tbcd_string_tt<8> imsi_t;
		typedef tbcd_string_tt<4> tmsi_t;
		typedef tbcd_string_tt<4> lmsi_t;

		/* TS 129 002, 17.7.8: Common data types
		 * SignalInfo ::= OCTET STRING (SIZE (1..maxSignalInfoLength))
		 * maxSignalInfoLength INTEGER ::= 200
		 * -- This NamedValue represents the theoretical maximum number
		 * -- of octets which is available to carry a single instance of
		 * -- the SignalInfo data type, without requiring segmentation
		 * -- to cope with the network layer service.
		 * -- However, the actual maximum size available for an instance
		 * -- of the data type maybe lower, especially when other information
		 * -- elements have to be included in the same component.
		 */
		typedef string_tt<200> signal_info_t;

		struct sm_rp_da_t {
			enum { has_imsi, has_lmsi, has_sc, has_nothing } has;
			union sm_rp_da_t_field {
				imsi_t imsi;
				lmsi_t lmsi;
				address_string_t sc_address_da;
				sm_rp_da_t_field(): imsi() {}
			} as ;

			bin::sz_t length() const {
				switch (has) {
					case has_imsi:
						return as.imsi.length();
					case has_lmsi:
						return as.lmsi.length();
					case has_sc:
						return as.sc_address_da.length();
					default:
						return 0;
				}
			}

			bin::sz_t size(bin::u64_t code = 0) const {
				return asn::ber::element_size(code, length());
			}
		};

		struct sm_rp_oa_t {
			enum { has_msisdn, has_sc, has_nothing } has;
			union sm_rp_oa_t_as_field {
				isdn_address_string_t msisdn;
				address_string_t sc_address_oa;
				sm_rp_oa_t_as_field(): msisdn() {}
			} as;

			bin::sz_t length() const {
				switch (has) {
					case has_msisdn:
						return as.msisdn.length();
					case has_sc:
						return as.sc_address_oa.length();
					default:
						return 0;
				}
			}

			bin::sz_t size(bin::u64_t code = 0) const {
				return asn::ber::element_size(code, length());
			}
		};

		struct location_info_with_lmsi_t {
			isdn_address_string_t	network_node_number;
			lmsi_t					lmsi;

			bin::sz_t length() const {
				return network_node_number.size(1)
					+ lmsi.size();
			}

			bin::sz_t size(bin::u64_t code = 0) const {
				return asn::ber::element_size(code, length());
			}
		};

		struct routing_info_for_sm_arg_t {
			isdn_address_string_t	msisdn;
			bool					sm_rp_pri;
			address_string_t		sc_address;

			bin::sz_t length() const {
				return msisdn.size(0)
					+ asn::ber::element_size(1, 1)
					+ sc_address.size(2);
			}

			bin::sz_t size(bin::u64_t code = 0) const {
				return asn::ber::element_size(code, length());
			}
		};

		struct routing_info_for_sm_res_t {
			imsi_t						imsi;
			location_info_with_lmsi_t	location_info_with_lmsi;

			bin::sz_t length() const {
				return imsi.size()
					+ location_info_with_lmsi.size(0);
			}

			bin::sz_t size(bin::u64_t code = 0) const {
				return asn::ber::element_size(code, length());
			}
		};

		struct mo_forward_sm_arg_t {
			sm_rp_da_t		sm_rp_da;
			sm_rp_oa_t		sm_rp_oa;
			signal_info_t	sm_rp_ui;
			bool			has_imsi;
			imsi_t			imsi;

			bin::sz_t length() const {
				return sm_rp_da.size()
					+ sm_rp_oa.size()
					+ sm_rp_ui.size()
					+ (has_imsi ? imsi.size() : 0);
			}

			bin::sz_t size(bin::u64_t code = 0) const {
				return asn::ber::element_size(code, length());
			}
		};

		struct mo_forward_sm_res_t {
			signal_info_t	sm_rp_ui;

			bin::sz_t length() const {
				return sm_rp_ui.size();
			}

			bin::sz_t size(bin::u64_t code = 0) const {
				return asn::ber::element_size(code, length());
			}
		};

	}

	namespace operation {
		const bin::u32_t send_routing_info_for_sm	= 0x0000002D;
		const bin::u32_t mo_forward_sm				= 0x0000002E;
	}

	/* MAP parser is based on tcap::parser, it overrides needed virtual
	 * functions and adds new ones for MAP events */

	template <class LogT>
	class parser: public tcap::parser<LogT> {

		typedef tcap::parser<LogT> base;

		public:
			parser(LogT & l): base(l) {}
			virtual ~parser() {}

		protected:
			using base::L;
			using base::stop;
			using base::resume;
			using base::skip;
			typedef typename base::action action;

			virtual action on_routing_info_for_sm_arg(const routing_info_for_sm_arg_t & msg) = 0;
			virtual action on_routing_info_for_sm_res(const routing_info_for_sm_res_t & msg) = 0;
			virtual action on_mo_forward_sm_arg(const mo_forward_sm_arg_t & msg) = 0;
			virtual action on_mo_forward_sm_res(const mo_forward_sm_res_t & msg) = 0;

		private:

			action on_uni(const tcap::element::uni & el) {
				(void)(el);
				return stop;
			}

			action on_begin(const tcap::element::begin & el) {
				(void)(el);
				//L << std::hex << el << std::endl;
				return resume;
			}

			action on_end(const tcap::element::end & el) {
				(void)(el);
				//L << el << std::endl;
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

				const bin::u8_t * buf, * bend;

				buf = inv.data;
				bend = inv.dend;

				if (inv.op_code.type == tcap::element::operation_code::local) {
					/* Skip payload of an invoke, as we parsed it already */
					switch (inv.op_code.as.local) {
						case operation::send_routing_info_for_sm:
							buf = parse_routing_info_for_sm_arg(buf, bend);
							if (buf == nullptr) {
								return stop;
							}
							return skip;
						case operation::mo_forward_sm:
							buf = parse_mo_forward_sm_arg(buf, bend);
							if (buf == nullptr) {
								return stop;
							}
							return skip;
						default: return stop; /* not supported */
					}
				} else {
					/* TODO: implement working with oids */
					return stop;
				}
			}

			action on_return_result_last(const tcap::element::return_result & r) {
				using namespace asn::ber;

				const bin::u8_t * buf, * bend;

				buf = r.data;
				bend = r.dend;

				if (r.op_code.type == tcap::element::operation_code::local) {
					/* Skip payload of an return_result_last, as we parsed it already */
					switch (r.op_code.as.local) {
						case operation::send_routing_info_for_sm:
							buf = parse_routing_info_for_sm_res(buf, bend);
							if (buf == nullptr) {
								return stop;
							}
							return skip;
						case operation::mo_forward_sm:
							buf = parse_mo_forward_sm_res(buf, bend);
							if (buf == nullptr) {
								return stop;
							}
							return skip;
						default: return stop; /* not supported */
					}
				} else {
					/* TODO: implement working with oids */
					return stop;
				}
			}

			action on_primitive(asn::ber::tag el, const tcap::bin::u8_t * data) {
				(void)(data);
				L << el << std::endl;
				return resume;
			}

			action on_constructor_start(asn::ber::tag el, const tcap::bin::u8_t * data) {
				(void)(data);
				L << el << std::endl;
				return resume;
			}

			action on_constructor_end() {
				return resume;
			}

			const bin::u8_t * parse_routing_info_for_sm_arg(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;

				/* RoutingInfoForSM-Arg SEQUENCE {
					msisdn						[0] ISDN-AddressString
					, sm-RP-PRI					[1] IMPLICIT BOOLEAN
					, serviceCentreAddress		[2] AddressString
					, extensionContainer		[6] ExtensionContainer OPTIONAL
					, ...
					, gprsSupportIndicator		[7] NULL OPTIONAL
					-- gprsSupportIndicator is set only if the SMS-GMSC supports
					-- receiving of two numbers from HLR
					, sm-RP-MTI					[8] SM-RP-MTI OPTIONAL
					, sm-RP-SMEA				[9] SM-RP-SMEA OPTIONAL
					, sm-deliveryNotIntended	[10] SM-DeliveryNotIntended OPTIONAL
					, ip-sm-gwGuidanceIndicator	[11] NULL OPTIONAL
					, imsi						[12] IMSI OPTIONAL
					, t4-Trigger-Indicator		[14] NULL OPTIONAL
				}*/

				tag t;
				routing_info_for_sm_arg_t ri;

				/* Parse SEQUENCE tag */
				buf = base::parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr || t != type::sequence);

				/* Parse msisdn */
				buf = parse_address_string(ri.msisdn, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				/* Parse sm-RP-PRI */
				buf = base::parse_boolean(ri.sm_rp_pri, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				/* Parse serviceCentreAddress */
				buf = parse_address_string(ri.sc_address, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				switch (on_routing_info_for_sm_arg(ri)) {
					/* We should return bend, as there maybe unparsed optional
					 * elements */
					case resume: return bend;
					default: return nullptr;
				}
			}

			const bin::u8_t * parse_routing_info_for_sm_res(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;

				/* RoutingInfoForSM-Res :: = SEQUENCE {
					imsi					IMSI
					, locationInfoWithLMSI	[0] LocationInfoWithLMSI
					, extensionContainer	[4] ExtensionContainer OPTIONAL
					, ...
					, ip-sm-gwGuidance		[5] IP-SM-GW-Guidance OPTIONAL
				}*/

				tag t;
				routing_info_for_sm_res_t ri;

				/* Parse SEQUENCE tag */
				buf = base::parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr || t != type::sequence);

				/* Parse imsi */
				buf = parse_tbcd_string(ri.imsi, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				/* Parse locationInfoWithLMSI */
				buf = parse_location_info_with_lmsi(ri.location_info_with_lmsi
						, buf, bend);

				switch (on_routing_info_for_sm_res(ri)) {
					/* We should return bend, as there maybe unparsed optional
					 * elements */
					case resume: return bend;
					default: return nullptr;
				}
			}

			const bin::u8_t * parse_mo_forward_sm_arg(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;

				/* MO-ForwardSM-Arg ::= SEQUENCE {
					sm-RP-DA				SM-RP-DA
					, sm-RP-OA				SM-RP-OA
					, sm-RP-UI				SignalInfo
					, extensionContainer	ExtensionContainer OPTIONAL
					, ...
					, imsi					IMSI OPTIONAL
				}*/

				tag t;
				const bin::u8_t * tmpbuf;
				mo_forward_sm_arg_t ri;

				/* Parse SEQUENCE tag */
				buf = base::parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr || t != type::sequence);

				buf = parse_sm_rp_da(ri.sm_rp_da, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				buf = parse_sm_rp_oa(ri.sm_rp_oa, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				buf = base::parse_octstring(ri.sm_rp_ui, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				tmpbuf = base::parse_octstring(ri.imsi, buf, bend);
				if (tmpbuf != nullptr) {
					buf = tmpbuf;
					ri.has_imsi = true;
				} else {
					ri.has_imsi = false;
				}

				switch (on_mo_forward_sm_arg(ri)) {
					/* We should return bend, as there maybe unparsed optional
					 * elements */
					case resume: return bend;
					default: return nullptr;
				}
			}

			const bin::u8_t * parse_mo_forward_sm_res(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;

				/* MO-ForwardSM-Res ::= SEQUENCE {
					sm-RP-UI				SignalInfo
					, extensionContainer	ExtensionContainer OPTIONAL
					, ...
				}*/

				tag t;
				mo_forward_sm_res_t ri;

				/* Parse SEQUENCE tag */
				buf = base::parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr || t != type::sequence);

				buf = base::parse_octstring(ri.sm_rp_ui, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				switch (on_mo_forward_sm_res(ri)) {
					/* We should return bend, as there maybe unparsed optional
					 * elements */
					case resume: return bend;
					default: return nullptr;
				}
			}

			const bin::u8_t * parse_location_info_with_lmsi(
					location_info_with_lmsi_t & linfo
					, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;

				/* LocationInfoWithLMSI :: = SEQUENCE {
					networkNodeNumber		[1] ISDN-AddressString
					, lmsi					LMSI OPTIONAL
					, extensionContainer	ExtensionContainer OPTIONAL
					, ...
					, gprsNodeIndicator		[5] NULL OPTIONAL
					-- gprsNodeIndicator is set only if the SGSN number is sent as the
					-- Network Node Number
					, additional-Number		[6] Additional-Number OPTIONAL
					, networkNodeDiameterAddress
											[7] NetworkNodeDiameterAddress OPTIONAL
					, additionalNetworkNodeDiameterAddress
											[8] NetworkNodeDiameterAddress OPTIONAL
					, thirdNumber			[9] Additional-Number OPTIONAL
					, thirdNetworkNodeDiameterAddress
											[10] NetworkNodeDiameterAddress OPTIONAL
					, imsNodeIndicator		[11] NULL OPTIONAL
					-- gprsNodeIndicator and imsNodeIndicator shall not both be present.
					-- additionalNumber and thirdNumber shall not both contain the same type of number.
				}*/

				tag t;
				const bin::u8_t * end = buf;

				/* Parse SEQUENCE tag */
				buf = base::parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				/* Remember the the end of the LocationInfoWithLMSI in
				 * the buffer */
				end += t.len;

				/* Parse networkNodeNumber */
				buf = parse_address_string(linfo.network_node_number
						, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				/* See if LMSI present: OCTET STRING (tag code = 0x04) */
				buf = base::parse_tag(t, buf, bend);
				if (buf != nullptr && t == type::octstring) {
					buf = parse_tbcd_string(linfo.lmsi, buf, bend);
				}

				return end;
			}

			const bin::u8_t * parse_sm_rp_da(
					sm_rp_da_t & rp
					, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;

				/* SM-RP-DA ::= CHOICE {
					imsi					[0] IMSI
					, lmsi					[1] LMSI
					, serviceCentreAddressDA[4] AddressString
					, noSM-RP-DA			[5] NULL
				}*/

				tag t;

				buf = base::parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr
						|| t.klass != tagclass_contextspec);

				switch (t.code) {
					case 0:
						rp.has = sm_rp_da_t::has_imsi;
						buf = base::parse_octstring(rp.as.imsi, t, buf, bend);
						return buf;
					case 1:
						rp.has = sm_rp_da_t::has_lmsi;
						buf = base::parse_octstring(rp.as.lmsi, t, buf, bend);
						return buf;
					case 4:
						rp.has = sm_rp_da_t::has_sc;
						buf = base::parse_octstring(rp.as.sc_address_da, t, buf, bend);
						return buf;
					case 5:
						buf += t.len;
						rp.has = sm_rp_da_t::has_nothing;
						return buf;
					default:
						return nullptr;
				}
			}

			const bin::u8_t * parse_sm_rp_oa(
					sm_rp_oa_t & rp
					, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;

				/* SM-RP-OA ::= CHOICE {
					msisdn					[2] ISDN-AddressString
					, serviceCentreAddressOA[4] AddressString
					, noSM-RP-OA			[5] NULL
				}*/

				tag t;

				buf = base::parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr
						|| t.klass != tagclass_contextspec);

				switch (t.code) {
					case 2:
						rp.has = sm_rp_oa_t::has_msisdn;
						buf = parse_address_string(rp.as.msisdn
								, t, buf, bend);
						return buf;
					case 4:
						rp.has = sm_rp_oa_t::has_sc;
						buf = parse_address_string(rp.as.sc_address_oa
								, t, buf, bend);
						return buf;
					case 5:
						buf += t.len;
						rp.has = sm_rp_oa_t::has_nothing;
						return buf;
					default:
						return nullptr;
				}
			}

			template <bin::sz_t MaxLen>
			const bin::u8_t * parse_address_string(address_string_tt<MaxLen> & r
					, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				/*
				 * AddressString ::= OCTET STRING (SIZE (1..maxAddressLength))
				 * this type is used to represent a number for addressing
				 * purposes. It is composed of
				 *		a) one octet for nature of address, and numbering plan
				 *		indicator.
				 *		b) digits of an address encoded as TBCD-STRING.
				 *
				 *	a) The first octet includes the one bit extension
				 *	indicator, a 3 bits nature of address indicator and a 4
				 *	bits numbering plan indicator, encoded as follows:
				 *		See enums: nature_of_address, numbering_plan
				 *
				 *	b) The following octets representing digits of an
				 *	address encoded as TBCD-STRING.
				 *
				 *	ISDN-AddressString ::= AddressString(SIZE (1..maxISDN-AddressLength))
				 *	used to represend ISDN numbers
				 */
				using namespace asn::ber;
				using namespace bin;

				tag t;

				buf = base::parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(buf + t.len > bend || t.len > r.maxlen());

				r.len = t.len - 1;

				buf = p::cp_u8(asbuf(&r), buf);
				return p::cpy(r.data, buf, t.len - 1);
			}

			template <bin::sz_t MaxLen>
			const bin::u8_t * parse_address_string(address_string_tt<MaxLen> & r
					, const asn::ber::tag & t
					, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;
				RETURN_NULL_IF(buf + t.len > bend || t.len > r.maxlen());
				r.len = t.len - 1;
				buf = p::cp_u8(asbuf(&r), buf);
				return p::cpy(r.data, buf, t.len - 1);
			}

			template <bin::sz_t MaxLen>
			const bin::u8_t * parse_tbcd_string(tbcd_string_tt<MaxLen> & r
					, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;
				using namespace asn::ber;

				tag t;

				buf = base::parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr || t.form != tagform_primitive);
				RETURN_NULL_IF(buf + t.len > bend || t.len > r.maxlen());

				r.len = t.len;

				return p::cpy(r.data, buf, t.len);
			}

			template <bin::sz_t MaxLen>
			const bin::u8_t * parse_tbcd_string(tbcd_string_tt<MaxLen> & r
					, const asn::ber::tag & t
					, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;
				RETURN_NULL_IF(buf + t.len > bend || t.len > r.maxlen());
				r.len = t.len;
				return p::cpy(r.data, buf, t.len - 1);
			}
	};

	template <class LogT>
	class writer: public tcap::writer<LogT> {

		typedef tcap::writer<LogT> base;

		public:
			writer(LogT & l): base(l) {}
			virtual ~writer() {}

		protected:
			using base::L;

			bin::u8_t * write_routing_info_for_sm_arg(bin::u8_t * buf
					, bin::u8_t * bend, const routing_info_for_sm_arg_t & r) {
				using namespace asn::ber;

				RETURN_NULL_IF(r.msisdn.len == 0 || r.sc_address.len == 0);

				buf = base::write_tag(buf, bend, type::sequence, r.length());
				RETURN_NULL_IF(buf == nullptr);

				buf = write_address_string(buf, bend, tagclass_contextspec
						, 0, r.msisdn);
				RETURN_NULL_IF(buf == nullptr);

				buf = base::write_boolean(buf, bend, tagclass_contextspec
						, 1, r.sm_rp_pri);
				RETURN_NULL_IF(buf == nullptr);

				buf = write_address_string(buf, bend, tagclass_contextspec
						, 2, r.sc_address);
				RETURN_NULL_IF(buf == nullptr);

				return buf;
			}

			bin::u8_t * write_routing_info_for_sm_res(bin::u8_t * buf
					, bin::u8_t * bend, const routing_info_for_sm_res_t & r) {
				using namespace asn::ber;

				buf = base::write_tag(buf, bend, type::sequence, r.length());
				RETURN_NULL_IF(buf == nullptr);

				buf = base::write_octstring(buf, bend, tagclass_universal
						, tagcode_octstring, r.imsi);
				RETURN_NULL_IF(buf == nullptr);

				return write_location_info_with_lmsi(buf, bend
						, r.location_info_with_lmsi);
			}

			bin::u8_t * write_mo_forward_sm_arg(bin::u8_t * buf
					, bin::u8_t * bend, const mo_forward_sm_arg_t & r) {
				using namespace asn::ber;

				buf = base::write_tag(buf, bend, type::sequence, r.length());
				RETURN_NULL_IF(buf == nullptr);

				buf = write_sm_rp_da(buf, bend, r.sm_rp_da);
				RETURN_NULL_IF(buf == nullptr);

				buf = write_sm_rp_oa(buf, bend, r.sm_rp_oa);
				RETURN_NULL_IF(buf == nullptr);

				buf = base::write_octstring(buf, bend, tagclass_universal
						, tagcode_octstring, r.sm_rp_ui);
				RETURN_NULL_IF(buf == nullptr);

				if (r.imsi.len > 0) {
					buf = base::write_octstring(buf, bend, tagclass_universal
							, tagcode_octstring, r.imsi);
				}

				return buf;
			}

			bin::u8_t * write_sm_rp_da(bin::u8_t * buf
					, bin::u8_t * bend, const sm_rp_da_t & r) {
				using namespace asn::ber;

				switch (r.has) {
					case sm_rp_da_t::has_imsi:
						return base::write_octstring(buf, bend
								, tagclass_contextspec
								, 0, r.as.imsi);
					case sm_rp_da_t::has_lmsi:
						return base::write_octstring(buf, bend
								, tagclass_contextspec
								, 1, r.as.lmsi);
					case sm_rp_da_t::has_sc:
						return write_address_string(buf, bend
								, tagclass_contextspec
								, 4, r.as.sc_address_da);
					default:
						return base::write_null(buf, bend
								, tagclass_contextspec
								, 5);
				}
			}

			bin::u8_t * write_sm_rp_oa(bin::u8_t * buf
					, bin::u8_t * bend, const sm_rp_oa_t & r) {
				using namespace asn::ber;

				switch (r.has) {
					case sm_rp_oa_t::has_msisdn:
						return write_address_string(buf, bend
								, tagclass_contextspec
								, 2, r.as.msisdn);
					case sm_rp_oa_t::has_sc:
						return write_address_string(buf, bend
								, tagclass_contextspec
								, 4, r.as.sc_address_oa);
					default:
						return base::write_null(buf, bend
								, tagclass_contextspec
								, 5);
				}
			}

			bin::u8_t * write_location_info_with_lmsi(bin::u8_t * buf
					, bin::u8_t * bend, const location_info_with_lmsi_t & r) {
				using namespace asn::ber;

				/* Sequence with tag code = 0x00 */
				buf = base::write_tag(buf, bend, tagclass_contextspec
						, tagform_constructor, 0, r.length());
				RETURN_NULL_IF(buf == nullptr);

				buf = write_address_string(buf, bend, tagclass_contextspec
						, 1, r.network_node_number);
				RETURN_NULL_IF(buf == nullptr);

				if (r.lmsi.len > 0) {
					return base::write_octstring(buf, bend, tagclass_universal
							, tagcode_octstring, r.lmsi);
				} else {
					return buf;
				}
			}

			template <bin::sz_t MaxLen>
			bin::u8_t * write_tbcd_string(bin::u8_t * buf, bin::u8_t * bend
					, asn::ber::tag_class klass
					, bin::u64_t code, const tbcd_string_tt<MaxLen> & r) {
				using namespace asn::ber;

				RETURN_NULL_IF(buf + element_size(code, r.length()) > bend);

				buf = base::write_tag(buf, bend
						, klass, tagform_primitive, code, r.length());
				RETURN_NULL_IF(buf == nullptr);

				return bin::w::cpy(buf, r.data, r.len);
			}

			template <bin::sz_t MaxLen>
			bin::u8_t * write_address_string(bin::u8_t * buf
					, bin::u8_t * bend
					, asn::ber::tag_class klass
					, bin::u64_t code, const address_string_tt<MaxLen> & r) {
				using namespace bin;
				using namespace asn::ber;

				buf = base::write_tag(buf, bend
						, klass, tagform_primitive, code, r.length());
				RETURN_NULL_IF(buf == nullptr);
				buf = w::cp_u8(buf, ascbuf(&r));
				return w::cpy(buf, r.data, r.len);
			}
	};

	std::string to_string(nature_of_address r) {
		std::stringstream out;
		out << "[nai:";
		switch (r) {
			case na_unknown:
				out << "unknown";
				break;
			case na_international:
				out << "international";
				break;
			case na_national_significant:
				out << "national significant";
				break;
			case na_network_specific:
				out << "network specific";
				break;
			case na_subscriber:
				out << "subscriber";
				break;
			case na_abbreviated:
				out << "abbreviated";
				break;
			case na_reserved_for_ext:
				out << "reserved for ext";
				break;
			default:
				out << "reserved:" << bin::as<unsigned>(r);
				break;
		}
		out << "]";
		return out.str();
	}

	std::string to_string(numbering_plan r) {
		std::stringstream out;
		out << "[np:";
		switch (r) {
			case np_unknown:
				out << "unknown";
				break;
			case np_isdn_telephony:
				out << "isdn_telephony";
				break;
			case np_spare_1:
				out << "spare_1";
				break;
			case np_data:
				out << "data";
				break;
			case np_telex:
				out << "telex";
				break;
			case np_spare_2:
				out << "spare_2";
				break;
			case np_land_mobile:
				out << "land_mobile";
				break;
			case np_spare_3:
				out << "spare_3";
				break;
			case np_national:
				out << "national";
				break;
			case np_private:
				out << "private";
				break;
			case np_reserved_for_ext:
				out << "reserved for ext";
				break;
			default:
				out << "reserved:" << bin::as<unsigned>(r);
				break;
		}
		out << "]";
		return out.str();
	}

	template <bin::sz_t MaxLen>
	std::string to_string(const address_string_tt<MaxLen> & r) {
		std::stringstream out;
		out << to_string(r.na) << to_string(r.np)
			<< "[digits:" << r.digits() << "]";
		return out.str();
	}

	std::string to_string(const imsi_t & r) {
		return bcd_decode(r.data, r.len, true);
	}

	std::string to_string(const lmsi_t & r) {
		return bcd_decode(r.data, r.len, true);
	}

	std::string to_string(const sm_rp_da_t & r) {
		std::stringstream out;
		out << "[sm_rp_da:";
		switch (r.has) {
			case sm_rp_da_t::has_imsi:
				out << "[imsi:" << to_string(r.as.imsi) << "]";
				break;
			case sm_rp_da_t::has_lmsi:
				out << "[lmsi:" << to_string(r.as.lmsi) << "]";
				break;
			case sm_rp_da_t::has_sc:
				out << "[sc-addr:" << to_string(r.as.sc_address_da) << "]";
				break;
			case sm_rp_da_t::has_nothing:
				out << "absent";
				break;
			default:
				out << "wrong";
				break;
		}
		out << "]";
		return out.str();
	}

	std::string to_string(const sm_rp_oa_t & r) {
		std::stringstream out;
		out << "[sm_rp_oa:";
		switch (r.has) {
			case sm_rp_oa_t::has_msisdn:
				out << "[msisdn:" << to_string(r.as.msisdn) << "]";
				break;
			case sm_rp_oa_t::has_sc:
				out << "[sc-addr:" << to_string(r.as.sc_address_oa) << "]";
				break;
			case sm_rp_oa_t::has_nothing:
				out << "absent";
				break;
			default:
				out << "wrong";
				break;
		}
		out << "]";
		return out.str();
	}

	std::string to_string(const location_info_with_lmsi_t & r) {
		std::stringstream out;
		out << "[locationInfoWithLMSI:";
		if (r.lmsi.len > 0) {
			out << "[lmsi:" << to_string(r.lmsi) << "]";
		}
		out << "[networkNodeNumber:" << to_string(r.network_node_number) << "]"
		<< "]";
		return out.str();
	}

	std::string to_string(const routing_info_for_sm_arg_t & r) {
		std::stringstream out;
		out << "[RoutingInfoForSM-Arg:"
			<< "[msisdn:" << to_string(r.msisdn) << "]"
			<< "[sm_rp_pri:" << r.sm_rp_pri << "]"
			<< "[sc_address:" << to_string(r.sc_address) << "]"
		<< "]";
		return out.str();
	}

	std::string to_string(const routing_info_for_sm_res_t & r) {
		std::stringstream out;
		out << "[RoutingInfoForSM-Res:"
			<< "[imsi:" << to_string(r.imsi) << "]"
			<< to_string(r.location_info_with_lmsi)
		<< "]";
		return out.str();
	}

	std::string to_string(const mo_forward_sm_arg_t & r) {
		std::stringstream out;
		out << "[MO-ForwardSM-Arg:"
			<< to_string(r.sm_rp_da)
			<< to_string(r.sm_rp_oa);
		if (r.has_imsi)
			out << "[imsi:" << to_string(r.imsi) << "]";
		out << "[sm_rp_ui:"
			<< bin::hex_str_ref(r.sm_rp_ui.data, r.sm_rp_ui.len)
				.delimit("").prefix("") << "]";
		out << "]";
		return out.str();
	}

	std::string to_string(const mo_forward_sm_res_t & r) {
		std::stringstream out;
		out << "[MO-ForwardSM-Res:";
		out << "[sm_rp_ui:"
			<< bin::hex_str_ref(r.sm_rp_ui.data, r.sm_rp_ui.len)
				.delimit("").prefix("") << "]";
		out << "]";
		return out.str();
	}

} } } }

#endif
