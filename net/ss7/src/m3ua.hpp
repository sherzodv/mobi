#ifndef mobi_net_ss7_m3ua_hpp
#define mobi_net_ss7_m3ua_hpp

#include <bitset>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <ostream>
#include <arpa/inet.h>

namespace mobi { namespace net { namespace ss7 { namespace m3ua {

	using namespace toolbox;

	enum message_class: bin::u8_t {
		mclass_mgmt         = 0x00,
		mclass_transfer     = 0x01,
		mclass_ss7_mgmt     = 0x02,
		mclass_asp_state    = 0x03,
		mclass_asp_traffic  = 0x04,
		mclass_reserved1    = 0x05,
		mclass_reserved2    = 0x05,
		mclass_reserved3    = 0x05,
		mclass_reserved4    = 0x05,
		mclass_route_mgmt   = 0x05,
	};

	enum parameter_tag: bin::u16_t {
		/* Common across adaptation layers */
		tag_reserved        = 0x0000,
		tag_info_string     = 0x0004,
		tag_routing_ctxt    = 0x0006,
		tag_diagnostics     = 0x0007,
		tag_heartbeat       = 0x0009,
		tag_traf_mode_type  = 0x000B,
		tag_error_code      = 0x000C,
		tag_status          = 0x000D,
		tag_asp_id          = 0x0011,
		tag_aff_point_code  = 0x0012,
		tag_correlation_id  = 0x0013,

		/* m3ua specific */
		tag_network_appearance  = 0x0200,
		tag_reserved1           = 0x0201,
		tag_reserved2           = 0x0202,
		tag_reserved3           = 0x0203,
		tag_user_cause          = 0x0204,
		tag_cong_indications    = 0x0205,
		tag_concerned_dst       = 0x0206,
		tag_routing_key         = 0x0207,
		tag_reg_result          = 0x0208,
		tag_dereg_result        = 0x0209,
		tag_local_routing_key_id= 0x020A,
		tag_dst_point_code      = 0x020B,
		tag_service_indicators  = 0x020C,
		tag_reserved4           = 0x020D,
		tag_orig_point_code_list= 0x020E,
		tag_reserved5           = 0x020F,
		tag_protocol_data       = 0x0210,
		tag_reserved6           = 0x0211,
		tag_reg_status          = 0x0212,
		tag_dereg_status        = 0x0213, 
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
 
		enum mgmt: bin::u8_t {
 			mgmt_error	= 0x0,
			mgmt_notify	= 0x1,
		};

		enum transfer: bin::u8_t {
 			trans_reserved	= 0x0,
			trans_data		= 0x1,	/* Payload Data */
		};

		enum ss7_mgmt: bin::u8_t {
 			ss7_mgmt_reserved	= 0x0,
			ss7_mgmt_duna		= 0x1,	/* Destination unavailable */
			ss7_mgmt_dava		= 0x2,	/* Destination available */
			ss7_mgmt_daud		= 0x3,	/* Destination state audit */
			ss7_mgmt_scon		= 0x4,	/* Signalling congestion */
			ss7_mgmt_dupu		= 0x5,	/* Destination user part unavailable */
			ss7_mgmt_drst		= 0x6,	/* Destination restricted */
		};

		enum asp_state: bin::u8_t {
 			asp_state_reserved	= 0x0,
			asp_state_up		= 0x1,	/* ASP up */
			asp_state_down		= 0x2,	/* ASP down */
			asp_state_hb		= 0x3,	/* Heartbeat */
			asp_state_up_ack	= 0x4,	/* ASP up acknowledgement */
			asp_state_down_ack	= 0x5,	/* ASP down acknowledgement */
			asp_state_hb_ack	= 0x6,	/* Heartbeat acknowledgement */
		};

		enum asp_traf: bin::u8_t {
 			asp_traf_reserved		= 0x0,
			asp_traf_active			= 0x1,	/* ASP active */
			asp_traf_inactive		= 0x2,	/* ASP inactive */
			asp_traf_active_ack		= 0x3,	/* ASP active acknowledgement */
			asp_traf_inactive_ack	= 0x4,	/* ASP inactive acknowledgement */
		};

		enum asp_route: bin::u8_t {
 			asp_route_reserved	= 0x0,
			asp_route_reg_req	= 0x1,	/* ASP registration request */
			asp_route_reg_rsp	= 0x2,	/* ASP registration response */
			asp_route_dereg_req	= 0x3,	/* ASP deregistration request */
			asp_route_dereg_rsp	= 0x4,	/* ASP deregistration response */
 		};
 
	}

	struct pdu {
 		bin::u8_t version;
		bin::u8_t reserved;
		message_class mclass: 8;
		union {
			mtype::mgmt         mgmt: 8;
			mtype::transfer     transfer: 8;
			mtype::ss7_mgmt     ss7mgmt: 8;
			mtype::asp_state    aspstate: 8;
			mtype::asp_traf     asptraf: 8;
 			mtype::asp_traf     asproute: 8;
		} type;
		bin::u32_t len;

	};

	template <typename ValueT>
	struct tlv {
		parameter_tag tag;
		bin::u16_t len;
		ValueT val;
 		tlv():  tag(tag_reserved), len(0) {}
	};

	namespace message {

		struct mtp3 {
			bin::u32_t          opc;	/* Originating point code */
			bin::u32_t          dpc;	/* Destination point code */
			service_indicator   si:8;	/* Service indicator */
			bin::u8_t           ni;		/* Network indicator */
			bin::u8_t           mp;		/* Message priority */
			bin::u8_t           sls;	/* Signalling link selection code */

			/* TODO: define max len for user part data */
			bin::sz_t data_len;
 			bin::u8_t data[255]; /* User Part data */
		};

		struct data {
			pdu              header;
			tlv<bin::u32_t>  network_appearance;
			tlv<bin::u32_t>  routing_ctxt;
			tlv<mtp3>        protocol_data;
 			tlv<bin::u32_t>  correlation_id;
 		};

	}

	template <class LogT>
	const bin::u8_t * parse(tlv<bin::u8_t> & t
 			, const bin::u8_t * buf, LogT & L) {
		(void)(L);
		using namespace bin;
		buf = p::cp_u16(asbuf(t.tag), buf);
		buf = p::cp_u16(asbuf(t.len), buf);
		buf = p::cp_u8(asbuf(t.val), buf);
		return  buf;
	}

	template <class LogT>
	bin::u8_t * write(bin::u8_t * buf
 			, const tlv<bin::u8_t> & t, LogT & L) {
		(void)(L);
		using namespace bin;
		buf = w::cp_u16(buf, ascbuf(t.tag));
		buf = w::cp_u16(buf, ascbuf(t.len));
		buf = w::cp_u8(buf, ascbuf(t.val));
		return buf;
	}

	template <class LogT>
	const bin::u8_t * parse(tlv<bin::u16_t> & t
			, const bin::u8_t * buf, LogT & L) {
		(void)(L);
		using namespace bin;
		buf = p::cp_u16(asbuf(t.tag), buf);
		buf = p::cp_u16(asbuf(t.len), buf);
		buf = p::cp_u16(asbuf(t.val), buf);
 		return buf;
	}

	template <class LogT>
	bin::u8_t * write(bin::u8_t * buf
 			, const tlv<bin::u16_t> & t, LogT & L) {
		(void)(L);
		using namespace bin;
		buf = w::cp_u16(buf, ascbuf(t.tag));
		buf = w::cp_u16(buf, ascbuf(t.len));
		buf = w::cp_u16(buf, ascbuf(t.val));
		return buf;
	}

	template <class LogT>
	const bin::u8_t * parse(tlv<bin::u32_t> & t
 			, const bin::u8_t * buf, LogT & L) {
		(void)(L);
		using namespace bin;
		buf = p::cp_u16(asbuf(t.tag), buf);
		buf = p::cp_u16(asbuf(t.len), buf);
		buf = p::cp_u32(asbuf(t.val), buf);
		return buf;
	}

	template <class LogT>
	bin::u8_t * write(bin::u8_t * buf
			, const tlv<bin::u32_t> & t, LogT & L) {
		(void)(L);
		using namespace bin;
		buf = w::cp_u16(buf, ascbuf(t.tag));
		buf = w::cp_u16(buf, ascbuf(t.len));
		buf = w::cp_u32(buf, ascbuf(t.val));
 		return buf;
	}

	template <class LogT>
	const bin::u8_t * parse(pdu & r
			, const bin::u8_t * buf, LogT & L) {
		(void)(L);
		using namespace bin;
		buf = p::cpy(asbuf(r), buf, 4);
		buf = p::cp_u32(asbuf(r.len), buf);
 		return buf;
	}

	template <class LogT>
	bin::u8_t * write(bin::u8_t * buf
			, const pdu & r, LogT & L) {
		(void)(L);
		using namespace bin;
		buf = w::cpy(buf, ascbuf(r), 4);
		buf = w::cp_u32(buf, ascbuf(r.len));
 		return buf;
	}

	template <class LogT>
	const bin::u8_t * parse(message::mtp3 & r
 			, const bin::u8_t * buf, bin::sz_t len, LogT & L) {
		(void)(L);
		using namespace bin;
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
	bin::u8_t * write(bin::u8_t * buf
			, const message::mtp3 & r, LogT & L) {
		(void)(L);
		using namespace bin;
		buf = w::cp_u32(buf, ascbuf(r.opc));
		buf = w::cp_u32(buf, ascbuf(r.dpc));
		/* Copy si, ni, mp, sls at once */
		buf = w::cpy(buf, ascbuf(r) + 8, 4);
		buf = w::cpy(buf, r.data, r.data_len);
 		return buf;
	}

	template <class LogT>
	const bin::u8_t * parse(tlv<message::mtp3> & t
 			, const bin::u8_t * buf, LogT & L) {
		(void)(L);
		using namespace bin;
		buf = p::cp_u16(asbuf(t.tag), buf);
		buf = p::cp_u16(asbuf(t.len), buf);
		buf = parse(t.val, buf, t.len - 4, L);
		return buf;
	}

	template <class LogT>
	bin::u8_t * write(bin::u8_t * buf
			, const tlv<message::mtp3> & t, LogT & L) {
		(void)(L);
		using namespace bin;
		buf = w::cp_u16(buf, ascbuf(t.tag));
		buf = w::cp_u16(buf, ascbuf(t.len));
		buf = write(buf, t.val, L);
 		return buf;
	}

	template <class LogT>
	const bin::u8_t * parse(message::data & r
			, const bin::u8_t * buf
 			, const bin::u8_t * bend, LogT & L) {
		(void)(L);
		using namespace bin;

		bin::u16_t tag;

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
	bin::u8_t * write(bin::u8_t * buf
			, const message::data & r, LogT & L) {
		(void)(L);
		using namespace bin;

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

	std::string to_string(service_indicator si) {
		std::stringstream out;
		out << "[si:";
		switch (si) {
			case si_mgmt      : out << "mgmt"; break;
			case si_test      : out << "test"; break;
			case si_special   : out << "special"; break;
			case si_sccp      : out << "sccp"; break;
			case si_tup       : out << "tup"; break;
			case si_isdnup    : out << "isdnup"; break;
			case si_dup1      : out << "dup1"; break;
			case si_dup2      : out << "dup2"; break;
			case si_broadband : out << "broadband"; break;
			case si_sattelite : out << "sattelite"; break;
			default           : out << "unknown:" << static_cast<unsigned>(si); break;
		}
		out << "]";
		return out.str();
	}

	std::string to_string(parameter_tag t) {
 		std::stringstream out;
		out << "[tag:";
		switch (t) {
 			case tag_info_string            : out << "info_string"; break;
			case tag_routing_ctxt           : out << "routing_ctxt"; break;
			case tag_diagnostics            : out << "diagnostics"; break;
			case tag_heartbeat              : out << "heartbeat"; break;
			case tag_traf_mode_type         : out << "traf_mode_type"; break;
			case tag_error_code             : out << "error_code"; break;
			case tag_status                 : out << "status"; break;
			case tag_asp_id                 : out << "asp_id"; break;
			case tag_aff_point_code         : out << "aff_point_code"; break;
			case tag_correlation_id	        : out << "correlation_id"; break;
			case tag_network_appearance     : out << "network_appearance"; break;
			case tag_user_cause             : out << "user_cause"; break;
			case tag_cong_indications       : out << "cong_indications"; break;
			case tag_concerned_dst          : out << "concerned_dst"; break;
			case tag_routing_key            : out << "routing_key"; break;
			case tag_reg_result             : out << "reg_result"; break;
			case tag_dereg_result           : out << "dereg_result"; break;
			case tag_local_routing_key_id   : out << "local_routing_key_id"; break;
			case tag_dst_point_code         : out << "dst_point_code"; break;
			case tag_service_indicators     : out << "service_indicators"; break;
			case tag_orig_point_code_list   : out << "orig_point_code_list"; break;
			case tag_protocol_data          : out << "protocol_data"; break;
			case tag_reg_status             : out << "reg_status"; break;
			case tag_dereg_status           : out << "dereg_status"; break;
			default                         : out << "tag:unknown"; break;
		}
		out << "]";
		return out.str(); 
	} 

	std::string to_string(message_class mc) {
		std::stringstream out;
		switch (mc) {
 			case mclass_mgmt        : out << "mgmt"; break;
			case mclass_transfer    : out << "transfer"; break;
			case mclass_ss7_mgmt    : out << "ss7mgmt"; break;
			case mclass_asp_state   : out << "aspstate"; break;
			case mclass_asp_traffic : out << "asptraf"; break;
			case mclass_route_mgmt  : out << "routemgmt"; break;
 			default                 : out << "unknown"; break;
		}
 		return out.str();
	}

	std::string to_string(mtype::mgmt mt) {
		std::stringstream out;
		switch (mt) {
			case mtype::mgmt_error  : out << "mgmt_error"; break;
			case mtype::mgmt_notify : out << "mgmt_notify"; break;
	 		default                 : out << "mgmt:unknown"; break;
		}
 		return out.str(); 
	}

	std::string to_string(mtype::transfer mt) {
		std::stringstream out; 
 		switch (mt) {
			case mtype::trans_data : out << "trans_data"; break;
			default                : out << "tr ans:unknown"; break;
		}
		return out.str(); 
	}
	
	std::string to_string(mtype::ss7_mgmt mt) {
		std::stringstream out;
	 	switch (mt) {
			case mtype::ss7_mgmt_duna: out << "ss7mgmt_DUNA"; break;
			case mtype::ss7_mgmt_dava: out << "ss7mgmt_DAVA"; break;
			case mtype::ss7_mgmt_daud: out << "ss7mgmt_DAUD"; break;
			case mtype::ss7_mgmt_scon: out << "ss7mgmt_SCON"; break;
			case mtype::ss7_mgmt_dupu: out << "ss7mgmt_DUPU"; break;
			case mtype::ss7_mgmt_drst: out << "ss7mgmt_DRST"; break;
	 		default                  : out << "ss7mgmt:unknown"; break;
		}
		return out.str(); 
	}

	std::string to_string(mtype::asp_state mt) {
		std::stringstream out;  
		switch (mt) {
			case mtype::asp_state_up        : out << "aspstate_UP"; break;
			case mtype::asp_state_down      : out << "aspstate_DOWN"; break;
			case mtype::asp_state_hb        : out << "aspstate_HB"; break;
			case mtype::asp_state_up_ack    : out << "aspstate_UPACK"; break;
			case mtype::asp_state_down_ack  : out << "aspstate_DOWNACK"; break;
			case mtype::asp_state_hb_ack    : out << "aspstate_HBACK"; break;
			default                         : out << "asp state:unknown"; break;
		}
		return out.str();
	}

	std::string to_string(mtype::asp_traf mt) {
		std::stringstream out;
 		switch (mt) {
 			case mtype::asp_traf_active       : out << "asptraf_ACTIVE"; break;
			case mtype::asp_traf_inactive     : out << "asptraf_INACTIVE"; break;
			case mtype::asp_traf_active_ack   : out << "asptraf_ACTIVEACK"; break;
			case mtype::asp_traf_inactive_ack : out << "asptraf_INACTIVEACK"; break;
		 	default                           : out << "asptraf:unknown"; break;
		} 
		return out.str();  
	}

	std::string to_string(mtype::asp_route mt) {
		std::stringstream out;
	 	switch (mt) {
			case mtype::asp_route_reg_req    : out << "asproute_REGREQ"; break;
			case mtype::asp_route_reg_rsp    : out << "asproute_REGRSP"; break;
			case mtype::asp_route_dereg_req  : out << "asproute_DEREGREQ"; break;
			case mtype::asp_route_dereg_rsp  : out << "asproute_DEREGRSP"; break;
		 	default                          : out << "asproute:unknown"; break;
		} 
		return out.str();  
	}

	std::string to_string(const pdu & r) {
		std::stringstream out;
		out << "[hdr: " 
			<< "[version:" << static_cast<unsigned>(r.version) << "]"
			<< "[class:" <<  r.mclass << "]";

		out << "[type:";
		switch (r.mclass) {
			case mclass_mgmt        : out << to_string(r.type.mgmt); break;
			case mclass_transfer    : out << to_string(r.type.transfer); break;
			case mclass_ss7_mgmt    : out << to_string(r.type.ss7mgmt); break;
			case mclass_asp_state   : out << to_string(r.type.aspstate); break;
			case mclass_asp_traffic : out << to_string(r.type.asptraf); break;
			case mclass_route_mgmt  : out << to_string(r.type.asproute); break;
			default                 : out <<  "unknown"; break;
		}
		out << "]";

		out << "[len:" << r.len << "]";
		out << "]";
		return out.str();   
	}

	std::string to_string(const message::mtp3 & r) {
		std::stringstream out;
		out << "[mtp3:"
			<< "[opc:" << r.opc << "]"
			<< "[dpc:" << r.dpc << "]"
			<< to_string(r.si)
			<< "[ni:" << static_cast<unsigned>(r.ni) << "]"
			<< "[mp:" << static_cast<unsigned>(r.mp) << "]"
			<< "[s ls:" <<  static_cast<unsigned>(r.sls) << "]"
		    << "]";
		return out.str();  
	}

	template <typename ValueT>
	std::string to_string(ValueT val) {
		std::stringstream out;
		out << val;
		return out.str();
	}

	template <typename ValueT>
	std::string to_string(const tlv<ValueT> & t) {
		std:: stringstream out;
		out << "[tlv:" 
			<< to_string(t .tag)
			<< "[len:" << t.len << "]"
			<<  "[val:" << to_string(t.val) << "]"
		    << "]";
		return out.str();
	}

	std::string to_string(const message::data & r) {
		std::stringstream out;
		out << "[M3UA_DATA:"
			<< to_string(r.header);
		if (r.network_appearance.tag != tag_reserved)
			out << to_string(r.network_appearance);
		if (r.routing_ctxt.tag != tag_reserved)
			out << to_string( r.routing_ctxt);
		if (r.protocol_data.tag != tag_reserved)
			out << to_string(r.protocol_data);
		if (r.correlation_id.tag != tag_reserved)
			out << to_string(r.correlation_id);
		out << "]";
		return out.str();
	}


} } } }

#endif
