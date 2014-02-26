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

	const bin::sz_t max_udl = 200;

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

	/* Address field: TS 23.040, 9.1.2.5 */
	template <bin::sz_t MaxLen>
	struct address_tt {
		numbering_plan	np: 4;
		type_of_number	ton: 3;
		bin::u8_t		pad: 1;
		bin::u8_t		len;
		bin::u8_t		data[MaxLen];
	};

	template <bin::sz_t MaxLen>
	struct string_tt {
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
		address_tt<11>	oa;		/* Originating address */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		dcs;	/* Data coding scheme */
		bin::u8_t		scts[7];/* SC timestamp */
		bin::u8_t		udl;	/* User data length */
		string_tt<255>	ud;		/* User data */
		dc_scheme		dcsd;	/* Decoded data coding scheme */
	};

	struct submit_t {
		/* 3GPP 23.040 has wrong placed srr and rp in table */
		bin::u8_t		mti	:2;	/* Message type indicator */
		bool			rd	:1;	/* Reject duplicates */
		validity_period_format vpf	:2;	/* Validity period format */
		bool			srr	:1;	/* Status report request */
		bool			udhi:1;	/* User data header indicator */
		bool			rp	:1;	/* Reply path existance */
		bin::u8_t		mr;		/* Message reference */
		address_tt<11>	da;		/* Destination address */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		dcs;	/* Data coding scheme */
		bin::u8_t		vp[8];	/* Validity period */
		bin::u8_t		udl;	/* User data length */
		string_tt<140>	ud;		/* User data */
		dc_scheme		dcsd;	/* Decoded data coding scheme */
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
		address_tt<11>	da;		/* Destination address */
		bin::u8_t		cdl;	/* Command data length */
		string_tt<156>	cd;		/* Command data */
	};

	struct status_report_t {
		bin::u8_t		mti	:2;	/* Message type indicator */
		bool			udhi:1;	/* User data header indicator */
		bool			mms	:1;	/* More messages to send */
		bool			lp	:1;	/* Loop prevention */
		bool			srq	:1;	/* Status report qualifier */
		bin::u8_t		pad	:2;
		bin::u8_t		mr;		/* Message reference */
		address_tt<11>	ra;		/* Recipient address */
		bin::u8_t		scts[7];/* SC timestamp */
		bin::u8_t		dt[7];	/* Discharge time */
		bin::u8_t		st;		/* Status */
		pi_t			pi;		/* Parameter indicator */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		dcs;	/* Data coding scheme */
		bin::u8_t		udl;	/* User data length */
		string_tt<143>	ud;		/* User data */
		dc_scheme		dcsd;	/* Decoded data coding scheme */
	};

	struct deliver_report_neg_t {
		bin::u8_t		mti;	/* Message type indicator */
		bool			udhi;	/* User data header indication */
		bin::u8_t		fcs;	/* Failure cause */
		pi_t			pi;		/* Parameter indicator */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		dcs;	/* Data coding scheme */
		bin::u8_t		udl;	/* User data length */
		string_tt<158>	ud;		/* User data */
		dc_scheme		dcsd;	/* Decoded data coding scheme */
	};

	struct deliver_report_pos_t {
		bin::u8_t		mti;	/* Message type indicator */
		bool			udhi;	/* User data header indication */
		pi_t			pi;		/* Parameter indicator */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		dcs;	/* Data coding scheme */
		bin::u8_t		udl;	/* User data length */
		string_tt<159>	ud;		/* User data */
		dc_scheme		dcsd;	/* Decoded data coding scheme */
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
		string_tt<151>	ud;		/* User data */
		dc_scheme		dcsd;	/* Decoded data coding scheme */
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
		string_tt<152>	ud;		/* User data */
		dc_scheme		dcsd;	/* Decoded data coding scheme */
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

			bin::sz_t octet_count(const dc_scheme &  dcs
					, bin::sz_t udl) {
				if (dcs.cs == cs_gsm_7bit) {
					return ((udl * 7) >> 3) + (((udl * 7) % 8) != 0);
				} else {
					return udl;
				}
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
				RETURN_NULL_IF(buf + (m.oa.len >> 1) + (m.oa.len % 2) > bend);
				buf = p::cpy(m.oa.data, buf, (m.oa.len >> 1) + (m.oa.len % 2));

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
				m.ud.len = octet_count(m.dcsd, m.udl);

				RETURN_NULL_IF(buf + m.ud.len > bend);
				buf = p::cpy(asbuf(m.ud.data), buf, m.ud.len);

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

				RETURN_NULL_IF(buf + (m.da.len >> 1) + (m.da.len % 2) > bend);
				buf = p::cpy(m.da.data, buf, (m.da.len >> 1) + (m.da.len % 2));

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

				/* Parse user data length */
				RETURN_NULL_IF(buf + 1 > bend);
				buf = p::cp_u8(asbuf(m.udl), buf);

				m.dcsd = decode_dcs(m.dcs);
				m.ud.len = octet_count(m.dcsd, m.udl);

				RETURN_NULL_IF(buf + m.ud.len > bend);
				buf = p::cpy(asbuf(m.ud.data), buf, m.ud.len);

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

				RETURN_NULL_IF(buf + (m.da.len >> 1) + (m.da.len % 2) > bend);
				buf = p::cpy(m.da.data, buf, (m.da.len >> 1) + (m.da.len % 2));

				/* Parse command data length */
				RETURN_NULL_IF(buf + 1 > bend);
				buf = p::cp_u8(asbuf(m.cdl), buf);

				/* Parse command data */
				m.cd.len = m.cdl;
				RETURN_NULL_IF(buf + m.cd.len > bend);
				buf = p::cpy(asbuf(m.cd), buf, m.cd.len);

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

				RETURN_NULL_IF(buf + (m.ra.len >> 1) + (m.ra.len % 2) > bend);
				buf = p::cpy(m.ra.data, buf, (m.ra.len >> 1) + (m.ra.len % 2));

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
					/* Parse user data len */
					RETURN_NULL_IF(buf + 1 > bend);
					buf = p::cp_u8(asbuf(m.udl), buf);

					m.dcsd = decode_dcs(m.dcs);
					m.ud.len = octet_count(m.dcsd, m.udl);

					RETURN_NULL_IF(buf + m.ud.len > bend);
					buf = p::cpy(asbuf(m.ud.data), buf, m.ud.len);
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
					/* Parse user data len */
					RETURN_NULL_IF(buf + 1 > bend);
					buf = p::cp_u8(asbuf(m.udl), buf);

					m.dcsd = decode_dcs(m.dcs);
					m.ud.len = octet_count(m.dcsd, m.udl);

					RETURN_NULL_IF(buf + m.ud.len > bend);
					buf = p::cpy(asbuf(m.ud.data), buf, m.ud.len);
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
					/* Parse user data len */
					RETURN_NULL_IF(buf + 1 > bend);
					buf = p::cp_u8(asbuf(m.udl), buf);

					m.dcsd = decode_dcs(m.dcs);
					m.ud.len = octet_count(m.dcsd, m.udl);

					RETURN_NULL_IF(buf + m.ud.len > bend);
					buf = p::cpy(asbuf(m.ud.data), buf, m.ud.len);
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
					/* Parse user data len */
					RETURN_NULL_IF(buf + 1 > bend);
					buf = p::cp_u8(asbuf(m.udl), buf);

					m.dcsd = decode_dcs(m.dcs);
					m.ud.len = octet_count(m.dcsd, m.udl);

					RETURN_NULL_IF(buf + m.ud.len > bend);
					buf = p::cpy(asbuf(m.ud.data), buf, m.ud.len);
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
					/* Parse user data len */
					RETURN_NULL_IF(buf + 1 > bend);
					buf = p::cp_u8(asbuf(m.udl), buf);

					m.dcsd = decode_dcs(m.dcs);
					m.ud.len = octet_count(m.dcsd, m.udl);

					RETURN_NULL_IF(buf + m.ud.len > bend);
					buf = p::cpy(asbuf(m.ud.data), buf, m.ud.len);
				} else {
					m.udl = 0;
				}

				switch (on_sms_submit_report_pos(m)) {
					case resume: return bend;
					default: return nullptr;
				}
			}
	};

	std::string to_string(const validity_period_format & r) {
		std::stringstream out;
		out << "[TP-VPF:";
		switch (r) {
			case vpf_not_present:
				out << "not present";
				break;
			case vpf_enhanced:
				out << "enhanced";
				break;
			case vpf_relative:
				out << "relative";
				break;
			case vpf_absolute:
				out << "absolute";
				break;
			default:
				out << "wrong";
				break;
		}
		out << "]";
		return out.str();
	}

	std::string to_string(const type_of_number & r) {
		std::stringstream out;
		out << "[ton:";
		switch (r) {
			case ton_unknown:
				out << "unknown";
				break;
			case ton_international:
				out << "international";
				break;
			case ton_national:
				out << "national";
				break;
			case ton_net_specific:
				out << "net-specific";
				break;
			case ton_subscriber:
				out << "subscriber-num";
				break;
			case ton_alphanum:
				out << "alpha-num";
				break;
			case ton_abbr:
				out << "abbr";
				break;
			default:
				out << "reserved";
				break;
		}
		out << "]";
		return out.str();
	}

	std::string to_string(const numbering_plan & r) {
		std::stringstream out;
		out << "[np:";
		switch (r) {
			case np_unknown:
				out << "unknown";
				break;
			case np_isdn_telephony:
				out << "isdn_telephony";
				break;
			case np_data:
				out << "data";
				break;
			case np_telex:
				out << "telex";
				break;
			case np_sc_specific1:
				out << "sc specific 1";
				break;
			case np_sc_specific2:
				out << "sc specific 2";
				break;
			case np_national:
				out << "national";
				break;
			case np_private:
				out << "private";
				break;
			case np_ermes:
				out << "ermes";
				break;
			default:
				out << "reserved";
				break;
		}
		out << "]";
		return out.str();
	}

	template <bin::sz_t MaxLen>
	std::string to_string(const address_tt<MaxLen> & r) {
		std::stringstream out;
		/* TODO: print value */
		out << to_string(r.np) << to_string(r.ton)
			<< "[val:"
			<< bin::hex_str_ref(r.data, (r.len >> 1) + (r.len % 2)) << "]";
		return out.str();
	}

	std::string to_string(const pi_t & r) {
		std::stringstream out;
		out << "[TP-PI:";
		if (r.pid) { out << "[pid]"; }
		if (r.dcs) { out << "[dcs]"; }
		if (r.udl) { out << "[udl]"; }
		if (r.r1) { out << "[r1]"; }
		if (r.r2) { out << "[r2]"; }
		if (r.r3) { out << "[r3]"; }
		if (r.r4) { out << "[r4]"; }
		if (r.ext) { out << "[ext]"; }
		out << "]";
		return out.str();
	}

	std::string to_string(const deliver_t & r) {
		/* TODO: decode and print PID */
		/* TODO: decode and print SCTS */
		/* TODO: decode and print DCS */
		std::stringstream out;
		out << "[SMS-DELIVER:"
			<< "[MMS:" << r.mms << "]"
			<< "[LP:" << r.lp << "]"
			<< "[RP:" << r.rp << "]"
			<< "[UDHI:" << r.udhi << "]"
			<< "[SRI:" << r.sri << "]"
			<< "[OA:" << to_string(r.oa) << "]"
			<< "[UDL:" << static_cast<unsigned>(r.udl) << "]"
			<< "[UD:" << bin::hex_str_ref(r.ud) << "]"
			<< "]";
		return out.str();
	}

	std::string to_string(const submit_t & r) {
		/* TODO: decode and print PID */
		/* TODO: decode and print SCTS */
		/* TODO: decode and print DCS */
		std::stringstream out;
		out << "[SMS-SUBMIT:"
			<< "[RD:" << r.rd << "]"
			<< to_string(r.vpf)
			<< "[RP:" << r.rp << "]"
			<< "[UDHI:" << r.udhi << "]"
			<< "[SRR:" << r.srr << "]"
			<< "[MR:" << static_cast<unsigned>(r.mr) << "]"
			<< "[DA:" << to_string(r.da) << "]"
			<< "[UDL:" << static_cast<unsigned>(r.udl) << "]"
			<< "[UD:" << bin::hex_str_ref(r.ud) << "]"
			<< "]";
		return out.str();
	}

	std::string to_string(const command_t & r) {
		/* TODO: print TP-PID */
		/* TODO: print TP-CT */
		std::stringstream out;
		out << "[SMS-COMMAND:"
			<< "[UDHI:" << r.udhi << "]"
			<< "[SRR:" << r.srr << "]"
			<< "[MR:" << static_cast<unsigned>(r.mr) << "]"
			<< "[MN:" << static_cast<unsigned>(r.mn) << "]"
			<< "[DA:" << to_string(r.da) << "]"
			<< "[CDL:" << static_cast<unsigned>(r.cdl) << "]"
			<< "[CD:" << bin::hex_str_ref(r.cd) << "]"
			<< "]";
		return out.str();
	}

	std::string to_string(const status_report_t & r) {
		/* TODO: decode and print TP-PID */
		/* TODO: decode and print TP-SCTS */
		/* TODO: print SCTS */
		/* TODO: print DT */
		/* TODO: print ST */
		std::stringstream out;
		out << "[SMS-STATUS-REPORT:"
			<< "[UDHI:" << r.udhi << "]"
			<< "[MMS:" << r.mms << "]"
			<< "[LP:" << r.lp << "]"
			<< "[SRQ:" << r.srq << "]"
			<< "[MR:" << static_cast<unsigned>(r.mr) << "]"
			<< "[RA:" << to_string(r.ra) << "]"
			<< to_string(r.pi)
			<< "[UDL:" << static_cast<unsigned>(r.udl) << "]"
			<< "[UD:" << bin::hex_str_ref(r.ud) << "]"
			<< "]";
		return out.str();
	}

	std::string to_string(const deliver_report_neg_t & r) {
		/* TODO: print FC */
		/* TODO: print SCTS */
		/* TODO: print DCTS */
		std::stringstream out;
		out << "[SMS-DELIVER-REPORT(ERR):"
			<< "[UDHI:" << r.udhi << "]"
			<< "[FCS:" << static_cast<unsigned>(r.fcs) << "]"
			<< to_string(r.pi)
			<< "[UDL:" << static_cast<unsigned>(r.udl) << "]"
			<< "[UD:" << bin::hex_str_ref(r.ud) << "]"
			<< "]";
		return out.str();
	}

	std::string to_string(const deliver_report_pos_t & r) {
		/* TODO: print SCTS */
		/* TODO: print DCS */
		std::stringstream out;
		out << "[SMS-DELIVER-REPORT(ACK):"
			<< "[UDHI:" << r.udhi << "]"
			<< to_string(r.pi)
			<< "[UDL:" << static_cast<unsigned>(r.udl) << "]"
			<< "[UD:" << bin::hex_str_ref(r.ud) << "]"
			<< "]";
		return out.str();
	}

	std::string to_string(const submit_report_neg_t & r) {
		/* TODO: print SCTS */
		/* TODO: print DCS */
		/* TODO: print FC */
		std::stringstream out;
		out << "[SMS-SUBMIT-REPORT(ERR):"
			<< "[UDHI:" << r.udhi << "]"
			<< to_string(r.pi)
			<< "[UDL:" << static_cast<unsigned>(r.udl) << "]"
			<< "[UD:" << bin::hex_str_ref(r.ud) << "]"
			<< "]";
		return out.str();
	}

	std::string to_string(const submit_report_pos_t & r) {
		/* TODO: print SCTS */
		/* TODO: print DCS */
		std::stringstream out;
		out << "[SMS-SUBMIT-REPORT(ACK):"
			<< "[UDHI:" << r.udhi << "]"
			<< to_string(r.pi)
			<< "[UDL:" << static_cast<unsigned>(r.udl) << "]"
			<< "[UD:" << bin::hex_str_ref(r.ud) << "]"
			<< "]";
		return out.str();
	}

} } }

#endif
