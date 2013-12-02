#ifndef smpp_proto_hpp
#define smpp_proto_hpp

#include <bitset>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <ostream>
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

	/* SMPP 3.4 TLV IDS */

	namespace option {
		const proto::u16_t dest_addr_subunit				= 0x0005;
		const proto::u16_t dest_networ_type					= 0x0006;
		const proto::u16_t dest_bearer_type					= 0x0007;
		const proto::u16_t dest_telematics_id				= 0x0008;
		const proto::u16_t source_addr_subunit				= 0x000D;
		const proto::u16_t source_networ_type				= 0x000E;
		const proto::u16_t source_bearer_type				= 0x000F;
		const proto::u16_t source_telematics_id				= 0x0010;
		const proto::u16_t qos_time_to_live					= 0x0017;
		const proto::u16_t payload_type						= 0x0019;
		const proto::u16_t additional_status_info_text		= 0x0019;
		const proto::u16_t receipted_message_id				= 0x001E;
		const proto::u16_t ms_msg_wait_facilities			= 0x0030;
		const proto::u16_t privacy_indicator				= 0x0201;
		const proto::u16_t source_subaddress				= 0x0202;
		const proto::u16_t dest_subaddress					= 0x0203;
		const proto::u16_t user_message_reference			= 0x0204;
		const proto::u16_t user_response_code				= 0x0205;
		const proto::u16_t source_port						= 0x020A;
		const proto::u16_t dest_port						= 0x020B;
		const proto::u16_t sar_msg_ref_num					= 0x020C;
		const proto::u16_t language_indicator				= 0x020D;
		const proto::u16_t sar_total_segments				= 0x020E;
		const proto::u16_t sar_segment_seqnum				= 0x020F;
		const proto::u16_t sc_interface_version				= 0x0210;
		const proto::u16_t callback_num_pres_ind			= 0x0302;
		const proto::u16_t callback_num_atag				= 0x0303;
		const proto::u16_t number_of_messages				= 0x0304;
		const proto::u16_t callback_num						= 0x0381;
		const proto::u16_t dpf_result						= 0x0420;
		const proto::u16_t set_dpf							= 0x0421;
		const proto::u16_t ms_availability_status			= 0x0422;
		const proto::u16_t network_error_code				= 0x0423;
		const proto::u16_t message_payload					= 0x0424;
		const proto::u16_t delivery_failure_reason			= 0x0425;
		const proto::u16_t more_messages_to_send			= 0x0426;
		const proto::u16_t message_state					= 0x0427;
		const proto::u16_t ussd_service_op					= 0x0501;
		const proto::u16_t display_time						= 0x1201;
		const proto::u16_t sms_signal						= 0x1203;
		const proto::u16_t ms_validity						= 0x1204;
		const proto::u16_t alert_on_message_delivery		= 0x130C;
		const proto::u16_t its_reply_type					= 0x1380;
		const proto::u16_t its_session_info					= 0x1383;
	}

	/* SMPP 3.4 ERROR CODES */

	namespace error {
		/* TODO: Finish work on error codes */
		const proto::u32_t esme_rok				= 0x00000000;
		const proto::u32_t esme_rinvmsglen 		= 0x00000000;
		const proto::u32_t esme_rinvcmdlen		= 0x00000000;
		const proto::u32_t esme_rinvcmdid		= 0x00000000;
		const proto::u32_t esme_rinvbndsts		= 0x00000000;
		const proto::u32_t esme_ralybnd			= 0x00000000;
		const proto::u32_t esme_rinvprtflg		= 0x00000000;
		const proto::u32_t esme_rinvregdlvflg	= 0x00000000;
		const proto::u32_t esme_rsyserr			= 0x00000000;
		const proto::u32_t esme_rinvsrcadr		= 0x00000000;
		const proto::u32_t esme_rinvdstadr		= 0x00000000;
		const proto::u32_t esme_rinvmsgid		= 0x00000000;
		const proto::u32_t esme_rbindfail		= 0x00000000;
		const proto::u32_t esme_rinvpaswd		= 0x00000000;
		const proto::u32_t esme_rinvsysid		= 0x00000000;
		const proto::u32_t esme_rcancelfail		= 0x00000000;
		const proto::u32_t esme_rreplacefail	= 0x00000000;
		const proto::u32_t esme_rmsgqful		= 0x00000000;
		const proto::u32_t esme_rinvsertyp		= 0x00000000;
		const proto::u32_t esme_rinvnumdests	= 0x00000000;
		const proto::u32_t esme_rinvdlname 		= 0x00000000;
		const proto::u32_t esme_rinvdestflag	= 0x00000000;
		const proto::u32_t esme_rinvsubrep 		= 0x00000000;
		const proto::u32_t esme_rinvesmclass	= 0x00000000;
		const proto::u32_t esme_rcntsubdl 		= 0x00000000;
		const proto::u32_t esme_rsubmitfail		= 0x00000000;
		const proto::u32_t esme_rinvsrcton 		= 0x00000000;
		const proto::u32_t esme_rinvsrcnpi 		= 0x00000000;
		const proto::u32_t esme_rinvdstton 		= 0x00000000;
		const proto::u32_t esme_rinvdstnpi 		= 0x00000000;
		const proto::u32_t esme_rinvsystyp 		= 0x00000000;
		const proto::u32_t esme_rinvrepflag		= 0x00000000;
		const proto::u32_t esme_rinvnummsgs		= 0x00000000;
		const proto::u32_t esme_rthrottled 		= 0x00000000;
		const proto::u32_t esme_rinvsched 		= 0x00000000;
		const proto::u32_t esme_rinvexpiry 		= 0x00000000;
		const proto::u32_t esme_rinvdftmsgid	= 0x00000000;
		const proto::u32_t esme_rx_t_appn 		= 0x00000000;
		const proto::u32_t esme_rx_r_appn 		= 0x00000000;
		const proto::u32_t esme_rqueryfail 		= 0x00000000;
		const proto::u32_t esme_rinvopt 		= 0x00000000;
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
		typedef tlv<proto::u8_t[23]> 	tlv_dest_subaddress;
		typedef tlv<proto::u16_t> 		tlv_user_message_reference;
		typedef tlv<proto::u8_t>  		tlv_user_response_code;
		typedef tlv<proto::u8_t>  		tlv_language_indicator;
		typedef tlv<proto::u16_t> 		tlv_source_port;
		typedef tlv<proto::u16_t> 		tlv_dest_port;
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

			proto::u8_t	service_type[6];

			/* Address of ESME which originated this message */
			proto::u8_t src_addr_ton;
			proto::u8_t src_addr_npi;
			proto::u8_t src_addr[21];

			/* Address  ME which originated this message */
			proto::u8_t dst_addr_ton;
			proto::u8_t dst_addr_npi;
			proto::u8_t dst_addr[21];

			proto::u8_t esm_class;
			proto::u8_t protocol_id;
			proto::u8_t priority_flag;

			proto::u8_t schedule_delivery_time[17];
			proto::u8_t validity_period[17];

			/* Indicator to signify if and SMSC delivery receipt
			 * or and SMS aknowledgment is required. */
			proto::u8_t registered_delivery;
			proto::u8_t replace_if_present_flag;
			proto::u8_t data_coding;

			proto::u8_t sm_default_msg_id;

			proto::u8_t sm_len;
			proto::u8_t short_message[254];

			tlv_user_message_reference		user_message_reference;
			tlv_source_port					source_port;
			tlv_source_addr_subunit			source_addr_subunit;
			tlv_dest_port					dest_port;
			tlv_dest_addr_subunit			dest_addr_subunit;
			tlv_sar_msg_ref_num				sar_msg_ref_num;
			tlv_sar_total_segments			sar_total_segments;
			tlv_sar_segment_seqnum			sar_segment_seqnum;
			tlv_more_messages_to_send		more_messages_to_send;
			tlv_payload_type				payload_type;
			tlv_message_payload				message_payload;
			tlv_privacy_indicator			privacy_indicator;
			tlv_callback_num				callback_num;
			tlv_callback_num_pres_ind		callback_num_pres_ind;
			tlv_callback_num_atag			callback_num_atag;
			tlv_source_subaddress			source_subaddress;
			tlv_dest_subaddress				dest_subaddress;
			tlv_user_response_code			user_response_code;
			tlv_display_time				display_time;
			tlv_sms_signal					sms_signal;
			tlv_ms_validity					ms_validity;
			tlv_ms_msg_wait_facilities		ms_msg_wait_facilities;
			tlv_number_of_messages			number_of_messages;
			tlv_alert_on_message_delivery	alert_on_message_delivery;
			tlv_language_indicator			language_indicator;
			tlv_its_reply_type				its_reply_type;
			tlv_its_session_info			its_session_info;
			tlv_ussd_service_op				ussd_service_op;

			std::size_t service_type_len;
			std::size_t src_addr_len;
			std::size_t dst_addr_len;

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

		/* TLV<32> P&W */

		template <class LogT>
		const proto::u8_t * parse(tlv<proto::u32_t> & t
				, const proto::u8_t * buf, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = p::cp_u32(asbuf(t.tag), buf);
			buf = p::cp_u32(asbuf(t.len), buf);
			buf = p::cp_u32(asbuf(t.val), buf);
			return buf;
		}

		template <class LogT>
		proto::u8_t * write(proto::u8_t * buf
				, const tlv<proto::u32_t> & t, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = w::cp_u32(buf, ascbuf(t.tag));
			buf = w::cp_u32(buf, ascbuf(t.len));
			buf = w::cp_u32(buf, ascbuf(t.val));
			return buf;
		}

		template <class LogT>
		const proto::u8_t * parse(tlv_source_subaddress & t
				, const proto::u8_t * buf, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = p::cp_u32(asbuf(t.tag), buf);
			buf = p::cp_u32(asbuf(t.len), buf);
			/* TODO: parse value: 5.3.2.15 */
			memset(t.val, 0, sizeof(t.val));
			buf += t.len;
			return buf;
		}

		template <class LogT>
		proto::u8_t * write(proto::u8_t * buf
				, const tlv_source_subaddress & t, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = w::cp_u32(buf, ascbuf(t.tag));
			buf = w::cp_u32(buf, ascbuf(t.len));
			buf = w::scpy(buf, t.val, t.len);
			return buf;
		}

		template <class LogT>
		const proto::u8_t * parse(tlv_callback_num & t
				, const proto::u8_t * buf, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = p::cp_u32(asbuf(t.tag), buf);
			buf = p::cp_u32(asbuf(t.len), buf);
			/* TODO: parse value: 5.3.2.36 */
			memset(t.val, 0, sizeof(t.val));
			buf += t.len;
			return buf;
		}

		template <class LogT>
		proto::u8_t * write(proto::u8_t * buf
				, const tlv_callback_num & t, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = w::cp_u32(buf, ascbuf(t.tag));
			buf = w::cp_u32(buf, ascbuf(t.len));
			buf = w::scpy(buf, t.val, t.len);
			return buf;
		}

		template <class LogT>
		const proto::u8_t * parse(tlv_callback_num_atag & t
				, const proto::u8_t * buf, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = p::cp_u32(asbuf(t.tag), buf);
			buf = p::cp_u32(asbuf(t.len), buf);
			/* TODO: parse value: 5.3.2.38 */
			memset(t.val, 0, sizeof(t.val));
			buf += t.len;
			return buf;
		}

		template <class LogT>
		proto::u8_t * write(proto::u8_t * buf
				, const tlv_callback_num_atag & t, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = w::cp_u32(buf, ascbuf(t.tag));
			buf = w::cp_u32(buf, ascbuf(t.len));
			buf = w::scpy(buf, t.val, t.len);
			return buf;
		}

		template <class LogT>
		const proto::u8_t * parse(tlv_its_session_info & t
				, const proto::u8_t * buf, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = p::cp_u32(asbuf(t.tag), buf);
			buf = p::cp_u32(asbuf(t.len), buf);
			/* TODO: parse value: 5.3.2.43 */
			memset(t.val, 0, sizeof(t.val));
			buf += t.len;
			return buf;
		}

		template <class LogT>
		proto::u8_t * write(proto::u8_t * buf
				, const tlv_its_session_info & t, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = w::cp_u32(buf, ascbuf(t.tag));
			buf = w::cp_u32(buf, ascbuf(t.len));
			buf = w::scpy(buf, t.val, t.len);
			return buf;
		}

		template <class LogT>
		const proto::u8_t * parse(tlv_ussd_service_op & t
				, const proto::u8_t * buf, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = p::cp_u32(asbuf(t.tag), buf);
			buf = p::cp_u32(asbuf(t.len), buf);
			/* TODO: parse value: 5.3.2.44 */
			memset(t.val, 0, sizeof(t.val));
			buf += t.len;
			return buf;
		}

		template <class LogT>
		proto::u8_t * write(proto::u8_t * buf
				, const tlv_ussd_service_op & t, LogT & L) {
			(void)(L);
			using namespace utl;
			buf = w::cp_u32(buf, ascbuf(t.tag));
			buf = w::cp_u32(buf, ascbuf(t.len));
			buf = w::scpy(buf, t.val, t.len);
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

		/* UNBIND P&W */

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

		/* SUBMIT_SM P&W */

		template <class LogT>
		void parse(submit_sm & r, const pdu * msg, LogT & L) {

			using namespace utl;

			proto::u16_t optid;
			const proto::u8_t *buf, *bend;

			buf = ascbuf(msg);
			bend = buf + msg->len;

			buf = parse(r.command, buf, L);
			buf = p::scpyl(r.service_type, buf
					, sizeof(r.service_type), r.service_type_len);
			buf = p::cp_u8(&r.src_addr_ton, buf);
			buf = p::cp_u8(&r.src_addr_npi, buf);
			buf = p::scpyl(r.src_addr, buf
					, sizeof(r.src_addr), r.src_addr_len);
			buf = p::cp_u8(&r.dst_addr_ton, buf);
			buf = p::cp_u8(&r.dst_addr_npi, buf);
			buf = p::scpyl(r.dst_addr, buf
					, sizeof(r.dst_addr), r.dst_addr_len);
			buf = p::cp_u8(&r.esm_class, buf);
			buf = p::cp_u8(&r.protocol_id, buf);
			buf = p::cp_u8(&r.priority_flag, buf);
			buf = p::scpyf(r.schedule_delivery_time, buf
					, sizeof(r.schedule_delivery_time));
			buf = p::scpyf(r.validity_period, buf, sizeof(r.validity_period));
			buf = p::cp_u8(&r.registered_delivery, buf);
			buf = p::cp_u8(&r.replace_if_present_flag, buf);
			buf = p::cp_u8(&r.data_coding, buf);
			buf = p::cp_u8(&r.sm_default_msg_id, buf);
			buf = p::cp_u8(&r.sm_len, buf);
			buf = p::cpy(r.short_message, buf
					, std::min(static_cast<std::size_t>(r.sm_len)
						, sizeof(r.sm_len)));

			buf = p::cp_u16(asbuf(optid), buf);
			buf -= sizeof(proto::u16_t);

			while (buf < bend) {
				switch (optid) {
					case option::user_message_reference:
						buf = parse(r.user_message_reference, buf, L);
						break;
					case option::source_port:
						buf = parse(r.source_port, buf, L);
						break;
					case option::source_addr_subunit:
						buf = parse(r.source_addr_subunit, buf, L);
						break;
					case option::dest_port:
						buf = parse(r.dest_port, buf, L);
						break;
					case option::dest_addr_subunit:
						buf = parse(r.dest_addr_subunit, buf, L);
						break;
					case option::sar_msg_ref_num:
						buf = parse(r.sar_msg_ref_num, buf, L);
						break;
					case option::sar_total_segments:
						buf = parse(r.sar_total_segments, buf, L);
						break;
					case option::sar_segment_seqnum:
						buf = parse(r.sar_segment_seqnum, buf, L);
						break;
					case option::more_messages_to_send:
						buf = parse(r.more_messages_to_send, buf, L);
						break;
					case option::payload_type:
						buf = parse(r.payload_type, buf, L);
						break;
					/* TODO: Parse message_payload*/
					/*
					case option::message_payload:
						buf = parse(r.message_payload, buf, L);
						break;
					*/
					case option::privacy_indicator:
						buf = parse(r.privacy_indicator, buf, L);
						break;
					case option::callback_num:
						buf = parse(r.callback_num, buf, L);
						break;
					case option::callback_num_pres_ind:
						buf = parse(r.callback_num_pres_ind, buf, L);
						break;
					case option::callback_num_atag:
						buf = parse(r.callback_num_atag, buf, L);
						break;
					case option::source_subaddress:
						buf = parse(r.source_subaddress, buf, L);
						break;
					case option::dest_subaddress:
						buf = parse(r.dest_subaddress, buf, L);
						break;
					case option::user_response_code:
						buf = parse(r.user_response_code, buf, L);
						break;
					case option::display_time:
						buf = parse(r.display_time, buf, L);
						break;
					case option::sms_signal:
						buf = parse(r.sms_signal, buf, L);
						break;
					case option::ms_validity:
						buf = parse(r.ms_validity, buf, L);
						break;
					case option::ms_msg_wait_facilities:
						buf = parse(r.ms_msg_wait_facilities, buf, L);
						break;
					case option::number_of_messages:
						buf = parse(r.number_of_messages, buf, L);
						break;
					case option::alert_on_message_delivery:
						buf = parse(r.alert_on_message_delivery, buf, L);
						break;
					case option::language_indicator:
						buf = parse(r.language_indicator, buf, L);
						break;
					case option::its_reply_type:
						buf = parse(r.its_reply_type, buf, L);
						break;
					case option::its_session_info:
						buf = parse(r.its_session_info, buf, L);
						break;
					case option::ussd_service_op:
						buf = parse(r.ussd_service_op, buf, L);
						break;
					default:
						return;
				}
			}
		}

		template <class LogT>
		void write(proto::u8_t * buf, const submit_sm & r, LogT & L) {
			using namespace utl;
			buf = write(buf, r.command, L);
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

	/* OPERATIONS DUMP */

	namespace {

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<proto::u8_t> & t) {
			L << std::bitset<8>(t.val);
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<proto::u16_t> & t) {
			L << t.val;
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<proto::u32_t> & t) {
			L << t.val;
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<proto::u8_t[256]> & t) {
			L << t.val;
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<proto::u8_t[65]> & t) {
			L << t.val;
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<proto::u8_t[23]> & t) {
			L << t.val;
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<proto::u8_t[19]> & t) {
			L << t.val;
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<proto::u8_t[3]> & t) {
			L << t.val;
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<proto::u8_t[2]> & t) {
			L << t.val;
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<proto::u8_t[1]> & t) {
			L << t.val;
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<proto::u8p_t> & t) {
			L << t.val;
			return L;
		}

		template< typename CharT, typename TraitsT >
		std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const submit_sm & r) {
			L << "submit_sm: "
				<< " [service_type:" << r.service_type << "]"
				<< " [src_addr_ton:" << std::bitset<8>(r.src_addr_ton) << "]"
				<< " [src_addr_npi:" << std::bitset<8>(r.src_addr_npi) << "]"
				<< " [src_addr:" << r.src_addr<< "]"
				<< " [dst_addr_ton:" << std::bitset<8>(r.dst_addr_ton) << "]"
				<< " [dst_addr_npi:" << std::bitset<8>(r.dst_addr_npi) << "]"
				<< " [dst_addr:" << r.dst_addr<< "]"
				<< " [esm_class:" << std::bitset<8>(r.esm_class) << "]"
				<< " [protocol_id:" << static_cast<int>(r.protocol_id) << "]"
				<< " [priority_flag:" << static_cast<int>(r.priority_flag) << "]"
				<< " [schedule_delivery_time:" << r.schedule_delivery_time << "]"
				<< " [validity_period:" << r.validity_period << "]"
				<< " [registered_delivery:" << static_cast<bool>(r.registered_delivery) << "]"
				<< " [replace_if_present_flag:" << static_cast<bool>(r.replace_if_present_flag) << "]"
				<< " [data_coding:" << std::bitset<8>(r.data_coding) << "]"
				<< " [sm_default_msg_id:" << static_cast<int>(r.sm_default_msg_id) << "]"
				<< " [sm_len:" << r.sm_len << "]"
				<< " [short_message:" << std::string(r.short_message, r.short_message + r.sm_len) << "]"
			;

			if (r.user_message_reference.tag != 0) {
				L << " [user_message_reference:"
					<< r.user_message_reference
					<< "]";
			}

			if (r.source_port.tag != 0) {
				L << " [source_port:"
					<< r.source_port
					<< "]";
			}

			if (r.source_addr_subunit.tag != 0) {
				L << " [source_addr_subunit:"
					<< r.source_addr_subunit
					<< "]";
			}

			if (r.dest_port.tag != 0) {
				L << " [dest_port:"
					<< r.dest_port
					<< "]";
			}

			if (r.dest_addr_subunit.tag != 0) {
				L << " [dest_addr_subunit:"
					<< r.dest_addr_subunit
					<< "]";
			}

			if (r.sar_msg_ref_num.tag != 0) {
				L << " [sar_msg_ref_num:"
					<< r.sar_msg_ref_num
					<< "]";
			}

			if (r.sar_total_segments.tag != 0) {
				L << " [sar_total_segments:"
					<< r.sar_total_segments
					<< "]";
			}

			if (r.sar_segment_seqnum.tag != 0) {
				L << " [sar_segment_seqnum:"
					<< r.sar_segment_seqnum
					<< "]";
			}

			if (r.more_messages_to_send.tag != 0) {
				L << " [more_messages_to_send:"
					<< r.more_messages_to_send
					<< "]";
			}

			if (r.payload_type.tag != 0) {
				L << " [payload_type:"
					<< r.payload_type
					<< "]";
			}

			if (r.message_payload.tag != 0) {
				L << " [message_payload:"
					<< r.payload_type
					<< "]";
			}

			if (r.privacy_indicator.tag != 0) {
				L << " [privacy_indicator:"
					<< r.privacy_indicator
					<< "]";
			}

			if (r.callback_num.tag != 0) {
				L << " [callback_num:"
					<< r.callback_num
					<< "]";
			}

			if (r.callback_num_pres_ind.tag != 0) {
				L << " [callback_num_pres_ind:"
					<< r.callback_num_pres_ind
					<< "]";
			}

			if (r.callback_num_atag.tag != 0) {
				L << " [callback_num_atag:"
					<< r.callback_num_atag
					<< "]";
			}

			if (r.source_subaddress.tag != 0) {
				L << " [source_subaddress:"
					<< r.source_subaddress
					<< "]";
			}

			if (r.dest_subaddress.tag != 0) {
				L << " [dest_subaddress:"
					<< r.dest_subaddress
					<< "]";
			}

			if (r.user_response_code.tag != 0) {
				L << " [user_response_code:"
					<< r.user_response_code
					<< "]";
			}

			if (r.display_time.tag != 0) {
				L << " [display_time:"
					<< r.display_time
					<< "]";
			}

			if (r.sms_signal.tag != 0) {
				L << " [sms_signal:"
					<< r.sms_signal
					<< "]";
			}

			if (r.ms_validity.tag != 0) {
				L << " [ms_validity:"
					<< r.ms_validity
					<< "]";
			}

			if (r.ms_msg_wait_facilities.tag != 0) {
				L << " [ms_msg_wait_facilities:"
					<< r.ms_msg_wait_facilities
					<< "]";
			}

			if (r.number_of_messages.tag != 0) {
				L << " [number_of_messages:"
					<< r.number_of_messages
					<< "]";
			}

			if (r.alert_on_message_delivery.tag != 0) {
				L << " [alert_on_message_delivery:"
					<< r.alert_on_message_delivery
					<< "]";
			}

			if (r.language_indicator.tag != 0) {
				L << " [language_indicator:"
					<< r.language_indicator
					<< "]";
			}

			if (r.its_reply_type.tag != 0) {
				L << " [its_reply_type:"
					<< r.its_reply_type
					<< "]";
			}

			if (r.its_session_info.tag != 0) {
				L << " [its_session_info:"
					<< r.its_session_info
					<< "]";
			}

			if (r.ussd_service_op.tag != 0) {
				L << " [ussd_service_op:"
					<< r.ussd_service_op
					<< "]";
			}

			return L;
		}
	}

	const std::size_t bind_pdu_min_len = sizeof(pdu) + sizeof(proto::u8_t) * 7;
}

#endif
