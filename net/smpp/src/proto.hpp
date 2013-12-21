#ifndef smpp_proto_hpp
#define smpp_proto_hpp

#define RETURN_NULL_IF(a) if (a) { return nullptr; }

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

	/* SMPP 3.4 COMMAND STATUS/ERROR CODES */
	namespace command_status {
		const proto::u32_t esme_rok				= 0x00000000;
		const proto::u32_t esme_rinvmsglen		= 0x00000001;
		const proto::u32_t esme_rinvcmdlen		= 0x00000002;
		const proto::u32_t esme_rinvcmdid		= 0x00000003;
		const proto::u32_t esme_rinvbndsts		= 0x00000004;
		const proto::u32_t esme_ralybnd			= 0x00000005;
		const proto::u32_t esme_rinvprtflg		= 0x00000006;
		const proto::u32_t esme_rinvregdlvflg	= 0x00000007;
		const proto::u32_t esme_rsyserr			= 0x00000008;
		const proto::u32_t esme_rinvsrcadr		= 0x0000000A;
		const proto::u32_t esme_rinvdstadr		= 0x0000000B;
		const proto::u32_t esme_rinvmsgid		= 0x0000000C;
		const proto::u32_t esme_rbindfail		= 0x0000000D;
		const proto::u32_t esme_rinvpaswd		= 0x0000000E;
		const proto::u32_t esme_rinvsysid		= 0x0000000F;
		const proto::u32_t esme_rcancelfail		= 0x00000011;
		const proto::u32_t esme_rreplacefail	= 0x00000013;
		const proto::u32_t esme_rmsgqful		= 0x00000014;
		const proto::u32_t esme_rinvsertyp		= 0x00000015;
		const proto::u32_t esme_rinvnumdests	= 0x00000033;
		const proto::u32_t esme_rinvdlname		= 0x00000034;
		const proto::u32_t esme_rinvdestflag	= 0x00000040;
		const proto::u32_t esme_rinvsubrep		= 0x00000042;
		const proto::u32_t esme_rinvesmclass	= 0x00000043;
		const proto::u32_t esme_rcntsubdl		= 0x00000044;
		const proto::u32_t esme_rsubmitfail		= 0x00000045;
		const proto::u32_t esme_rinvsrcton		= 0x00000048;
		const proto::u32_t esme_rinvsrcnpi		= 0x00000049;
		const proto::u32_t esme_rinvdstton		= 0x00000050;
		const proto::u32_t esme_rinvdstnpi		= 0x00000051;
		const proto::u32_t esme_rinvsystyp		= 0x00000053;
		const proto::u32_t esme_rinvrepflag		= 0x00000054;
		const proto::u32_t esme_rinvnummsgs		= 0x00000055;
		const proto::u32_t esme_rthrottled		= 0x00000058;
		const proto::u32_t esme_rinvsched		= 0x00000061;
		const proto::u32_t esme_rinvexpiry		= 0x00000062;
		const proto::u32_t esme_rinvdftmsgid	= 0x00000063;
		const proto::u32_t esme_rx_t_appn		= 0x00000064;
		const proto::u32_t esme_rx_p_appn		= 0x00000065;
		const proto::u32_t esme_rx_r_appn		= 0x00000066;
		const proto::u32_t esme_rqueryfail		= 0x00000067;
		const proto::u32_t esme_rinvoptparstream= 0x000000C0;
		const proto::u32_t esme_roptparnotallwd	= 0x000000C1;
		const proto::u32_t esme_rinvparlen		= 0x000000C2;
		const proto::u32_t esme_rmissingoptparam= 0x000000C3;
		const proto::u32_t esme_rinvoptparamval	= 0x000000C4;
		const proto::u32_t esme_rdeliveryfailure= 0x000000FE;
		const proto::u32_t esme_runknownerr		= 0x000000FF;
	}

	/* SMPP 3.4 TLV IDS */

	namespace option {
		const proto::u16_t dest_addr_subunit				= 0x0005;
		const proto::u16_t dest_networ_type					= 0x0006;
		const proto::u16_t dest_bearer_type					= 0x0007;
		const proto::u16_t dest_telematics_id				= 0x0008;
		const proto::u16_t src_addr_subunit					= 0x000D;
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

	/* SMPP 3.4 TON VALUES */

	namespace ton {
		proto::u32_t unknown			= 0x00000000;
		proto::u32_t international		= 0x00000001;
		proto::u32_t national			= 0x00000010;
		proto::u32_t network_specific	= 0x00000011;
		proto::u32_t subscriber_number	= 0x00000100;
		proto::u32_t alphanumeric		= 0x00000101;
		proto::u32_t abbreviated		= 0x00000110;
	}

	/* SMPP 3.4 NPI VALUES */

	namespace npi {
		proto::u32_t unknown			= 0x00000000;
		proto::u32_t isdn				= 0x00000001;
		proto::u32_t data				= 0x00000011;
		proto::u32_t telex				= 0x00000100;
		proto::u32_t land_mobile		= 0x00000110;
		proto::u32_t national			= 0x00001000;
		proto::u32_t private_			= 0x00001001;
		proto::u32_t ermes				= 0x00001010;
		proto::u32_t internet			= 0x00001110;
		proto::u32_t wap_client_id		= 0x00010010;
	}

	/* SMPP 3.4 SERVICE TYPES */

	namespace service_types {
		const proto::u8p_t	default_				= NULL;
		const proto::u8_t cell_msging[]				= "CMT";
		const proto::u8_t cell_paging[]				= "CPT";
		const proto::u8_t voice_mail_notif[]		= "VMN";
		const proto::u8_t voice_mail_alert[]		= "VMA";
		const proto::u8_t wireless_app_protocol[]	= "WAP";
		const proto::u8_t unstruct_sup_serv_dat[]	= "USSD";
	}

	/* SMPP 3.4 esm class consts */

	namespace esm_c {

		namespace submit_sm {
			/* messaging mode, you should use switch (var&0x03) */
			const proto::u8_t default_sms_mode		= 0x00;
			const proto::u8_t datagram_mode			= 0x01;
			const proto::u8_t forward_mode			= 0x02;
			const proto::u8_t store_n_forward_mode	= 0x03;

			/* message type, you should use switch (var&0x3C) */
			const proto::u8_t default_msg_type		= 0x00;
			const proto::u8_t esme_delivery_ack		= 0x08;
			const proto::u8_t esme_manual_ack		= 0x10;

			/* gsm network specific features, your should use switch (var&0xC0)*/
			const proto::u8_t no_spec_features_sel	= 0x00;
			const proto::u8_t udhi_ind				= 0x40;
			const proto::u8_t set_reply_path		= 0x80;
			const proto::u8_t set_udhi_reply_path	= 0xC0;
		}

		namespace submit_multi {
			/* messaging mode, you should use switch (var&0x03) */
			const proto::u8_t default_sms_mode		= 0x00;
			const proto::u8_t datagram_mode			= 0x01;
			const proto::u8_t forward_mode			= 0x02;
			const proto::u8_t store_n_forward_mode	= 0x03;

			/* message type, you should use switch (var&0x3C) */
			const proto::u8_t default_msg_type		= 0x00;
			const proto::u8_t esme_delivery_ack		= 0x08;
			const proto::u8_t esme_manual_ack		= 0x10;

			/* gsm network specific features, your should use switch (var&0xC0)*/
			const proto::u8_t no_spec_features_sel	= 0x00;
			const proto::u8_t udhi_ind				= 0x40;
			const proto::u8_t set_reply_path		= 0x80;
			const proto::u8_t set_udhi_reply_path	= 0xC0;
		}

		namespace data_sm {
			const proto::u8_t not_applicable		= 0x00;

			/* message type, you should use switch (var & 0x3C) */
			const proto::u8_t default_msg_typ		= 0x00;
			const proto::u8_t smsc_delivery_rec		= 0x04;
			const proto::u8_t sme_delivery_ack		= 0x08;
			const proto::u8_t sme_manual_ack		= 0x10;
			const proto::u8_t conv_abort			= 0x18;
			const proto::u8_t inter_delivery_notf	= 0x20;

			/* gsm network specific features, 0xC0 */
			const proto::u8_t no_spec_select		= 0x00;
			const proto::u8_t udhi_ind_set			= 0x40;
			const proto::u8_t reply_path			= 0x80;
			const proto::u8_t udhi_n_reply_path		= 0xC0;
			/* messaging mode, you should use switch (var&0x03) */
			const proto::u8_t default_sms_mode		= 0x00;
			const proto::u8_t datagram_mode			= 0x01;
			const proto::u8_t forward_mode			= 0x02;
			const proto::u8_t store_n_forward_mode	= 0x03;

			/* message type, you should use switch (var&0x3C) */
			const proto::u8_t default_msg_type		= 0x00;
			const proto::u8_t esme_delivery_ack		= 0x08;
			const proto::u8_t esme_manual_ack		= 0x10;

			/* gsm network specific features, your should use switch (var&0xC0)*/
			const proto::u8_t no_spec_features_sel	= 0x00;
			const proto::u8_t udhi_ind				= 0x40;
			const proto::u8_t set_reply_path		= 0x80;
			const proto::u8_t set_udhi_reply_path	= 0xC0;
		}

		namespace deliver_sm {
			const proto::u8_t not_applicable		= 0x00;

			/* message type, you should use switch (var & 0x3C) */
			const proto::u8_t default_msg_typ		= 0x00;
			const proto::u8_t smsc_delivery_rec		= 0x04;
			const proto::u8_t sme_delivery_ack		= 0x08;
			const proto::u8_t sme_manual_ack		= 0x10;
			const proto::u8_t conv_abort			= 0x18;
			const proto::u8_t inter_delivery_notf	= 0x20;

			/* gsm network specific features, 0xC0 */
			const proto::u8_t no_spec_select		= 0x00;
			const proto::u8_t udhi_ind_set			= 0x40;
			const proto::u8_t reply_path			= 0x80;
			const proto::u8_t udhi_n_reply_path		= 0xC0;
		}
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
		typedef tlv<proto::u8_t>		tlv_src_addr_subunit;
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

			/* this values will not be writed to buffer */
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
			tlv_src_addr_subunit			src_addr_subunit;
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

		/* SUBMIT MULTI Operations */

		struct submit_multi {
			pdu command;
			proto::u8_t service_type[6];
			proto::u8_t src_addr_ton;
			proto::u8_t src_addr_npi;
			proto::u8_t src_addr[21];
			proto::u8_t number_of_dests;
			/* TODO: 4.5.1 dest_address(es) */
			proto::u8_t esm_class;
			proto::u8_t protocol_id;
			proto::u8_t priority_flag;
			proto::u8_t schedule_delivery_time[17];
			proto::u8_t validity_period[17];
			proto::u8_t registered_delivery;
			proto::u8_t replace_if_present_flag;
			proto::u8_t data_coding;
			proto::u8_t sm_default_msg_id;
			proto::u8_t sm_length;
			proto::u8_t short_message[254];

			tlv_user_message_reference	user_message_reference;
			tlv_source_port				source_port;
			tlv_src_addr_subunit		src_addr_subunit;
			tlv_dest_port				dest_port;
			tlv_dest_addr_subunit		dest_addr_subunit;
			tlv_sar_msg_ref_num			sar_msg_ref_num;
			tlv_sar_total_segments		sar_total_segments;
			tlv_sar_segment_seqnum		sar_segment_seqnum;
			tlv_payload_type			payload_type;
			tlv_message_payload			message_payload;
			tlv_privacy_indicator		privacy_indicator;
			tlv_callback_num			callback_num;
			tlv_callback_num_pres_ind	callback_num_pres_ind;
			tlv_callback_num_atag		callback_num_atag;
			tlv_source_subaddress		source_subaddress;
			tlv_dest_subaddress			dest_subaddress;
			tlv_display_time			display_time;
			tlv_sms_signal				sms_signal;
			tlv_ms_validity				ms_validity;
			tlv_ms_msg_wait_facilities	ms_msg_wait_facilities;
			tlv_alert_on_message_delivery	alert_on_message_delivery;
			tlv_language_indicator		language_indicator;

			std::size_t service_type_len;
			std::size_t src_addr_len;
			std::size_t dst_addr_len;
			std::size_t short_message_len;

			submit_multi()
				: command()
			{}
		};

		struct dest_address {
			proto::u8_t 	dest_flag;
			/* TODO: 4.5.1.1 SME_Address */
		};

		struct SME_dest_address {
			proto::u8_t	dest_addr_ton;
			proto::u8_t dest_addr_npi;
			proto::u8_t	dest_addr[21];
		};

		struct DL_name {
			proto::u8_t	dl_name[21];
		};

		struct submit_multi_r {
			pdu command;

			proto::u8_t	msg_id[65];
			proto::u8_t	no_unsuccess;
			/* TODO: 4.5.2 unsuccess_sme(s) */

			size_t msg_id_len;
			submit_multi_r()
				: command()
			{}
		};

		struct Unsuccess_smes {
			proto::u8_t		dest_addr_ton;
			proto::u8_t		dest_addr_npi;
			proto::u8_t		dest_addr[21];
			proto::u32_t	error_status_code;
		};

		/* DELIVER SM Operations */

		struct deliver_sm {
			pdu command;

			proto::u8_t	service_type[6];
			proto::u8_t src_addr_ton;
			proto::u8_t	src_addr_npi;
			proto::u8_t	src_addr[21];
			proto::u8_t	dest_addr_ton;
			proto::u8_t dest_addr_npi;
			proto::u8_t dest_addr[21];
			proto::u8_t	esm_class;
			proto::u8_t	protocol_id;
			proto::u8_t	priority_flag;
			proto::u8_t	schedule_delivery_time;
			proto::u8_t	validity_period;
			proto::u8_t registered_delivery;
			proto::u8_t	replace_if_present_flag;
			proto::u8_t	data_coding;
			proto::u8_t	sm_default_msg_id;
			proto::u8_t	sm_length;
			proto::u8_t short_message[254];

			/* OPTIONAL PARAMETERS for DELIVER_SM */
			tlv_user_message_reference	user_message_reference;
			tlv_source_port				source_port;
			tlv_dest_port				dest_port;
			tlv_sar_msg_ref_num			sar_msg_ref_num;
			tlv_sar_total_segments		sar_total_segments;
			tlv_sar_segment_seqnum		sar_segment_seqnum;
			tlv_user_response_code		user_response_code;
			tlv_privacy_indicator		privacy_indicator;
			tlv_payload_type			payload_type;
			tlv_callback_num			callback_num;
			tlv_source_subaddress		source_subaddress;
			tlv_dest_subaddress			dest_subaddress;
			tlv_language_indicator		language_indicator;
			tlv_its_session_info		its_session_info;
			tlv_network_error_code		network_error_code;
			tlv_message_state			message_state;
			tlv_receipted_message_id	receipted_message_id;

			deliver_sm()
				: command()
			{}
		};

		struct deliver_sm_r {
			pdu command;
			proto::u8_t	message_id;

			deliver_sm_r()
				: command()
			{}
		};

		/* DATA_SM Operations */

		struct data_sm {
			pdu command;

			proto::u8_t		service_type[6];
			proto::u8_t		src_addr_ton;
			proto::u8_t		src_addr_npi;
			proto::u8_t		src_addr[65];
			proto::u8_t		dest_addr_ton;
			proto::u8_t		dest_addr_npi;
			proto::u8_t		dest_addr[65];
			proto::u8_t		esm_class;
			proto::u8_t		registered_delivery;
			proto::u8_t		data_coding;

			/* OPTIONAL PARAMETERS for DATA SM */

			tlv_source_port					source_port;
			tlv_src_addr_subunit			src_addr_subunit;
			tlv_source_network_type			source_network_type;
			tlv_source_bearer_type			source_bearer_type;
			tlv_source_telematics_id		source_telematics_id;
			tlv_dest_port					dest_port;
			tlv_dest_addr_subunit			dest_addr_subunit;
			tlv_dest_network_type			dest_network_type;
			tlv_dest_bearer_type			dest_bearer_type;
			tlv_dest_telematics_id			dest_telematics_id;
			tlv_sar_msg_ref_num				sar_msg_ref_num;
			tlv_sar_total_segments			sar_total_segments;
			tlv_sar_segment_seqnum			sar_segment_seqnum;
			tlv_more_messages_to_send		more_messages_to_send;
			tlv_qos_time_to_live			qos_time_to_live;
			tlv_payload_type				payload_type;
			tlv_message_payload				message_payload;
			tlv_set_dpf						set_dpf;
			tlv_receipted_message_id		receipted_message_id;
			tlv_message_state				message_state;
			tlv_network_error_code			network_error_code;
			tlv_user_message_reference		user_message_reference;
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

			data_sm()
				: data_sm()
			{}
		};

		struct data_sm_r {
			pdu command;

			proto::u8_t message_id[65];

			/* OPTIONAL PARAMETERS for DATA SM RESP */

			tlv_delivery_failure_reason			delivery_failure_reason;
			tlv_network_error_code				network_error_code;
			tlv_additional_status_info_text		additional_status_info_text;
			tlv_dpf_result						dpf_result;

			data_sm_r()
				: command()
			{}
		};

		/* QUERY SM Operations */

		struct query_sm {
			pdu command;

			proto::u8_t	message_id[65];
			proto::u8_t	src_addr_ton;
			proto::u8_t	src_addr_npi;
			proto::u8_t	src_addr[21];

			query_sm()
				: command()
			{}
		};

		struct query_sm_r {
			pdu command;

			proto::u8_t	message_id[65];
			proto::u8_t	final_date[17];
			proto::u8_t	message_state;
			proto::u8_t	error_code;

			query_sm_r()
				: command()
			{}
		};

		/* CANCEL SM Operations */

		struct cancel_sm {
			pdu command;

			proto::u8_t	service_type[6];
			proto::u8_t	message_id[65];
			proto::u8_t	src_addr_ton;
			proto::u8_t	src_addr_npi;
			proto::u8_t	src_addr[21];
			proto::u8_t	dest_addr_ton;
			proto::u8_t dest_addr_npi;
			proto::u8_t dest_addr[21];

			cancel_sm()
				: command()
			{}
		};

		struct cancel_sm_r {
			pdu command;

			cancel_sm_r()
				: command()
			{}
		};

		/* REPLACE SM Operations */

		struct replace_sm {
			pdu command;

			proto::u8_t	message_id[65];
			proto::u8_t	src_addr_ton;
			proto::u8_t	src_addr_npi;
			proto::u8_t	src_addr[21];
			proto::u8_t schedule_delivery_time[17];
			proto::u8_t validity_period[17];
			proto::u8_t registered_delivery;
			proto::u8_t sm_default_msg_id;
			proto::u8_t sm_length;
			proto::u8_t short_message[254];

			replace_sm()
				: command()
			{}
		};
		
		struct replace_sm_r {
			pdu command;

			replace_sm_r()
				: command()
			{}
		};

		/* ENQUIRE LINK Operation */

		struct enquire_link {
			pdu command;

			enquire_link()
				: command()
			{}
		};

		struct enquire_link_r {
			pdu command;

			enquire_link_r()
				: command()
			{}
		};

		/* ALERT NOTIFICATIONS Operation */

		struct alert_notification {
			pdu command;

			proto::u8_t	src_addr_ton;
			proto::u8_t	src_addr_npi;
			proto::u8_t src_addr[65];
			proto::u8_t	esme_addr_ton;
			proto::u8_t esme_addr_npi;
			proto::u8_t	esme_addr;

			/* OPTIONAL PARAMETERS for ALERT NOTIFICATION */

			tlv_ms_availability_status	ms_availability_status;

			alert_notification()
				: alert_notification()
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
					, const u8_t * srcend, sz_t len, sz_t & l) {
				for (l = 0; (l < len) && (src < srcend); ++l) {
					if (*src == 0) {
						*dst++ = *src++;
						return src;
					}
					*dst++ = *src++;
				}
				RETURN_NULL_IF(l > len);
				RETURN_NULL_IF(src == srcend);
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

			inline u8_t * scpyf(u8_t * dst, const u8_t * src, sz_t len) {
				while (len--)
					*dst++ = *src++;
				*dst++ = 0;
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
				*dst++ = 0;

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
		const proto::u8_t * parse_bind(BindT & r, const proto::u8_t * buf
				, const proto::u8_t * bend, LogT & L) {
			using namespace utl;

			RETURN_NULL_IF(buf + sizeof (r.command) >= bend);
			buf = parse(r.command, buf, L);

			RETURN_NULL_IF(!(buf = p::scpyl(r.sys_id, buf, bend
						, sizeof(r.sys_id), r.sys_id_len)));

			RETURN_NULL_IF(!(buf = p::scpyl(r.password, buf, bend
						, sizeof(r.password), r.password_len)));

			RETURN_NULL_IF(!(buf = p::scpyl(r.sys_type, buf, bend
						, sizeof(r.sys_type), r.sys_type_len)));

			RETURN_NULL_IF(buf + sizeof (r.interface_version) >= bend);
			buf = p::cp_u8(&r.interface_version, buf);

			RETURN_NULL_IF(buf + sizeof (r.addr_ton) >= bend);
			buf = p::cp_u8(&r.addr_ton, buf);

			RETURN_NULL_IF(buf + sizeof (r.addr_npi) >= bend);
			buf = p::cp_u8(&r.addr_npi, buf);

			RETURN_NULL_IF(!(buf = p::scpyl(r.addr_range, buf, bend
					, sizeof(r.addr_range), r.addr_range_len)));

			return buf;
		}

		template <class BindT, class LogT>
		proto::u8_t * write_bind(proto::u8_t * buf, proto::u8_t * bend
				, const BindT & r, LogT & L) {
			using namespace utl;

			RETURN_NULL_IF(buf + sizeof (r.command) >= bend);
			buf = write(buf, r.command, L);

			RETURN_NULL_IF(buf + r.sys_id_len >= bend);
			buf = w::scpyf(buf, r.sys_id, r.sys_id_len);

			RETURN_NULL_IF(buf + r.password_len >= bend);
			buf = w::scpyf(buf, r.password, r.password_len);

			RETURN_NULL_IF(buf + r.sys_type_len >= bend);
			buf = w::scpyf(buf, r.sys_type, r.sys_type_len);

			RETURN_NULL_IF(buf + sizeof (r.interface_version) >= bend);
			buf = w::cp_u8(buf, &r.interface_version);

			RETURN_NULL_IF(buf + sizeof (r.addr_ton) >= bend);
			buf = w::cp_u8(buf, &r.addr_ton);

			RETURN_NULL_IF(buf + sizeof (r.addr_npi) >= bend);
			buf = w::cp_u8(buf, &r.addr_npi);

			RETURN_NULL_IF(buf + r.addr_range_len > bend);
			buf = w::scpyf(buf, r.addr_range, r.addr_range_len+1);

			return buf;
		}

		template <class BindRT, class LogT>
		void parse_bind_r(BindRT & r, const proto::u8_t * buf
				, const proto::u8_t * bend, LogT & L) {
			using namespace utl;

			RETURN_NULL_IF(buf + sizeof (r.command) >= bend);
			buf = parse(r.command, buf, L);

			/* TODO */
			RETURN_NULL_IF(buf + sizeof (r.command) > bend);
			buf = p::scpyl(r.sys_id, buf, bend, sizeof(r.sys_id), r.sys_id_len);
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
		void parse(unbind & r, const proto::u8_t * buf
				, const proto::u8_t * bend, LogT & L) {
			RETURN_NULL_IF(buf + sizeof (r.command) > bend);
			parse(r.command, buf, L);
		}

		template <class LogT>
		void write(proto::u8_t * buf, const unbind & r, LogT & L) {
			write(buf, r.command, L);
		}

		template <class LogT>
		void parse(unbind_r & r, const proto::u8_t * buf
				, const proto::u8_t * bend, LogT & L) {
			RETURN_NULL_IF(buf + sizeof (r.command) > bend);
			parse(r.command, buf, L);
		}

		template <class LogT>
		void write(proto::u8_t * buf, const unbind_r & r, LogT & L) {
			write(buf, r.command, L);
		}

		/* SUBMIT_SM P&W */

		template <class LogT>
		void parse(submit_sm & r, const proto::u8_t * buf
				, const proto::u8_t * bend, LogT & L) {

			using namespace utl;

			proto::u16_t optid;
			buf = ascbuf(buf);
			/*bend = buf + msg->len; */

			RETURN_NULL_IF(buf + sizeof (r.command) >= bend);
			buf = parse(r.command, buf, L);

			RETURN_NULL_IF(buf + sizeof (r.command) >= bend);
			buf = p::scpyl(r.service_type, buf, bend
					, sizeof(r.service_type), r.service_type_len);

			RETURN_NULL_IF(buf + sizeof (r.src_addr_ton) >= bend);
			buf = p::cp_u8(&r.src_addr_ton, buf);

			RETURN_NULL_IF(buf + sizeof (r.src_addr_npi) >= bend);
			buf = p::cp_u8(&r.src_addr_npi, buf);

			RETURN_NULL_IF(buf + sizeof (r.src_addr) >= bend);
			buf = p::scpyl(r.src_addr, buf, bend
					, sizeof(r.src_addr), r.src_addr_len);

			RETURN_NULL_IF(buf + sizeof (r.dst_addr_ton) >= bend);
			buf = p::cp_u8(&r.dst_addr_ton, buf);

			RETURN_NULL_IF(buf + sizeof (r.dst_addr_npi) >= bend);
			buf = p::cp_u8(&r.dst_addr_npi, buf);

			buf = p::scpyl(r.dst_addr, buf, bend
					, sizeof(r.dst_addr), r.dst_addr_len);

			RETURN_NULL_IF(buf + sizeof (r.esm_class) >= bend);
			buf = p::cp_u8(&r.esm_class, buf);

			RETURN_NULL_IF(buf + sizeof (r.protocol_id) >= bend);
			buf = p::cp_u8(&r.protocol_id, buf);

			RETURN_NULL_IF(buf + sizeof (r.priority_flag) >= bend);
			buf = p::cp_u8(&r.priority_flag, buf);

			RETURN_NULL_IF(buf + sizeof (r.schedule_delivery_time));
			buf = p::scpyf(r.schedule_delivery_time, buf
					, sizeof(r.schedule_delivery_time));

			RETURN_NULL_IF(buf + sizeof (r.validity_period) >= bend);
			buf = p::scpyf(r.validity_period, buf, sizeof(r.validity_period));
			
			RETURN_NULL_IF(buf + sizeof (r.registered_delivery) >= bend);
			buf = p::cp_u8(&r.registered_delivery, buf);

			RETURN_NULL_IF(buf + sizeof (r.replace_if_present_flag) >= bend);
			buf = p::cp_u8(&r.replace_if_present_flag, buf);

			RETURN_NULL_IF(buf + sizeof (r.data_coding) >= bend);
			buf = p::cp_u8(&r.data_coding, buf);

			RETURN_NULL_IF(buf + sizeof (r.sm_default_msg_id) >= bend);
			buf = p::cp_u8(&r.sm_default_msg_id, buf);

			RETURN_NULL_IF(buf + sizeof (r.sm_len) >= bend)
			buf = p::cp_u8(&r.sm_len, buf);

			RETURN_NULL_IF(buf + sizeof (r.short_message) >= bend);
			buf = p::cpy(r.short_message, buf
					, std::min(static_cast<std::size_t>(r.sm_len)
						, sizeof(r.sm_len)));

			buf = p::cp_u16(asbuf(optid), buf);
			/*
			buf += sizeof(proto::u16_t);
			*/

			while (buf < bend) {
				switch (optid) {
					case option::user_message_reference:
						buf = parse(r.user_message_reference, buf, L); break;
					case option::source_port:
						buf = parse(r.source_port, buf, L); break;
					case option::src_addr_subunit:
						buf = parse(r.src_addr_subunit, buf, L); break;
					case option::dest_port:
						buf = parse(r.dest_port, buf, L); break;
					case option::dest_addr_subunit:
						buf = parse(r.dest_addr_subunit, buf, L); break;
					case option::sar_msg_ref_num:
						buf = parse(r.sar_msg_ref_num, buf, L); break;
					case option::sar_total_segments:
						buf = parse(r.sar_total_segments, buf, L); break;
					case option::sar_segment_seqnum:
						buf = parse(r.sar_segment_seqnum, buf, L); break;
					case option::more_messages_to_send:
						buf = parse(r.more_messages_to_send, buf, L); break;
					case option::payload_type:
						buf = parse(r.payload_type, buf, L); break;
					/* TODO: Parse message_payload*/
					/*
					case option::message_payload:
						buf = parse(r.message_payload, buf, L);
						break;
					*/
					case option::privacy_indicator:
						buf = parse(r.privacy_indicator, buf, L); break;
					case option::callback_num:
						buf = parse(r.callback_num, buf, L); break;
					case option::callback_num_pres_ind:
						buf = parse(r.callback_num_pres_ind, buf, L); break;
					case option::callback_num_atag:
						buf = parse(r.callback_num_atag, buf, L); break;
					case option::source_subaddress:
						buf = parse(r.source_subaddress, buf, L); break;
					case option::dest_subaddress:
						buf = parse(r.dest_subaddress, buf, L); break;
					case option::user_response_code:
						buf = parse(r.user_response_code, buf, L); break;
					case option::display_time:
						buf = parse(r.display_time, buf, L); break;
					case option::sms_signal:
						buf = parse(r.sms_signal, buf, L); break;
					case option::ms_validity:
						buf = parse(r.ms_validity, buf, L); break;
					case option::ms_msg_wait_facilities:
						buf = parse(r.ms_msg_wait_facilities, buf, L); break;
					case option::number_of_messages:
						buf = parse(r.number_of_messages, buf, L); break;
					case option::alert_on_message_delivery:
						buf = parse(r.alert_on_message_delivery, buf, L); break;
					case option::language_indicator:
						buf = parse(r.language_indicator, buf, L); break;
					case option::its_reply_type:
						buf = parse(r.its_reply_type, buf, L); break;
					case option::its_session_info:
						buf = parse(r.its_session_info, buf, L); break;
					case option::ussd_service_op:
						buf = parse(r.ussd_service_op, buf, L); break;
					default: return;
				}

				/* may be optional parameter wasn't writed corretly */
				RETURN_NULL_IF(buf + sizeof (optid) > bend);
				buf = p::cp_u16(asbuf(optid), buf);
			}
		}

		template <class LogT>
		void write(proto::u8_t * buf, const submit_sm & r, LogT & L) {
			using namespace utl;
			buf = write(buf, r.command, L);
			buf = w::scpy(buf, r.service_type, 6);
			/* Writing address of ESME */
			buf = w::cp_u8(buf, r.src_addr_ton);
			buf = w::cp_u8(buf, r.src_addr_npi);
			buf = w::scpy(buf, r.src_addr, 21);
			/* Writing ME */
			buf = w::cp_u8(buf, r.dst_addr_ton);
			buf = w::cp_u8(buf, r.dst_addr_npi);
			buf = w::scpy(buf, r.dst_addr, 21);
			buf = w::cp_u8(buf, r.esm_class);
			buf = w::cp_u8(buf, r.protocol_id);
			buf = w::cp_u8(buf, r.priority_flag);
			buf = w::scpy(buf, r.schedule_delivery_time, 17);
			buf = w::scpy(buf, r.validity_period, 17);
			buf = w::cp_u8(buf, r.registered_delivery);
			buf = w::cp_u8(buf, r.replace_if_present_flag);
			buf = w::cp_u8(buf, r.data_coding);
			buf = w::cp_u8(buf, r.sm_default_msg_id);
			buf = w::cp_u8(buf, r.sm_len);
			buf = w::scpy(buf, r.short_message, 254);

			if (r.user_message_reference.tag != 0)	buf = write(buf, r.user_message_reference, L);
			if (r.source_port.tag != 0)				buf = write(buf, r.source_port, L);
			if (r.src_addr_subunit.tag != 0)		buf = write(buf, r.src_addr_subunit, L);
			if (r.dest_port.tag != 0)				buf = write(buf, r.dest_port.tag, L);
			if (r.dest_addr_subunit.tag != 0)		buf = write(buf, r.dest_addr_subunit, L);
			if (r.sar_msg_ref_num.tag != 0)			buf = write(buf, r.sar_msg_ref_num, L);
			if (r.sar_total_segments.tag != 0)		buf = write(buf, r.sar_total_segments, L);
			if (r.sar_segment_seqnum.tag != 0)		buf = write(buf, r.sar_segment_seqnum, L);
			if (r.more_messages_to_send.tag != 0)	buf = write(buf, r.more_messages_to_send, L);
			if (r.payload_type.tag != 0)			buf = write(buf, r.payload_type, L);
			if (r.message_payload.tag != 0)			buf = write(buf, r.message_payload.tag, L);
			if (r.privacy_indicator.tag != 0)		buf = write(buf, r.privacy_indicator, L);
			if (r.callback_num.tag != 0)			buf = write(buf, r.callback_num, L);
			if (r.callback_num_pres_ind.tag != 0)	buf = write(buf, r.callback_num_pres_ind, L);
			if (r.callback_num_atag.tag != 0)		buf = write(buf, r.callback_num_atag, L);
			if (r.source_subaddress.tag != 0)		buf = write(buf, r.source_subaddress, L);
			if (r.user_response_code.tag != 0)		buf = write(buf, r.user_response_code, L);
			if (r.display_time.tag != 0)			buf = write(buf, r.display_time, L);
			if (r.sms_signal.tag != 0)				buf = write(buf, r.sms_signal, L);
			if (r.ms_validity.tag != 0)				buf = write(buf, r.ms_validity, L);
			if (r.ms_msg_wait_facilities.tag != 0)	buf = write(buf, r.ms_msg_wait_facilities, L);
			if (r.number_of_messages.tag != 0)		buf =write(buf, r.number_of_messages, L);
			if (r.alert_on_message_delivery.tag!=0)	buf = write(buf, r.alert_on_message_delivery, L);
			if (r.language_indicator.tag != 0)		buf = write(buf, r.language_indicator, L);
			if (r.its_reply_type.tag != 0)			buf = write(buf, r.its_reply_type, L);
			if (r.its_session_info.tag != 0)		buf = write(buf, r.its_session_info, L);
			if (r.ussd_service_op.tag != 0)			buf = write(buf, r.ussd_service_op, L);
		}

		template <class LogT>
		void parse(submit_sm_r & r, const proto::u8_t * buf 
				, const proto::u8_t * bend, LogT & L) {
			using namespace utl;

			RETURN_NULL_IF(buf + sizeof (r.command) >= bend);
			buf = parse(r.command, buf, L);

			RETURN_NULL_IF(buf + sizeof (r.msg_id) > bend);
			buf = p::scpyl(r.msg_id, buf, bend, sizeof(r.msg_id), r.msg_id_len);
		}

		template <class LogT>
		void write(proto::u8_t * buf, const submit_sm_r & r, LogT & L) {
			using namespace utl;
			buf = write(buf, r.command, L);
			buf = w::scpy(buf, r.msg_id, r.msg_id_len + 1);
		}

		/* SUBMIT_MULTI P&W */
		template <class LogT>
		void parse(submit_multi & r, const proto::u8_t * buf
				, const proto::u8_t * bend, LogT & L) {

			using namespace utl;

			proto::u16_t optid;
			buf = ascbuf(buf);

			RETURN_NULL_IF(buf + sizeof (r.command) >= bend);
			buf = parse(r.command, buf, L);

			RETURN_NULL_IF(buf + sizeof (r.service_type) >= bend);
			buf = p::scpyl(r.service_type, buf, bend
					, sizeof (r.service_type), r.service_type_len);

			RETURN_NULL_IF(buf + sizeof (r.src_addr_ton) >= bend);
			buf = p::cp_u8(&r.src_addr_ton, buf);

			RETURN_NULL_IF(buf + sizeof (r.src_addr_npi) >= bend);
			buf = p::cp_u8(&r.src_addr_npi, buf);

			RETURN_NULL_IF(buf + sizeof (r.src_addr) >= bend);
			buf = p::scpyl(r.src_addr, buf, bend
					, sizeof (r.src_addr), r.src_addr_len);

			RETURN_NULL_IF(buf + sizeof (r.number_of_dests) >= bend);
			buf = p::cp_u8(&r.number_of_dests, buf);

			RETURN_NULL_IF(buf + sizeof (r.esm_class) >= bend);
			buf = p::cp_u8(&r.esm_class, buf);

			RETURN_NULL_IF(buf + sizeof (r.protocol_id) >= bend);
			buf = p::cp_u8(&r.protocol_id, buf);

			RETURN_NULL_IF(buf + sizeof (r.priority_flag) >= bend);
			buf = p::cp_u8(&r.priority_flag, buf);

			RETURN_NULL_IF(buf + sizeof (r.schedule_delivery_time) >= bend);
			buf = p::scpyf(r.schedule_delivery_time, buf
					, sizeof (r.schedule_delivery_time));

			RETURN_NULL_IF(buf + sizeof (r.validity_period) >= bend);
			buf = p::scpyf(r.validity_period, buf
					, sizeof (r.validity_period));

			RETURN_NULL_IF(buf + sizeof (r.registered_delivery) >= bend);
			buf = p::cp_u8(&r.registered_delivery, buf);

			RETURN_NULL_IF(buf + sizeof (r.replace_if_present_flag) >= bend);
			buf = p::cp_u8(&r.replace_if_present_flag, buf);

			RETURN_NULL_IF(buf + sizeof (r.data_coding) >= bend);
			buf = p::cp_u8(&r.data_coding, buf);

			RETURN_NULL_IF(buf + sizeof (r.sm_default_msg_id) >= bend);
			buf = p::cp_u8(&r.sm_default_msg_id, buf);

			RETURN_NULL_IF(buf + sizeof (r.sm_length) >= bend);
			buf = p::cp_u8(&r.sm_length, buf);

			RETURN_NULL_IF(buf + sizeof (r.short_message) >= bend);
			buf = p::scpyl(r.short_message, buf, bend
					, sizeof (r.short_message), r.short_message_len);

			buf = p::cp_u16(asbuf(optid), buf);
			/*
			buf += sizeof(proto::u16_t);
			*/

			while (buf < bend) {
				switch (optid) {
					case option::user_message_reference:
						buf = parse(r.user_message_reference, buf, L); break;
					case option::source_port:
						buf = parse(r.source_port, buf, L); break;
					case option::src_addr_subunit:
						buf = parse(r.src_addr_subunit, buf, L); break;
					case option::dest_port:
						buf = parse(r.dest_port, buf, L); break;
					case option::dest_addr_subunit:
						buf = parse(r.dest_addr_subunit, buf, L); break;
					case option::sar_msg_ref_num:
						buf = parse(r.sar_msg_ref_num, buf, L); break;
					case option::sar_total_segments:
						buf = parse(r.sar_total_segments, buf, L); break;
					case option::sar_segment_seqnum:
						buf = parse(r.sar_segment_seqnum, buf, L); break;
					case option::payload_type:
						buf = parse(r.payload_type, buf, L); break;
					/* TODO: Parse message_payload*/
					/*
					case option::message_payload:
						buf = parse(r.message_payload, buf, L);
						break;
					*/
					case option::privacy_indicator:
						buf = parse(r.privacy_indicator, buf, L); break;
					case option::callback_num:
						buf = parse(r.callback_num, buf, L); break;
					case option::callback_num_pres_ind:
						buf = parse(r.callback_num_pres_ind, buf, L); break;
					case option::callback_num_atag:
						buf = parse(r.callback_num_atag, buf, L); break;
					case option::source_subaddress:
						buf = parse(r.source_subaddress, buf, L); break;
					case option::dest_subaddress:
						buf = parse(r.dest_subaddress, buf, L); break;
					case option::display_time:
						buf = parse(r.display_time, buf, L); break;
					case option::sms_signal:
						buf = parse(r.sms_signal, buf, L); break;
					case option::ms_validity:
						buf = parse(r.ms_validity, buf, L); break;
					case option::ms_msg_wait_facilities:
						buf = parse(r.ms_msg_wait_facilities, buf, L); break;
					case option::alert_on_message_delivery:
						buf = parse(r.alert_on_message_delivery, buf, L); break;
					case option::language_indicator:
						buf = parse(r.language_indicator, buf, L); break;
					default: return;
				}
				RETURN_NULL_IF(buf + sizeof (optid) > bend);
				buf = p::cp_u16(asbuf(optid), buf);
			}
		}

		template <class LogT>
		void write(proto::u8_t * buf, const submit_multi & r, LogT & L) {
			using namespace utl;
			buf = write(buf, r.command, L);
			buf = w::scpy(buf, r.service_type, 6);
			
			buf = w::cp_u8(buf, r.src_addr_ton);
			buf = w::cp_u8(buf, r.src_addr_npi);
			buf = w::scpy(buf, r.src_addr, 21);
			buf = w::cp_u8(buf, r.number_of_dests);
			/*
			TODO dest_address(es)
			*/
			buf = w::cp_u8(buf, r.esm_class);
			buf = w::cp_u8(buf, r.protocol_id);
			buf = w::cp_u8(buf, r.priority_flag);
			buf = w::scpy(buf, r.schedule_delivery_time, 17);
			buf = w::scpy(buf, r.validity_period, 17);
			buf = w::cp_u8(buf, r.registered_delivery);
			buf = w::cp_u8(buf, r.replace_if_present_flag);
			buf = w::cp_u8(buf, r.data_coding);
			buf = w::cp_u8(buf, r.sm_default_msg_id);
			buf = w::cp_u8(buf, r.sm_length);
			buf = w::scpy(buf, r.short_message, 254);

			if (r.user_message_reference.tag != 0)	buf = write(buf, r.user_message_reference, L);
			if (r.source_port.tag != 0)				buf = write(buf, r.source_port, L);
			if (r.src_addr_subunit.tag != 0)		buf = write(buf, r.src_addr_subunit, L);
			if (r.dest_port.tag != 0)				buf = write(buf, r.dest_port.tag, L);
			if (r.dest_addr_subunit.tag != 0)		buf = write(buf, r.dest_addr_subunit, L);
			if (r.sar_msg_ref_num.tag != 0)			buf = write(buf, r.sar_msg_ref_num, L);
			if (r.sar_total_segments.tag != 0)		buf = write(buf, r.sar_total_segments, L);
			if (r.sar_segment_seqnum.tag != 0)		buf = write(buf, r.sar_segment_seqnum, L);
			if (r.payload_type.tag != 0)			buf = write(buf, r.payload_type, L);
			if (r.message_payload.tag != 0)			buf = write(buf, r.message_payload.tag, L);
			if (r.privacy_indicator.tag != 0)		buf = write(buf, r.privacy_indicator, L);
			if (r.callback_num.tag != 0)			buf = write(buf, r.callback_num, L);
			if (r.callback_num_pres_ind.tag != 0)	buf = write(buf, r.callback_num_pres_ind, L);
			if (r.callback_num_atag.tag != 0)		buf = write(buf, r.callback_num_atag, L);
			if (r.source_subaddress.tag != 0)		buf = write(buf, r.source_subaddress, L);
			if (r.dest_subaddress.tag != 0)			buf = write(buf, r.dest_subaddress, L);
			if (r.display_time.tag != 0)			buf = write(buf, r.display_time, L);
			if (r.sms_signal.tag != 0)				buf = write(buf, r.sms_signal, L);
			if (r.ms_validity.tag != 0)				buf = write(buf, r.ms_validity, L);
			if (r.ms_msg_wait_facilities.tag != 0)	buf = write(buf, r.ms_msg_wait_facilities, L);
			if (r.alert_on_message_delivery.tag!=0)	buf = write(buf, r.alert_on_message_delivery, L);
			if (r.language_indicator.tag != 0)		buf = write(buf, r.language_indicator, L);
		}

		/* DEST_ADDRESS P&W */
		template <class LogT>
		void parse(dest_address & r, const proto::u8_t * buf
				, const proto::u8_t * bend, LogT & L) {
			using namespace utl;

			RETURN_NULL_IF(buf + sizeof (r.dest_flag) >= bend);
			buf = p::cp_u8(&r.dest_flag, buf);
			/* TODO: 4.5.1.1 SME_Address */
		}

		template <class LogT>
		void write(proto::u8_t * buf, const dest_address & r, LogT & L) {
			using namespace utl;
			buf = w::cp_u8(buf, &r.dest_flag);
			/* TODO: 4.5.1.1 SME_Address */
		}

		/* SME_DEST_ADDRESS P&W */
		template <class LogT>
		void parse(SME_dest_address & r, const proto::u8_t * buf
				, const proto::u8_t * bend, LogT & L) {
			using namespace utl;
			RETURN_NULL_IF(buf + sizeof (r.dest_addr_ton) >= bend);
			buf = p::cp_u8(&r.dest_addr_ton, buf);
			RETURN_NULL_IF(buf + sizeof (r.dest_addr_npi) >= bend);
			buf = p::cp_u8(&r.dest_addr_npi, buf);
			RETURN_NULL_IF(buf + sizeof (r.dest_addr) > bend);
			buf = p::scpyf(r.dest_addr, buf, sizeof (r.dest_addr));
		}

		template <class LogT>
		void write(proto::u8_t * buf, const SME_dest_address & r, LogT & L) {
			using namespace utl;
			buf = w::cp_u8(buf, &r.dest_addr_ton);
			buf = w::cp_u8(buf, &r.dest_addr_npi);
			buf = w::scpy(buf, r.dest_addr, sizeof (r.dest_addr));
		}

		/* DL_NAME P&W */
		template <class LogT>
		void parse(DL_name & r, const proto::u8_t * buf
				, const proto::u8_t * bend, LogT & L) {
			using namespace utl;
			RETURN_NULL_IF((buf + sizeof (r.dl_name) > bend)
					&& (buf+1 != bend));
			buf = p::scpyf(r.dl_name, buf, sizeof (r.dl_name));
		}

		template <class LogT>
		void write(proto::u8_t * buf, const DL_name & r, LogT & L) {
			using namespace utl;
			buf = p::scpyf(buf, r.dl_name, sizeof (r.dl_name));
		}

		/* SUBMIT_MULTI_R P&W */
		template <class LogT>
		void parse(submit_multi_r & r, const proto::u8_t * buf
				, const proto::u8_t * bend, LogT & L) {
			using namespace utl;
			buf = ascbuf(buf);

			RETURN_NULL_IF(buf + sizeof (r.command) >= bend);
			buf = parse(r.command, buf, L);

			RETURN_NULL_IF(!(buf = p::scpyl(r.msg_id, buf, bend
						, sizeof (r.msg_id), r.msg_id_len)));

			RETURN_NULL_IF(buf + sizeof (r.no_unsuccess) >= bend);
			buf = p::cp_u8(&r.no_unsuccess, buf);

			/* TODO unsuccess_sme(s) */
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
		std::basic_ostream< CharT, TraitsT >&
		operator<<(std::basic_ostream< CharT, TraitsT >& L
				, const submit_sm & r) {
			L	<< "submit_sm: " << " [service_type:" << r.service_type << "]"

				<< " [src_addr_ton:" << std::bitset<8>(r.src_addr_ton) << "]"

				<< " [src_addr_npi:" << std::bitset<8>(r.src_addr_npi) << "]"

				<< " [src_addr:" << r.src_addr << "]"

				<< " [dst_addr_ton:" << std::bitset<8>(r.dst_addr_ton) << "]"

				<< " [dst_addr_npi:" << std::bitset<8>(r.dst_addr_npi) << "]"

				<< " [dst_addr:" << r.dst_addr << "]"

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

				<< " [short_message:" << std::string(r.short_message, r.short_message + r.sm_len) << "]" ;

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

			if (r.src_addr_subunit.tag != 0) {
				L << " [src_addr_subunit:"
					<< r.src_addr_subunit
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
