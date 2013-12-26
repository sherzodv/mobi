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

	const bin::sz_t max_address_string_len = 20;
	const bin::sz_t max_isdn_address_string_len = 9;

	typedef bin::u8_t address_string[max_address_string_len];
	typedef bin::u8_t isdn_address_string[max_isdn_address_string_len];

	namespace {

		struct isdn_address_string_t {
			bin::sz_t len;
			/* TODO: determine needed capacity */
			bin::u8_t data[20];
		};

		struct address_string_t {
			bin::sz_t len;
			/* TODO: determine needed capacity */
			bin::u8_t data[20];
		};

		struct imsi_t {
			/* IMSI ::= TBCD-STRING (SIZE (3..8)) */
			/* Digits of MCC, MNC, MSIN are concatenated in this order */
			bin::sz_t len;
			/* TODO: determine needed capacity */
			bin::u8_t data[20];
		};

		struct tmsi_t {
			bin::sz_t len;
			/* TODO: determine needed capacity */
			bin::u8_t data[4];
		};

		struct lmsi_t {
			/* LMSI ::= OCTET STRING (SIZE (4)) */
			/* len = 0 indicates that lmsi is not set */
			bin::sz_t len;
			bin::u8_t data[4];
		};

		enum sm_rp_mti_t {
			sms_deliver			= 0x00,
			sms_status_report	= 0x01
		};

		struct sm_rp_da_t {
			enum { has_imsi, has_lmsi, has_sc, has_nothing } has;
			union {
				imsi_t imsi;
				lmsi_t lmsi;
				address_string_t sc_address_da;
			} as ;
		};

		struct sm_rp_oa_t {
			enum { has_msisdn, has_sc, has_nothing } has;
			union {
				isdn_address_string_t msisdn;
				address_string_t sc_address_oa;
			} as;
		};

		struct signal_info_t {
			bin::sz_t len;
			bin::u8_t data[200];
		};

		struct location_info_with_lmsi_t {
			isdn_address_string_t	network_node_number;
			lmsi_t					lmsi;
		};

		struct routing_info_for_sm_arg_t {
			isdn_address_string_t	msisdn;
			bool					sm_rp_pri;
			address_string_t		sc_address;
		};

		struct routing_info_for_sm_res_t {
			imsi_t						imsi;
			bool						has_location_info_with_lmsi;
			location_info_with_lmsi_t	location_info_with_lmsi;
		};

		struct mo_forward_sm_arg_t {
			sm_rp_da_t		sm_rp_da;
			sm_rp_oa_t		sm_rp_oa;
			signal_info_t	sm_rp_ui;
			bool			has_imsi;
			imsi_t			imsi;
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
		public:
			parser(LogT & l): tcap::parser<LogT>(l), L(l) {}
			virtual ~parser() {}

		protected:
			LogT & L;

			using typename tcap::parser<LogT>::action;
			using tcap::parser<LogT>::stop;
			using tcap::parser<LogT>::resume;
			using tcap::parser<LogT>::skip;

			virtual action on_routing_info_for_sm_arg(const routing_info_for_sm_arg_t & msg) = 0;
			virtual action on_routing_info_for_sm_res(const routing_info_for_sm_res_t & msg) = 0;
			virtual action on_mo_forward_sm_arg(const mo_forward_sm_arg_t & msg) = 0;

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

				const bin::u8_t * buf, * bend;

				buf = inv.data;
				bend = inv.dend;

				/* Skip payload of an invoke, as we parsed it already */
				switch (inv.op_code) {
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
			}

			action on_return_result_last(const tcap::element::return_result & rres) {
				using namespace asn::ber;
				using tcap::operator<<;

				const bin::u8_t * buf, * bend;

				buf = rres.data;
				bend = rres.dend;

				switch (rres.op_code) {
					case operation::send_routing_info_for_sm:
						buf = parse_routing_info_for_sm_res(buf, bend);
						if (buf == nullptr) {
							return stop;
						}
						/* Skip payload of an return_result_last,
						 * as we parsed it already */
						return skip;
					default: return stop; /* not supported */
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
				routing_info_for_sm_arg_t ri;

				/* Parse SEQUENCE tag */
				buf = parse_tag(t, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr || t != type::sequence);

				/* Parse msisdn */
				buf = parse_el_octstring(ri.msisdn, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);

				/* Parse sm-RP-PRI */
				buf = parse_el_boolean(ri.sm_rp_pri, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);

				/* Parse serviceCentreAddress */
				buf = parse_el_octstring(ri.sc_address, buf, bend, L);
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
				const bin::u8_t * tmpbuf;
				routing_info_for_sm_res_t ri;

				/* Parse SEQUENCE tag */
				buf = parse_tag(t, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr || t != type::sequence);

				/* Parse imsi */
				buf = parse_el_octstring(ri.imsi, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);

				/* Parse locationInfoWithLMSI */
				tmpbuf = parse_location_info_with_lmsi(ri.location_info_with_lmsi
						, buf, bend);
				if (tmpbuf == nullptr) {
					ri.has_location_info_with_lmsi = false;
				} else {
					buf = tmpbuf;
					ri.has_location_info_with_lmsi = true;
				}

				switch (on_routing_info_for_sm_res(ri)) {
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
					networkNodeNumber		[1] isdn_address_string_t
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
				const bin::u8_t * tmpbuf;

				/* Parse SEQUENCE tag, context-spec, 0 */
				buf = parse_tag(t, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr
						|| t.code != 0 || t.klass != tagclass_contextspec);

				/* Parse networkNodeNumber */
				buf = parse_el_octstring(linfo.network_node_number, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);

				tmpbuf = parse_el_octstring(linfo.lmsi, buf, bend, L);
				if (tmpbuf == nullptr) {
					linfo.lmsi.len = 0;
					return buf;
				} else {
					return tmpbuf;
				}
			}

			const bin::u8_t * parse_mo_forward_sm_arg(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;

				/* MO-Forward-SM-Arg ::= SEQUENCE {
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
				buf = parse_tag(t, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr || t != type::sequence);

				buf = parse_sm_rp_da(ri.sm_rp_da, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				buf = parse_sm_rp_oa(ri.sm_rp_oa, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				buf = parse_el_octstring(ri.sm_rp_ui, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);

				tmpbuf = parse_el_octstring(ri.imsi, buf, bend, L);
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

				buf = parse_tag(t, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr
						|| t.klass != tagclass_contextspec);

				switch (t.code) {
					case 0:
						rp.has = sm_rp_da_t::has_imsi;
						buf = parse_el_octstring(rp.as.imsi, t, buf, bend, L);
						return buf;
					case 1:
						rp.has = sm_rp_da_t::has_lmsi;
						buf = parse_el_octstring(rp.as.lmsi, t, buf, bend, L);
						return buf;
					case 4:
						rp.has = sm_rp_da_t::has_sc;
						buf = parse_el_octstring(rp.as.sc_address_da, t, buf, bend, L);
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

				buf = parse_tag(t, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr
						|| t.klass != tagclass_contextspec);

				switch (t.code) {
					case 2:
						rp.has = sm_rp_oa_t::has_msisdn;
						buf = parse_el_octstring(rp.as.msisdn, t, buf, bend, L);
						return buf;
					case 4:
						rp.has = sm_rp_oa_t::has_sc;
						buf = parse_el_octstring(rp.as.sc_address_oa, t, buf, bend, L);
						return buf;
					case 5:
						buf += t.len;
						rp.has = sm_rp_oa_t::has_nothing;
						return buf;
					default:
						return nullptr;
				}
			}

	};

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const isdn_address_string_t & n) {
		char num[50];
		bin::bcd_decode_z(num, n.data + 1, n.len - 1, true);
		L << bin::hex_str_ref(n.data, n.len).prefix("").delimit("") << ":" << num;
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const address_string_t & n) {
		char num[50];
		bin::bcd_decode_z(num, n.data + 1, n.len - 1, true);
		L << num;
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const imsi_t & n) {
		char num[50];
		bin::bcd_decode_z(num, n.data, n.len, true);
		L << num;
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const lmsi_t & n) {
		char num[50];
		bin::bcd_decode_z(num, n.data + 1, n.len - 1, true);
		L << num;
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const sm_rp_da_t & rp) {
		L << "[sm_rp_da:";
		switch (rp.has) {
			case sm_rp_da_t::has_imsi: L << "[imsi:" << rp.as.imsi << "]"; break;
			case sm_rp_da_t::has_lmsi: L << "[lmsi:" << rp.as.lmsi << "]"; break;
			case sm_rp_da_t::has_sc: L << "[sc-addr:" << rp.as.sc_address_da << "]"; break;
			case sm_rp_da_t::has_nothing: L << "absent"; break;
			default: L << "wrong"; break;
		}
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const sm_rp_oa_t & rp) {
		L << "[sm_rp_oa:";
		switch (rp.has) {
			case sm_rp_oa_t::has_msisdn: L << "[msisdn:" << rp.as.msisdn << "]"; break;
			case sm_rp_oa_t::has_sc: L << "[sc-addr:" << rp.as.sc_address_oa << "]"; break;
			case sm_rp_oa_t::has_nothing: L << "absent"; break;
			default: L << "wrong"; break;
		}
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const routing_info_for_sm_arg_t & el) {
		L << "[RoutingInfoForSM-Arg:"
			<< "[msisdn:" << el.msisdn << "]"
			<< "[sm_rp_pri:" << el.sm_rp_pri << "]"
			<< "[sc_address:" << el.sc_address << "]"
		<< "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const routing_info_for_sm_res_t & el) {
		L << "[RoutingInfoForSM-Res:"
			<< "[imsi:" << el.imsi << "]"
		<< "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const mo_forward_sm_arg_t & el) {
		L << "[MO-ForwardSM-Arg:" << el.sm_rp_da << el.sm_rp_oa;
			if (el.has_imsi)
				L << "[imsi:" << el.imsi << "]";
			L << "[sm_rp_ui:" << bin::hex_str_ref(el.sm_rp_ui.data, el.sm_rp_ui.len).delimit("").prefix("") << "]";
		L << "]";
		return L;
	}

} } } }

#endif
