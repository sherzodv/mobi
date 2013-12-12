#ifndef mobi_net_ss7_m3ua_hpp
#define mobi_net_ss7_m3ua_hpp

#include <bitset>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <ostream>
#include <arpa/inet.h>

namespace ss7 { namespace m3ua {

	namespace proto {
		typedef std::uint8_t	u8_t;
		typedef std::uint16_t	u16_t;
		typedef std::uint32_t	u32_t;

		typedef const std::uint8_t	cu8_t;
		typedef const std::uint16_t	cu16_t;
		typedef const std::uint32_t	cu32_t;

		typedef std::uint8_t	*	u8p_t;
		typedef std::uint16_t	*	u16p_t;
		typedef std::uint32_t	*	u32p_t;

		typedef const std::uint8_t	*	cu8p_t;
		typedef const std::uint16_t	*	cu16p_t;
		typedef const std::uint32_t	*	cu32p_t;
	}

	/* MEMORY MANIPULATION UTILITIES */

	namespace utl {
		using namespace proto;
		typedef std::size_t sz_t;

		/* Byte order handling routines */
		namespace bo {
			inline u16_t tohost(u16_t v) { return ntohs(v); }
			inline u32_t tohost(u32_t v) { return ntohl(v); }
			inline u16_t tonet(u16_t v) { return htons(v); }
			inline u32_t tonet(u32_t v) { return htonl(v); }
		}

		template <typename T>
		inline const u8_t * ascbuf(T & src) {
			return reinterpret_cast<const u8_t *>(&src);
		}

		template <typename T>
		inline const u8_t * ascbuf(T * src) {
			return reinterpret_cast<const u8_t *>(src);
		}

		template <typename T>
		inline u8_t * asbuf(T & src) {
			return reinterpret_cast<u8_t *>(&src);
		}

		template <typename T>
		inline u8_t * asbuf(T * src) {
			return reinterpret_cast<u8_t *>(src);
		}

		/* Parse from buffer to struct , move buffer pointer */
		namespace p {

			inline const u8_t * cp_u8(u8_t * dst, const u8_t * src) {
				*dst = *src++;
				return src;
			}

			inline const u8_t * cp_u16(u8_t * dst, const u8_t * src) {
				/* Copy with byte order handling */
				dst += 1;
				*dst-- = *src++;
				*dst-- = *src++;
				return src;
			}

			inline const u8_t * cp_u32(u8_t * dst, const u8_t * src) {
				/* Copy with byte order handling */
				dst += 3;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				return src;
			}

			inline const u8_t * cp_u64(u8_t * dst, const u8_t * src) {
				/* Copy with byte order handling */
				dst += 7;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				return src;
			}

			inline const u8_t * cpy(u8_t * dst, const u8_t * src, sz_t len) {
				while (len) {
					*dst++ = *src++;
					--len;
				}
				return src;
			}

			/* Parse fixed size string. It will contain either exactly
			 * len-1 characters and a terminating zero, or only and only
			 * terminating zero, see SMPP 3.4 spec for details */
			inline const u8_t * scpyf(u8_t * dst, const u8_t * src, sz_t len) {
				if (*src == 0) {
					*dst++ = *src++;
					return src;
				}
				while (len) {
					*dst++ = *src++;
					--len;
				}
				return src;
			}

			/* Parse zero terminated string with specified max length.
			 * l will contain number of characters parsed not counting the
			 * terminating zero. */
			inline const u8_t * scpyl(u8_t * dst, const u8_t * src
					, sz_t len, sz_t & l) {
				l = 0;
				while (len) {
					if (*src == 0) {
						*dst++ = *src++;
						return src;
					}
					*dst++ = *src++;
					--len;
					++l;
				}
				return src;
			}
		}

		/* Write from struct to buffer, move buffer pointer */
		namespace w {

			inline u8_t * cp_u8(u8_t * dst, const u8_t * src) {
				*dst++ = *src;
				return dst;
			}

			inline u8_t * cp_u16(u8_t * dst, const u8_t * src) {
				src += 1;
				*dst++ = *src--;
				*dst++ = *src;
				return dst;
			}

			inline u8_t * cp_u32(u8_t * dst, const u8_t * src) {
				src += 3;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				return dst;
			}

			inline u8_t * cp_u64(u8_t * dst, const u8_t * src) {
				src += 7;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				return dst;
			}

			inline u8_t * cpy(u8_t * dst, const u8_t * src, sz_t len) {
				while (len) {
					*dst++ = *src++;
					--len;
				}
				return dst;
			}

			inline u8_t * scpy(u8_t * dst, const u8_t * src, sz_t len) {
				while (len) {
					if (*src == 0) {
						*dst++ = *src++;
						return dst;
					}
					*dst++ = *src++;
					--len;
				}
				return dst;
			}
		}
	}

	enum message_class: proto::u8_t {
		mclass_mgmt			= 0x00,
		mclass_transfer		= 0x01,
		mclass_ss7_mgmt		= 0x02,
		mclass_asp_state	= 0x03,
		mclass_asp_traffic	= 0x04,
		mclass_reserved1	= 0x05,
		mclass_reserved2	= 0x05,
		mclass_reserved3	= 0x05,
		mclass_reserved4	= 0x05,
		mclass_route_mgmt	= 0x05,
	};

	enum parameter_tag: proto::u16_t {
		/* Common across adaptation layers */
		tag_reserved		= 0x0000,
		tag_info_string		= 0x0004,
		tag_routing_ctxt	= 0x0006,
		tag_diagnostics		= 0x0007,
		tag_heartbeat		= 0x0009,
		tag_traf_mode_type	= 0x000B,
		tag_error_code		= 0x000C,
		tag_status			= 0x000D,
		tag_asp_id			= 0x0011,
		tag_aff_point_code	= 0x0012,
		tag_correlation_id	= 0x0013,

		/* m3ua specific */
		tag_network_appearance	= 0x0200,
		tag_reserved1			= 0x0201,
		tag_reserved2			= 0x0202,
		tag_reserved3			= 0x0203,
		tag_user_cause			= 0x0204,
		tag_cong_indications	= 0x0205,
		tag_concerned_dst		= 0x0206,
		tag_routing_key			= 0x0207,
		tag_reg_result			= 0x0208,
		tag_dereg_result		= 0x0209,
		tag_local_routing_key_id= 0x020A,
		tag_dst_point_code		= 0x020B,
		tag_service_indicators	= 0x020C,
		tag_reserved4			= 0x020D,
		tag_orig_point_code_list= 0x020E,
		tag_reserved5			= 0x020F,
		tag_protocol_data		= 0x0210,
		tag_reserved6			= 0x0211,
		tag_reg_status			= 0x0212,
		tag_dereg_status		= 0x0213,
	};

	enum service_indicator {
		si_mgmt		= 0x00,
		si_test		= 0x01,
		si_special	= 0x02,
		si_sccp		= 0x03,
		si_tup		= 0x04,
		si_isdnup	= 0x05,
		si_dup1		= 0x06,
		si_dup2		= 0x07,
		si_broadband= 0x09,
		si_sattelite= 0x0A,
	};

	/* Message types depend on message class */

	namespace mtype {

		enum mgmt: proto::u8_t {
			mgmt_error	= 0x0,
			mgmt_notify	= 0x1,
		};

		enum transfer: proto::u8_t {
			trans_reserved	= 0x0,
			trans_data		= 0x1,	/* Payload Data */
		};

		enum ss7_mgmt: proto::u8_t {
			ss7_mgmt_reserved	= 0x0,
			ss7_mgmt_duna		= 0x1,	/* Destination unavailable */
			ss7_mgmt_dava		= 0x2,	/* Destination available */
			ss7_mgmt_daud		= 0x3,	/* Destination state audit */
			ss7_mgmt_scon		= 0x4,	/* Signalling congestion */
			ss7_mgmt_dupu		= 0x5,	/* Destination user part unavailable */
			ss7_mgmt_drst		= 0x6,	/* Destination restricted */
		};

		enum asp_state: proto::u8_t {
			asp_state_reserved	= 0x0,
			asp_state_up		= 0x1,	/* ASP up */
			asp_state_down		= 0x2,	/* ASP down */
			asp_state_hb		= 0x3,	/* Heartbeat */
			asp_state_up_ack	= 0x4,	/* ASP up acknowledgement */
			asp_state_down_ack	= 0x5,	/* ASP down acknowledgement */
			asp_state_hb_ack	= 0x6,	/* Heartbeat acknowledgement */
		};

		enum asp_traf: proto::u8_t {
			asp_traf_reserved		= 0x0,
			asp_traf_active			= 0x1,	/* ASP active */
			asp_traf_inactive		= 0x2,	/* ASP inactive */
			asp_traf_active_ack		= 0x3,	/* ASP active acknowledgement */
			asp_traf_inactive_ack	= 0x4,	/* ASP inactive acknowledgement */
		};

		enum asp_route: proto::u8_t {
			asp_route_reserved	= 0x0,
			asp_route_reg_req	= 0x1,	/* ASP registration request */
			asp_route_reg_rsp	= 0x2,	/* ASP registration response */
			asp_route_dereg_req	= 0x3,	/* ASP deregistration request */
			asp_route_dereg_rsp	= 0x4,	/* ASP deregistration response */
		};

	}

	struct pdu {
		proto::u8_t version;
		proto::u8_t reserved;
		message_class mclass: 8;
		union {
			mtype::mgmt			mgmt: 8;
			mtype::transfer		transfer: 8;
			mtype::ss7_mgmt		ss7mgmt: 8;
			mtype::asp_state	aspstate: 8;
			mtype::asp_traf		asptraf: 8;
			mtype::asp_traf		asproute: 8;
		} type;
		proto::u32_t len;
	};

	template <typename ValueT>
	struct tlv {
		parameter_tag tag;
		proto::u16_t len;
		ValueT val;
		tlv(): tag(tag_reserved), len(0) {}
	};

	namespace message {

		struct mtp3 {
			proto::u32_t		opc;	/* Originating point code */
			proto::u32_t		dpc;	/* Destination point code */
			service_indicator	si:8;	/* Service indicator */
			proto::u8_t			ni;		/* Network indicator */
			proto::u8_t			mp;		/* Message priority */
			proto::u8_t			sls;	/* Signalling link selection code */

			/* TODO: define max len for user part data */
			std::size_t data_len;
			proto::u8_t data[255]; /* User Part data */
		};

		struct data {
			pdu					header;
			tlv<proto::u32_t>	network_appearance;
			tlv<proto::u32_t>	routing_ctxt;
			tlv<mtp3>			protocol_data;
			tlv<proto::u32_t>	correlation_id;
		};

	}

	template <class LogT>
	const proto::u8_t * parse(tlv<proto::u8_t> & t
			, const proto::u8_t * buf, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = p::cp_u16(asbuf(t.tag), buf);
		buf = p::cp_u16(asbuf(t.len), buf);
		buf = p::cp_u8(asbuf(t.val), buf);
		return buf;
	}

	template <class LogT>
	proto::u8_t * write(proto::u8_t * buf
			, const tlv<proto::u8_t> & t, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = w::cp_u16(buf, ascbuf(t.tag));
		buf = w::cp_u16(buf, ascbuf(t.len));
		buf = w::cp_u8(buf, ascbuf(t.val));
		return buf;
	}

	template <class LogT>
	const proto::u8_t * parse(tlv<proto::u16_t> & t
			, const proto::u8_t * buf, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = p::cp_u16(asbuf(t.tag), buf);
		buf = p::cp_u16(asbuf(t.len), buf);
		buf = p::cp_u16(asbuf(t.val), buf);
		return buf;
	}

	template <class LogT>
	proto::u8_t * write(proto::u8_t * buf
			, const tlv<proto::u16_t> & t, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = w::cp_u16(buf, ascbuf(t.tag));
		buf = w::cp_u16(buf, ascbuf(t.len));
		buf = w::cp_u16(buf, ascbuf(t.val));
		return buf;
	}

	template <class LogT>
	const proto::u8_t * parse(tlv<proto::u32_t> & t
			, const proto::u8_t * buf, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = p::cp_u16(asbuf(t.tag), buf);
		buf = p::cp_u16(asbuf(t.len), buf);
		buf = p::cp_u32(asbuf(t.val), buf);
		return buf;
	}

	template <class LogT>
	proto::u8_t * write(proto::u8_t * buf
			, const tlv<proto::u32_t> & t, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = w::cp_u16(buf, ascbuf(t.tag));
		buf = w::cp_u16(buf, ascbuf(t.len));
		buf = w::cp_u32(buf, ascbuf(t.val));
		return buf;
	}

	template <class LogT>
	const proto::u8_t * parse(pdu & r
			, const proto::u8_t * buf, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = p::cpy(asbuf(r), buf, 4);
		buf = p::cp_u32(asbuf(r.len), buf);
		return buf;
	}

	template <class LogT>
	proto::u8_t * write(proto::u8_t * buf
			, const pdu & r, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = w::cpy(buf, ascbuf(r), 4);
		buf = w::cp_u32(buf, ascbuf(r.len));
		return buf;
	}

	template <class LogT>
	const proto::u8_t * parse(message::mtp3 & r
			, const proto::u8_t * buf, std::size_t len, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = p::cp_u32(asbuf(r.opc), buf);
		buf = p::cp_u32(asbuf(r.dpc), buf);
		/* Copy si, ni, mp, sls at once */
		buf = p::cpy(asbuf(r) + 8, buf, 4);
		/* Remaining part is data */
		r.data_len = len - 12;
		buf = p::cpy(r.data, buf, r.data_len);
		return buf;
	}

	template <class LogT>
	proto::u8_t * write(proto::u8_t * buf
			, const message::mtp3 & r, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = w::cp_u32(buf, ascbuf(r.opc));
		buf = w::cp_u32(buf, ascbuf(r.dpc));
		/* Copy si, ni, mp, sls at once */
		buf = w::cpy(buf, ascbuf(r) + 8, 4);
		buf = w::cpy(buf, r.data, r.data_len);
		return buf;
	}

	template <class LogT>
	const proto::u8_t * parse(tlv<message::mtp3> & t
			, const proto::u8_t * buf, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = p::cp_u16(asbuf(t.tag), buf);
		buf = p::cp_u16(asbuf(t.len), buf);
		buf = parse(t.val, buf, t.len - 4, L);
		return buf;
	}

	template <class LogT>
	proto::u8_t * write(proto::u8_t * buf
			, const tlv<message::mtp3> & t, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = w::cp_u16(buf, ascbuf(t.tag));
		buf = w::cp_u16(buf, ascbuf(t.len));
		buf = write(buf, t.val, L);
		return buf;
	}

	template <class LogT>
	const proto::u8_t * parse(message::data & r
			, const proto::u8_t * buf
			, const proto::u8_t * bend, LogT & L) {
		(void)(L);
		using namespace utl;

		proto::u16_t tag;

		buf = parse(r.header, buf, L);

		while (buf < bend) {
			p::cp_u16(asbuf(tag), buf);
			switch (tag) {
				case tag_network_appearance:
					buf = parse(r.network_appearance, buf, L);
					continue;
				case tag_routing_ctxt:
					buf = parse(r.routing_ctxt, buf, L);
					continue;
				case tag_protocol_data:
					buf = parse(r.protocol_data, buf, L);
					continue;
				case tag_correlation_id:
					buf = parse(r.correlation_id, buf, L);
					continue;
				default:
					/* TODO: handle wrong parameter and padding correct */
					if (*buf == 0x00) {
						++buf;
						continue;
					}
					return buf;
			}
		}

		return buf;
	}

	template <class LogT>
	proto::u8_t * write(proto::u8_t * buf
			, const message::data & r, LogT & L) {
		(void)(L);
		using namespace utl;

		buf = write(buf, r.header, L);

		if (r.network_appearance.tag != tag_reserved)
			buf = write(buf, r.network_appearance, L);

		if (r.routing_ctxt.tag != tag_reserved)
			buf = write(buf, r.routing_ctxt, L);

		if (r.protocol_data.tag != tag_reserved)
			buf = write(buf, r.protocol_data, L);

		if (r.correlation_id.tag != tag_reserved)
			buf = write(buf, r.correlation_id, L);

		return buf;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, service_indicator si) {
		L << "[si:";
		switch (si) {
			case si_mgmt		: L << "mgmt"; break;
			case si_test		: L << "test"; break;
			case si_special		: L << "special"; break;
			case si_sccp		: L << "sccp"; break;
			case si_tup			: L << "tup"; break;
			case si_isdnup		: L << "isdnup"; break;
			case si_dup1		: L << "dup1"; break;
			case si_dup2		: L << "dup2"; break;
			case si_broadband	: L << "broadband"; break;
			case si_sattelite	: L << "sattelite"; break;
			default: L << "unknown:" << static_cast<unsigned>(si); break;
		}
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, parameter_tag t) {
		L << "[tag:";
		switch (t) {
			case tag_info_string			: L << "info_string"; break;
			case tag_routing_ctxt			: L << "routing_ctxt"; break;
			case tag_diagnostics			: L << "diagnostics"; break;
			case tag_heartbeat				: L << "heartbeat"; break;
			case tag_traf_mode_type			: L << "traf_mode_type"; break;
			case tag_error_code				: L << "error_code"; break;
			case tag_status					: L << "status"; break;
			case tag_asp_id					: L << "asp_id"; break;
			case tag_aff_point_code			: L << "aff_point_code"; break;
			case tag_correlation_id			: L << "correlation_id"; break;
			case tag_network_appearance		: L << "network_appearance"; break;
			case tag_user_cause				: L << "user_cause"; break;
			case tag_cong_indications		: L << "cong_indications"; break;
			case tag_concerned_dst			: L << "concerned_dst"; break;
			case tag_routing_key			: L << "routing_key"; break;
			case tag_reg_result				: L << "reg_result"; break;
			case tag_dereg_result			: L << "dereg_result"; break;
			case tag_local_routing_key_id	: L << "local_routing_key_id"; break;
			case tag_dst_point_code			: L << "dst_point_code"; break;
			case tag_service_indicators		: L << "service_indicators"; break;
			case tag_orig_point_code_list	: L << "orig_point_code_list"; break;
			case tag_protocol_data			: L << "protocol_data"; break;
			case tag_reg_status				: L << "reg_status"; break;
			case tag_dereg_status			: L << "dereg_status"; break;
			default: L << "tag:unknown"; break;
		}
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, message_class mc) {
		switch (mc) {
			case mclass_mgmt: L << "mgmt"; break;
			case mclass_transfer: L << "transfer"; break;
			case mclass_ss7_mgmt: L << "ss7mgmt"; break;
			case mclass_asp_state: L << "aspstate"; break;
			case mclass_asp_traffic: L << "asptraf"; break;
			case mclass_route_mgmt: L << "routemgmt"; break;
			default: L << "unknown"; break;
		}
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, mtype::mgmt mt) {
		switch (mt) {
			case mtype::mgmt_error: L << "mgmt_error"; break;
			case mtype::mgmt_notify: L << "mgmt_notify"; break;
			default: L << "mgmt:unknown"; break;
		}
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, mtype::transfer mt) {
		switch (mt) {
			case mtype::trans_data: L << "trans_data"; break;
			default: L << "trans:unknown"; break;
		}
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, mtype::ss7_mgmt mt) {
		switch (mt) {
			case mtype::ss7_mgmt_duna: L << "ss7mgmt_DUNA"; break;
			case mtype::ss7_mgmt_dava: L << "ss7mgmt_DAVA"; break;
			case mtype::ss7_mgmt_daud: L << "ss7mgmt_DAUD"; break;
			case mtype::ss7_mgmt_scon: L << "ss7mgmt_SCON"; break;
			case mtype::ss7_mgmt_dupu: L << "ss7mgmt_DUPU"; break;
			case mtype::ss7_mgmt_drst: L << "ss7mgmt_DRST"; break;
			default: L << "ss7mgmt:unknown"; break;
		}
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, mtype::asp_state mt) {
		switch (mt) {
			case mtype::asp_state_up: L << "aspstate_UP"; break;
			case mtype::asp_state_down: L << "aspstate_DOWN"; break;
			case mtype::asp_state_hb: L << "aspstate_HB"; break;
			case mtype::asp_state_up_ack: L << "aspstate_UPACK"; break;
			case mtype::asp_state_down_ack: L << "aspstate_DOWNACK"; break;
			case mtype::asp_state_hb_ack: L << "aspstate_HBACK"; break;
			default: L << "aspstate:unknown"; break;
		}
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, mtype::asp_traf mt) {
		switch (mt) {
			case mtype::asp_traf_active: L << "asptraf_ACTIVE"; break;
			case mtype::asp_traf_inactive: L << "asptraf_INACTIVE"; break;
			case mtype::asp_traf_active_ack: L << "asptraf_ACTIVEACK"; break;
			case mtype::asp_traf_inactive_ack: L << "asptraf_INACTIVEACK"; break;
			default: L << "asptraf:unknown"; break;
		}
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, mtype::asp_route mt) {
		switch (mt) {
			case mtype::asp_route_reg_req: L << "asproute_REGREQ"; break;
			case mtype::asp_route_reg_rsp: L << "asproute_REGRSP"; break;
			case mtype::asp_route_dereg_req: L << "asproute_DEREGREQ"; break;
			case mtype::asp_route_dereg_rsp: L << "asproute_DEREGRSP"; break;
			default: L << "asproute:unknown"; break;
		}
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const pdu & r) {
		L << "[hdr:"
			<< "[version:" << static_cast<unsigned>(r.version) << "]"
			<< "[class:" << r.mclass << "]";

		L << "[type:";
		switch (r.mclass) {
			case mclass_mgmt: L << r.type.mgmt; break;
			case mclass_transfer: L << r.type.transfer; break;
			case mclass_ss7_mgmt: L << r.type.ss7mgmt; break;
			case mclass_asp_state: L << r.type.aspstate; break;
			case mclass_asp_traffic: L << r.type.asptraf; break;
			case mclass_route_mgmt: L << r.type.asproute; break;
			default: L << "unknown"; break;
		}
		L << "]";

		L << "[len:" << r.len << "]";
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const message::mtp3 & r) {
		L << "[mtp3:"
			<< "[opc:" << r.opc << "]"
			<< "[dpc:" << r.dpc << "]"
			<< r.si
			<< "[ni:" << static_cast<unsigned>(r.ni) << "]"
			<< "[mp:" << static_cast<unsigned>(r.mp) << "]"
			<< "[sls:" << static_cast<unsigned>(r.sls) << "]"
		<< "]";
		return L;
	}

	template< typename CharT, typename TraitsT, typename ValueT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const tlv<ValueT> & t) {
		L << "[tlv:"
			<< t.tag
			<< "[len:" << t.len << "]"
			<< "[val:" << t.val << "]"
		<< "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const message::data & r) {
		L << "[M3UA_DATA:"
			<< r.header;
		if (r.network_appearance.tag != tag_reserved)
			L << r.network_appearance;
		if (r.routing_ctxt.tag != tag_reserved)
			L << r.routing_ctxt;
		if (r.protocol_data.tag != tag_reserved)
			L << r.protocol_data;
		if (r.correlation_id.tag != tag_reserved)
			L << r.correlation_id;
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	void dump(std::basic_ostream< CharT, TraitsT >& L, const proto::u8_t * buf, std::size_t len) {
		L << std::hex;
		while (len) {
			L << "0x" << static_cast<unsigned>(*buf);
			if (len != 1)
				L << " ";
			len--;
			buf++;
		}
		L << std::endl;
	}

} }

#endif
