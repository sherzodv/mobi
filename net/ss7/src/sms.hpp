#ifndef mobi_net_sms_hpp
#define mobi_net_sms_hpp

/* 3GPP TS 23.040 */

#include <bitset>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <ostream>

#include <ss7/decod.hpp>
#include <toolbox/bin.hpp>

namespace mobi { namespace net { namespace sms {

	using namespace toolbox;

	const bin::sz_t max_udl = 200;

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

	enum protocol_identifier {
		pi_telematic_implicit			= 0x20
		, pi_telematic_telex			= 0x21
		, pi_telematic_group3_telefax	= 0x22
		, pi_telematic_group4_telefax	= 0x23
		, pi_telematic_voice_telephone	= 0x24
		, pi_telematic_ermes			= 0x25
		, pi_telematic_national_paging	= 0x26
		, pi_telematic_videotex			= 0x27
		, pi_telematic_telex_no_carrier	= 0x28
		, pi_telematic_telex_pspdn		= 0x29
		, pi_telematic_telex_cspdn		= 0x2A
		, pi_telematic_telex_pstn		= 0x2B
		, pi_telematic_telex_isdn		= 0x2C
		, pi_telematic_telex_uci		= 0x2D
		, pi_telematic_reserved1		= 0x2E
		, pi_telematic_reserved2		= 0x2F
		, pi_telematic_msg_handling		= 0x30
		, pi_telematic_x400_msg_handling= 0x31
		, pi_telematic_email			= 0x32
		, pi_telematic_reserved3		= 0x33
		, pi_telematic_reserved4		= 0x34
		, pi_telematic_reserved5		= 0x35
		, pi_telematic_reserved6		= 0x36
		, pi_telematic_reserved7		= 0x37
		, pi_telematic_sc_specific1		= 0x38
		, pi_telematic_sc_specific2		= 0x39
		, pi_telematic_sc_specific3		= 0x3A
		, pi_telematic_sc_specific4		= 0x3B
		, pi_telematic_sc_specific5		= 0x3C
		, pi_telematic_sc_specific6		= 0x3D
		, pi_telematic_sc_specific7		= 0x3E
		, pi_telematic_mobile_station	= 0x3F

		, pi_sms_type_0				= 0x40
		, pi_replace_sms_type_1		= 0x41
		, pi_replace_sms_type_2		= 0x42
		, pi_replace_sms_type_3		= 0x43
		, pi_replace_sms_type_4		= 0x44
		, pi_replace_sms_type_5		= 0x45
		, pi_replace_sms_type_6		= 0x46
		, pi_replace_sms_type_7		= 0x47
		, pi_device_trigger_sms		= 0x48
		, pi_enhanced_msg_service	= 0x5E
		, pi_return_call_msg		= 0x5F
		, pi_ansi_136_r_data		= 0x7C
		, pi_me_data_download		= 0x7D
		, pi_me_depers_sms			= 0x7E
		, pi_sim_data_download		= 0x7F
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

	struct timestamp {
		bin::u8_t year;
		bin::u8_t month;
		bin::u8_t day;
		bin::u8_t hour;
		bin::u8_t minute;
		bin::u8_t second;
		bin::u8_t zone;
	};

	/* Address field: TS 23.040, 9.1.2.5 */
	template <bin::sz_t MaxLen>
	struct address_tt {
		numbering_plan	np: 4;
		type_of_number	ton: 3;
		bin::u8_t		pad: 1;
		bin::u8_t		len;
		bin::u8_t		data[MaxLen];

		/* Calculate number of octets occupied by data */
		bin::sz_t length() const {
			return (len >> 1) + (len % 2);
		}

		void set_digits(const char * addr, bin::sz_t l) {
			len = l;
			bcd_encode(data, addr, len);
		}
	};

	template <bin::sz_t MaxLen>
	struct string_tt {
		bin::sz_t len;
		bin::u8_t data[MaxLen];
	};

	template <bin::sz_t MaxLen>
	struct utf16_string_tt {
		bin::sz_t len;
		bin::u16_t data[MaxLen];
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
		timestamp		scts;	/* SC timestamp */
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
		timestamp		scts;	/* SC timestamp */
		timestamp		dt;		/* Discharge time */
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
		timestamp		scts;	/* SC timestamp */
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
		timestamp		scts;	/* SC timestamp */
		bin::u8_t		pid;	/* Protocol identifier */
		bin::u8_t		dcs;	/* Data coding scheme */
		bin::u8_t		udl;	/* User data length */
		string_tt<152>	ud;		/* User data */
		dc_scheme		dcsd;	/* Decoded data coding scheme */
	};

	/* User Data Header Information Elements */
	namespace ie {

		struct header {
			bin::u8_t id;
			bin::u8_t len;

			enum {
				concat				= 0x00
				, special_ind		= 0x01
				, port_addr_8bit	= 0x04
				, port_addr_16bit	= 0x05
				, smsc_control		= 0x06
				, udh_source_ind	= 0x07
				, concat_enhanced	= 0x08
				, wireless_control	= 0x09
			} type;
		};

		/* Concatenated sm-s */
		struct concat {
			/* Concatenated sm reference number */
			bin::u8_t refno;
			/* Maximum number of short messages in the concatenated sm */
			bin::u8_t maxnum;
			/* Sequence number of the current sm */
			bin::u8_t seqno;
		};

		/* Special sm message indication */
		struct special_ind {
			struct {
				bool			store		: 1;
				bin::u8_t		profile_id	: 2;
				bin::u8_t		extind		: 3;
				indication_type	ind			: 2;
			} type;
			bin::u8_t msg_count;
		};

		/* Application Port Addressing 8 bit address */
		struct port_addr_8bit {
			bin::u8_t dst_port;
			bin::u8_t src_port;
		};

		/* Application Port Addressing 16 bit address */
		struct port_addr_16bit {
			bin::u16_t dst_port;
			bin::u16_t src_port;
		};

		/* SMSC Control Parameters */
		struct smsc_control {
			struct {
				bool		include_udhl: 1;
				bool		cancel_srr: 1;
				bool		if_temp_error_more: 1;
				bool		if_temp_error: 1;
				bool		if_perm_error: 1;
				bool		if_completed: 1;
			} report;
		};

		/* UDH Source Indicator */
		struct udh_source_ind {
			enum: bin::u8_t { sender, receiver, smsc } src;
		};

		/* (U)SIM Toolkit Security Headers */

		/* Concatenated sm, 16-bit reference number */
		struct concat_enhanced {
			/* Concatenated sm reference number */
			bin::u16_t refno;
			/* Maximum number of short messages in the concatenated sm */
			bin::u8_t maxnum;
			/* Sequence number of the current sm */
			bin::u8_t seqno;
		};

		/* Wireless Control Message Protocol */
		struct wireless_control {
			bin::u8_t len;
			bin::u8_t data[254];
		};
	}

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

			const bin::u8_t * parse_user_data_header(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				u8_t len;
				ie::header hdr;

				RETURN_NULL_IF(buf + 1 > bend);

				/* Parse user header len */
				buf = p::cp_u8(asbuf(len), buf);

				while (len) {
					RETURN_NULL_IF(buf + 2 > bend || len < 2);
					/* Read Information Element tag and length */
					buf = p::cp_u8(asbuf(hdr.id), buf);
					buf = p::cp_u8(asbuf(hdr.len), buf);
					/* See if len is correct and we have enough data to parse
					 * body of information element */
					RETURN_NULL_IF(buf + hdr.len > bend || len < hdr.len + 2);
					len -= hdr.len + 2;
					switch (hdr.id) {
						case ie::header::concat: {
							RETURN_NULL_IF(hdr.len != 3);
							ie::concat r;
							buf = p::cp_u8(asbuf(r.refno), buf);
							buf = p::cp_u8(asbuf(r.maxnum), buf);
							buf = p::cp_u8(asbuf(r.seqno), buf);
							switch (on_sms_ie_concat(r)) {
								case resume: continue;
								case skip: continue;
								default: return nullptr;
							}
						}
						case ie::header::special_ind: {
							RETURN_NULL_IF(hdr.len != 2);
							ie::special_ind r;
							buf = p::cp_u8(asbuf(r.type), buf);
							buf = p::cp_u8(asbuf(r.msg_count), buf);
							switch (on_sms_ie_special_ind(r)) {
								case resume: continue;
								case skip: continue;
								default: return nullptr;
							}
						}
						case ie::header::port_addr_8bit: {
							RETURN_NULL_IF(hdr.len != 2);
							ie::port_addr_8bit r;
							buf = p::cp_u8(asbuf(r.dst_port), buf);
							buf = p::cp_u8(asbuf(r.src_port), buf);
							switch (on_sms_ie_port_addr_8bit(r)) {
								case resume: continue;
								case skip: continue;
								default: return nullptr;
							}
						}
						case ie::header::port_addr_16bit: {
							RETURN_NULL_IF(hdr.len != 4);
							ie::port_addr_16bit r;
							buf = p::cp_u16(asbuf(r.dst_port), buf);
							buf = p::cp_u16(asbuf(r.src_port), buf);
							switch (on_sms_ie_port_addr_16bit(r)) {
								case resume: continue;
								case skip: continue;
								default: return nullptr;
							}
						}
						case ie::header::smsc_control: {
							RETURN_NULL_IF(hdr.len != 1);
							ie::smsc_control r;
							buf = p::cp_u8(asbuf(r.report), buf);
							switch (on_sms_ie_smsc_control(r)) {
								case resume: continue;
								case skip: continue;
								default: return nullptr;
							}
						}
						case ie::header::udh_source_ind: {
							RETURN_NULL_IF(hdr.len != 1);
							ie::udh_source_ind r;
							buf = p::cp_u8(asbuf(r.src), buf);
							switch (on_sms_ie_udh_source_ind(r)) {
								case resume: continue;
								case skip: continue;
								default: return nullptr;
							}
						}
						case ie::header::concat_enhanced: {
							RETURN_NULL_IF(hdr.len != 4);
							ie::concat_enhanced r;
							buf = p::cp_u16(asbuf(r.refno), buf);
							buf = p::cp_u8(asbuf(r.maxnum), buf);
							buf = p::cp_u8(asbuf(r.seqno), buf);
							switch (on_sms_ie_concat_enhanced(r)) {
								case resume: continue;
								case skip: continue;
								default: return nullptr;
							}
						}
						case ie::header::wireless_control: {
							RETURN_NULL_IF(hdr.len < 1);
							ie::wireless_control r;
							buf = p::cp_u8(asbuf(r.len), buf);
							RETURN_NULL_IF(buf + r.len > bend);
							buf = p::cpy(r.data, buf, r.len);
							switch (on_sms_ie_wireless_control(r)) {
								case resume: continue;
								case skip: continue;
								default: return nullptr;
							}
						}
						// TODO: parse EMS headers
					}
				}

				return buf;
			}

			template <class MessageT, bin::sz_t MaxLen>
			const bin::u8_t * parse_text(string_tt<MaxLen> & r
					, const MessageT & msg) {

				using namespace bin;

				bin::u8_t offs;
				const bin::u8_t * cur = msg.ud.data;
				const bin::u8_t * cend = cur + msg.ud.len;

				RETURN_NULL_IF(cur >= cend || msg.ud.len > 256);

				offs = 0;
				r.len = 0;

				if (msg.udhi) {
					/* Skip header */
					cur += *cur;
					RETURN_NULL_IF(cur >= cend);
					if (msg.dcsd.cs == cs_gsm_7bit) {
						/* Count the number of fill bits */
						offs += 7 - (*cur * 8) % 7;
						if (offs == 7) {
							offs = 0;
						}
					}
				}

				switch (msg.dcsd.cs) {
					case cs_gsm_7bit:
						return gsm::sept_to_oct(r, cur, cend, offs);
					case cs_8bit:
						std::memcpy(r.data, cur, cend - cur);
						r.len = cend - cur;
						return cend;
					default:
						return nullptr;
				}
			}

			template <class MessageT, bin::sz_t MaxLen>
			const bin::u8_t * parse_text(utf16_string_tt<MaxLen> & r
					, const MessageT & msg) {

				using namespace bin;

				bin::u8_t offs;
				const bin::u8_t * cur = msg.ud.data;
				const bin::u8_t * cend = cur + msg.ud.len;

				RETURN_NULL_IF(cur >= cend || msg.ud.len > 256);

				offs = 0;
				r.len = 0;

				if (msg.udhi) {
					/* Skip header */
					cur += *cur;
					RETURN_NULL_IF(cur >= cend);
					if (msg.dcsd.cs == cs_gsm_7bit) {
						/* Count the number of fill bits */
						offs += 7 - (*cur * 8) % 7;
						if (offs == 7) {
							offs = 0;
						}
					}
				}

				string_tt<MaxLen> temp;

				switch (msg.dcsd.cs) {
					case cs_gsm_7bit:
						gsm::sept_to_oct(temp, cur, cend, offs);
						break;
					case cs_8bit:
						std::memcpy(temp.data, cur, cend - cur);
						break;
					case cs_ucs2:
						std::memcpy(r.data, cur, cend - cur);
						break;
					default:
						return nullptr;
				}

				if (gsm::sbit_to_utf16(r, temp) == temp.len) {
					return cend;
				} else {
					return nullptr;
				}
			}

			void parse_dcs(bin::u8_t dcs, dc_scheme & r) {

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

			virtual action on_sms_ie_concat(const ie::concat & r) = 0;
			virtual action on_sms_ie_special_ind(const ie::special_ind & r) = 0;
			virtual action on_sms_ie_port_addr_8bit(const ie::port_addr_8bit & r) = 0;
			virtual action on_sms_ie_port_addr_16bit(const ie::port_addr_16bit & r) = 0;
			virtual action on_sms_ie_smsc_control(const ie::smsc_control & r) = 0;
			virtual action on_sms_ie_udh_source_ind(const ie::udh_source_ind & r) = 0;
			virtual action on_sms_ie_sim_security(const ie::header & r) = 0;
			virtual action on_sms_ie_concat_enhanced(const ie::concat_enhanced & r) = 0;
			virtual action on_sms_ie_wireless_control(const ie::wireless_control & r) = 0;

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
				RETURN_NULL_IF(buf + m.oa.length() > bend);
				buf = p::cpy(m.oa.data, buf, m.oa.length());

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

				parse_dcs(m.dcs, m.dcsd);
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

				RETURN_NULL_IF(buf + m.da.length() > bend);
				buf = p::cpy(m.da.data, buf, m.da.length());

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

				parse_dcs(m.dcs, m.dcsd);
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

				RETURN_NULL_IF(buf + m.da.length() > bend);
				buf = p::cpy(m.da.data, buf, m.da.length());

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

				RETURN_NULL_IF(buf + m.ra.length() > bend);
				buf = p::cpy(m.ra.data, buf, m.ra.length());

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

					parse_dcs(m.dcs, m.dcsd);
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

					parse_dcs(m.dcs, m.dcsd);
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

					parse_dcs(m.dcs, m.dcsd);
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

					parse_dcs(m.dcs, m.dcsd);
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

					parse_dcs(m.dcs, m.dcsd);
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

	template <class LogT>
	class writer {
		protected:
			LogT & L;
		public:
			writer(LogT & l): L(l) {}
			virtual ~writer() {}

			bin::u8_t * write_sms_deliver(bin::u8_t * buf, bin::u8_t * bend
					, const deliver_t & r) {

				using namespace bin;

				RETURN_NULL_IF(buf
					+ sizeof(bin::u8_t)		/* mti, mms, lp, rp, udhi, sri */
					+ sizeof(bin::u16_t)	/* address len and type */
					> bend);

				/* Write mti, mms, lp, rp, udhi, sri */
				buf = w::cp_u8(buf, ascbuf(r));

				/* Write len and type-of-address at once */
				buf = w::cp_u16(buf, ascbuf(r.oa));

				RETURN_NULL_IF(buf + r.oa.length() > bend);
				buf = w::cpy(buf, r.oa.data, r.oa.length());

				RETURN_NULL_IF(buf
					+ sizeof(bin::u8_t)	/* Protocol identifier */
					+ sizeof(bin::u8_t)	/* Code scheme */
					+ 7					/* SC timestamp */
					+ sizeof(bin::u8_t)	/* User data length */
					> bend);

				/* Write protocol identifier */
				buf = w::cp_u8(buf, ascbuf(r.pid));
				/* Write data coding scheme */
				buf = w::cp_u8(buf, ascbuf(r.dcs));
				/* Parse sc timestamp */
				buf = w::cpy(buf, ascbuf(r.scts), 7);
				/* Parse user data length */
				buf = w::cp_u8(buf, ascbuf(r.udl));

				RETURN_NULL_IF(buf + r.ud.len > bend);
				return w::cpy(buf, ascbuf(r.ud.data), r.ud.len);
			}

			bin::u8_t * write_sms_submit(bin::u8_t * buf, bin::u8_t * bend
					, const submit_t & r) {
				using namespace bin;

				RETURN_NULL_IF(buf
					+ sizeof(bin::u8_t)		/* mti, rd, vpf, rp, udhi, srr */
					+ sizeof(bin::u8_t)		/* mr */
					+ sizeof(bin::u16_t)	/* address len and type */
					> bend);

				/* Write mti, rd, vpf, rp, udhi, srr */
				buf = w::cp_u8(buf, ascbuf(r));
				/* Write message reference */
				buf = w::cp_u8(buf, ascbuf(r.mr));
				/* Write len and type-of-address at once */
				buf = w::cp_u16(buf, ascbuf(r.da));

				RETURN_NULL_IF(buf + r.da.length() > bend);
				buf = w::cpy(buf, r.da.data, r.da.length());

				RETURN_NULL_IF(buf
					+ sizeof(bin::u8_t)		/* pid */
					+ sizeof(bin::u8_t)		/* dcs */
					> bend);

				/* Write protocol identifier */
				buf = w::cp_u8(buf, ascbuf(r.pid));
				/* Write data coding scheme */
				buf = w::cp_u8(buf, ascbuf(r.dcs));

				/* Write validity period if present */
				switch (r.vpf) {
					case vpf_relative:
						RETURN_NULL_IF(buf + 1 > bend);
						buf = w::cpy(buf, ascbuf(r.vp), buf);
						break;
					case vpf_enhanced:
					case vpf_absolute:
						RETURN_NULL_IF(buf + 7 > bend);
						buf = w::cpy(buf, ascbuf(r.vp), 7);
						break;
					default: break;
				}

				/* Write user data length */
				RETURN_NULL_IF(buf + 1 > bend);
				buf = p::cp_u8(buf, ascbuf(r.udl));

				RETURN_NULL_IF(buf + r.ud.len > bend);
				buf = p::cpy(buf, ascbuf(r.ud.data), r.ud.len);

				return buf;
			}

			bin::u8_t * write_sms_command(bin::u8_t * buf, bin::u8_t * bend
					, const command_t & msg) {
				return nullptr;
			}

			bin::u8_t * write_sms_status_report(bin::u8_t * buf
					, bin::u8_t * bend, const status_report_t & msg) {
				return nullptr;
			}

			bin::u8_t * write_sms_deliver_report_neg(bin::u8_t * buf
					, bin::u8_t * bend
					, const deliver_report_neg_t & msg) {
				return nullptr;
			}

			bin::u8_t * write_sms_deliver_report_pos(bin::u8_t * buf
					, bin::u8_t * bend, const deliver_report_pos_t & msg) {
				return nullptr;
			}

			bin::u8_t * write_sms_submit_report_neg(bin::u8_t * buf
					, bin::u8_t * bend, const submit_report_neg_t & msg) {
				return nullptr;
			}

			bin::u8_t * write_sms_submit_report_pos(bin::u8_t * buf
					, bin::u8_t * bend, const submit_report_pos_t & msg) {
				return nullptr;
			}

			bin::u8_t * write_sms_ie_concat(bin::u8_t * buf
					, bin::u8_t * bend, const ie::concat & r) {
				using namespace bin;
				RETURN_NULL_IF(buf + 5 > bend);
				*buf++ = ie::header::concat;
				*buf++ = 0x03;
				*buf++ = r.refno;
				*buf++ = r.maxnum;
				*buf++ = r.seqno;
				return buf;
			}

			bin::u8_t * write_sms_ie_special_ind(bin::u8_t * buf
					, bin::u8_t * bend, const ie::special_ind & r) {
				using namespace bin;
				RETURN_NULL_IF(buf + 4 > bend);
				*buf++ = ie::header::special_ind;
				*buf++ = 0x02;
				buf = w::cp_u8(buf, ascbuf(r.type));
				return w::cp_u8(buf, ascbuf(r.msg_count));
			}

			bin::u8_t * write_sms_ie_port_addr_8bit(bin::u8_t * buf
					, bin::u8_t * bend, const ie::port_addr_8bit & r) {
				using namespace bin;
				RETURN_NULL_IF(buf + 4 > bend);
				*buf++ = ie::header::port_addr_8bit;
				*buf++ = 0x02;
				*buf++ = r.dst_port;
				*buf++ = r.src_port;
				return buf;
			}

			bin::u8_t * write_sms_ie_port_addr_16bit(bin::u8_t * buf
					, bin::u8_t * bend, const ie::port_addr_16bit & r) {
				using namespace bin;
				RETURN_NULL_IF(buf + 6 > bend);
				*buf++ = ie::header::port_addr_16bit;
				*buf++ = 0x04;
				buf = w::cp_u16(buf, ascbuf(r.dst_port));
				return w::cp_u16(buf, ascbuf(r.src_port));
			}

			bin::u8_t * write_sms_ie_smsc_control(bin::u8_t * buf
					, bin::u8_t * bend, const ie::smsc_control & r) {
				using namespace bin;
				RETURN_NULL_IF(buf + 3 > bend);
				*buf++ = ie::header::smsc_control;
				*buf++ = 0x01;
				return w::cp_u8(buf, ascbuf(r.report));
			}

			bin::u8_t * write_sms_ie_udh_source_ind(bin::u8_t * buf
					, bin::u8_t * bend, const ie::udh_source_ind & r) {
				using namespace bin;
				RETURN_NULL_IF(buf + 3 > bend);
				*buf++ = ie::header::udh_source_ind;
				return w::cp_u8(buf, ascbuf(r.src));
			}

			bin::u8_t * write_sms_ie_sim_security(bin::u8_t * buf
					, bin::u8_t * bend, bin::u8_t value) {
				using namespace bin;
				RETURN_NULL_IF(buf + 3 > bend);
				*buf++ = value;
				*buf++ = 0;
				return buf;
			}

			bin::u8_t * write_sms_ie_concat_enhanced(bin::u8_t * buf
					, bin::u8_t * bend, const ie::concat_enhanced & r) {
				using namespace bin;
				RETURN_NULL_IF(buf + 6 > bend);
				*buf++ = ie::header::concat_enhanced;
				*buf++ = 0x04;
				buf = w::cp_u16(buf, ascbuf(r.refno));
				*buf++ = r.maxnum;
				*buf++ = r.seqno;
				return buf;
			}

			bin::u8_t * write_sms_ie_wireless_control(bin::u8_t * buf
					, bin::u8_t * bend, const ie::wireless_control & r) {
				using namespace bin;
				RETURN_NULL_IF(buf + 2 + r.len > bend);
				*buf++ = ie::header::wireless_control;
				*buf++ = r.len;
				return w::cpy(buf, r.data, r.len);
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

	std::string to_string(const dc_scheme & r) {
		std::stringstream out;
		out << "[DCS:";

		switch (r.dcs) {
			case dcs_general:
				out << "general";
				break;
			case dcs_auto_deletion:
				out << "auto-deletion";
				break;
			case dcs_reserved:
				out << "reserved";
				break;
			case dcs_discard:
				out << "discard";
				break;
			case dcs_store:
				out << "store";
				break;
			case dcs_me_indicator:
				out << "me-indicator";
				break;
			case dcs_store_ucs2:
				out << "store-ucs2";
				break;
			case dcs_coding:
				out << "coding";
				break;
			case dcs_special:
				out << "special";
				break;
		}

		out << ":";
		switch (r.cs) {
			case cs_gsm_7bit:
				out << "cs-gsm-7-bit";
				break;
			case cs_8bit:
				out << "cs-8-bit";
				break;
			case cs_ucs2:
				out << "cs-ucs2";
				break;
			default:
				out << "reserved";
				break;
		}

		if (r.has_message_class) {
			out << ":";
			switch (r.klass) {
				case mc_class0:
					out << "class-0";
					break;
				case mc_class1:
					out << "class-1";
					break;
				case mc_class2:
					out << "class-2";
					break;
				case mc_class3:
					out << "class-3";
					break;
			}
		}

		if (r.compressed) {
			out << ":compressed";
		}

		if (r.indicate) {
			out << ":";
			switch (r.ind) {
				case indication_voice:
					out << "ind-voice";
					break;
				case indication_fax:
					out << "ind-fax";
					break;
				case indication_email:
					out << "ind-email";
					break;
				case indication_other:
					out << "ind-other";
					break;
			}
		}

		out << "]";
		return out.str();
	}

	std::string to_string(const timestamp & r) {
		std::stringstream out;
		out << bcd_decode(r.year & 0x0F) << bcd_decode(r.year >> 4);
		out << "-" << bcd_decode(r.month & 0x0F) << bcd_decode(r.month >> 4);
		out << "-" << bcd_decode(r.day & 0x0F) << bcd_decode(r.day >> 4);
		out << ":" << bcd_decode(r.hour & 0x0F) << bcd_decode(r.hour >> 4);
		out << "-" << bcd_decode(r.minute & 0x0F) << bcd_decode(r.minute >> 4);
		out << "-" << bcd_decode(r.second & 0x0F) << bcd_decode(r.second >> 4);
		out << ":" << bcd_decode(r.zone & 0x0F) << bcd_decode(r.zone >> 4);
		return out.str();
	}

	std::string pid_to_string(bin::u8_t r) {
		std::stringstream out;
		out << "[PID:";
		switch (r & 0xC0) {
			case 0x00:
				if ((r & 0x20) == 0x20) {
					/* telematic internetworking */
					out << "telematic:";
					switch (r & 0x1F) {
						case 0x00:
							out << "implicit";
							break;
						case 0x01:
							out << "telex";
							break;
						case 0x02:
							out << "group3_telefax";
							break;
						case 0x03:
							out << "group4_telefax";
							break;
						case 0x04:
							out << "voice_telephone";
							break;
						case 0x05:
							out << "ermes";
							break;
						case 0x06:
							out << "national_paging";
							break;
						case 0x07:
							out << "videotex";
							break;
						case 0x08:
							out << "telex_no_carrier";
							break;
						case 0x09:
							out << "telex_pspdn";
							break;
						case 0x0A:
							out << "telex_cspdn";
							break;
						case 0x0B:
							out << "telex_pstn";
							break;
						case 0x0C:
							out << "telex_isdn";
							break;
						case 0x0D:
							out << "telex_uci";
							break;
						case 0x10:
							out << "msg_handling";
							break;
						case 0x11:
							out << "x400_msg_handling";
							break;
						case 0x12:
							out << "email";
							break;
						case 0x1F:
							out << "mobile_station";
							break;
						case 0x0E:
						case 0x0F:
						case 0x13:
						case 0x14:
						case 0x15:
						case 0x16:
						case 0x17:
							out << "reserved";
							break;
						default:
							out << "sc specific";
							break;
					}
				} else {
					out << "SME-to-SME:" << static_cast<bin::sz_t>(r & 0x1F);
				}
				break;
			case 0x01:
				switch (r & 0x3F) {
					case 0x00:
						out << "sms_type_0";
						break;
					case 0x01:
						out << "replace_sms_type_1";
						break;
					case 0x02:
						out << "replace_sms_type_2";
						break;
					case 0x03:
						out << "replace_sms_type_3";
						break;
					case 0x04:
						out << "replace_sms_type_4";
						break;
					case 0x05:
						out << "replace_sms_type_5";
						break;
					case 0x06:
						out << "replace_sms_type_6";
						break;
					case 0x07:
						out << "replace_sms_type_7";
						break;
					case 0x08:
						out << "device_trigger_sms";
						break;
					case 0x1E:
						out << "enhanced_msg_service";
						break;
					case 0x1F:
						out << "return_call_msg";
						break;
					case 0x3C:
						out << "ansi_136_r_data";
						break;
					case 0x3D:
						out << "me_data_download";
						break;
					case 0x3E:
						out << "me_depers_sms";
						break;
					case 0x3F:
						out << "sim_data_download";
						break;
					default:
						out << "reserved";
						break;
				}
				break;
			case 0x02:
				out << "reserved";
				break;
			case 0x03:
				out << "sc-specific:" << static_cast<bin::sz_t>(r & 0x3F);
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
			<< bin::hex_str_ref(r.data, r.length()) << "]";
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
		std::stringstream out;
		out << "[SMS-DELIVER:"
			<< "[MMS:" << r.mms << "]"
			<< "[LP:" << r.lp << "]"
			<< "[RP:" << r.rp << "]"
			<< "[UDHI:" << r.udhi << "]"
			<< "[SRI:" << r.sri << "]"
			<< "[OA:" << to_string(r.oa) << "]"
			<< pid_to_string(r.pid)
			<< to_string(r.dcsd)
			<< "[SCTS:" << to_string(r.scts) << "]"
			<< "[UDL:" << static_cast<unsigned>(r.udl) << "]"
			<< "[UD:" << bin::hex_str_ref(r.ud) << "]"
			<< "]";
		return out.str();
	}

	std::string to_string(const submit_t & r) {
		std::stringstream out;
		out << "[SMS-SUBMIT:"
			<< "[RD:" << r.rd << "]"
			<< to_string(r.vpf)
			<< "[RP:" << r.rp << "]"
			<< "[UDHI:" << r.udhi << "]"
			<< "[SRR:" << r.srr << "]"
			<< "[MR:" << static_cast<unsigned>(r.mr) << "]"
			<< "[DA:" << to_string(r.da) << "]"
			<< pid_to_string(r.pid)
			<< to_string(r.dcsd)
			<< "[UDL:" << static_cast<unsigned>(r.udl) << "]"
			<< "[UD:" << bin::hex_str_ref(r.ud) << "]"
			<< "]";
		return out.str();
	}

	std::string to_string(const command_t & r) {
		/* TODO: print TP-CT */
		std::stringstream out;
		out << "[SMS-COMMAND:"
			<< "[UDHI:" << r.udhi << "]"
			<< "[SRR:" << r.srr << "]"
			<< "[MR:" << static_cast<unsigned>(r.mr) << "]"
			<< pid_to_string(r.pid)
			<< "[MN:" << static_cast<unsigned>(r.mn) << "]"
			<< "[DA:" << to_string(r.da) << "]"
			<< "[CDL:" << static_cast<unsigned>(r.cdl) << "]"
			<< "[CD:" << bin::hex_str_ref(r.cd) << "]"
			<< "]";
		return out.str();
	}

	std::string to_string(const status_report_t & r) {
		/* TODO: print ST */
		std::stringstream out;
		out << "[SMS-STATUS-REPORT:"
			<< "[UDHI:" << r.udhi << "]"
			<< "[MMS:" << r.mms << "]"
			<< "[LP:" << r.lp << "]"
			<< "[SRQ:" << r.srq << "]"
			<< "[MR:" << static_cast<unsigned>(r.mr) << "]"
			<< "[RA:" << to_string(r.ra) << "]"
			<< "[SCTS:" << to_string(r.scts) << "]"
			<< "[DT:" << to_string(r.dt) << "]"
			<< to_string(r.pi)
			<< pid_to_string(r.pid)
			<< to_string(r.dcsd)
			<< "[UDL:" << static_cast<unsigned>(r.udl) << "]"
			<< "[UD:" << bin::hex_str_ref(r.ud) << "]"
			<< "]";
		return out.str();
	}

	std::string to_string(const deliver_report_neg_t & r) {
		/* TODO: print FC */
		std::stringstream out;
		out << "[SMS-DELIVER-REPORT(ERR):"
			<< "[UDHI:" << r.udhi << "]"
			<< "[FCS:" << static_cast<unsigned>(r.fcs) << "]"
			<< to_string(r.pi)
			<< pid_to_string(r.pid)
			<< to_string(r.dcsd)
			<< "[UDL:" << static_cast<unsigned>(r.udl) << "]"
			<< "[UD:" << bin::hex_str_ref(r.ud) << "]"
			<< "]";
		return out.str();
	}

	std::string to_string(const deliver_report_pos_t & r) {
		std::stringstream out;
		out << "[SMS-DELIVER-REPORT(ACK):"
			<< "[UDHI:" << r.udhi << "]"
			<< to_string(r.pi)
			<< pid_to_string(r.pid)
			<< to_string(r.dcsd)
			<< "[UDL:" << static_cast<unsigned>(r.udl) << "]"
			<< "[UD:" << bin::hex_str_ref(r.ud) << "]"
			<< "]";
		return out.str();
	}

	std::string to_string(const submit_report_neg_t & r) {
		/* TODO: print FC */
		std::stringstream out;
		out << "[SMS-SUBMIT-REPORT(ERR):"
			<< "[UDHI:" << r.udhi << "]"
			<< to_string(r.pi)
			<< "[SCTS:" << to_string(r.scts) << "]"
			<< pid_to_string(r.pid)
			<< to_string(r.dcsd)
			<< "[UDL:" << static_cast<unsigned>(r.udl) << "]"
			<< "[UD:" << bin::hex_str_ref(r.ud) << "]"
			<< "]";
		return out.str();
	}

	std::string to_string(const submit_report_pos_t & r) {
		std::stringstream out;
		out << "[SMS-SUBMIT-REPORT(ACK):"
			<< "[UDHI:" << r.udhi << "]"
			<< to_string(r.pi)
			<< "[SCTS:" << to_string(r.scts) << "]"
			<< pid_to_string(r.pid)
			<< to_string(r.dcsd)
			<< "[UDL:" << static_cast<unsigned>(r.udl) << "]"
			<< "[UD:" << bin::hex_str_ref(r.ud) << "]"
			<< "]";
		return out.str();
	}

} } }

#endif
