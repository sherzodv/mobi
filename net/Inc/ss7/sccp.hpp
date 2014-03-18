#ifndef mobi_net_ss7_sccp_hpp
#define mobi_net_ss7_sccp_hpp

#include <string>
#include <cstdint>
#include <ostream>
#include <iomanip>
#include <bitset>

#include <toolbox/bin.hpp>

namespace mobi { namespace net { namespace ss7 { namespace sccp {

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
			case 'A': return 0x0A;
			case 'B': return 0x0B;
			case 'C': return 0x0C;
			case 'D': return 0x0D;
			case 'E': return 0x0E;
			case 'F': return 0x0F;
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
			case 0x0A: return 'A';
			case 0x0B: return 'B';
			case 0x0C: return 'C';
			case 0x0D: return 'D';
			case 0x0E: return 'E';
			case 0x0F: return 'F';
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

	/* Generic SCCP message */
	struct message {
		bin::u8_t type;
	};

	/* Generic SCCP parameter */
	struct parameter {
		bin::u8_t len;
	};

	/* ITU-T REC Q.713, 2.1, Table 1 */
	/* SCCP message types */
	enum message_type {
		msg_connection_request	= 0x01,
		msg_connection_confirm	= 0x02,
		msg_connection_refused	= 0x03,
		msg_released			= 0x04,
		msg_release_complete	= 0x05,
		msg_data_form_1			= 0x06,
		msg_data_form_2			= 0x07,
		msg_data_ack			= 0x08,
		msg_unit_data			= 0x09,
		msg_unit_data_service	= 0x0A,
		msg_expedited_data		= 0x0B,
		msg_expedited_data_ack	= 0x0C,
		msg_reset_request		= 0x0D,
		msg_reset_confirmation	= 0x0E,
		msg_proto_data_unit_err	= 0x0F,
		msg_inactivity_test		= 0x10,
		msg_x_unit_data			= 0x11,
		msg_x_unit_data_service	= 0x12,
		msg_l_unit_data			= 0x13,
		msg_l_unit_data_service	= 0x14,
		msg_reserved			= 0xFF
	};

	/* ITU-T REC Q.713, 3, Table 2 */
	/* SCCP parameter name codes */
	enum param_name_code {
		param_name_code_end				= 0x00,
		param_name_code_dest_local_ref	= 0x01,
		param_name_code_src_local_ref	= 0x02,
		param_name_code_src_address		= 0x03,
		param_name_code_dst_address		= 0x04,
		param_name_code_proto_class		= 0x05,
		param_name_code_segm_reassemble	= 0x06,
		param_name_code_recv_seq_no		= 0x07,
		param_name_code_seq_segmenting	= 0x08,
		param_name_code_credit			= 0x09,
		param_name_code_release_cause	= 0x0A,
		param_name_code_return_cause	= 0x0B,
		param_name_code_reset_cause		= 0x0C,
		param_name_code_error_cause		= 0x0D,
		param_name_code_refusal_cause	= 0x0E,
		param_name_code_data			= 0x0F,
		param_name_code_segmentation	= 0x10,
		param_name_code_hop_counter		= 0x11,
		param_name_code_importance		= 0x12,
		param_name_code_long_data		= 0x13,
		param_name_code_reserved		= 0xFF
	};

	/* ITU-T REC Q.713, 3.6 */
	/* Protocol class, 4 bits */
	enum protocol_class {
		proto_class_0	= 0x00,
		proto_class_1	= 0x01,
		proto_class_2	= 0x02,
		proto_class_3	= 0x03,
	};

	/* GT indicator part of the address indicator */
	enum gt_indicator {
		/* No GT included */
		gt_has_nothing		= 0x00,
		/* GT includes nature of address indicator only */
		gt_has_nai_only		= 0x01, /* 0b0001 */
		/* GT includes translation type only */
		gt_has_tt_only		= 0x02, /* 0b0010 */
		/* GT includes translation type, numbering plan, encoding scheme */
		gt_has_tt_np_es		= 0x03, /* 0b0011 */
		/* GT includes translation type, numbering plan, encoding scheme,
		 * nature of address indicator */
		gt_has_tt_np_es_nai	= 0x04, /* 0b0010 */
	};

	/* Route indicator part of the address indicator */
	enum routing_indicator {
		route_on_gt		= 0x00,
		route_on_ssn	= 0x01,
	};

	/* 3.4.2.2: The subsystem number (SSN) identifies an SCCP user function
	 * and, when provided, consists of 1 octed */
	enum subsystem: bin::u8_t {
		ssn_unknown		= 0x00,
		ssn_mgmt		= 0x01,
		ssn_reserveditu	= 0x02,
		ssn_isdnup		= 0x03,
		ssn_omap		= 0x04,
		ssn_map			= 0x05,
		ssn_hlr			= 0x06,
		ssn_vlr			= 0x07,
		ssn_msc			= 0x08,
		ssn_eic			= 0x09,
		ssn_auc			= 0x10,
		ssn_isdnsuppl	= 0x11,
		ssn_reservedint	= 0x12,
		ssn_broadband	= 0x13,
		ssn_tctest		= 0x14,
	};

	/* Values of a nature of address indicator: nai: 7 bits */
	enum nature_of_address {
		nai_unknown						= 0x00,
		nai_subscriber_number			= 0x01,
		nai_reserved_for_national_use	= 0x02,
		nai_national_significant_number	= 0x03,
		nai_international_number		= 0x04,
	};

	/* Values of a numbering plan field: np: 4 bits */
	enum numbering_plan {
		np_unknown			= 0x00,
		np_isdn_telephony	= 0x01,
		np_generic			= 0x02,
		np_data				= 0x03,
		np_telex			= 0x04,
		np_maritime			= 0x05,
		np_land_mobile		= 0x06,
		np_isdn_mobile		= 0x07,
		np_private			= 0x0E,
		np_reserved			= 0x0F,
	};

	/* Values of an encoding scheme field: es: 4 bits */
	enum encoding_scheme {
		es_unknown			= 0x00,
		es_bcd_odd			= 0x01,
		es_bcd_even			= 0x02,
		es_national			= 0x03,
		es_reserved			= 0x0F,
	};

	/* TODO: determine correct max len for GT data */
	const bin::sz_t max_gt_len = 50;

	/* ITU-T REC Q.713, 3.4.1: Address indicator */
	struct address_indicator {
		bool pci				: 1;	/* Point code indicator */
		bool ssni				: 1;	/* Subsystem number indicator */
		gt_indicator gti		: 4;	/* Global title indicator */
		routing_indicator ri	: 1;	/* Routing indicator */
		bin::u8_t reserved	: 1;
	};

	/* The various elements when provided, occur in the order: point code
	 * , subsystem number, global title. The order in a struct is changed
	 * for more efficient packing. */

	struct address {
		address_indicator	indicator;
		subsystem			ssn;		/* Subsystem number */
		bin::u16_t		point_code;	/* Signalling point code */
		bin::u8_t			gt_len;		/* Length of GT for unitdata */
		union {
			/* These structs has 1 to 1 packing so we can
			 * read them from binary stream directly by copying */
			struct {
				nature_of_address	nai: 7;	/* Nature of address indicator */
				bin::u8_t			odd: 1;	/* Odd = 0x01, Even = 0x00 */
				bin::u8_t			data[max_gt_len];
			} x01;
			struct {
				bin::u8_t			tt; /* Translation type */
				bin::u8_t			data[max_gt_len];
			} x02;
			struct {
				bin::u8_t			tt; /* Translation type */
				encoding_scheme		es: 4; /* Encoding scheme */
				numbering_plan		np: 4; /* Numbering plan */
				bin::u8_t			data[max_gt_len];
			} x03;
			struct {
				bin::u8_t			tt; /* Translation type */
				encoding_scheme		es: 4; /* Encoding scheme */
				numbering_plan		np: 4; /* Numbering plan */
				nature_of_address	nai: 7;	/* Nature of address indicator */
				bin::u8_t			gap: 1;	/* Should be 0 */
				bin::u8_t			data[max_gt_len];
			} x04;
		} gt;
	};

	struct unitdata {
		protocol_class pclass;
		address dst_addr;
		address src_addr;
		bin::u8_t data_len;
		bin::u8_t data[255];
	};

	/* Generic type trait */
	template <class T>
	struct the {};

	template <>
	struct the<address> {
		typedef address type;
		static bin::sz_t length(const type & addr) {
			return 1 /* Address indicator */
				/* Size of point code, if included */
				+ (addr.indicator.pci ? 2 : 0)
				/* Size of SSN, if included */
				+ (addr.indicator.ssni ? 1 : 0)
				+ addr.gt_len
			;
		}
	};

	template <class LogT>
	const bin::u8_t * parse(unitdata & msg
			, const bin::u8_t * buf, LogT & L) {
		using namespace bin;

		bin::u8_t dst_addr_ofs, src_addr_ofs, data_ofs;

		/* Read protocol class */
		buf = p::cp_u8(asbuf(msg.pclass), buf);

		/* Parse called party address */
		buf = p::cp_u8(asbuf(dst_addr_ofs), buf);
		parse(msg.dst_addr, buf + dst_addr_ofs - 1, L);

		/* Parse calling party address */
		buf = p::cp_u8(asbuf(src_addr_ofs), buf);
		parse(msg.src_addr, buf + src_addr_ofs - 1, L);

		/* Parse data */
		buf = p::cp_u8(asbuf(data_ofs), buf);
		buf = p::cp_u8(asbuf(msg.data_len), buf + data_ofs - 1);

		buf = p::cpy(msg.data, buf, static_cast<bin::sz_t>(msg.data_len));

		return buf;
	}

	template <class LogT>
	bin::u8_t * write(bin::u8_t * buf
			, const unitdata & msg, LogT & L) {
		using namespace bin;

		bin::u8_t dst_addr_ofs, src_addr_ofs, data_ofs;

		/* Calling party address goes immediately after all pointers:
		 * 1 octet pointer to dst address
		 * 1 octet pointer to src address
		 * 1 octet pointer to data */
		dst_addr_ofs = 1 + 1 + 1;

		/* Calling party address goes after all pointers
		 * and the calling party address */
		src_addr_ofs = dst_addr_ofs + the<address>::length(msg.dst_addr);

		/* Data field goes after all pointers
		 * and the calling party address
		 * and the called party address */
		data_ofs = src_addr_ofs + the<address>::length(msg.src_addr);

		/* Write protocol class */
		buf = w::cp_u8(buf, ascbuf(msg.pclass));

		/* Write pointers */
		buf = w::cp_u8(buf, ascbuf(dst_addr_ofs));
		buf = w::cp_u8(buf, ascbuf(src_addr_ofs));
		buf = w::cp_u8(buf, ascbuf(data_ofs));

		/* Write calling party address */
		buf = write(buf, msg.dst_addr, L);
		/* Write called party address */
		buf = write(buf, msg.src_addr, L);

		/* Write data */
		buf = w::cp_u8(buf, ascbuf(msg.data_len));
		buf = w::cpy(buf, msg.data, static_cast<bin::sz_t>(msg.data_len));

		return buf;
	}

	/* Parse address for unitdata (not long unit data) */
	template <class LogT>
	const bin::u8_t * parse(address & addr
			, const bin::u8_t * buf, LogT & L) {
		(void)(L);
		using namespace bin;

		/* TODO: check length before parsing next element */

		/* Parse address's overall length */
		buf = p::cp_u8(asbuf(addr.gt_len), buf);

		/* Parse address indicator */
		buf = p::cp_u8(asbuf(addr.indicator), buf);
		addr.gt_len -= 1;

		/* Point code indicator is set, address contains point code */
		if (addr.indicator.pci) {
			buf = p::cp_u16(asbuf(addr.point_code), buf);
			addr.gt_len -= 2;
		}

		/* SSN indicator is set, address contains SSN */
		if (addr.indicator.ssni) {
			buf = p::cp_u8(asbuf(addr.ssn), buf);
			addr.gt_len -= 1;
		}

		/* Analyze GT indicator */
		switch (static_cast<gt_indicator>(addr.indicator.gti)) {
			/* Address does not contain GT, gti = 0x0 */
			case gt_has_nothing: {
				return buf;
			}
			/* GT includes nature of address indicator only, gti == 0x1 */
			case gt_has_nai_only: {
				buf = p::cpy(asbuf(addr.gt.x01), buf
						, static_cast<bin::sz_t>(addr.gt_len));
				return buf;
			}
			/* GT includes translation type only, gti == 0x2 */
			case gt_has_tt_only: {
				buf = p::cpy(asbuf(addr.gt.x02), buf
						, static_cast<bin::sz_t>(addr.gt_len));
				return buf;
			}
			/* GT includes translation type, numbering plan, encoding scheme
			 * , gti == 0x3 */
			case gt_has_tt_np_es: {
				buf = p::cpy(asbuf(addr.gt.x03), buf
						, static_cast<bin::sz_t>(addr.gt_len));
				return buf;
			}
			/* GT includes translation type, numbering plan, encoding scheme,
			 * nature of address indicator, gti == 0x4 */
			case gt_has_tt_np_es_nai: {
				buf = p::cpy(asbuf(addr.gt.x04), buf
						, static_cast<bin::sz_t>(addr.gt_len));
				return buf;
			}
			default: /* TODO: handle reserved value for GTI */ return buf;
		}
	}

	/* Write address for unit data (not long unit data) */
	template <class LogT>
	bin::u8_t * write(bin::u8_t * buf
			, const address & addr , LogT & L) {
		(void)(L);
		using namespace bin;

		bin::u8_t len = the<address>::length(addr);

		/* Write address's overall length */
		buf = w::cp_u8(buf, ascbuf(len));

		/* Write address indicator */
		buf = w::cp_u8(buf, ascbuf(addr.indicator));

		/* Point code indicator is set, address contains point code */
		if (addr.indicator.pci == 0x01) {
			buf = w::cp_u16(buf, ascbuf(addr.point_code));
		}

		/* SSN indicator is set, address contains SSN */
		if (addr.indicator.ssni == 0x01) {
			buf = w::cp_u8(buf, ascbuf(addr.ssn));
		}

		/* Analyze GT indicator */
		switch (addr.indicator.gti) {
			/* Address does not contain GT, gti = 0x00 */
			case gt_has_nothing: {
				return buf;
			}
			/* GT includes nature of address indicator only, gti == 0x01 */
			case gt_has_nai_only: {
				buf = w::cpy(buf, ascbuf(addr.gt.x01)
						, static_cast<bin::sz_t>(addr.gt_len));
				return buf;
			}
			/* GT includes translation type only, gti == 0x02 */
			case gt_has_tt_only: {
				buf = w::cpy(buf, ascbuf(addr.gt.x02)
						, static_cast<bin::sz_t>(addr.gt_len));
				return buf;
			}
			/* GT includes translation type, numbering plan, encoding scheme
			 * , gti == 0x03 */
			case gt_has_tt_np_es: {
				buf = w::cpy(buf, ascbuf(addr.gt.x03)
						, static_cast<bin::sz_t>(addr.gt_len));
				return buf;
			}
			/* GT includes translation type, numbering plan, encoding scheme,
			 * nature of address indicator, gti == 0x04 */
			case gt_has_tt_np_es_nai: {
				buf = w::cpy(buf, ascbuf(addr.gt.x04)
						, static_cast<bin::sz_t>(addr.gt_len));
				return buf;
			}
			default: /* TODO: handle reserved value for GTI */ return buf;
		}
	}

	std::string to_string(protocol_class pclass) {
		std::stringstream out;
		out << "[proto class:";
		switch (pclass) {
			case protocol_class::proto_class_0:
				out << "class 0";
				break;
			case protocol_class::proto_class_1:
				out << "class 1";
				break;
			case protocol_class::proto_class_2:
				out << "class 2";
				break;
			case protocol_class::proto_class_3:
				out << "class 3";
				break;
			default: out << "wrong:" << static_cast<unsigned>(pclass); break;
		}
		out << "]";
		return out.str();
	}

	std::string to_string(subsystem ssn) {
		std::stringstream out;
		out << "[SSN:";
		switch (ssn) {
			case ssn_unknown: out << "unknown"; break;
			case ssn_mgmt: out << "mgmt"; break;
			case ssn_reserveditu: out << "reserveditu"; break;
			case ssn_isdnup: out << "isdnup"; break;
			case ssn_omap: out << "omap"; break;
			case ssn_map: out << "map"; break;
			case ssn_hlr: out << "hlr"; break;
			case ssn_vlr: out << "vlr"; break;
			case ssn_msc: out << "msc"; break;
			case ssn_eic: out << "eic"; break;
			case ssn_auc: out << "auc"; break;
			case ssn_isdnsuppl: out << "isdnsuppl"; break;
			case ssn_reservedint: out << "reservedint"; break;
			case ssn_broadband: out << "broadband"; break;
			case ssn_tctest: out << "tctest"; break;
			default:
				out << "private:" << static_cast<unsigned>(ssn);
				break;
		}
		out << "]";
		return out.str();
	}

	std::string to_string(nature_of_address nai) {
		std::stringstream out;
		out << "[nai:";
		switch (nai) {
			case nai_unknown:
				out << "unknown";
				break;
			case nai_subscriber_number:
				out << "subscriber number";
				break;
			case nai_reserved_for_national_use:
				out << "national use";
				break;
			case nai_national_significant_number:
				out << "national significant number";
				break;
			case nai_international_number:
				out << "international number";
				break;
			default:
				out << "wrong:" << static_cast<unsigned>(nai);
				break;
		}
		out << "]";
		return out.str();
	}

	std::string to_string(encoding_scheme es) {
		std::stringstream out;
		out << "[es:";
		switch (es) {
			case es_unknown:
				out << "unknown";
				break;
			case es_bcd_odd:
				out << "bsd odd";
				break;
			case es_bcd_even:
				out << "bsd even";
				break;
			case es_national:
				out << "national";
				break;
			case es_reserved:
				out << "reserved";
				break;
			default:
				out << "wrong:" << static_cast<unsigned>(es);
				break;
		}
		out << "]";
		return out.str();
	}

	std::string to_string(numbering_plan np) {
		std::stringstream out;
		out << "[np:";
		switch (np) {
			case np_unknown:
				out << "unknown";
				break;
			case np_isdn_telephony:
				out << "isdn_telephony";
				break;
			case np_generic:
				out << "generic";
				break;
			case np_telex:
				out << "telex";
				break;
			case np_maritime:
				out << "maritime";
				break;
			case np_land_mobile:
				out << "land_mobile";
				break;
			case np_isdn_mobile:
				out << "isdn_mobile";
				break;
			case np_private:
				out << "private";
				break;
			default:
				out << "wrong:" << static_cast<unsigned>(np);
				break;
		}
		out << "]";
		return out.str();
	}

	std::string to_string(const address & r) {
		std::stringstream out;

		switch (r.indicator.ri) {
			case route_on_gt: out << "[route on GT]"; break;
			case route_on_ssn: out << "[route on SSN]"; break;
		}

		if (r.indicator.pci) {
			out << "[point code: " << r.point_code << "]";
		} else {
			out << "[no point code]";
		}

		if (r.indicator.ssni) {
			out << to_string(r.ssn);
		} else {
			out << "[no SSN]";
		}

		switch (r.indicator.gti) {
			case gt_has_nothing:
				out << "[no GT]";
				break;
			case gt_has_nai_only:
				out << "[GT:"
					<< r.gt.x01.nai
					<< "]";
				break;
			case gt_has_tt_only:
				out << "[GT:"
					<< "[tt: " << static_cast<unsigned>(r.gt.x02.tt) << "]"
					<< "]"
				;
				break;
			case gt_has_tt_np_es: {
				std::string num
					= bcd_decode(r.gt.x03.data, r.gt_len - 2
							, r.gt.x03.es == es_bcd_odd);
				out << "[GT:"
					<< "[tt: " << static_cast<unsigned>(r.gt.x03.tt) << "]"
					<< r.gt.x03.np
					<< r.gt.x03.es
					<< "[data:" << num << "]"
					<< "]"
				;
				break;
			}
			case gt_has_tt_np_es_nai: {
				std::string num
					= bcd_decode(r.gt.x04.data, r.gt_len - 2
							, r.gt.x04.es == es_bcd_odd);
				out << "[GT:"
					<< r.gt.x04.nai
					<< "[tt:" << static_cast<unsigned>(r.gt.x04.tt) << "]"
					<< r.gt.x04.np
					<< r.gt.x04.es
					<< "[data:" << num << "]"
					<< "]"
				;
				break;
			}
			default:
				out << "[GT:indicator:"
					<< std::bitset<4>(r.indicator.gti)
					<< "]";
				break;
		}

		return out.str();
	}

	std::string to_string(const unitdata & r) {
		std::stringstream out;
		out << "[UNITDATA:[proto class: " << r.pclass << "]"
			<< "]"
		;
		/* TODO:
			<< "[calling party:" << r.dst_addr << "]"
			<< "[called party:" << r.src_addr << "]"
			<< " " << std::string(r.data, r.data_len)
		*/
		return out.str();
	}

} } } }

#endif
