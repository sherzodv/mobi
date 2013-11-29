#ifndef smpp_proto_hpp
#define smpp_proto_hpp

#include <cstdint>
#include <cstring>
#include <arpa/inet.h>

namespace smpp {

	/* INTEGER & RELATED POINTERS TYPEDEFS */

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

	/* SMPP 3.4 COMMAND IDS */

	namespace command {
		const proto::u32_t generic_nack			= 0x80000000;
		const proto::u32_t bind_receiver		= 0x00000001;
		const proto::u32_t bind_receiver_r		= 0x80000001;
		const proto::u32_t bind_transmitter		= 0x00000002;
		const proto::u32_t bind_transmitter_r	= 0x80000002;
		const proto::u32_t query_sm				= 0x00000003;
		const proto::u32_t query_sm_r			= 0x80000003;
		const proto::u32_t submit_sm			= 0x00000004;
		const proto::u32_t submit_sm_r			= 0x80000004;
		const proto::u32_t deliver_sm			= 0x00000005;
		const proto::u32_t deliver_sm_r			= 0x80000005;
		const proto::u32_t unbind				= 0x00000006;
		const proto::u32_t unbind_r				= 0x80000006;
		const proto::u32_t replace_sm			= 0x00000007;
		const proto::u32_t replace_sm_r			= 0x80000007;
		const proto::u32_t cancel_sm			= 0x00000008;
		const proto::u32_t cancel_sm_r			= 0x80000008;
		const proto::u32_t bind_transceiver		= 0x00000009;
		const proto::u32_t bind_transceiver_r	= 0x80000009;
		const proto::u32_t outbind				= 0x0000000B;
		const proto::u32_t enquire_link			= 0x00000015;
		const proto::u32_t enquire_link_r		= 0x80000015;
		const proto::u32_t submit_multi_sm		= 0x00000021;
		const proto::u32_t submit_multi_sm_r	= 0x80000021;
		const proto::u32_t alert_notification	= 0x00000102;
		const proto::u32_t data_sm				= 0x00000103;
		const proto::u32_t data_sm_r			= 0x80000103;
	}

	/* SMPP 3.4 PDU header */
	struct pdu {
		proto::u32_t len;
		proto::u32_t id;
		proto::u32_t status;
		proto::u32_t seqno;
	};

	/* SMPP 3.4 generic tag-length-value type */
	template <typename ValueT>
	struct tlv {
		proto::u16_t tag;
		proto::u16_t len;
		ValueT val;
		tlv(): tag(0), len(0) {}
	};

	/* SMPP 3.4 TLV specializations */
	namespace {

		typedef tlv<proto::u8_t>		tlv_dest_addr_subunit;
		typedef tlv<proto::u8_t>		tlv_source_addr_subunit;
		typedef tlv<proto::u8_t>		tlv_dest_network_type;
		typedef tlv<proto::u8_t>		tlv_source_network_type;
		typedef tlv<proto::u8_t>		tlv_dest_bearer_type;
		typedef tlv<proto::u8_t>		tlv_source_bearer_type;
		typedef tlv<proto::u16_t>		tlv_dest_telematics_id;
		typedef tlv<proto::u8_t>		tlv_source_telematics_id;
		typedef tlv<proto::u32_t>		tlv_qos_time_to_live;
		typedef tlv<proto::u8_t>		tlv_payload_type;
		typedef tlv<proto::u8_t[256]>	tlv_additional_status_info_text;
		typedef tlv<proto::u8_t[65]>	tlv_receipted_message_id;
		typedef tlv<proto::u8_t>		tlv_ms_msg_wait_facilities;
		typedef tlv<proto::u8_t>		tlv_privacy_indicator;
		typedef tlv<proto::u8_t[23]>	tlv_source_subaddress;
		typedef tlv<proto::u8_t[23]> 	lv_dest_subaddress;
		typedef tlv<proto::u16_t> 		lv_user_message_reference;
		typedef tlv<proto::u8_t>  		tlv_user_response_code;
		typedef tlv<proto::u8_t>  		tlv_language_indicator;
		typedef tlv<proto::u16_t> 		tlv_source_port;
		typedef tlv<proto::u16_t> 		tlv_destination_port;
		typedef tlv<proto::u16_t> 		tlv_sar_msg_ref_num;
		typedef tlv<proto::u8_t>  		tlv_sar_total_segments;
		typedef tlv<proto::u8_t>  		tlv_sar_segment_seqnum;
		typedef tlv<proto::u8_t>  		tlv_sc_interface_version;
		typedef tlv<proto::u8_t>  		tlv_display_time;
		typedef tlv<proto::u8_t>  		tlv_ms_validity;
		typedef tlv<proto::u8_t>  		tlv_dpf_result;
		typedef tlv<proto::u8_t>  		tlv_set_dpf;
		typedef tlv<proto::u8_t>  		tlv_ms_availability_status;
		typedef tlv<proto::u8_t[3]>		tlv_network_error_code;

		/* length of field 'value' is variable */
		typedef tlv<proto::u8p_t>		tlv_message_payload;

		typedef tlv<proto::u8_t>		tlv_delivery_failure_reason;
		typedef tlv<proto::u8_t>		tlv_more_messages_to_send;
		typedef tlv<proto::u8_t>		tlv_message_state;
		typedef tlv<proto::u8_t[19]>	tlv_callback_num;
		typedef tlv<proto::u8_t>		tlv_callback_num_pres_ind;
		typedef tlv<proto::u8_t[65]>	tlv_callback_num_atag;
		typedef tlv<proto::u8_t>		tlv_number_of_messages;
		typedef tlv<proto::u16_t>		tlv_sms_signal;
		typedef tlv<proto::u8_t>		tlv_alert_on_message_delivery;
		typedef tlv<proto::u8_t>		tlv_its_reply_type;
		typedef tlv<proto::u8_t[2]>		tlv_its_session_info;
		typedef tlv<proto::u8_t[1]>		tlv_ussd_service_op;

	}

	/* SMPP 3.4 OPERATION STRUCTURES */

	namespace {

		/* BIND OPERATION */

		struct bind_transmitter {
			pdu command;
			proto::u8_t sys_id[16];
			proto::u8_t password[9];
			proto::u8_t sys_type[13];
			proto::u8_t interface_version;
			proto::u8_t addr_ton;
			proto::u8_t addr_npi;
			proto::u8_t addr_range[41];

			std::size_t sys_id_len;
			std::size_t password_len;
			std::size_t sys_type_len;
			std::size_t addr_range_len;
		};

		struct bind_transmitter_r {
			pdu command;
			proto::u8_t sys_id[16];
			tlv_sc_interface_version sc_interface_version;

			std::size_t sys_id_len;

			bind_transmitter_r()
				: command()
				, sc_interface_version()
				, sys_id_len(0)
			{}
		};

		struct bind_receiver {
			pdu command;
			proto::u8_t sys_id[16];
			proto::u8_t password[9];
			proto::u8_t sys_type[13];
			proto::u8_t interface_version;
			proto::u8_t addr_ton;
			proto::u8_t addr_npi;
			proto::u8_t addr_range[41];

			std::size_t sys_id_len;
			std::size_t password_len;
			std::size_t sys_type_len;
			std::size_t addr_range_len;
		};

		struct bind_receiver_r {
			pdu command;
			proto::u8_t sys_id[16];
			tlv_sc_interface_version sc_interface_version;

			std::size_t sys_id_len;

			bind_receiver_r()
				: command()
				, sc_interface_version()
				, sys_id_len(0)
			{}
		};

		struct bind_transceiver {
			pdu command;
			proto::u8_t sys_id[16];
			proto::u8_t password[9];
			proto::u8_t sys_type[13];
			proto::u8_t interface_version;
			proto::u8_t addr_ton;
			proto::u8_t addr_npi;
			proto::u8_t addr_range[41];

			std::size_t sys_id_len;
			std::size_t password_len;
			std::size_t sys_type_len;
			std::size_t addr_range_len;
		};

		struct bind_transceiver_r {
			pdu command;
			proto::u8_t sys_id[16];
			tlv_sc_interface_version sc_interface_version;

			std::size_t sys_id_len;

			bind_transceiver_r()
				: command()
				, sc_interface_version()
				, sys_id_len(0)
			{}
		};

		/* BIND OPERATION */

		struct unbind {
			pdu command;
			unbind()
				: command()
			{}
		};

		struct unbind_r {
			pdu command;
			unbind_r()
				: command()
			{}
		};

		/* SUBMIT SM OPERATION */

		struct submit_sm {
			pdu command;

			submit_sm()
				: command()
			{}
		};

		struct submit_sm_r {
			pdu command;
			proto::u8_t msg_id[65];

			std::size_t msg_id_len;

			submit_sm_r()
				: command()
				, msg_id_len(0)
			{}
		};

	}

	/* TYPE TRAITS */

	namespace {

		/* Generic type trait */
		template <class T>
		struct the {};

		/* Fields prefixed with r_ represent properties
		 * of corresponding response PDUs
		 */

		template <>
		struct the<bind_receiver> {
			/* Corresponding response type */
			typedef bind_receiver_r r_type;

			/* Corresponding response command id */
			static const proto::u32_t r_id = command::bind_receiver_r;

			/* Response overall message length */
			static std::size_t r_size(const r_type & r) {
				return sizeof(pdu)
					+ (r.sc_interface_version.tag != 0)
						? sizeof(tlv_sc_interface_version) : 0
					+ r.sys_id_len;
			}
		};

		template <>
		struct the<bind_transmitter> {
			/* Corresponding response type */
			typedef bind_transmitter_r r_type;

			/* Corresponding response command id */
			static const proto::u32_t r_id = command::bind_transmitter_r;

			/* Response overall message length */
			static std::size_t r_size(const r_type & r) {
				return sizeof(pdu)
					+ (r.sc_interface_version.tag != 0)
						? sizeof(tlv_sc_interface_version) : 0
					+ r.sys_id_len;
			}
		};

		template <>
		struct the<bind_transceiver> {
			/* Corresponding response type */
			typedef bind_transceiver_r r_type;

			/* Corresponding response command id */
			static const proto::u32_t r_id = command::bind_transceiver_r;

			/* Response overall message length */
			static std::size_t r_size(const r_type & r) {
				return sizeof(pdu)
					+ ((r.sc_interface_version.tag != 0)
						? sizeof(tlv_sc_interface_version) : 0)
					+ r.sys_id_len;
			}
		};

		template <>
		struct the<submit_sm> {
			typedef submit_sm type;
			static const proto::u32_t id = command::submit_sm;

			/* Corresponding response type */
			typedef submit_sm_r r_type;

			/* Corresponding response command id */
			static const proto::u32_t r_id = command::submit_sm_r;

			/* Response overall message length */
			static std::size_t r_size(const r_type & r) {
				return sizeof(pdu) + r.msg_id_len;
			}
		};
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

			inline const u8_t * scpy(u8_t * dst, const u8_t * src, sz_t len) {
				while (len) {
					if (*src == 0) {
						*dst++ = *src++;
						return src;
					}
					*dst++ = *src++;
					--len;
				}
				return src;
			}

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
				*dst = *src++;
				return dst;
			}

			inline u8_t * cp_u16(u8_t * dst, const u8_t * src) {
				src += 1;
				*dst++ = *src--;
				*dst++ = *src--;
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

	/* PARSERS & WRITERS */

	/* Parsers take a pointer to a buffer end when PDU may have
	 * optional parameters, in order to be able to check, if we
	 * are done with parsing or have to parse more optional parameters */

	/* TLV P&W */

	namespace {

		/* TLV<8> P&W */

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

		/* TLV<16> P&W */

		template <class LogT>
		const proto::u16_t * parse(tlv<proto::u16_t> & t
				, const proto::u8_t * buf, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = p::cp_u16(asbuf(t.tag), buf);
			buf = p::cp_u16(asbuf(t.len), buf);
			buf = p::cp_u16(asbuf(t.val), buf);
			return buf;
		}

		template <class LogT>
		proto::u16_t * write(proto::u8_t * buf
				, const tlv<proto::u16_t> & t, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = w::cp_u16(buf, ascbuf(t.tag));
			buf = w::cp_u16(buf, ascbuf(t.len));
			buf = w::cp_u16(buf, ascbuf(t.val));
			return buf;
		}

		/* TLV<32> P&W */

		template <class LogT>
		const proto::u32_t * parse(tlv<proto::u32_t> & t
				, const proto::u8_t * buf, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = p::cp_u32(asbuf(t.tag), buf);
			buf = p::cp_u32(asbuf(t.len), buf);
			buf = p::cp_u32(asbuf(t.val), buf);
			return buf;
		}

		template <class LogT>
		proto::u32_t * write(proto::u8_t * buf
				, const tlv<proto::u32_t> & t, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = w::cp_u32(buf, ascbuf(t.tag));
			buf = w::cp_u32(buf, ascbuf(t.len));
			buf = w::cp_u32(buf, ascbuf(t.val));
			return buf;
		}

	}

	/* PDU P&W */

	namespace {

		template <class LogT>
		const proto::u8_t * parse(pdu & cmd
				, const proto::u8_t * buf, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = p::cp_u32(asbuf(cmd.len), buf);
			buf = p::cp_u32(asbuf(cmd.id), buf);
			buf = p::cp_u32(asbuf(cmd.status), buf);
			buf = p::cp_u32(asbuf(cmd.seqno), buf);
			return buf;
		}

		template <class LogT>
		proto::u8_t * write(proto::u8_t * buf
				, const pdu & cmd, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = w::cp_u32(buf, ascbuf(cmd.len));
			buf = w::cp_u32(buf, ascbuf(cmd.id));
			buf = w::cp_u32(buf, ascbuf(cmd.status));
			buf = w::cp_u32(buf, ascbuf(cmd.seqno));
			return buf;
		}

	}

	/* OPERATIONS P&W */

	namespace {

		/* GENERIC BIND P&W */

		template <class BindT, class LogT>
		void parse_bind(BindT & r, const proto::u8_t * buf, LogT & L) {
			using namespace utl;
			buf = parse(r.command, buf, L);
			buf = p::scpyl(r.sys_id, buf, sizeof(r.sys_id), r.sys_id_len);
			buf = p::scpyl(r.password, buf , sizeof(r.password), r.password_len);
			buf = p::scpyl(r.sys_type, buf, sizeof(r.sys_type), r.sys_id_len);
			buf = p::cp_u8(&r.interface_version, buf);
			buf = p::cp_u8(&r.addr_ton, buf);
			buf = p::cp_u8(&r.addr_npi, buf);
			buf = p::scpyl(r.addr_range, buf
					, sizeof(r.addr_range), r.addr_range_len);
		}

		template <class BindT, class LogT>
		void write_bind(proto::u8_t * buf, const BindT & r, LogT & L) {
			using namespace utl;
			buf = write(buf, r.command, L);
			buf = w::scpy(buf, r.sys_id, r.sys_id_len + 1);
			buf = w::scpy(buf, r.password, r.password_len + 1);
			buf = w::scpy(buf, r.sys_type, r.sys_id_len + 1);
			buf = w::cp_u8(buf, &r.interface_version);
			buf = w::cp_u8(buf, &r.addr_ton);
			buf = w::cp_u8(buf, &r.addr_npi);
			buf = w::scpy(buf, r.addr_range, r.addr_range_len + 1);
		}

		template <class BindRT, class LogT>
		void parse_bind_r(BindRT & r, const proto::u8_t * buf
				, const proto::u8_t * bend, LogT & L) {
			using namespace utl;
			buf = parse(r.command, buf, L);
			buf = p::scpyl(r.sys_id, buf, sizeof(r.sys_id), r.sys_id_len);
			if (buf < bend) {
				buf = parse(r.sc_interface_version, buf, L);
			}
		}

		template <class BindRT, class LogT>
		void write_bind_r(proto::u8_t * buf, const BindRT & r, LogT & L) {
			using namespace utl;
			buf = write(buf, r.command, L);
			buf = w::scpy(buf, r.sys_id, r.sys_id_len + 1);
			if (r.sc_interface_version.tag != 0) {
				buf = write(buf, r.sc_interface_version, L);
			}
		}

		template <class LogT>
		void parse(unbind & r, const proto::u8_t * buf, LogT & L) {
			parse(r.command, buf, L);
		}

		template <class LogT>
		void write(proto::u8_t * buf, const unbind & r, LogT & L) {
			write(buf, r.command, L);
		}

		template <class LogT>
		void parse(unbind_r & r, const proto::u8_t * buf, LogT & L) {
			parse(r.command, buf, L);
		}

		template <class LogT>
		void write(proto::u8_t * buf, const unbind_r & r, LogT & L) {
			write(buf, r.command, L);
		}

		template <class LogT>
		void parse(submit_sm_r & r, const proto::u8_t * buf
				, const proto::u8_t * bend, LogT & L) {
			using namespace utl;
			buf = parse(r.command, buf, L);
			buf = p::scpyl(r.msg_id, buf, sizeof(r.msg_id), r.msg_id_len);
		}

		template <class LogT>
		void write(proto::u8_t * buf, const submit_sm_r & r, LogT & L) {
			using namespace utl;
			buf = write(buf, r.command, L);
			buf = w::scpy(buf, r.msg_id, r.msg_id_len + 1);
		}

	}

	const std::size_t bind_pdu_min_len = sizeof(pdu) + sizeof(proto::u8_t) * 7;
}

#endif
