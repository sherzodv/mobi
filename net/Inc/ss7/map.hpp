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
			default: return 0xFF;
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
		while (len--) {
			*dst = bcd_encode(*src++);
			++rlen;
			if (len) {
				*dst |= bcd_encode(*src++) << 4;
				--len;
			}
			++dst;
		}
		return rlen;
	}

	template <typename DU8T, typename SU8T>
	bin::sz_t bcd_decode(DU8T * dst, const SU8T * src
			, bin::sz_t len, bool odd = false) {
		bin::sz_t rlen = 0;
		while (len--) {
			*dst++ = bcd_decode(*src & 0x0F);
			++rlen;
			if (odd && len == 1)
				break;
			*dst++ = bcd_decode((*src++ & 0xF0) >> 4);
			++rlen;
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

	/* TODO: determine needed capacity */
	const bin::sz_t max_address_string_len = 20;
	const bin::sz_t max_isdn_address_string_len = 21;

	typedef bin::u8_t address_string[max_address_string_len];
	typedef bin::u8_t isdn_address_string[max_isdn_address_string_len];

	namespace {

		struct isdn_address_string_t {
			bin::sz_t len;
			bin::u8_t data[max_isdn_address_string_len];
		};

		struct address_string_t {
			bin::sz_t len;
			bin::u8_t data[max_address_string_len];
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

			bin::sz_t size() const {
				return msisdn.len + sc_address.len + 1;
			}
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

		struct mo_forward_sm_res_t {
			signal_info_t	sm_rp_ui;
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
				using tcap::operator<<;
				(void)(el);
				//L << std::hex << el << std::endl;
				return resume;
			}

			action on_end(const tcap::element::end & el) {
				using tcap::operator<<;
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

				/* Skip payload of an return_result_last, as we parsed it already */
				switch (rres.op_code) {
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
				buf = base::parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr || t != type::sequence);

				/* Parse msisdn */
				buf = base::parse_octstring(ri.msisdn, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				/* Parse sm-RP-PRI */
				buf = base::parse_boolean(ri.sm_rp_pri, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				/* Parse serviceCentreAddress */
				buf = base::parse_octstring(ri.sc_address, buf, bend);
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
				buf = base::parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr || t != type::sequence);

				/* Parse imsi */
				buf = base::parse_octstring(ri.imsi, buf, bend);
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
				buf = base::parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr
						|| t.code != 0 || t.klass != tagclass_contextspec);

				/* Parse networkNodeNumber */
				buf = base::parse_octstring(linfo.network_node_number, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				tmpbuf = base::parse_octstring(linfo.lmsi, buf, bend);
				if (tmpbuf == nullptr) {
					linfo.lmsi.len = 0;
					return buf;
				} else {
					return tmpbuf;
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
						buf = base::parse_octstring(rp.as.msisdn, t, buf, bend);
						return buf;
					case 4:
						rp.has = sm_rp_oa_t::has_sc;
						buf = base::parse_octstring(rp.as.sc_address_oa, t, buf, bend);
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

	template <class LogT>
	class writer: public tcap::writer<LogT> {

		typedef tcap::writer<LogT> base;

		public:
			writer(LogT & l): base(l) {}
			virtual ~writer() {}

		protected:
			using base::L;

			bin::u8_t * write(bin::u8_t * buf, bin::u8_t * bend
					, const routing_info_for_sm_arg_t & r) {
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

				buf = base::write_tag(buf, bend, type::sequence, r.size());
				RETURN_NULL_IF(buf == nullptr);

				buf = base::write_octstring(buf, bend
						, tagclass_application, 0, r.msisdn);
				RETURN_NULL_IF(buf == nullptr);

				return buf;
			}
	};

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const isdn_address_string_t & n) {
		std::string num = bcd_decode(n.data + 1, n.len - 1, true);
		L << bin::hex_str_ref(n.data, n.len).prefix("").delimit("") << ":" << num;
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const address_string_t & n) {
		std::string num = bcd_decode(n.data + 1, n.len - 1, true);
		L << num;
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const imsi_t & n) {
		std::string num = bcd_decode(n.data, n.len, true);
		L << num;
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const lmsi_t & n) {
		std::string num = bcd_decode(n.data + 1, n.len - 1, true);
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

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const mo_forward_sm_res_t & el) {
		L << "[MO-ForwardSM-Res:";
			L << "[sm_rp_ui:" << bin::hex_str_ref(el.sm_rp_ui.data, el.sm_rp_ui.len).delimit("").prefix("") << "]";
		L << "]";
		return L;
	}

} } } }

#endif
