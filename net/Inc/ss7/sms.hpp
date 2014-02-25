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

	enum data_coding_scheme {
		dcs_general
		, dcs_auto_deletion
		, dcs_reserved
		, dcs_discard
		, dcs_store
		, dcs_me_indicator
		, dcs_store_ucs2
		, dcs_coding
		, dcs_special
	};

	enum character_set {
		cs_gsm_7bit		= 0x0
		, cs_8bit		= 0x1
		, cs_ucs2		= 0x2
		, cs_reserved	= 0x3
	};

	enum message_class {
		mc_class0	= 0x0
		, mc_class1	= 0x1
		, mc_class2	= 0x2
		, mc_class3	= 0x3
	};

	enum indication_type {
		indication_voice	= 0x0
		, indication_fax	= 0x1
		, indication_email	= 0x2
		, indication_other	= 0x3
	};

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

	struct dc_scheme {
		data_coding_scheme dcs;
		character_set cs;
		message_class klass;
		indication_type ind;
		bool indicate;
		bool compressed;
		bool has_message_class;
	};

	dc_scheme decode_dcs(bin::u8_t dcs) {
		dc_scheme r;

		/* 3GPP TS 23.038 4. SMS Data Coding Scheme */

		if (dcs >> 6 == 0x00 || dcs >> 6 == 0x01) {
			if (dcs == 0) {
				r.dcs = dcs_special;
				r.cs = cs_gsm_7bit;
			} else {
				if (dcs >> 6 == 0x01) {
					r.dcs = dcs_auto_deletion;
				} else {
					r.dcs = dcs_general;
				}
				r.compressed = dcs & 0x20;
				r.has_message_class = dcs & 0x10;
				switch ((dcs & 0x0C) >> 2) {
					case 0x00:
						r.cs = cs_gsm_7bit;
						break;
					case 0x01:
						r.cs = cs_8bit;
						break;
					case 0x02:
						r.cs = cs_ucs2;
						break;
					case 0x03:
						r.cs = cs_reserved;
						break;
				}
				switch (dcs & 0x03) {
					case 0x00:
						r.klass = mc_class0;
						break;
					case 0x01:
						r.klass = mc_class1;
						break;
					case 0x02:
						r.klass = mc_class2;
						break;
					case 0x03:
						r.klass = mc_class3;
						break;
				}
			}
		} else {
			r.cs = cs_gsm_7bit;
			switch (dcs >> 4) {
				case 0x08:
				case 0x09:
				case 0x0A:
				case 0x0B:
					r.dcs = dcs_reserved;
					break;
				case 0x0C:
				case 0x0D:
				case 0x0E: {
					switch (dcs >> 4) {
						case 0x0C:
							r.dcs = dcs_discard;
							break;
						case 0x0D:
							r.dcs = dcs_store;
							break;
						case 0x0E:
							r.dcs = dcs_store_ucs2;
							break;
					}
					r.cs = cs_gsm_7bit;
					r.indicate = dcs & 0x08;
					switch (dcs & 0x03) {
						case 0x00:
							r.ind = indication_voice;
							break;
						case 0x01:
							r.ind = indication_fax;
							break;
						case 0x02:
							r.ind = indication_email;
							break;
						case 0x03:
							r.ind = indication_other;
							break;
					}
					break;
				}
				case 0x0F:
					r.dcs = dcs_coding;
					if ((dcs & 0x04) == 0x01) {
						r.cs = cs_8bit;
					} else {
						r.cs = cs_gsm_7bit;
					}
					switch (dcs & 0x03) {
						case 0x00:
							r.klass = mc_class0;
							break;
						case 0x01:
							r.klass = mc_class1;
							break;
						case 0x02:
							r.klass = mc_class2;
							break;
						case 0x03:
							r.klass = mc_class3;
							break;
					}
					break;
			}
		}

		return r;
	}

	/* Address field: TS 23.040, 9.1.2.5 */
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

	struct pi_t {
		bool	pid: 1;	/* Protocol identifier is present */
		bool	dcs: 1;	/* Data coding scheme is present */
		bool	udl: 1;	/* User data length is present */
		bool	r1: 1;
		bool	r2: 1;
		bool	r3: 1;
		bool	r4: 1;
		bool	ext: 1;	/* Extension bit */
	};

	struct deliver_t {
		bin::u8_t		mti	:2;	/* Message type indicator */
		bool			mms	:1;	/* More messages to send */
		bool			lp	:1;	/* Loop prevention */
		bool			rp	:1;	/* Reply path existance */
		bool			udhi:1;	/* User data header indicator */
		bool			sri	:1;	/* Status report indicator */
		bin::u8_t		pad	:1;
		address_t<12>	oa;		/* Originating address */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		dcs;	/* Data coding scheme */
		bin::u8_t		scts[7];/* SC timestamp */
		bin::u8_t		udl;	/* User data length */
		bin::u8_t		ud[200];/* User data */
		dc_scheme		dcsd;	/* Decoded data coding scheme */
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

	struct command_t {
		bin::u8_t		mti	:2;	/* Message type indicator */
		bool			udhi:1;	/* User data header indicator */
		bool			srr	:1;	/* Status report request */
		bin::u8_t		pad	:4;
		bin::u8_t		mr;		/* Message reference */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		ct;		/* Command type */
		bin::u8_t		mn;		/* Message number */
		address_t<12>	da;		/* Destination address */
		bin::u8_t		cdl;	/* Command data length */
		bin::u8_t		cd[200];/* User data */
	};

	struct status_report_t {
		bin::u8_t		mti	:2;	/* Message type indicator */
		bool			udhi:1;	/* User data header indicator */
		bool			mms	:1;	/* More messages to send */
		bool			lp	:1;	/* Loop prevention */
		bool			srq	:1;	/* Status report qualifier */
		bin::u8_t		pad	:2;
		bin::u8_t		mr;		/* Message reference */
		address_t<12>	ra;		/* Recipient address */
		bin::u8_t		scts[7];/* SC timestamp */
		bin::u8_t		dt[7];	/* Discharge time */
		bin::u8_t		st;		/* Status */
		pi_t			pi;		/* Parameter indicator */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		dcs;	/* Data coding scheme */
		bin::u8_t		udl;	/* User data length */
		bin::u8_t		ud[200];/* User data */
	};

	struct deliver_report_neg_t {
		bin::u8_t		mti;	/* Message type indicator */
		bool			udhi;	/* User data header indication */
		bin::u8_t		fcs;	/* Failure cause */
		pi_t			pi;		/* Parameter indicator */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		dcs;	/* Data coding scheme */
		bin::u8_t		udl;	/* User data length */
		string_t<200>	ud;		/* User data */
	};

	struct deliver_report_pos_t {
		bin::u8_t		mti;	/* Message type indicator */
		bool			udhi;	/* User data header indication */
		pi_t			pi;		/* Parameter indicator */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		dcs;	/* Data coding scheme */
		bin::u8_t		udl;	/* User data length */
		string_t<200>	ud;		/* User data */
	};

	struct submit_report_neg_t {
		bin::u8_t		mti	:2;	/* Message type indicator */
		bool			udhi:1;	/* User data header indicator */
		bin::u8_t		pad	:5;
		bin::u8_t		fcs;	/* Failure cause */
		pi_t			pi;		/* Parameter indicator */
		bin::u8_t		scts[7];/* SC timestamp */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		dcs;	/* Data coding scheme */
		bin::u8_t		udl;	/* User data length */
		bin::u8_t		ud[200];/* User data */
	};

	struct submit_report_pos_t {
		bin::u8_t		mti	:2;	/* Message type indicator */
		bool			udhi:1;	/* User data header indicator */
		bin::u8_t		pad	:5;
		pi_t			pi;		/* Parameter indicator */
		bin::u8_t		scts[7];/* SC timestamp */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		dcs;	/* Data coding scheme */
		bin::u8_t		udl;	/* User data length */
		bin::u8_t		ud[200];/* User data */
	};

	template <class LogT>
	class parser {
		public:
			parser(LogT & l): L(l) {}
			virtual ~parser() {}

			const bin::u8_t * parse_in(const bin::u8_t * buf
				, const bin::u8_t * bend, bool is_err = false) {
				using namespace asn::ber;
				return parse_ms2sc_message(buf, bend, is_err);
			}

			const bin::u8_t * parse_out(const bin::u8_t * buf
				, const bin::u8_t * bend, bool is_err = false) {
				using namespace asn::ber;
				return parse_sc2ms_message(buf, bend, is_err);
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
			virtual action on_sms_submit(const submit_t & msg) = 0;
			virtual action on_sms_command(const command_t & msg) = 0;
			virtual action on_sms_status_report(const status_report_t & msg) = 0;
			virtual action on_sms_deliver_report_neg(const deliver_report_neg_t & msg) = 0;
			virtual action on_sms_deliver_report_pos(const deliver_report_pos_t & msg) = 0;
			virtual action on_sms_submit_report_neg(const submit_report_neg_t & msg) = 0;
			virtual action on_sms_submit_report_pos(const submit_report_pos_t & msg) = 0;

		private:
			const bin::u8_t * parse_ms2sc_message(const bin::u8_t * buf
					, const bin::u8_t * bend, bool is_err) {
				using namespace asn::ber;

				RETURN_NULL_IF(buf + 1 > bend);

				/* First 2 bits - message type */
				switch (*buf & 0x03) {
					case 0x00: /* SMS-DELIVER-REPORT (MS -> SC) */
						if (is_err) {
							return parse_deliver_report_neg(buf, bend);
						} else {
							return parse_deliver_report_pos(buf, bend);
						}
					case 0x01: /* SMS-SUBMIT (MS -> SC) */
						return parse_submit(buf, bend);
					case 0x02: /* SMS-COMMAND (MS -> SC) */
						return parse_command(buf, bend);
					/* Reserved value is not supported */
					default: return nullptr;
				}
			}

			const bin::u8_t * parse_sc2ms_message(const bin::u8_t * buf
					, const bin::u8_t * bend, bool is_err) {
				using namespace asn::ber;

				RETURN_NULL_IF(buf + 1 > bend);

				/* First 2 bits - message type */
				switch (*buf & 0x03) {
					case 0x00: /* SMS-DELIVER (SC -> MS) */
						return parse_deliver(buf, bend);
					case 0x01: /* SMS-SUBMIT-REPORT (SC -> MS) */
						if (is_err) {
							return parse_submit_report_neg(buf, bend);
						} else {
							return parse_submit_report_pos(buf, bend);
						}
					case 0x02: /* SMS-STATUS-REPORT (SC -> MS) */
						return parse_status_report(buf, bend);
					/* Reserved value is not supported */
					default: return nullptr;
				}

				return nullptr;
			}

			const bin::u8_t * parse_deliver(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

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

				deliver_t m;

				RETURN_NULL_IF(buf
					+ sizeof(bin::u8_t)		/* mti, mms, lp, rp, udhi, sri */
					+ sizeof(bin::u16_t)	/* address len and type */
					> bend);

				/* Parse mti, mms, lp, rp, udhi, sri */
				buf = p::cp_u8(asbuf(m), buf);
				/* Parse len and type-of-address at once */
				buf = p::cp_u16(asbuf(m.oa), buf);

				/* Parse address value, len is number of useful semi-octets */
				RETURN_NULL_IF(buf + m.oa.len / 2 + m.oa.len % 2 > bend);
				buf = p::cpy(m.oa.val, buf, m.oa.len / 2 + m.oa.len % 2);

				RETURN_NULL_IF(buf
					+ sizeof(bin::u8_t)	/* Protocol identifier */
					+ sizeof(bin::u8_t)	/* Code scheme */
					+ 7					/* SC timestamp */
					+ sizeof(bin::u8_t)	/* User data length */
					> bend);

				/* Parse protocol identifier */
				buf = p::cp_u8(asbuf(m.pid), buf);
				/* Parse data coding scheme */
				buf = p::cp_u8(asbuf(m.dcs), buf);
				/* Parse sc timestamp */
				buf = p::cpy(asbuf(m.scts), buf, 7);
				/* Parse user data length */
				buf = p::cp_u8(asbuf(m.udl), buf);

				m.dcsd = decode_dcs(m.dcs);

				if (m.dcsd.cs == cs_gsm_7bit) {
					RETURN_NULL_IF(buf + ((m.udl * 7)>>3) > bend);
				} else {
					RETURN_NULL_IF(buf + m.udl > bend);
				}

				/* Parse user data */
				buf = p::cpy(asbuf(m.ud), buf, m.udl);

				switch (on_sms_deliver(m)) {
					case resume: return bend;
					default: return nullptr;
				}
				return nullptr;
			}

			const bin::u8_t * parse_submit(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				submit_t m;

				RETURN_NULL_IF(buf
					+ sizeof(bin::u8_t)		/* mti, rd, vpf, rp, udhi, srr */
					+ sizeof(bin::u8_t)		/* mr */
					+ sizeof(bin::u16_t)	/* address len and type */
					> bend);

				/* Parse mti, rd, vpf, rp, udhi, srr */
				buf = p::cp_u8(asbuf(m), buf);
				/* Parse message reference */
				buf = p::cp_u8(asbuf(m.mr), buf);
				/* Parse len and type-of-address at once */
				buf = p::cp_u16(asbuf(m.da), buf);

				RETURN_NULL_IF(buf + m.da.len / 2 + m.da.len % 2 > bend);

				/* Parse address value, len is number of useful semi-octets */
				buf = p::cpy(m.da.val, buf, m.da.len / 2 + m.da.len % 2);

				RETURN_NULL_IF(buf
					+ sizeof(bin::u8_t)		/* pid */
					+ sizeof(bin::u8_t)		/* dcs */
					> bend);

				/* Parse protocol identifier */
				buf = p::cp_u8(asbuf(m.pid), buf);
				/* Parse data coding scheme */
				buf = p::cp_u8(asbuf(m.dcs), buf);

				/* Parse validity period if present */
				switch (m.vpf) {
					case vpf_relative:
						RETURN_NULL_IF(buf + 1 > bend);
						buf = p::cpy(asbuf(m.vp), buf, 1);
						std::memset(asbuf(m.vp) + 1, 0, 6);
						break;
					case vpf_enhanced:
					case vpf_absolute:
						RETURN_NULL_IF(buf + 7 > bend);
						buf = p::cpy(asbuf(m.vp), buf, 7);
						break;
					default: break;
				}

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
				/* Parse user data length */
				buf = p::cp_u8(asbuf(m.udl), buf);

				RETURN_NULL_IF(buf + m.udl > bend);
				/* Parse user data */
				buf = p::cpy(asbuf(m.ud), buf, m.udl);

				switch (on_sms_submit(m)) {
					case resume: return bend;
					default: return nullptr;
				}
			}

			const bin::u8_t * parse_command(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				command_t m;

				RETURN_NULL_IF(buf
					+ sizeof(bin::u8_t)		/* mti, udhi, srr */
					+ sizeof(bin::u8_t)		/* mr */
					+ sizeof(bin::u8_t)		/* pid */
					+ sizeof(bin::u8_t)		/* ct */
					+ sizeof(bin::u8_t)		/* mn */
					+ sizeof(bin::u16_t)	/* address len and type */
					> bend);

				/* Parse mti, udhi, srr */
				buf = p::cp_u8(asbuf(m), buf);
				/* Parse message reference */
				buf = p::cp_u8(asbuf(m.mr), buf);
				/* Parse protocol identifier */
				buf = p::cp_u8(asbuf(m.pid), buf);
				/* Parse command type */
				buf = p::cp_u8(asbuf(m.ct), buf);
				/* Parse message number */
				buf = p::cp_u8(asbuf(m.mn), buf);
				/* Parse len and type-of-address at once */
				buf = p::cp_u16(asbuf(m.da), buf);

				RETURN_NULL_IF(buf + m.da.len / 2 + m.da.len % 2 > bend);

				/* Parse address value, len is number of useful semi-octets */
				buf = p::cpy(m.da.val, buf, m.da.len / 2 + m.da.len % 2);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
				/* Parse command data length */
				buf = p::cp_u8(asbuf(m.cdl), buf);

				RETURN_NULL_IF(buf + m.cdl > bend);
				/* Parse command data */
				buf = p::cpy(asbuf(m.cd), buf, m.cdl);

				switch (on_sms_command(m)) {
					case resume: return bend;
					default: return nullptr;
				}
			}

			const bin::u8_t * parse_status_report(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				status_report_t m;

				RETURN_NULL_IF(buf
					+ sizeof(bin::u8_t)		/* mti, udhi, mms, lp, srq */
					+ sizeof(bin::u8_t)		/* mr */
					+ sizeof(bin::u16_t)	/* address len and type */
					> bend);

				/* Parse mti, udhi, mms, lp, srq */
				buf = p::cp_u8(asbuf(m), buf);
				/* Parse message reference */
				buf = p::cp_u8(asbuf(m.mr), buf);
				/* Parse len and type-of-address at once */
				buf = p::cp_u16(asbuf(m.ra), buf);

				RETURN_NULL_IF(buf + m.ra.len / 2 + m.ra.len % 2 > bend);
				/* Parse address value, len is number of useful semi-octets */
				buf = p::cpy(m.ra.val, buf, m.ra.len / 2 + m.ra.len % 2);

				RETURN_NULL_IF(buf + 14 > bend);
				/* Parse sc timestamp */
				buf = p::cpy(asbuf(m.scts), buf, 7);
				/* Parse discharge time */
				buf = p::cpy(asbuf(m.dt), buf, 7);

				RETURN_NULL_IF(buf
					+ sizeof(bin::u8_t)		/* st */
					+ sizeof(bin::u8_t)		/* pi */
					> bend);

				/* Parse status */
				buf = p::cp_u8(asbuf(m.st), buf);
				/* Parse parameter indicator */
				buf = p::cp_u8(asbuf(m.pi), buf);

				if (m.pi.pid) {
					RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
					/* Parse protocol identifier */
					buf = p::cp_u8(asbuf(m.pid), buf);
				} else {
					m.pid = 0;
				}

				if (m.pi.dcs) {
					RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
					/* Parse data coding scheme */
					buf = p::cp_u8(asbuf(m.dcs), buf);
				} else {
					m.dcs = 0;
				}

				if (m.pi.udl) {
					RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
					/* Parse user data len */
					buf = p::cp_u8(asbuf(m.udl), buf);
					RETURN_NULL_IF(buf + m.udl > bend);
					/* Parse user data */
					buf = p::cpy(asbuf(m.ud), buf, m.udl);
				} else {
					m.udl = 0;
				}

				switch (on_sms_status_report(m)) {
					case resume: return bend;
					default: return nullptr;
				}
			}

			const bin::u8_t * parse_deliver_report_neg(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				deliver_report_neg_t m;

				RETURN_NULL_IF(buf
					+ sizeof(bin::u8_t)		/* mti, udhi */
					+ sizeof(bin::u8_t)		/* fcs */
					+ sizeof(bin::u8_t)		/* pi */
					> bend);

				/* Parse mti, udhi */
				buf = p::cp_u8(asbuf(m), buf);
				/* Parse failure cause */
				buf = p::cp_u8(asbuf(m.fcs), buf);
				/* Parse parameter indicator */
				buf = p::cp_u8(asbuf(m.pi), buf);

				if (m.pi.pid) {
					RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
					/* Parse protocol identifier */
					buf = p::cp_u8(asbuf(m.pid), buf);
				} else {
					m.pid = 0;
				}

				if (m.pi.dcs) {
					RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
					/* Parse data coding scheme */
					buf = p::cp_u8(asbuf(m.dcs), buf);
				} else {
					m.dcs = 0;
				}

				if (m.pi.udl) {
					RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
					/* Parse user data len */
					buf = p::cp_u8(asbuf(m.udl), buf);
					RETURN_NULL_IF(buf + m.udl > bend);
					/* Parse user data */
					buf = p::cpy(asbuf(m.ud), buf, m.udl);
				} else {
					m.udl = 0;
				}

				switch (on_sms_deliver_report_neg(m)) {
					case resume: return bend;
					default: return nullptr;
				}
			}

			const bin::u8_t * parse_deliver_report_pos(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				deliver_report_pos_t m;

				RETURN_NULL_IF(buf
					+ sizeof(bin::u8_t)		/* mti, udhi */
					+ sizeof(bin::u8_t)		/* pi */
					> bend);

				/* Parse mti, udhi */
				buf = p::cp_u8(asbuf(m), buf);
				/* Parse parameter indicator */
				buf = p::cp_u8(asbuf(m.pi), buf);

				if (m.pi.pid) {
					RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
					/* Parse protocol identifier */
					buf = p::cp_u8(asbuf(m.pid), buf);
				} else {
					m.pid = 0;
				}

				if (m.pi.dcs) {
					RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
					/* Parse data coding scheme */
					buf = p::cp_u8(asbuf(m.dcs), buf);
				} else {
					m.dcs = 0;
				}

				if (m.pi.udl) {
					RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
					/* Parse user data len */
					buf = p::cp_u8(asbuf(m.udl), buf);
					RETURN_NULL_IF(buf + m.udl > bend);
					/* Parse user data */
					buf = p::cpy(asbuf(m.ud), buf, m.udl);
				} else {
					m.udl = 0;
				}

				switch (on_sms_deliver_report_pos(m)) {
					case resume: return bend;
					default: return nullptr;
				}
			}

			const bin::u8_t * parse_submit_report_neg(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				submit_report_neg_t m;

				RETURN_NULL_IF(buf
					+ sizeof(bin::u8_t)		/* mti, udhi */
					+ sizeof(bin::u8_t)		/* fcs */
					+ sizeof(bin::u8_t)		/* pi */
					+ 7						/* scts */
					> bend);

				/* Parse mti, udhi */
				buf = p::cp_u8(asbuf(m), buf);
				/* Parse failure cause */
				buf = p::cp_u8(asbuf(m.fcs), buf);
				/* Parse parameter indicator */
				buf = p::cp_u8(asbuf(m.pi), buf);

				RETURN_NULL_IF(buf + 7 > bend);
				/* Parse sc timestamp */
				buf = p::cpy(asbuf(m.scts), buf, 7);

				if (m.pi.pid) {
					RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
					/* Parse protocol identifier */
					buf = p::cp_u8(asbuf(m.pid), buf);
				} else {
					m.pid = 0;
				}

				if (m.pi.dcs) {
					RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
					/* Parse data coding scheme */
					buf = p::cp_u8(asbuf(m.dcs), buf);
				} else {
					m.dcs = 0;
				}

				if (m.pi.udl) {
					RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
					/* Parse user data len */
					buf = p::cp_u8(asbuf(m.udl), buf);
					RETURN_NULL_IF(buf + m.udl > bend);
					/* Parse user data */
					buf = p::cpy(asbuf(m.ud), buf, m.udl);
				} else {
					m.udl = 0;
				}

				switch (on_sms_submit_report_neg(m)) {
					case resume: return bend;
					default: return nullptr;
				}
			}

			const bin::u8_t * parse_submit_report_pos(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				submit_report_pos_t m;

				RETURN_NULL_IF(buf
					+ sizeof(bin::u8_t)		/* mti, udhi */
					+ sizeof(bin::u8_t)		/* pi */
					+ 7						/* scts */
					> bend);

				/* Parse mti, udhi */
				buf = p::cp_u8(asbuf(m), buf);
				/* Parse parameter indicator */
				buf = p::cp_u8(asbuf(m.pi), buf);

				RETURN_NULL_IF(buf + 7 > bend);
				/* Parse sc timestamp */
				buf = p::cpy(asbuf(m.scts), buf, 7);

				if (m.pi.pid) {
					RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
					/* Parse protocol identifier */
					buf = p::cp_u8(asbuf(m.pid), buf);
				} else {
					m.pid = 0;
				}

				if (m.pi.dcs) {
					RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
					/* Parse data coding scheme */
					buf = p::cp_u8(asbuf(m.dcs), buf);
				} else {
					m.dcs = 0;
				}

				if (m.pi.udl) {
					RETURN_NULL_IF(buf + sizeof(bin::u8_t) > bend);
					/* Parse user data len */
					buf = p::cp_u8(asbuf(m.udl), buf);
					RETURN_NULL_IF(buf + m.udl > bend);
					/* Parse user data */
					buf = p::cpy(asbuf(m.ud), buf, m.udl);
				} else {
					m.udl = 0;
				}

				switch (on_sms_submit_report_pos(m)) {
					case resume: return bend;
					default: return nullptr;
				}
			}
	};

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
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const pi_t & pi) {
		L << "[pi:";
		if (pi.pid) { L << "[pid]"; }
		if (pi.dcs) { L << "[dcs]"; }
		if (pi.udl) { L << "[udl]"; }
		if (pi.r1) { L << "[r1]"; }
		if (pi.r2) { L << "[r2]"; }
		if (pi.r3) { L << "[r3]"; }
		if (pi.r4) { L << "[r4]"; }
		if (pi.ext) { L << "[ext]"; }
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const deliver_t & msg) {
		L << "[SMS-DELIVER:"
			<< "[mms:" << msg.mms << "]"
			<< "[lp:" << msg.lp << "]"
			<< "[rp:" << msg.rp << "]"
			<< "[udhi:" << msg.udhi << "]"
			<< "[sri:" << msg.sri << "]"
			<< "[oa:" << msg.oa << "]"
			<< "[pid:" << static_cast<unsigned>(msg.pid) << "]"
			<< "[dcs:" << bin::bin_str_ref(&msg.dcs, 1) << "]"
			<< "[udl:" << static_cast<unsigned>(msg.udl) << "]"
			<< "[ud:" << bin::hex_str_ref(msg.ud, msg.udl) << "]"
		;
		L << "]";
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

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const command_t & msg) {
		L << "[SMS-COMMAND:"
			<< "[udhi:" << msg.udhi << "]"
			<< "[srr:" << msg.srr << "]"
			<< "[mr:" << static_cast<unsigned>(msg.mr) << "]"
			<< "[pid:" << static_cast<unsigned>(msg.pid) << "]"
			<< "[ct:" << static_cast<unsigned>(msg.ct) << "]"
			<< "[mn:" << static_cast<unsigned>(msg.mn) << "]"
			<< "[da:" << msg.da << "]"
			<< "[cdl:" << static_cast<unsigned>(msg.cdl) << "]"
			<< "[cd:" << bin::hex_str_ref(msg.cd, msg.cdl) << "]"
		;
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const status_report_t & msg) {
		L << "[SMS-STATUS-REPORT:"
			<< "[udhi:" << msg.udhi << "]"
			<< "[mms:" << msg.mms << "]"
			<< "[lp:" << msg.lp << "]"
			<< "[srq:" << msg.srq << "]"
			<< "[mr:" << static_cast<unsigned>(msg.mr) << "]"
			<< "[ra:" << msg.ra << "]"
			/* TODO: L << SC timestamp */
			/* TODO: L << discharge time */
			<< "[st:" << static_cast<unsigned>(msg.st) << "]"
			<< msg.pi
			<< "[pid:" << static_cast<unsigned>(msg.pid) << "]"
			<< "[dcs:" << static_cast<unsigned>(msg.dcs) << "]"
			<< "[udl:" << static_cast<unsigned>(msg.udl) << "]"
			<< "[ud:" << bin::hex_str_ref(msg.ud, msg.udl) << "]"
		;
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const deliver_report_neg_t & msg) {
		L << "[SMS-DELIVER-REPORT(ERR):"
			<< "[udhi:" << msg.udhi << "]"
			<< "[fcs:" << static_cast<unsigned>(msg.fcs) << "]"
			<< msg.pi
			/* TODO: L << SC timestamp */
			<< "[dcs:" << static_cast<unsigned>(msg.dcs) << "]"
			<< "[udl:" << static_cast<unsigned>(msg.udl) << "]"
			<< "[ud:" << bin::hex_str_ref(msg.ud, msg.udl) << "]"
		;
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const deliver_report_pos_t & msg) {
		L << "[SMS-DELIVER-REPORT(ACK):"
			<< "[udhi:" << msg.udhi << "]"
			<< msg.pi
			/* TODO: L << SC timestamp */
			<< "[dcs:" << static_cast<unsigned>(msg.dcs) << "]"
			<< "[udl:" << static_cast<unsigned>(msg.udl) << "]"
			<< "[ud:" << bin::hex_str_ref(msg.ud, msg.udl) << "]"
		;
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const submit_report_neg_t & msg) {
		L << "[SMS-SUBMIT-REPORT(ERR):"
			<< "[udhi:" << msg.udhi << "]"
			<< "[fcs:" << static_cast<unsigned>(msg.fcs) << "]"
			<< msg.pi
			/* TODO: L << SC timestamp */
			<< "[dcs:" << static_cast<unsigned>(msg.dcs) << "]"
			<< "[udl:" << static_cast<unsigned>(msg.udl) << "]"
			<< "[ud:" << bin::hex_str_ref(msg.ud, msg.udl) << "]"
		;
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const submit_report_pos_t & msg) {
		L << "[SMS-SUBMIT-REPORT(ACK):"
			<< "[udhi:" << msg.udhi << "]"
			<< msg.pi
			/* TODO: L << SC timestamp */
			<< "[dcs:" << static_cast<unsigned>(msg.dcs) << "]"
			<< "[udl:" << static_cast<unsigned>(msg.udl) << "]"
			<< "[ud:" << bin::hex_str_ref(msg.ud, msg.udl) << "]"
		;
		L << "]";
		return L;
	}

} } }

#endif
