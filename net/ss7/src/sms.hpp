#ifndef mobi_net_sms_hpp
#define mobi_net_sms_hpp

/* 3GPP TS 23.040 */

#include <bitset>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <ostream>

#include <toolbox/bin.hpp>

namespace mobi { namespace net { namespace sms {

	using namespace toolbox;

	enum type_of_number {
		ton_unknown			= 0x00
		, ton_international	= 0x01
		, ton_national		= 0x02
		, ton_net_specific	= 0x03
		, ton_subscriber	= 0x04
		, ton_alphanum		= 0x05
		, ton_abbr			= 0x06
		, ton_reserved		= 0x07
	};

	enum numbering_plan {
		np_unknown			= 0x00
		, np_isdn_telephony	= 0x01
		, np_data			= 0x03
		, np_telex			= 0x04
		, np_sc_specific1	= 0x05
		, np_sc_specific2	= 0x06
		, np_national		= 0x08
		, np_private		= 0x09
		, np_ermes			= 0x0A
		, np_reserved		= 0x0F
	};

	enum validity_period_format {
		vpf_not_present		= 0x00
		, vpf_enhanced		= 0x01
		, vpf_relative		= 0x02
		, vpf_absolute		= 0x03
	};

	template <bin::sz_t MaxLen>
	struct address_t {
		numbering_plan	np: 4;
		type_of_number	ton: 3;
		bin::u8_t		pad: 1;
		bin::u8_t		len;
		bin::u8_t		val[MaxLen];
	};

	template <bin::sz_t MaxLen>
	struct string_t {
		bin::sz_t len;
		bin::u8_t data[MaxLen];
	};

	enum pdu_type {
		pdu_sms_deliver				= 0x00
		, pdu_sms_deliver_report	= 0x00
	};

	struct deliver_t {
		bin::u8_t		mti;	/* Message type indicator */
		bool			mms;	/* More messages to send */
		bool			lp;		/* Loop prevention */
		bool			rp;		/* Reply path existance */
		bool			udhi;	/* User data header indicator */
		bool			sri;	/* Status report indicator */
		string_t<12>	oa;		/* Originating address */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		dcs;	/* Data coding scheme */
		string_t<7>		scts;	/* SC timestamp */
		string_t<200>	ud;		/* User data */
	};

	struct deliver_report_t {
		bin::u8_t		mti;	/* Message type indicator */
		bool			udhi;	/* User data header indication */
		bin::u16_t		fcs;	/* Failure cause */ /* TODO: determine right int type */
		bin::u8_t		pi;		/* Parameter indicator */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		dcs;	/* Data coding scheme */
		bin::u8_t		udl;	/* User data length */
		string_t<200>	ud;		/* User data */
	};

	struct command_t {
	};

	struct status_report_t {
	};

	struct submit_t {
		bin::u8_t		mti	:2;	/* Message type indicator */
		bool			rd	:1;	/* Reject duplicates */
		validity_period_format vpf	:2;	/* Validity period format */
		bool			rp	:1;	/* Reply path existance */
		bool			udhi:1;	/* User data header indicator */
		bool			srr	:1;	/* Status report request */
		bin::u8_t		mr;		/* Message reference */
		address_t<12>	da;		/* Destination address */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		dcs;	/* Data coding scheme */
		bin::u8_t		vp[8];	/* Validity period */
		bin::u8_t		udl;	/* User data length */
		bin::u8_t		ud[200];/* User data */
	};

	struct submit_report_t {
	};

	template <class LogT>
	class parser {
		public:
			parser(LogT & l): L(l) {}
			virtual ~parser() {}

			const bin::u8_t * parse_in(const bin::u8_t * buf
				, const bin::u8_t * bend) {
				using namespace asn::ber;
				buf = parse_ms2sc_message(buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				return buf;
			}

			const bin::u8_t * parse_out(const bin::u8_t * buf
				, const bin::u8_t * bend) {
				using namespace asn::ber;
				buf = parse_sc2ms_message(buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				return buf;
			}

		protected:
			LogT & L;

			enum action {
				stop		/* Stop parsing immediately */
				, resume	/* Resume parsing */
				, skip		/* Skip the value (may be constructor)
							   of an element */
			};

			virtual action on_sms_deliver(const deliver_t & msg) = 0;
			virtual action on_sms_deliver_report(const deliver_report_t & msg) = 0;
			virtual action on_sms_submit(const submit_t & msg) = 0;
			virtual action on_sms_submit_report(const submit_report_t & msg) = 0;
			virtual action on_sms_command(const command_t & msg) = 0;
			virtual action on_sms_status_report(const status_report_t & msg) = 0;

		private:
			const bin::u8_t * parse_ms2sc_message(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;

				RETURN_NULL_IF(buf >= bend);

				/* First 2 bits - message type */
				switch (*buf & 0x03) {
					case 0x00: /* SMS-DELIVER-REPORT (MS -> SC) */
						return parse_deliver_report(buf, bend);
					case 0x01: /* SMS-SUBMIT (MS -> SC) */
						return parse_submit(buf, bend);
					case 0x02: /* SMS-COMMAND (MS -> SC) */
						return parse_command(buf, bend);
					/* Reserved value is not supported */
					default: return nullptr;
				}
			}

			const bin::u8_t * parse_sc2ms_message(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;

				RETURN_NULL_IF(buf >= bend);

				/* First 2 bits - message type */
				switch (*buf & 0x03) {
					case 0x00: /* SMS-DELIVER (SC -> MS) */
						return parse_deliver(buf, bend);
					case 0x01: /* SMS-SUBMIT-REPORT (SC -> MS) */
						return parse_submit_report(buf, bend);
					case 0x02: /* SMS-STATUS-REPORT (SC -> MS) */
						return parse_status_report(buf, bend);
					/* Reserved value is not supported */
					default: return nullptr;
				}

				return nullptr;
			}

			const bin::u8_t * parse_deliver_report(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				(void)(buf);
				(void)(bend);
				return nullptr;
			}

			const bin::u8_t * parse_submit(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				submit_t sm;

				/* Parse mti, rd, vpf, rp, udhi, srr */
				buf = p::cp_u8(asbuf(sm), buf);
				RETURN_NULL_IF(buf >= bend);

				/* Parse message reference */
				buf = p::cp_u8(asbuf(sm.mr), buf);
				RETURN_NULL_IF(buf >= bend);

				/* Parse address field: TS 23.040, 9.1.2.5 */

				/* Parse len and type-of-address at once */
				buf = p::cp_u16(asbuf(sm.da), buf);
				RETURN_NULL_IF(buf >= bend);

				/* Parse address value, len is number of useful semi-octets */
				buf = p::cpy(sm.da.val, buf, sm.da.len / 2 + sm.da.len % 2);
				RETURN_NULL_IF(buf >= bend);

				/* Parse protocol identifier */
				buf = p::cp_u8(asbuf(sm.pid), buf);
				RETURN_NULL_IF(buf >= bend);

				/* Parse data coding scheme */
				buf = p::cp_u8(asbuf(sm.dcs), buf);
				RETURN_NULL_IF(buf >= bend);

				L << sm.vpf << std::endl;

				/* Parse validity period if present */
				switch (sm.vpf) {
					case vpf_relative:
						buf = p::cpy(asbuf(sm.vp), buf, 1);
						std::memset(asbuf(sm.vp) + 1, 0, 6);
						RETURN_NULL_IF(buf >= bend);
						break;
					case vpf_enhanced:
					case vpf_absolute:
						buf = p::cpy(asbuf(sm.vp), buf, 7);
						RETURN_NULL_IF(buf >= bend);
						break;
					default: break;
				}

				/* Parse user data length */
				buf = p::cp_u8(asbuf(sm.udl), buf);
				RETURN_NULL_IF(buf >= bend);

				/* Parse user data */
				buf = p::cpy(asbuf(sm.ud), buf, sm.udl);

				switch (on_sms_submit(sm)) {
					case resume: return bend;
					default: return nullptr;
				}
			}

			const bin::u8_t * parse_command(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				(void)(buf);
				(void)(bend);

				return nullptr;
			}

			const bin::u8_t * parse_deliver(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				(void)(buf);
				(void)(bend);

				/*
					Abbr. 	 Reference 							P1) R2) 	Description
					TP-MTI  	TP-Message-Type-Indicator 		M 	2b 		Parameter describing the message type.
					TP-MMS  	TP-More-Messages-to-Send 		M 	b 		Parameter indicating whether or not there are more messages to send
					TP-LP  		TP-Loop-Prevention 				O 	b 		Parameter indicating that SMS applications should inhibit forwarding
																			or automatic message generation that could cause infinite looping.
					TP-RP 		TP-Reply-Path 					M 	b 		Parameter indicating that Reply Path exists.
					TP-UDHI 	TP-User-Data-Header-Indicator 	O 	b 		Parameter indicating that the TP-UD field contains a Header
					TP-SRI 		TP-Status-Report-Indication 	O 	b 		Parameter indicating if the SME has requested a status report.
					TP-OA 		TP-Originating-Address 			M 	2-12o 	Address of the originating SME.
					TP-PID 		TP-Protocol-Identifier 			M 	o 		Parameter identifying the above layer protocol, if any.
					TP-DCS 		TP-Data-Coding-Scheme 			M 	o 		Parameter identifying the coding scheme within the TP-User-Data.
					TP-SCTS 	TP-Service-Centre-Time-Stamp 	M 	7o 		Parameter identifying time when the SC received the message.
					TP-UDL 		TP-User-Data-Length 			M 	I 		Parameter indicating the length of the TP-User-Data field to follow.
					TP-UD 		TP-User-Data 					O	Depends on TP-DCS
				*/

				return nullptr;
			}

			const bin::u8_t * parse_submit_report(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				(void)(buf);
				(void)(bend);
				return nullptr;
			}

			const bin::u8_t * parse_status_report(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				(void)(buf);
				(void)(bend);
				return nullptr;
			}
	};

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const deliver_t & el) {
		(void)(el);
		L << "[SMS-DELIVER:";
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const validity_period_format & vpf) {
		L << "[vp-format:";
		switch (vpf) {
			case vpf_not_present: L << "not present"; break;
			case vpf_enhanced: L << "enhanced"; break;
			case vpf_relative: L << "relative"; break;
			case vpf_absolute: L << "absolute"; break;
			default: L << "wrong"; break;
		}
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const type_of_number & ton) {
		L << "[ton:";
		switch (ton) {
			case ton_unknown		: L << "unknown"; break;
			case ton_international	: L << "international"; break;
			case ton_national		: L << "national"; break;
			case ton_net_specific	: L << "net-specific"; break;
			case ton_subscriber		: L << "subscriber-num"; break;
			case ton_alphanum		: L << "alpha-num"; break;
			case ton_abbr			: L << "abbr"; break;
			default: L << "reserved"; break;
		}
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const numbering_plan & np) {
		L << "[np:";
		switch (np) {
			case np_unknown			: L << "unknown"; break;
			case np_isdn_telephony	: L << "isdn_telephony"; break;
			case np_data			: L << "data"; break;
			case np_telex			: L << "telex"; break;
			case np_sc_specific1	: L << "sc specific 1"; break;
			case np_sc_specific2	: L << "sc specific 2"; break;
			case np_national		: L << "national"; break;
			case np_private			: L << "private"; break;
			case np_ermes			: L << "ermes"; break;
			default					: L << "reserved"; break;
		}
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT, bin::sz_t MaxLen >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const address_t<MaxLen> & addr) {
		/* TODO: print value */
		L << "[len:" << static_cast<unsigned>(addr.len) << "]"
			<< addr.np << addr.ton;
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const submit_t & msg) {
		L << "[SMS-SUBMIT:"
			<< "[rd:" << msg.rd << "]"
			<< msg.vpf
			<< "[rp:" << msg.rp << "]"
			<< "[udhi:" << msg.udhi << "]"
			<< "[srr:" << msg.srr << "]"
			<< "[mr:" << static_cast<unsigned>(msg.mr) << "]"
			<< "[da:" << msg.da << "]"
			<< "[pid:" << static_cast<unsigned>(msg.pid) << "]"
			<< "[dcs:" << bin::bin_str_ref(&msg.dcs, 1) << "]"
			<< "[udl:" << static_cast<unsigned>(msg.udl) << "]"
			<< "[ud:" << bin::hex_str_ref(msg.ud, msg.udl) << "]"
		;

		L << "]";
		return L;
	}

} } }

#endif
