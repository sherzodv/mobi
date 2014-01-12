#ifndef smpp_proto_hpp
#define smpp_proto_hpp

#define RETURN_NULL_IF(a) if ((a)) { return nullptr; }

#include <bitset>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <ostream>
#include <arpa/inet.h>
#include <toolbox/bin.hpp>

namespace mobi { namespace net { namespace smpp {

	using namespace toolbox;

	/* SMPP 3.4 COMMAND IDS */
	namespace command { enum id: bin::u32_t {
			null				= 0x00000000,
			generic_nack		= 0x80000000,
			bind_receiver		= 0x00000001,
			bind_receiver_r		= 0x80000001,
			bind_transmitter	= 0x00000002,
			bind_transmitter_r	= 0x80000002,
			query_sm			= 0x00000003,
			query_sm_r			= 0x80000003,
			submit_sm			= 0x00000004,
			submit_sm_r			= 0x80000004,
			deliver_sm			= 0x00000005,
			deliver_sm_r		= 0x80000005,
			unbind				= 0x00000006,
			unbind_r			= 0x80000006,
			replace_sm			= 0x00000007,
			replace_sm_r		= 0x80000007,
			cancel_sm			= 0x00000008,
			cancel_sm_r			= 0x80000008,
			bind_transceiver	= 0x00000009,
			bind_transceiver_r	= 0x80000009,
			outbind				= 0x0000000B,
			enquire_link		= 0x00000015,
			enquire_link_r		= 0x80000015,
			submit_multi_sm		= 0x00000021,
			submit_multi_sm_r	= 0x80000021,
			alert_notification	= 0x00000102,
			data_sm				= 0x00000103,
			data_sm_r			= 0x80000103,
		}; }

	/* SMPP 3.4 COMMAND STATUS/ERROR CODES */
	/* TODO modify to enum */
	namespace command_status {
		const bin::u32_t esme_rok				= 0x00000000;
		const bin::u32_t esme_rinvmsglen		= 0x00000001;
		const bin::u32_t esme_rinvcmdlen		= 0x00000002;
		const bin::u32_t esme_rinvcmdid			= 0x00000003;
		const bin::u32_t esme_rinvbndsts		= 0x00000004;
		const bin::u32_t esme_ralybnd			= 0x00000005;
		const bin::u32_t esme_rinvprtflg		= 0x00000006;
		const bin::u32_t esme_rinvregdlvflg		= 0x00000007;
		const bin::u32_t esme_rsyserr			= 0x00000008;
		const bin::u32_t esme_rinvsrcadr		= 0x0000000A;
		const bin::u32_t esme_rinvdstadr		= 0x0000000B;
		const bin::u32_t esme_rinvmsgid			= 0x0000000C;
		const bin::u32_t esme_rbindfail			= 0x0000000D;
		const bin::u32_t esme_rinvpaswd			= 0x0000000E;
		const bin::u32_t esme_rinvsysid			= 0x0000000F;
		const bin::u32_t esme_rcancelfail		= 0x00000011;
		const bin::u32_t esme_rreplacefail		= 0x00000013;
		const bin::u32_t esme_rmsgqful			= 0x00000014;
		const bin::u32_t esme_rinvsertyp		= 0x00000015;
		const bin::u32_t esme_rinvnumdsts		= 0x00000033;
		const bin::u32_t esme_rinvdlname		= 0x00000034;
		const bin::u32_t esme_rinvdstflag		= 0x00000040;
		const bin::u32_t esme_rinvsubrep		= 0x00000042;
		const bin::u32_t esme_rinvesmclass		= 0x00000043;
		const bin::u32_t esme_rcntsubdl			= 0x00000044;
		const bin::u32_t esme_rsubmitfail		= 0x00000045;
		const bin::u32_t esme_rinvsrcton		= 0x00000048;
		const bin::u32_t esme_rinvsrcnpi		= 0x00000049;
		const bin::u32_t esme_rinvdstton		= 0x00000050;
		const bin::u32_t esme_rinvdstnpi		= 0x00000051;
		const bin::u32_t esme_rinvsystyp		= 0x00000053;
		const bin::u32_t esme_rinvrepflag		= 0x00000054;
		const bin::u32_t esme_rinvnummsgs		= 0x00000055;
		const bin::u32_t esme_rthrottled		= 0x00000058;
		const bin::u32_t esme_rinvsched			= 0x00000061;
		const bin::u32_t esme_rinvexpiry		= 0x00000062;
		const bin::u32_t esme_rinvdftmsgid		= 0x00000063;
		const bin::u32_t esme_rx_t_appn			= 0x00000064;
		const bin::u32_t esme_rx_p_appn			= 0x00000065;
		const bin::u32_t esme_rx_r_appn			= 0x00000066;
		const bin::u32_t esme_rqueryfail		= 0x00000067;
		const bin::u32_t esme_rinvoptparstream	= 0x000000C0;
		const bin::u32_t esme_roptparnotallwd	= 0x000000C1;
		const bin::u32_t esme_rinvparlen		= 0x000000C2;
		const bin::u32_t esme_rmissingoptparam	= 0x000000C3;
		const bin::u32_t esme_rinvoptparamval	= 0x000000C4;
		const bin::u32_t esme_rdeliveryfailure	= 0x000000FE;
		const bin::u32_t esme_runknownerr		= 0x000000FF;
	}

	/* SMPP 3.4 TLV IDS */
	namespace option {
		const bin::u16_t dst_addr_subunit					= 0x0005;
		const bin::u16_t dst_network_type					= 0x0006;
		const bin::u16_t dst_bearer_type					= 0x0007;
		const bin::u16_t dst_telematics_id					= 0x0008;
		const bin::u16_t src_addr_subunit					= 0x000D;
		const bin::u16_t src_network_type					= 0x000E;
		const bin::u16_t src_bearer_type					= 0x000F;
		const bin::u16_t src_telematics_id					= 0x0010;
		const bin::u16_t qos_time_to_live					= 0x0017;
		const bin::u16_t payload_type						= 0x0019;
		const bin::u16_t additional_status_info_text		= 0x0019;
		const bin::u16_t receipted_msg_id					= 0x001E;
		const bin::u16_t ms_msg_wait_fclts					= 0x0030;
		const bin::u16_t privacy_ind						= 0x0201;
		const bin::u16_t src_subaddr						= 0x0202;
		const bin::u16_t dst_subaddr						= 0x0203;
		const bin::u16_t user_msg_reference					= 0x0204;
		const bin::u16_t user_resp_code						= 0x0205;
		const bin::u16_t src_port							= 0x020A;
		const bin::u16_t dst_port							= 0x020B;
		const bin::u16_t sar_msg_ref_num					= 0x020C;
		const bin::u16_t lang_ind							= 0x020D;
		const bin::u16_t sar_total_segments					= 0x020E;
		const bin::u16_t sar_segment_seqnum					= 0x020F;
		const bin::u16_t sc_interface_version				= 0x0210;
		const bin::u16_t callback_num_pres_ind				= 0x0302;
		const bin::u16_t callback_num_atag					= 0x0303;
		const bin::u16_t number_of_msgs						= 0x0304;
		const bin::u16_t callback_num						= 0x0381;
		const bin::u16_t dpf_result							= 0x0420;
		const bin::u16_t set_dpf							= 0x0421;
		const bin::u16_t ms_availability_status				= 0x0422;
		const bin::u16_t network_error_code					= 0x0423;
		const bin::u16_t msg_payload						= 0x0424;
		const bin::u16_t delivery_failure_reason			= 0x0425;
		const bin::u16_t more_msgs_to_send					= 0x0426;
		const bin::u16_t msg_state							= 0x0427;
		const bin::u16_t ussd_serv_op						= 0x0501;
		const bin::u16_t display_time						= 0x1201;
		const bin::u16_t sms_signal							= 0x1203;
		const bin::u16_t ms_validity						= 0x1204;
		const bin::u16_t alert_on_msg_delivery				= 0x130C;
		const bin::u16_t its_reply_type						= 0x1380;
		const bin::u16_t its_session_info					= 0x1383;
	}

	/* SMPP 3.4 TON VALUES */
	namespace ton {
		bin::u32_t unknown				= 0x00000000;
		bin::u32_t international		= 0x00000001;
		bin::u32_t national				= 0x00000010;
		bin::u32_t network_specific		= 0x00000011;
		bin::u32_t subscriber_number	= 0x00000100;
		bin::u32_t alphanumeric			= 0x00000101;
		bin::u32_t abbreviated			= 0x00000110;
	}

	/* SMPP 3.4 NPI VALUES */
	namespace npi {
		bin::u32_t unknown				= 0x00000000;
		bin::u32_t isdn					= 0x00000001;
		bin::u32_t data					= 0x00000011;
		bin::u32_t telex				= 0x00000100;
		bin::u32_t land_mobile			= 0x00000110;
		bin::u32_t national				= 0x00001000;
		bin::u32_t private_				= 0x00001001;
		bin::u32_t ermes				= 0x00001010;
		bin::u32_t internet				= 0x00001110;
		bin::u32_t wap_client_id		= 0x00010010;
	}

	/* SMPP 3.4 SERVICE TYPES */
	namespace serv_types {
		const bin::u8_t *	default_				= NULL;
		const bin::u8_t		cell_msging[]			= "CMT";
		const bin::u8_t		cell_paging[]			= "CPT";
		const bin::u8_t		voice_mail_notif[]		= "VMN";
		const bin::u8_t		voice_mail_alert[]		= "VMA";
		const bin::u8_t		wireless_app_protocol[]	= "WAP";
		const bin::u8_t		unstruct_sup_serv_dat[]	= "USSD";
	}

	/* SMPP 3.4 esm class consts */
	namespace esm_c {

		namespace submit_sm {
			/* messaging mode, you should use switch (var&0x03) */
			const bin::u8_t default_sms_mode		= 0x00;
			const bin::u8_t datagram_mode			= 0x01;
			const bin::u8_t forward_mode			= 0x02;
			const bin::u8_t store_n_forward_mode	= 0x03;

			/* msg type, you should use switch (var&0x3C) */
			const bin::u8_t default_msg_type		= 0x00;
			const bin::u8_t esme_delivery_ack		= 0x08;
			const bin::u8_t esme_manual_ack			= 0x10;

			/* gsm network specific features, your should use switch (var&0xC0)*/
			const bin::u8_t no_spec_features_sel	= 0x00;
			const bin::u8_t udhi_ind				= 0x40;
			const bin::u8_t set_reply_path			= 0x80;
			const bin::u8_t set_udhi_reply_path		= 0xC0;
		}

		namespace submit_multi {
			/* messaging mode, you should use switch (var&0x03) */
			const bin::u8_t default_sms_mode		= 0x00;
			const bin::u8_t datagram_mode			= 0x01;
			const bin::u8_t forward_mode			= 0x02;
			const bin::u8_t store_n_forward_mode	= 0x03;

			/* msg type, you should use switch (var&0x3C) */
			const bin::u8_t default_msg_type		= 0x00;
			const bin::u8_t esme_delivery_ack		= 0x08;
			const bin::u8_t esme_manual_ack			= 0x10;

			/* gsm network specific features, your should use switch (var&0xC0)*/
			const bin::u8_t no_spec_features_sel	= 0x00;
			const bin::u8_t udhi_ind				= 0x40;
			const bin::u8_t set_reply_path			= 0x80;
			const bin::u8_t set_udhi_reply_path		= 0xC0;
		}

		namespace data_sm {
			const bin::u8_t not_applicable			= 0x00;

			/* msg type, you should use switch (var & 0x3C) */
			const bin::u8_t default_msg_typ			= 0x00;
			const bin::u8_t smsc_delivery_rec		= 0x04;
			const bin::u8_t sme_delivery_ack		= 0x08;
			const bin::u8_t sme_manual_ack			= 0x10;
			const bin::u8_t conv_abort				= 0x18;
			const bin::u8_t inter_delivery_notf		= 0x20;

			/* gsm network specific features, 0xC0 */
			const bin::u8_t no_spec_select			= 0x00;
			const bin::u8_t udhi_ind_set			= 0x40;
			const bin::u8_t reply_path				= 0x80;
			const bin::u8_t udhi_n_reply_path		= 0xC0;
			/* messaging mode, you should use switch (var&0x03) */
			const bin::u8_t default_sms_mode		= 0x00;
			const bin::u8_t datagram_mode			= 0x01;
			const bin::u8_t forward_mode			= 0x02;
			const bin::u8_t store_n_forward_mode	= 0x03;

			/* msg type, you should use switch (var&0x3C) */
			const bin::u8_t default_msg_type		= 0x00;
			const bin::u8_t esme_delivery_ack		= 0x08;
			const bin::u8_t esme_manual_ack			= 0x10;

			/* gsm network specific features, your should use switch (var&0xC0)*/
			const bin::u8_t no_spec_features_sel	= 0x00;
			const bin::u8_t udhi_ind				= 0x40;
			const bin::u8_t set_reply_path			= 0x80;
			const bin::u8_t set_udhi_reply_path		= 0xC0;
		}

		namespace deliver_sm {
			const bin::u8_t not_applicable			= 0x00;

			/* msg type, you should use switch (var & 0x3C) */
			const bin::u8_t default_msg_typ			= 0x00;
			const bin::u8_t smsc_delivery_rec		= 0x04;
			const bin::u8_t sme_delivery_ack		= 0x08;
			const bin::u8_t sme_manual_ack			= 0x10;
			const bin::u8_t conv_abort				= 0x18;
			const bin::u8_t inter_delivery_notf		= 0x20;

			/* gsm network specific features, 0xC0 */
			const bin::u8_t no_spec_select			= 0x00;
			const bin::u8_t udhi_ind_set			= 0x40;
			const bin::u8_t reply_path				= 0x80;
			const bin::u8_t udhi_n_reply_path		= 0xC0;
		}
	}

	/* SMPP 3.4 PDU header */
	struct pdu {
		bin::u32_t		len;
		command::id		id;
		bin::u32_t		status;
		bin::u32_t 		seqno;

		pdu(): len(0)
			, id(command::null)
			, status(0)
			, seqno(0)
		{}

		pdu(command::id cid): len(0)
			, id(cid)
			, status(0)
			, seqno(0)
		{}
	};

	/* SMPP 3.4 generic tag-len-value type */
	template <typename ValueT>
	struct tlv {
		bin::u16_t tag;
		bin::u16_t len;
		ValueT val;
		tlv(): tag(0), len(0) {}
	};

	/* SMPP 3.4 TLV specializations */
	namespace {
		struct tlv_dst_addr_subunit: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_dst_addr_subunit(): base() {}

			void set(bin::u8_t v) {
				tag = option::dst_addr_subunit;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_src_addr_subunit: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_src_addr_subunit(): base() {}

			void set(bin::u8_t v) {
				tag = option::src_addr_subunit;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_dst_network_type: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_dst_network_type(): base() {}

			void set(bin::u8_t v) {
				tag = option::dst_network_type;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_src_network_type: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_src_network_type(): base() {}

			void set(bin::u8_t v) {
				tag = option::src_network_type;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_dst_bearer_type: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_dst_bearer_type(): base() {}

			void set(bin::u8_t v) {
				tag = option::dst_bearer_type;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_src_bearer_type: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_src_bearer_type(): base() {}

			void set(bin::u8_t v) {
				tag = option::src_bearer_type;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_dst_telematics_id: tlv<bin::u16_t> {
			typedef tlv<bin::u16_t> base;
			tlv_dst_telematics_id(): base() {}

			void set(bin::u16_t v) {
				tag = option::dst_telematics_id;
				len = sizeof(bin::u16_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_src_telematics_id: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_src_telematics_id(): base() {}

			void set(bin::u8_t v) {
				tag = option::src_telematics_id;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_qos_time_to_live: tlv<bin::u32_t> {
			typedef tlv<bin::u32_t> base;
			tlv_qos_time_to_live(): base() {}

			void set(bin::u32_t v) {
				tag = option::qos_time_to_live;
				len = sizeof(bin::u32_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_payload_type: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_payload_type(): base() {}

			void set(bin::u8_t v) {
				tag = option::payload_type;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_additional_status_info_text: tlv<bin::u8_t[256]> {
			typedef tlv<bin::u8_t[256]>	base;
			tlv_additional_status_info_text(): base() {}

			void set(const bin::u8_t * v, bin::sz_t l) {
				tag = option::payload_type;
				len = std::min(sizeof(val), l);
				std::memcpy(val, v, len);
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_receipted_msg_id: tlv<bin::u8_t[65]> {
			typedef tlv<bin::u8_t[65]> base;
			tlv_receipted_msg_id(): base() {}

			void set(const bin::u8_t * v, bin::sz_t l) {
				tag = option::receipted_msg_id;
				len = std::min(sizeof(val), l);
				std::memcpy(val, v, len);
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_ms_msg_wait_fclts: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_ms_msg_wait_fclts(): base() {}

			void set(bin::u8_t v) {
				tag = option::ms_msg_wait_fclts;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_privacy_ind: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_privacy_ind(): base() {}

			void set(bin::u8_t v) {
				tag = option::privacy_ind;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_src_subaddr: tlv<bin::u8_t[23]> {
			typedef tlv<bin::u8_t[23]> base;
			tlv_src_subaddr(): base() {}

			void set(const bin::u8_t * v, bin::sz_t l) {
				tag = option::src_subaddr;
				len = std::min(sizeof(val), l);
				std::memcpy(val, v, len);
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_dst_subaddr: tlv<bin::u8_t[23]> {
			typedef tlv<bin::u8_t[23]> base;
			tlv_dst_subaddr(): base() {}

			void set(const bin::u8_t * v, bin::sz_t l) {
				tag = option::dst_subaddr;
				len = std::min(sizeof(val), l);
				std::memcpy(val, v, len);
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_user_msg_reference: tlv<bin::u16_t> {
			typedef tlv<bin::u16_t> base;
			tlv_user_msg_reference(): base() {}

			void set(bin::u16_t v) {
				tag = option::user_msg_reference;
				len = sizeof(bin::u16_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_user_resp_code: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_user_resp_code(): base() {}

			void set(bin::u8_t v) {
				tag = option::user_resp_code;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_lang_ind: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_lang_ind(): base() {}

			void set(bin::u8_t v) {
				tag = option::lang_ind;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_src_port: tlv<bin::u16_t> {
			typedef tlv<bin::u16_t> base;
			tlv_src_port(): base() {}

			void set(bin::u16_t v) {
				tag = option::src_port;
				len = sizeof(bin::u16_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_dst_port: tlv<bin::u16_t> {
			typedef tlv<bin::u16_t> base;
			tlv_dst_port(): base() {}

			void set(bin::u16_t v) {
				tag = option::dst_port;
				len = sizeof(bin::u16_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_sar_msg_ref_num: tlv<bin::u16_t> {
			typedef tlv<bin::u16_t> base;
			tlv_sar_msg_ref_num(): base() {}

			void set(bin::u16_t v) {
				tag = option::sar_msg_ref_num;
				len = sizeof(bin::u16_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_sar_total_segments: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_sar_total_segments(): base() {}

			void set(bin::u8_t v) {
				tag = option::sar_total_segments;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_sar_segment_seqnum: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_sar_segment_seqnum(): base() {}

			void set(bin::u8_t v) {
				tag = option::sar_segment_seqnum;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_sc_interface_version: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_sc_interface_version(): base() {}

			void set(bin::u8_t v) {
				tag = option::sc_interface_version;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_display_time: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_display_time(): base() {}

			void set(bin::u8_t v) {
				tag = option::display_time;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_ms_validity: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_ms_validity(): base() {}

			void set(bin::u8_t v) {
				tag = option::ms_validity;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_dpf_result: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_dpf_result(): base() {}

			void set(bin::u8_t v) {
				tag = option::dpf_result;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_set_dpf: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_set_dpf(): base() {}

			void set(bin::u8_t v) {
				tag = option::set_dpf;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_ms_availability_status: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_ms_availability_status(): base() {}

			void set(bin::u8_t v) {
				tag = option::ms_availability_status;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_network_error_code: tlv<bin::u8_t[3]> {
			typedef tlv<bin::u8_t[3]> base;
			tlv_network_error_code(): base() {}

			void set(const bin::u8_t * v, bin::sz_t l) {
				tag = option::network_error_code;
				len = std::min(sizeof(val), l);
				std::memcpy(val, v, len);
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_msg_payload: tlv<bin::u8_t *> {
			typedef tlv<bin::u8_t *> base;
			tlv_msg_payload(): base() {}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_delivery_failure_reason: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_delivery_failure_reason(): base() {}

			void set(bin::u8_t v) {
				tag = option::delivery_failure_reason;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_more_msgs_to_send: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_more_msgs_to_send(): base() {}

			void set(bin::u8_t v) {
				tag = option::more_msgs_to_send;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_msg_state: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_msg_state(): base() {}

			void set(bin::u8_t v) {
				tag = option::msg_state;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_callback_num: tlv<bin::u8_t[19]> {
			typedef tlv<bin::u8_t[19]> base;
			tlv_callback_num(): base() {}

			void set(const bin::u8_t * v, bin::sz_t l) {
				tag = option::callback_num;
				len = std::min(sizeof(val), l);
				std::memcpy(val, v, len);
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_callback_num_pres_ind: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_callback_num_pres_ind(): base() {}

			void set(bin::u8_t v) {
				tag = option::callback_num_pres_ind;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_callback_num_atag: tlv<bin::u8_t[65]> {
			typedef tlv<bin::u8_t[65]> base;
			tlv_callback_num_atag(): base() {}

			void set(const bin::u8_t * v, bin::sz_t l) {
				tag = option::callback_num_atag;
				len = std::min(sizeof(val), l);
				std::memcpy(val, v, len);
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_number_of_msgs: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_number_of_msgs(): base() {}

			void set(bin::u8_t v) {
				tag = option::number_of_msgs;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_sms_signal: tlv<bin::u16_t> {
			typedef tlv<bin::u16_t> base;
			tlv_sms_signal(): base() {}

			void set(bin::u16_t v) {
				tag = option::sms_signal;
				len = sizeof(bin::u16_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_alert_on_msg_delivery: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_alert_on_msg_delivery(): base() {}

			void set(bin::u8_t v) {
				tag = option::alert_on_msg_delivery;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_its_reply_type: tlv<bin::u8_t> {
			typedef tlv<bin::u8_t> base;
			tlv_its_reply_type(): base() {}

			void set(bin::u8_t v) {
				tag = option::its_reply_type;
				len = sizeof(bin::u8_t);
				val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_its_session_info: tlv<bin::u8_t[2]> {
			typedef tlv<bin::u8_t[2]> base;
			tlv_its_session_info(): base() {}

			void set(const bin::u8_t * v, bin::sz_t l) {
				tag = option::its_session_info;
				len = std::min(sizeof(val), l);
				std::memcpy(val, v, len);
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
		struct tlv_ussd_serv_op: tlv<bin::u8_t[1]> {
			typedef tlv<bin::u8_t[1]> base;
			tlv_ussd_serv_op(): base() {}

			void set(bin::u8_t v) {
				tag = option::ussd_serv_op;
				len = sizeof(bin::u8_t);
				*val = v;
			}

			bin::sz_t raw_size() const { return len + sizeof(bin::u16_t)*2; }
		};
	}

	/* SMPP 3.4 OPERATION STRUCTURES */
	namespace {

		/* BIND OPERATION */
		struct bind_transmitter {
			pdu command;
			bin::u8_t sys_id[16];
			bin::u8_t password[9];
			bin::u8_t sys_type[13];
			bin::u8_t interface_version;
			bin::u8_t addr_ton;
			bin::u8_t addr_npi;
			bin::u8_t addr_range[41];

			bind_transmitter(): command(command::bind_transmitter)
				, sys_id_len(0)
				, password_len(0)
				, sys_type_len(0)
				, addr_range_len(0)
			{}

			bin::sz_t raw_size() const {
				return	sizeof(command)
						+ sys_id_len
						+ password_len
						+ sys_type_len
						+ 3
						+ addr_range_len;
			}

			/* this values will not be writed to buffer */
			std::size_t sys_id_len;
			std::size_t password_len;
			std::size_t sys_type_len;
			std::size_t addr_range_len;
		};

		struct bind_transmitter_r {
			pdu command;
			bin::u8_t sys_id[16];
			tlv_sc_interface_version sc_interface_version;

			std::size_t sys_id_len;

			bind_transmitter_r()
				: command(command::bind_transmitter_r)
				, sc_interface_version()
				, sys_id_len(0)
			{}

			bin::sz_t raw_size() const {
				return	sizeof(command)
						+ sys_id_len
						+ (sc_interface_version.tag == option::sc_interface_version
								? sc_interface_version.raw_size() : 0);
			}
		};

		struct bind_receiver {
			pdu command;
			bin::u8_t sys_id[16];
			bin::u8_t password[9];
			bin::u8_t sys_type[13];
			bin::u8_t interface_version;
			bin::u8_t addr_ton;
			bin::u8_t addr_npi;
			bin::u8_t addr_range[41];

			std::size_t sys_id_len;
			std::size_t password_len;
			std::size_t sys_type_len;
			std::size_t addr_range_len;

			bind_receiver()
				: command(command::bind_receiver)
				, sys_id_len(0)
				, password_len(0)
				, sys_type_len(0)
				, addr_range_len(0)
			{}

			bin::sz_t raw_size() const {
				return	sizeof(command)
						+ sys_id_len
						+ password_len
						+ sys_type_len
						+ 3
						+ addr_range_len;
			}
		};

		struct bind_receiver_r {
			pdu command;
			bin::u8_t sys_id[16];
			tlv_sc_interface_version sc_interface_version;

			std::size_t sys_id_len;

			bind_receiver_r()
				: command(command::bind_receiver_r)
				, sc_interface_version()
				, sys_id_len(0)
			{}

			bin::sz_t raw_size() const {
				return	sizeof(command)
						+ sys_id_len
						+ (sc_interface_version.tag == option::sc_interface_version
							? sc_interface_version.raw_size()
							: 0);
			}
		};

		struct bind_transceiver {
			pdu command;
			bin::u8_t sys_id[16];
			bin::u8_t password[9];
			bin::u8_t sys_type[13];
			bin::u8_t interface_version;
			bin::u8_t addr_ton;
			bin::u8_t addr_npi;
			bin::u8_t addr_range[41];

			bind_transceiver()
				: command(command::bind_transceiver)
				, sys_id_len(0)
				, password_len(0)
				, sys_type_len(0)
				, addr_range_len(0)
			{}

			bin::sz_t raw_size() const {
				return	sizeof(command)
						+ sys_id_len
						+ password_len
						+ sys_type_len
						+ 3
						+ addr_range_len;
			}

			std::size_t sys_id_len;
			std::size_t password_len;
			std::size_t sys_type_len;
			std::size_t addr_range_len;
		};

		struct bind_transceiver_r {
			pdu command;
			bin::u8_t sys_id[16];
			tlv_sc_interface_version sc_interface_version;

			bind_transceiver_r()
				: command(command::bind_transceiver_r)
				, sc_interface_version()
				, sys_id_len(0)
			{}

			bin::sz_t raw_size() const {
				return	sizeof(command)
						+ sys_id_len
						+ (sc_interface_version.tag == option::sc_interface_version
							? sc_interface_version.raw_size()
							: 0);
			}

			std::size_t sys_id_len;
		};

		struct outbind {
			pdu command;
			bin::u8_t sys_id[16];
			bin::u8_t password[9];

			outbind()
				: command(command::outbind)
				, sys_id_len(0)
				, password_len(0)
			{}

			bin::sz_t raw_size() const {
				return	sizeof(command)
						+ sys_id_len
						+ password_len;
			}

			std::size_t sys_id_len;
			std::size_t password_len;
	};

		/* BIND OPERATION */

		struct unbind {
			pdu command;

			unbind()
				: command(command::unbind)
			{}

			bin::sz_t raw_size() const {
				return	sizeof(command);
			}
		};

		struct unbind_r {
			pdu command;
			unbind_r()
				: command(command::unbind_r)
			{}
			bin::sz_t raw_size() const {
				return	sizeof(command);
			}
		};

		/* SUBMIT SM OPERATION */

		struct submit_sm {
			pdu command;

			bin::u8_t	serv_type[6];

			/* Address of ESME which originated this msg */
			bin::u8_t src_addr_ton;
			bin::u8_t src_addr_npi;
			bin::u8_t src_addr[21];

			/* Address  ME which originated this msg */
			bin::u8_t dst_addr_ton;
			bin::u8_t dst_addr_npi;
			bin::u8_t dst_addr[21];

			bin::u8_t esm_class;
			bin::u8_t protocol_id;
			bin::u8_t priority_flag;
			bin::u8_t schedule_delivery_time[17];
			bin::u8_t validity_period[17];
			bin::u8_t registered_delivery;
			bin::u8_t replace_if_present_flag;
			bin::u8_t data_coding;
			bin::u8_t sm_default_msg_id;
			bin::u8_t short_msg_len;
			bin::u8_t short_msg[254];

			tlv_user_msg_reference			user_msg_reference;
			tlv_src_port					src_port;
			tlv_src_addr_subunit			src_addr_subunit;
			tlv_dst_port					dst_port;
			tlv_dst_addr_subunit			dst_addr_subunit;
			tlv_sar_msg_ref_num				sar_msg_ref_num;
			tlv_sar_total_segments			sar_total_segments;
			tlv_sar_segment_seqnum			sar_segment_seqnum;
			tlv_more_msgs_to_send			more_msgs_to_send;
			tlv_payload_type				payload_type;
			tlv_msg_payload					msg_payload;
			tlv_privacy_ind					privacy_ind;
			tlv_callback_num				callback_num;
			tlv_callback_num_pres_ind		callback_num_pres_ind;
			tlv_callback_num_atag			callback_num_atag;
			tlv_src_subaddr					src_subaddr;
			tlv_dst_subaddr				dst_subaddr;
			tlv_user_resp_code				user_resp_code;
			tlv_display_time				display_time;
			tlv_sms_signal					sms_signal;
			tlv_ms_validity					ms_validity;
			tlv_ms_msg_wait_fclts			ms_msg_wait_fclts;
			tlv_number_of_msgs				number_of_msgs;
			tlv_alert_on_msg_delivery		alert_on_msg_delivery;
			tlv_lang_ind					lang_ind;
			tlv_its_reply_type				its_reply_type;
			tlv_its_session_info			its_session_info;
			tlv_ussd_serv_op				ussd_serv_op;

			std::size_t serv_type_len;
			std::size_t src_addr_len;
			std::size_t dst_addr_len;
			std::size_t registered_delivery_len;
			std::size_t schedule_delivery_time_len;
			std::size_t validity_period_len;

			submit_sm()
				: command(command::submit_sm)
			    , short_msg_len(0)
				, serv_type_len(0)
				, src_addr_len(0)
				, dst_addr_len(0)
				, registered_delivery_len(0)
			{}

			bin::sz_t raw_size() const {
				bin::sz_t len =
						sizeof(command)
						+ serv_type_len
						+ 2
						+ src_addr_len
						+ 2
						+ dst_addr_len
						+ 3
						+ schedule_delivery_time_len
						+ validity_period_len
						+ 5
						+ short_msg_len
						+ (user_msg_reference.tag == option::user_msg_reference
							? user_msg_reference.raw_size(): 0)
						+ (src_port.tag == option::src_port
							? src_port.raw_size() : 0)
						+ (src_addr_subunit.tag == option::src_addr_subunit
							? src_addr_subunit.raw_size(): 0)
						+ (dst_port.tag == option::dst_port
							? dst_port.raw_size(): 0)
						+ (dst_addr_subunit.tag == option::dst_addr_subunit
							? dst_addr_subunit.raw_size(): 0)
						+ (sar_msg_ref_num.tag == option::sar_msg_ref_num
							? sar_msg_ref_num.raw_size(): 0)
						+ (sar_total_segments.tag == option::sar_total_segments
							? sar_total_segments.raw_size(): 0)
						+ (sar_segment_seqnum.tag == option::sar_segment_seqnum
							? sar_segment_seqnum.raw_size(): 0)
						+ (more_msgs_to_send.tag == option::more_msgs_to_send
							? more_msgs_to_send.raw_size(): 0)
						+ (payload_type.tag == option::payload_type
							? payload_type.raw_size(): 0)
						+ (msg_payload.tag == option::msg_payload
							? payload_type.tag: 0)
						+ (privacy_ind.tag == option::privacy_ind
							? privacy_ind.raw_size(): 0)
						+ (callback_num.tag == option::callback_num
							? callback_num.raw_size(): 0)
						+ (callback_num_pres_ind.tag == option::callback_num_pres_ind
							? callback_num_pres_ind.raw_size(): 0)
						+ (callback_num_atag.tag == option::callback_num_atag
							? callback_num_atag.raw_size(): 0)
						+ (src_subaddr.tag == option::src_subaddr
							? src_subaddr.raw_size(): 0)
						+ (dst_subaddr.tag == option::dst_subaddr
							? dst_subaddr.raw_size(): 0)
						+ (user_resp_code.tag == option::user_resp_code
							? user_resp_code.raw_size(): 0)
						+ (display_time.tag == option::display_time
							? display_time.raw_size(): 0)
						+ (sms_signal.tag == option::sms_signal
							? sms_signal.raw_size(): 0)
						+ (ms_validity.tag == option::ms_validity
							? ms_validity.raw_size(): 0)
						+ (ms_msg_wait_fclts.tag == option::ms_msg_wait_fclts
							? ms_msg_wait_fclts.raw_size(): 0)
						+ (number_of_msgs.tag == option::number_of_msgs
							? number_of_msgs.raw_size(): 0)
						+ (alert_on_msg_delivery.tag == option::alert_on_msg_delivery
							? alert_on_msg_delivery.raw_size(): 0)
						+ (lang_ind.tag == option::lang_ind
							? lang_ind.raw_size(): 0)
						+ (its_reply_type.tag == option::its_reply_type
							? its_reply_type.raw_size(): 0)
						+ (its_session_info.tag == option::its_session_info
							? its_session_info.raw_size(): 0)
						+ (ussd_serv_op.tag == option::ussd_serv_op
							? ussd_serv_op.raw_size(): 0);
				return len;
			}
		};

		struct submit_sm_r {
			pdu command;
			bin::u8_t msg_id[65];

			submit_sm_r()
				: command(command::submit_sm_r)
				, msg_id_len(0)
			{}

			bin::sz_t raw_size() const {
				return	sizeof(command)
						+ msg_id_len;
			}

			std::size_t msg_id_len;
		};

		/* SUBMIT MULTI Operations */

		/* Additions used in submit_multi_sm */
		struct dl_name {
			bin::u8_t	value[21];

			dl_name(): value_len(0) {}
			bin::sz_t raw_size() const { return value_len; }

			std::size_t value_len;
		};
		struct sme_dst_addr {
			bin::u8_t	dst_addr_ton;
			bin::u8_t	dst_addr_npi;
			bin::u8_t	dst_addr[21];

			sme_dst_addr(): dst_addr_len(0) {}
			bin::sz_t raw_size() const { return 2 + dst_addr_len; }

			std::size_t dst_addr_len;
		};
		struct dst_addr {
			bin::u8_t 		dst_flag; /* 1 - sme_addr, 2 - dlist_name */

			bin::sz_t raw_size() const {
				switch (dst_flag) {
					case 1:
						return sme_addr.raw_size();
					case 2:
						return dlist_name.raw_size();
					default:
						return 0; /* error */
				}
			}

			sme_dst_addr	sme_addr;
			dl_name			dlist_name;
		};

		struct submit_multi_sm {
			pdu command;
			bin::u8_t serv_type[6];
			bin::u8_t src_addr_ton;
			bin::u8_t src_addr_npi;
			bin::u8_t src_addr[21];
			bin::u8_t num_of_dsts;
			//dst_addr dst_addr[24];
			bin::u8_t esm_class;
			bin::u8_t protocol_id;
			bin::u8_t priority_flag;
			bin::u8_t schedule_delivery_time[17];
			bin::u8_t validity_period[17];
			bin::u8_t registered_delivery;
			bin::u8_t replace_if_present_flag;
			bin::u8_t data_coding;
			bin::u8_t sm_default_msg_id;
			bin::u8_t short_msg_len;
			bin::u8_t short_msg[254];

			tlv_user_msg_reference		user_msg_reference;
			tlv_src_port				src_port;
			tlv_src_addr_subunit		src_addr_subunit;
			tlv_dst_port				dst_port;
			tlv_dst_addr_subunit		dst_addr_subunit;
			tlv_sar_msg_ref_num			sar_msg_ref_num;
			tlv_sar_total_segments		sar_total_segments;
			tlv_sar_segment_seqnum		sar_segment_seqnum;
			tlv_payload_type			payload_type;
			tlv_msg_payload				msg_payload;
			tlv_privacy_ind				privacy_ind;
			tlv_callback_num			callback_num;
			tlv_callback_num_pres_ind	callback_num_pres_ind;
			tlv_callback_num_atag		callback_num_atag;
			tlv_src_subaddr				src_subaddr;
			tlv_dst_subaddr			dst_subaddr;
			tlv_display_time			display_time;
			tlv_sms_signal				sms_signal;
			tlv_ms_validity				ms_validity;
			tlv_ms_msg_wait_fclts		ms_msg_wait_fclts;
			tlv_alert_on_msg_delivery	alert_on_msg_delivery;
			tlv_lang_ind				lang_ind;

			submit_multi_sm(): command(command::submit_multi_sm) {}

			bin::sz_t raw_size() const {
				bin::sz_t len = sizeof(command)
								+ serv_type_len
								+ 2
								+ src_addr_len
								+ 1;
				//for (int i = 0; i < num_of_dsts; ++i)
					//len += dst_addr[i].raw_size();
				len = len		+ 3
								+ (schedule_delivery_time[0] == '\0' ? 1: sizeof(schedule_delivery_time))
								+ (validity_period[0] == '\0' ? 1: sizeof(validity_period))
								+ 5
								+ short_msg_len
								+ (user_msg_reference.tag == option::user_msg_reference
									? user_msg_reference.raw_size(): 0)
								+ (src_port.tag == option::src_port
									? src_port.raw_size() : 0)
								+ (src_addr_subunit.tag == option::src_addr_subunit
									? src_addr_subunit.raw_size(): 0)
								+ (dst_port.tag == option::dst_port
									? dst_port.raw_size(): 0)
								+ (dst_addr_subunit.tag == option::dst_addr_subunit
									? dst_addr_subunit.raw_size(): 0)
								+ (sar_msg_ref_num.tag == option::sar_msg_ref_num
									? sar_msg_ref_num.raw_size(): 0)
								+ (sar_total_segments.tag == option::sar_total_segments
									? sar_total_segments.raw_size(): 0)
								+ (sar_segment_seqnum.tag == option::sar_segment_seqnum
									? sar_segment_seqnum.raw_size(): 0)
								+ (payload_type.tag == option::payload_type
									? payload_type.raw_size(): 0)
								+ (msg_payload.tag == option::msg_payload
									? payload_type.tag: 0)
								+ (privacy_ind.tag == option::privacy_ind
									? privacy_ind.raw_size(): 0)
								+ (callback_num.tag == option::callback_num
									? callback_num.raw_size(): 0)
								+ (callback_num_pres_ind.tag == option::callback_num_pres_ind
									? callback_num_pres_ind.raw_size(): 0)
								+ (callback_num_atag.tag == option::callback_num_atag
									? callback_num_atag.raw_size(): 0)
								+ (src_subaddr.tag == option::src_subaddr
									? src_subaddr.raw_size(): 0)
								+ (dst_subaddr.tag == option::dst_subaddr
									? dst_subaddr.raw_size(): 0)
								+ (display_time.tag == option::display_time
									? display_time.raw_size(): 0)
								+ (sms_signal.tag == option::sms_signal
									? sms_signal.raw_size(): 0)
								+ (ms_validity.tag == option::ms_validity
									? ms_validity.raw_size(): 0)
								+ (ms_msg_wait_fclts.tag == option::ms_msg_wait_fclts
									? ms_msg_wait_fclts.raw_size(): 0)
								+ (alert_on_msg_delivery.tag == option::alert_on_msg_delivery
									? alert_on_msg_delivery.raw_size(): 0)
								+ (lang_ind.tag == option::lang_ind
									? lang_ind.raw_size(): 0);

					return len;
				}

			std::size_t serv_type_len;
			std::size_t src_addr_len;
		};

		struct submit_multi_r {
			pdu command;

			bin::u8_t	msg_id[65];
			bin::u8_t	no_unsuccess;
			/* TODO: 4.5.2 unsuccess_sme(s) */

			size_t msg_id_len;
			submit_multi_r()
				: command(command::submit_multi_sm_r)
			{}
		};

		struct unsuccess_smes {
			bin::u8_t		dst_addr_ton;
			bin::u8_t		dst_addr_npi;
			bin::u8_t		dst_addr[21];
			bin::u32_t	error_status_code;
		};

		/* DELIVER SM Operations */
		struct deliver_sm {
			pdu command;

			bin::u8_t	serv_type[6];
			bin::u8_t 	src_addr_ton;
			bin::u8_t	src_addr_npi;
			bin::u8_t	src_addr[21];
			bin::u8_t	dst_addr_ton;
			bin::u8_t	dst_addr_npi;
			bin::u8_t 	dst_addr[21];
			bin::u8_t	esm_class;
			bin::u8_t	protocol_id;
			bin::u8_t	priority_flag;
			bin::u8_t	schedule_delivery_time[1];
			bin::u8_t	validity_period[1];
			bin::u8_t 	registered_delivery;
			bin::u8_t	replace_if_present_flag;
			bin::u8_t	data_coding;
			bin::u8_t	sm_default_msg_id;
			bin::u8_t	short_msg_len;
			bin::u8_t 	short_msg[254];

			/* OPTIONAL PARAMETERS for DELIVER_SM */
			tlv_user_msg_reference		user_msg_reference;
			tlv_src_port				src_port;
			tlv_dst_port				dst_port;
			tlv_sar_msg_ref_num			sar_msg_ref_num;
			tlv_sar_total_segments		sar_total_segments;
			tlv_sar_segment_seqnum		sar_segment_seqnum;
			tlv_user_resp_code			user_resp_code;
			tlv_privacy_ind				privacy_ind;
			tlv_payload_type			payload_type;
			tlv_callback_num			callback_num;
			tlv_src_subaddr				src_subaddr;
			tlv_dst_subaddr			dst_subaddr;
			tlv_lang_ind				lang_ind;
			tlv_its_session_info		its_session_info;
			tlv_network_error_code		network_error_code;
			tlv_msg_state				msg_state;
			tlv_receipted_msg_id		receipted_msg_id;

			std::size_t serv_type_len;
			std::size_t src_addr_len;
			std::size_t dst_addr_len;
			std::size_t schedule_delivery_time_len;
			std::size_t validity_period_len;

			deliver_sm()
				: command(command::deliver_sm)
			{}

			bin::sz_t raw_size() const {
				bin::sz_t len =
						sizeof(command)
						+ serv_type_len
						+ 2
						+ src_addr_len
						+ 2
						+ dst_addr_len
						+ 3
						+ schedule_delivery_time_len
						+ validity_period_len
						+ 5
						+ short_msg_len
						+ (user_msg_reference.tag == option::user_msg_reference ? user_msg_reference.raw_size(): 0)
						+ (src_port.tag == option::src_port ? src_port.raw_size() : 0)
						+ (dst_port.tag == option::dst_port ? dst_port.raw_size(): 0)
						+ (sar_msg_ref_num.tag == option::sar_msg_ref_num ? sar_msg_ref_num.raw_size(): 0)
						+ (sar_total_segments.tag == option::sar_total_segments ? sar_total_segments.raw_size(): 0)
						+ (sar_segment_seqnum.tag == option::sar_segment_seqnum ? sar_segment_seqnum.raw_size(): 0)
						+ (payload_type.tag == option::payload_type ? payload_type.raw_size(): 0)
						+ (privacy_ind.tag == option::privacy_ind ? privacy_ind.raw_size(): 0)
						+ (callback_num.tag == option::callback_num ? callback_num.raw_size(): 0)
						+ (src_subaddr.tag == option::src_subaddr ? src_subaddr.raw_size(): 0)
						+ (dst_subaddr.tag == option::dst_subaddr ? dst_subaddr.raw_size(): 0)
						+ (user_resp_code.tag == option::user_resp_code ? user_resp_code.raw_size(): 0)
						+ (lang_ind.tag == option::lang_ind ? lang_ind.raw_size(): 0)
						+ (its_session_info.tag == option::its_session_info ? its_session_info.raw_size(): 0)
						+ (network_error_code.tag == option::network_error_code ? network_error_code.raw_size(): 0)
						+ (msg_state.tag == option::msg_state ? msg_state.raw_size(): 0)
						+ (receipted_msg_id.tag == option::receipted_msg_id ? receipted_msg_id.raw_size(): 0);
				return len;
			}
		};

		struct deliver_sm_r {
			pdu command;
			bin::u8_t	msg_id[65];

			bin::sz_t raw_size() const {
				return sizeof(command)
					+ msg_id_len;
			}

			std::size_t msg_id_len;

			deliver_sm_r()
				: command(command::deliver_sm_r)
				, msg_id_len(0)
			{}
		};

		/* DATA_SM Operations */

		struct data_sm {
			pdu command;

			bin::u8_t		serv_type[6];
			bin::u8_t		src_addr_ton;
			bin::u8_t		src_addr_npi;
			bin::u8_t		src_addr[65];
			bin::u8_t		dst_addr_ton;
			bin::u8_t		dst_addr_npi;
			bin::u8_t		dst_addr[65];
			bin::u8_t		esm_class;
			bin::u8_t		registered_delivery;
			bin::u8_t		data_coding;

			std::size_t		serv_type_len;
			std::size_t		src_addr_len;
			std::size_t		dst_addr_len;

			tlv_src_port					src_port;
			tlv_src_addr_subunit			src_addr_subunit;
			tlv_src_network_type			src_network_type;
			tlv_src_bearer_type				src_bearer_type;
			tlv_src_telematics_id			src_telematics_id;
			tlv_dst_port					dst_port;
			tlv_dst_addr_subunit			dst_addr_subunit;
			tlv_dst_network_type			dst_network_type;
			tlv_dst_bearer_type				dst_bearer_type;
			tlv_dst_telematics_id			dst_telematics_id;
			tlv_sar_msg_ref_num				sar_msg_ref_num;
			tlv_sar_total_segments			sar_total_segments;
			tlv_sar_segment_seqnum			sar_segment_seqnum;
			tlv_more_msgs_to_send			more_msgs_to_send;
			tlv_qos_time_to_live			qos_time_to_live;
			tlv_payload_type				payload_type;
			tlv_msg_payload					msg_payload;
			tlv_set_dpf						set_dpf;
			tlv_receipted_msg_id			receipted_msg_id;
			tlv_msg_state					msg_state;
			tlv_network_error_code			network_error_code;
			tlv_user_msg_reference			user_msg_reference;
			tlv_privacy_ind					privacy_ind;
			tlv_callback_num				callback_num;
			tlv_callback_num_pres_ind		callback_num_pres_ind;
			tlv_callback_num_atag			callback_num_atag;
			tlv_src_subaddr					src_subaddr;
			tlv_dst_subaddr					dst_subaddr;
			tlv_user_resp_code				user_resp_code;
			tlv_display_time				display_time;
			tlv_sms_signal					sms_signal;
			tlv_ms_validity					ms_validity;
			tlv_ms_msg_wait_fclts			ms_msg_wait_fclts;
			tlv_number_of_msgs				number_of_msgs;
			tlv_alert_on_msg_delivery		alert_on_msg_delivery;
			tlv_lang_ind					lang_ind;
			tlv_its_reply_type				its_reply_type;
			tlv_its_session_info			its_session_info;

			data_sm()
				: command(command::data_sm)
				, serv_type_len(0)
			    , src_addr_len(0)
		     	, dst_addr_len(0)
			{}

			bin::sz_t raw_size() const {
				bin::sz_t len =
						sizeof(command)
						+ serv_type_len
						+ 2
						+ src_addr_len
						+ 2
						+ dst_addr_len
						+ 3
						+ (user_msg_reference.tag == option::user_msg_reference
							? user_msg_reference.raw_size(): 0)
						+ (src_port.tag == option::src_port
							? src_port.raw_size() : 0)
						+ (src_addr_subunit.tag == option::src_addr_subunit
							? src_addr_subunit.raw_size(): 0)
						+ (dst_port.tag == option::dst_port
							? dst_port.raw_size(): 0)
						+ (dst_addr_subunit.tag == option::dst_addr_subunit
							? dst_addr_subunit.raw_size(): 0)
						+ (sar_msg_ref_num.tag == option::sar_msg_ref_num
							? sar_msg_ref_num.raw_size(): 0)
						+ (sar_total_segments.tag == option::sar_total_segments
							? sar_total_segments.raw_size(): 0)
						+ (sar_segment_seqnum.tag == option::sar_segment_seqnum
							? sar_segment_seqnum.raw_size(): 0)
						+ (more_msgs_to_send.tag == option::more_msgs_to_send
							? more_msgs_to_send.raw_size(): 0)
						+ (payload_type.tag == option::payload_type
							? payload_type.raw_size(): 0)
						+ (msg_payload.tag == option::msg_payload
							? msg_payload.len: 0)
						+ (privacy_ind.tag == option::privacy_ind
							? privacy_ind.raw_size(): 0)
						+ (callback_num.tag == option::callback_num
							? callback_num.raw_size(): 0)
						+ (callback_num_pres_ind.tag == option::callback_num_pres_ind
							? callback_num_pres_ind.raw_size(): 0)
						+ (callback_num_atag.tag == option::callback_num_atag
							? callback_num_atag.raw_size(): 0)
						+ (src_subaddr.tag == option::src_subaddr
							? src_subaddr.raw_size(): 0)
						+ (dst_subaddr.tag == option::dst_subaddr
							? dst_subaddr.raw_size(): 0)
						+ (user_resp_code.tag == option::user_resp_code
							? user_resp_code.raw_size(): 0)
						+ (display_time.tag == option::display_time
							? display_time.raw_size(): 0)
						+ (sms_signal.tag == option::sms_signal
							? sms_signal.raw_size(): 0)
						+ (ms_validity.tag == option::ms_validity
							? ms_validity.raw_size(): 0)
						+ (ms_msg_wait_fclts.tag == option::ms_msg_wait_fclts
							? ms_msg_wait_fclts.raw_size(): 0)
						+ (number_of_msgs.tag == option::number_of_msgs
							? number_of_msgs.raw_size(): 0)
						+ (alert_on_msg_delivery.tag == option::alert_on_msg_delivery
							? alert_on_msg_delivery.raw_size(): 0)
						+ (lang_ind.tag == option::lang_ind
							? lang_ind.raw_size(): 0)
						+ (its_reply_type.tag == option::its_reply_type
							? its_reply_type.raw_size(): 0)
						+ (its_session_info.tag == option::its_session_info
							? its_session_info.raw_size(): 0)
						+ (src_network_type.tag == option::src_network_type
							? src_network_type.raw_size(): 0)
						+ (src_bearer_type.tag == option::src_bearer_type
							? src_bearer_type.raw_size(): 0)
						+ (src_telematics_id.tag == option::src_telematics_id
							? src_telematics_id.raw_size(): 0)
						+ (dst_network_type.tag == option::dst_network_type
							? dst_network_type.raw_size(): 0)
						+ (set_dpf.tag == option::set_dpf
							? set_dpf.raw_size(): 0)
						+ (receipted_msg_id.tag == option::receipted_msg_id
							? receipted_msg_id.raw_size(): 0)
						+ (msg_state.tag == option::msg_state
							? msg_state.raw_size(): 0)
						+ (network_error_code.tag == option::network_error_code
							? network_error_code.raw_size(): 0)
						+ (dst_bearer_type.tag == option::dst_bearer_type
							? dst_bearer_type.raw_size(): 0)
						+ (qos_time_to_live.tag == option::qos_time_to_live
							? qos_time_to_live.raw_size(): 0);
				return len;
			}
		};

		struct data_sm_r {
			pdu command;

			bin::u8_t msg_id[65];
			std::size_t msg_id_len;

			/* OPTIONAL PARAMETERS for DATA SM RESP */

			tlv_delivery_failure_reason			delivery_failure_reason;
			tlv_network_error_code				network_error_code;
			tlv_additional_status_info_text		additional_status_info_text;
			tlv_dpf_result						dpf_result;

			data_sm_r()
				: command(command::data_sm_r)
			{}

			bin::sz_t raw_size() const {
				return	sizeof(command)
						+ msg_id_len
						+ (additional_status_info_text.tag
								== option::additional_status_info_text ?
								additional_status_info_text.raw_size(): 0)
						+ (network_error_code.tag
								== option::network_error_code ?
								network_error_code.raw_size(): 0)
						+ (delivery_failure_reason.tag
								== option::delivery_failure_reason?
								delivery_failure_reason.raw_size(): 0)
						+ (dpf_result.tag == option::dpf_result
								? dpf_result.raw_size(): 0);
			}
		};

		/* QUERY SM Operations */

		struct query_sm {
			pdu command;

			bin::u8_t	msg_id[65];
			bin::u8_t	src_addr_ton;
			bin::u8_t	src_addr_npi;
			bin::u8_t	src_addr[21];

			std::size_t msg_id_len;
			std::size_t src_addr_len;

			query_sm()
				: command(command::query_sm)
			{}

			bin::sz_t raw_size() const {
				return	sizeof(command)
						+ msg_id_len
						+ 2
						+ src_addr_len;
			}
		};

		struct query_sm_r {
			pdu command;

			bin::u8_t	msg_id[65];
			bin::u8_t	final_date[17];
			bin::u8_t	msg_state;
			bin::u8_t	error_code;

			std::size_t msg_id_len;
			std::size_t final_date_len;

			query_sm_r()
				: command(command::query_sm_r)
			{}

			bin::sz_t raw_size() const {
				return	sizeof(command)
						+ msg_id_len
						+ final_date_len
						+ 2;
			}
		};

		/* CANCEL SM Operations */

		struct cancel_sm {
			pdu command;

			bin::u8_t	serv_type[6];
			bin::u8_t	msg_id[65];
			bin::u8_t	src_addr_ton;
			bin::u8_t	src_addr_npi;
			bin::u8_t	src_addr[21];
			bin::u8_t	dst_addr_ton;
			bin::u8_t	dst_addr_npi;
			bin::u8_t	dst_addr[21];

			std::size_t serv_type_len;
			std::size_t msg_id_len;
			std::size_t src_addr_len;
			std::size_t dst_addr_len;

			cancel_sm()
				: command(command::cancel_sm)
			{}

			bin::sz_t raw_size() const {
				return 	sizeof (command)
						+ serv_type_len
						+ msg_id_len
						+ 2
						+ src_addr_len
						+ 2
						+ dst_addr_len;
			}
		};

		struct cancel_sm_r {
			pdu command;

			cancel_sm_r()
				: command(command::cancel_sm_r)
			{}

			bin::sz_t raw_size() const {
				return sizeof(command);
			}
		};

		/* REPLACE SM Operations */

		struct replace_sm {
			pdu command;

			bin::u8_t	msg_id[65];
			bin::u8_t	src_addr_ton;
			bin::u8_t	src_addr_npi;
			bin::u8_t	src_addr[21];
			bin::u8_t	schedule_delivery_time[17];
			bin::u8_t	validity_period[17];
			bin::u8_t	registered_delivery;
			bin::u8_t	sm_default_msg_id;
			bin::u8_t	short_msg_len;
			bin::u8_t	short_msg[254];

			std::size_t msg_id_len;
			std::size_t src_addr_len;
			std::size_t schedule_delivery_time_len;
			std::size_t validity_period_len;

			replace_sm()
				: command(command::replace_sm)
			{}

			bin::sz_t raw_size() const {
				return	sizeof(command)
						+ msg_id_len
						+ 2
						+ src_addr_len
						+ schedule_delivery_time_len
						+ validity_period_len
						+ 3
						+ short_msg_len;
			}
		};

		struct replace_sm_r {
			pdu command;

			replace_sm_r()
				: command(command::replace_sm_r)
			{}

			bin::sz_t raw_size() const {
				return	sizeof(command);
			}
		};

		/* ENQUIRE LINK Operation */

		struct enquire_link {
			pdu command;

			enquire_link()
				: command(command::enquire_link)
			{}

			bin::sz_t raw_size() const {
				return 	sizeof(command);
			}
		};

		struct enquire_link_r {
			pdu command;

			enquire_link_r()
				: command(command::enquire_link_r)
			{}

			bin::sz_t raw_size() const {
				return 	sizeof(command);
			}
		};

		/* ALERT NOTIFICATIONS Operation */

		struct alert_notification {
			pdu command;

			bin::u8_t	src_addr_ton;
			bin::u8_t	src_addr_npi;
			bin::u8_t	src_addr[65];
			bin::u8_t	esme_addr_ton;
			bin::u8_t	esme_addr_npi;
			bin::u8_t	esme_addr[65];

			std::size_t src_addr_len;
			std::size_t esme_addr_len;

			/* OPTIONAL PARAMETERS for ALERT NOTIFICATION */

			tlv_ms_availability_status	ms_availability_status;

			alert_notification()
				: command(command::alert_notification)
			{}

			bin::sz_t raw_size() const {
				return 	sizeof(command)
						+ 2
						+ src_addr_len
						+ 2
						+ esme_addr_len
						+ (ms_availability_status.tag
								== option::ms_availability_status
								? ms_availability_status.raw_size(): 0);
			}
		};

		struct generic_nack {
			pdu command;

			generic_nack():
				command(command::generic_nack)
			{}

			inline bin::sz_t raw_size() const {
				return sizeof(command);
			}
		};
	}

	/* TYPE TRAITS */
	namespace {

		/* Generic type trait */
		template <class T>
		struct the {};

		/* Fields prefixed with r_ represent properties
		 * of corresponding resp PDUs
		 */

		template <>
		struct the<bind_receiver> {
			/* Corresponding resp type */
			typedef bind_receiver_r r_type;

			/* Corresponding resp command id */
			static const bin::u32_t r_id = command::bind_receiver_r;

			/* Response overall msg len */
			static std::size_t r_size(const r_type & r) {
				return sizeof(pdu)
					+ (r.sc_interface_version.tag != 0)
						? sizeof(tlv_sc_interface_version) : 0
					+ r.sys_id_len;
			}
		};

		template <>
		struct the<bind_transmitter> {
			/* Corresponding resp type */
			typedef bind_transmitter_r r_type;

			/* Corresponding resp command id */
			static const bin::u32_t r_id = command::bind_transmitter_r;

			/* Response overall msg len */
			static std::size_t r_size(const r_type & r) {
				return sizeof(pdu)
					+ (r.sc_interface_version.tag != 0)
						? sizeof(tlv_sc_interface_version) : 0
					+ r.sys_id_len;
			}
		};

		template <>
		struct the<bind_transceiver> {
			/* Corresponding resp type */
			typedef bind_transceiver_r r_type;

			/* Corresponding resp command id */
			static const bin::u32_t r_id = command::bind_transceiver_r;

			/* Response overall msg len */
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
			static const bin::u32_t id = command::submit_sm;

			/* Corresponding resp type */
			typedef submit_sm_r r_type;

			/* Corresponding resp command id */
			static const bin::u32_t r_id = command::submit_sm_r;

			/* Response overall msg len */
			static std::size_t r_size(const r_type & r) {
				return sizeof(pdu) + r.msg_id_len;
			}
		};
	}

	/* OPERATIONS P&W */
	namespace {

		/* BIND_TRANSMITTER	*/
		/* BIND_RECEIVER	*/
		/* BIND_TRANSCEIVER	*/
		template <class BindT, class LogT>
		const bin::u8_t * parse(BindT & r, const bin::u8_t * buf
				, const bin::u8_t * bend, LogT & L) {
			using namespace bin;

			RETURN_NULL_IF(buf + sizeof(r.command) >= bend);
			buf = parse(r.command, buf, L);

			/* scpyl returns NULL if error has occurred */
			buf = p::scpyl(r.sys_id, buf, bend
					, sizeof(r.sys_id), r.sys_id_len);
			RETURN_NULL_IF(buf == NULL);

			/* scpyl returns NULL if error has occurred */
			buf = p::scpyl(r.password, buf, bend
					, sizeof(r.password), r.password_len);
			RETURN_NULL_IF(buf == NULL);

			/* scpyl returns NULL if error has occurred */
			buf = p::scpyl(r.sys_type, buf, bend
						, sizeof(r.sys_type), r.sys_type_len);

			RETURN_NULL_IF(buf + sizeof(r.interface_version) >= bend);
			buf = p::cp_u8(&r.interface_version, buf);

			RETURN_NULL_IF(buf + sizeof(r.addr_ton) >= bend);
			buf = p::cp_u8(&r.addr_ton, buf);

			RETURN_NULL_IF(buf + sizeof(r.addr_npi) >= bend);
			buf = p::cp_u8(&r.addr_npi, buf);

			/* scpyl returns NULL if error has occurred */
			buf = p::scpyl(r.addr_range, buf, bend
					, sizeof(r.addr_range), r.addr_range_len);
			RETURN_NULL_IF(buf == NULL);

			return buf;
		}

		template <class BindT, class LogT>
		bin::u8_t * write(bin::u8_t * buf, bin::u8_t * bend
				, const BindT & r, LogT & L) {
			using namespace bin;

			RETURN_NULL_IF(buf + sizeof(r.command) >= bend);
			buf = write(buf, r.command, L);

			RETURN_NULL_IF(buf + r.sys_id_len >= bend);
			buf = w::scpyf(buf, bend, r.sys_id, r.sys_id_len);

			RETURN_NULL_IF(buf + r.password_len >= bend);
			buf = w::scpyf(buf, bend, r.password, r.password_len);

			RETURN_NULL_IF(buf + r.sys_type_len >= bend);
			buf = w::scpyf(buf, bend, r.sys_type, r.sys_type_len);

			RETURN_NULL_IF(buf + sizeof(r.interface_version) >= bend);
			buf = w::cp_u8(buf, &r.interface_version);

			RETURN_NULL_IF(buf + sizeof(r.addr_ton) >= bend);
			buf = w::cp_u8(buf, &r.addr_ton);

			RETURN_NULL_IF(buf + sizeof(r.addr_npi) >= bend);
			buf = w::cp_u8(buf, &r.addr_npi);

			RETURN_NULL_IF(buf + r.addr_range_len > bend);
			buf = w::scpyf(buf, bend, r.addr_range, r.addr_range_len+1);

			return buf;
		}

		/* BIND_TRANSMITTER_R	*/
		/* BIND_RECEIVER_R		*/
		/* BIND_TRANSCEIVER_R	*/
		template <class BindT, class LogT>
		const bin::u8_t * parse_r(BindT & r
				, const bin::u8_t * buf, const bin::u8_t * bend, LogT & L) {
			using namespace bin;

			bin::u16_t optid;

			RETURN_NULL_IF(buf + sizeof(r.command) >= bend);
			buf = parse(r.command, buf, L);

			/* scpyl returns NULL if error has occurred */
			buf = p::scpyl(r.sys_id, buf, bend
					, sizeof(r.sys_id), r.sys_id_len);
			RETURN_NULL_IF(buf == NULL);

			if (buf == bend) /* there is no optional parameter */
				return bend;

			p::cp_u16(asbuf(optid), buf);
			RETURN_NULL_IF(optid != option::sc_interface_version);
			buf = parse(r.sc_interface_version, buf, L);

			return buf;
		}

		template <class BindT, class LogT>
		const bin::u8_t * write_r(bin::u8_t * buf, bin::u8_t * bend
				, const BindT & r, LogT & L) {
			using namespace bin;

			RETURN_NULL_IF(buf + sizeof(r.command) >= bend);
			buf = write(buf, r.command, L);

			buf = w::scpyf(buf, bend, r.sys_id, r.sys_id_len);
			RETURN_NULL_IF(buf == NULL);

			if (r.sc_interface_version.tag == option::sc_interface_version) {
				RETURN_NULL_IF(buf + sizeof(r.sc_interface_version) > bend);
				buf = write(buf, r.sc_interface_version, L);
			} else if (r.sc_interface_version.tag != 0) {
				return NULL; /* tag's value is incorrect */
			}

			return buf;
		}

		/* OUTBIND P&W */
		template <class LogT>
		const bin::u8_t * parse(outbind & r, bin::u8_t * buf
				, bin::u8_t * bend, LogT & L) {
			using namespace bin;

			RETURN_NULL_IF(buf + sizeof(r.command) >= bend);
			buf = write(buf, r.command, L);

			buf = p::scpyl(r.sys_id, buf, bend
					, sizeof(r.sys_id), r.sys_id_len);
			RETURN_NULL_IF(buf == NULL);

			buf = p::scpyl(r.password, buf, bend
					, sizeof(r.sys_id), r.password_len);
			RETURN_NULL_IF(buf == NULL);

			return buf;
		}

		template <class LogT>
		const bin::u8_t * write(bin::u8_t * buf, bin::u8_t * bend
				, const outbind & r, LogT & L) {
			using namespace bin;

			RETURN_NULL_IF(buf + sizeof(r.command) >= bend);
			buf = write(buf, r.command, L);

			buf = w::scpyf(buf, bend, r.sys_id, r.sys_id_len);
			RETURN_NULL_IF(buf == NULL);

			buf = w::scpyf(buf, bend, r.password, r.password_len);
			RETURN_NULL_IF(buf == NULL);
		}

		/* UNBIND P&W		*/
		/* UNBIND_R P&W		*/
		/* GENERIC_NACK P&W	*/
		template <class BindT, class LogT>
		void parse(BindT & r, const bin::u8_t * buf
				, const bin::u8_t * bend, LogT & L) {
			RETURN_NULL_IF(buf + sizeof(r.command) > bend);
			parse(r.command, buf, L);
		}

		template <class BindT, class LogT>
		void write(bin::u8_t * buf, bin::u8_t * bend
				,const unbind & r, LogT & L) {
			RETURN_NULL_IF(buf + sizeof(r.command) > bend);
			write(buf, r.command, L);
		}

		/* SUBMIT_SM P&W */
		template <class LogT>
		const bin::u8_t * parse(submit_sm & r, const bin::u8_t * buf
				, const bin::u8_t * bend, LogT & L) {

			using namespace bin;

			bin::u16_t optid;
			buf = ascbuf(buf);
			/*bend = buf + msg->len; */

			RETURN_NULL_IF(buf + sizeof(r.command) >= bend);
			buf = parse(r.command, buf, L);

			/* scpyl returns NULL if error has occurred */
			buf = p::scpyl(r.serv_type, buf, bend
					, sizeof(r.serv_type), r.serv_type_len);
			RETURN_NULL_IF(buf == NULL);

			RETURN_NULL_IF(buf + sizeof(r.src_addr_ton) >= bend);
			buf = p::cp_u8(&r.src_addr_ton, buf);

			RETURN_NULL_IF(buf + sizeof(r.src_addr_npi) >= bend);
			buf = p::cp_u8(&r.src_addr_npi, buf);

			/* scpyl returns NULL if error has occurred */
			buf = p::scpyl(r.src_addr, buf, bend
					, sizeof(r.src_addr), r.src_addr_len);
			RETURN_NULL_IF(buf == NULL);

			RETURN_NULL_IF(buf + sizeof(r.dst_addr_ton) >= bend);
			buf = p::cp_u8(&r.dst_addr_ton, buf);

			RETURN_NULL_IF(buf + sizeof(r.dst_addr_npi) >= bend);
			buf = p::cp_u8(&r.dst_addr_npi, buf);

			/* scpyl returns NULL if error has occurred */
			buf = p::scpyl(r.dst_addr, buf, bend
					, sizeof(r.dst_addr), r.dst_addr_len);
			RETURN_NULL_IF(buf == NULL);

			RETURN_NULL_IF(buf + sizeof(r.esm_class) >= bend);
			buf = p::cp_u8(&r.esm_class, buf);

			RETURN_NULL_IF(buf + sizeof(r.protocol_id) >= bend);
			buf = p::cp_u8(&r.protocol_id, buf);

			RETURN_NULL_IF(buf + sizeof(r.priority_flag) >= bend);
			buf = p::cp_u8(&r.priority_flag, buf);

			/* scpyl returns NULL if error has occurred */
			buf = p::scpyf(r.schedule_delivery_time
							, buf, bend, sizeof(r.schedule_delivery_time));
			RETURN_NULL_IF(buf == NULL);
// TAG1
			/* scpyl returns NULL if error has occurred */
			buf = p::scpyf(r.validity_period
							, buf, bend, sizeof(r.validity_period));
			RETURN_NULL_IF(buf == NULL);
			
			RETURN_NULL_IF(buf + sizeof(r.registered_delivery) >= bend);
			buf = p::cp_u8(&r.registered_delivery, buf);

			RETURN_NULL_IF(buf + sizeof(r.replace_if_present_flag) >= bend);
			buf = p::cp_u8(&r.replace_if_present_flag, buf);

			RETURN_NULL_IF(buf + sizeof(r.data_coding) >= bend);
			buf = p::cp_u8(&r.data_coding, buf);

			RETURN_NULL_IF(buf + sizeof(r.sm_default_msg_id) >= bend);
			buf = p::cp_u8(&r.sm_default_msg_id, buf);

			RETURN_NULL_IF(buf + sizeof(r.short_msg_len) >= bend)
			buf = p::cp_u8(&r.short_msg_len, buf);

			/* scpyl returns NULL if error has occurred */
			buf = p::scpyl(r.short_msg, buf, bend
					, sizeof(r.short_msg), r.short_msg_len);
			RETURN_NULL_IF(buf == NULL);

			/* RETURN_NULL_IF(buf + sizeof(tlv) > bend); */
			p::cp_u16(asbuf(optid), buf);
			/*
			buf += sizeof(bin::u16_t);
			*/

			while (buf < bend) {
				switch (optid) {
					case option::user_msg_reference:
						buf = parse(r.user_msg_reference, buf, L); break;
					case option::src_port:
						buf = parse(r.src_port, buf, L); break;
					case option::src_addr_subunit:
						buf = parse(r.src_addr_subunit, buf, L); break;
					case option::dst_port:
						buf = parse(r.dst_port, buf, L); break;
					case option::dst_addr_subunit:
						buf = parse(r.dst_addr_subunit, buf, L); break;
					case option::sar_msg_ref_num:
						buf = parse(r.sar_msg_ref_num, buf, L); break;
					case option::sar_total_segments:
						buf = parse(r.sar_total_segments, buf, L); break;
					case option::sar_segment_seqnum:
						buf = parse(r.sar_segment_seqnum, buf, L); break;
					case option::more_msgs_to_send:
						buf = parse(r.more_msgs_to_send, buf, L); break;
					case option::payload_type:
						buf = parse(r.payload_type, buf, L); break;
					/* TODO: Parse msg_payload*/
					case option::msg_payload:
						/*buf = parse(r.msg_payload, buf, L);*/
						buf += sizeof(r.msg_payload);
						break;
					case option::privacy_ind:
						buf = parse(r.privacy_ind, buf, L); break;
					case option::callback_num:
						buf = parse(r.callback_num, buf, L); break;
					case option::callback_num_pres_ind:
						buf = parse(r.callback_num_pres_ind, buf, L); break;
					case option::callback_num_atag:
						buf = parse(r.callback_num_atag, buf, L); break;
					case option::src_subaddr:
						buf = parse(r.src_subaddr, buf, L); break;
					case option::dst_subaddr:
						buf = parse(r.dst_subaddr, buf, L); break;
					case option::user_resp_code:
						buf = parse(r.user_resp_code, buf, L); break;
					case option::display_time:
						buf = parse(r.display_time, buf, L); break;
					case option::sms_signal:
						buf = parse(r.sms_signal, buf, L); break;
					case option::ms_validity:
						buf = parse(r.ms_validity, buf, L); break;
					case option::ms_msg_wait_fclts:
						buf = parse(r.ms_msg_wait_fclts, buf, L); break;
					case option::number_of_msgs:
						buf = parse(r.number_of_msgs, buf, L); break;
					case option::alert_on_msg_delivery:
						buf = parse(r.alert_on_msg_delivery, buf, L); break;
					case option::lang_ind:
						buf = parse(r.lang_ind, buf, L); break;
					case option::its_reply_type:
						buf = parse(r.its_reply_type, buf, L); break;
					case option::its_session_info:
						buf = parse(r.its_session_info, buf, L); break;
					case option::ussd_serv_op:
						buf = parse(r.ussd_serv_op, buf, L); break;
					default: return NULL;
				}
				/* may be optional parameter wasn't writed corretly */
				/* RETURN_NULL_IF(buf + sizeof(tlv) > bend);
				 */
				p::cp_u16(asbuf(optid), buf);
			}
			return buf;
		}

		template <class LogT>
		bin::u8_t * write(bin::u8_t * buf
				, bin::u8_t * bend, const submit_sm & r, LogT & L) {
			using namespace bin;
			/* len of var strings are stored
			 * in additional field of struct */

			RETURN_NULL_IF(buf + sizeof(r.command) >= bend);
			buf = write(buf, r.command, L);

			RETURN_NULL_IF(buf + r.serv_type_len >= bend);
			buf = w::scpy(buf, r.serv_type, r.serv_type_len);
			RETURN_NULL_IF(buf == NULL);

			/* Writing addr of ESME */
			RETURN_NULL_IF(buf + sizeof(r.src_addr_ton) >= bend);
			buf = w::cp_u8(buf, &r.src_addr_ton);

			RETURN_NULL_IF(buf + sizeof(r.src_addr_npi) >= bend);
			buf = w::cp_u8(buf, &r.src_addr_npi);

			RETURN_NULL_IF(buf + r.src_addr_len >= bend);
			buf = w::scpy(buf, r.src_addr, r.src_addr_len);

			/* Writing ME */

			RETURN_NULL_IF(buf + sizeof(r.dst_addr_ton) >= bend);
			buf = w::cp_u8(buf, &r.dst_addr_ton);

			RETURN_NULL_IF(buf + sizeof(r.dst_addr_npi) >= bend);
			buf = w::cp_u8(buf, &r.dst_addr_npi);

			RETURN_NULL_IF(buf + r.dst_addr_len >= bend);
			buf = w::scpy(buf, r.dst_addr, r.dst_addr_len);

			RETURN_NULL_IF(buf + sizeof(r.esm_class) >= bend);
			buf = w::cp_u8(buf, &r.esm_class);

			RETURN_NULL_IF(buf + sizeof(r.protocol_id) >= bend);
			buf = w::cp_u8(buf, &r.protocol_id);

			RETURN_NULL_IF(buf + sizeof(r.priority_flag) >= bend);
			buf = w::cp_u8(buf, &r.priority_flag);

			buf = w::scpyf(buf, bend, r.schedule_delivery_time
					, sizeof(r.schedule_delivery_time));
			RETURN_NULL_IF(buf == NULL);

			/* scpyf return NULL if error has occured */
			buf = w::scpyf(buf, bend, r.validity_period
					, sizeof(r.validity_period));
			RETURN_NULL_IF(buf == NULL);

			RETURN_NULL_IF(buf + sizeof(r.registered_delivery_len) >= bend);
			buf = w::cp_u8(buf, &r.registered_delivery);

			RETURN_NULL_IF(buf + sizeof(r.replace_if_present_flag) >= bend);
			buf = w::cp_u8(buf, &r.replace_if_present_flag);

			RETURN_NULL_IF(buf + sizeof(r.data_coding) >= bend);
			buf = w::cp_u8(buf, &r.data_coding);

			RETURN_NULL_IF(buf + sizeof(r.sm_default_msg_id) >= bend);
			buf = w::cp_u8(buf, &r.sm_default_msg_id);

			RETURN_NULL_IF(buf + sizeof(r.short_msg_len) >= bend);
			buf = w::cp_u8(buf, &r.short_msg_len);

			RETURN_NULL_IF(buf + r.short_msg_len > bend);
			buf = w::scpy(buf, r.short_msg, r.short_msg_len);
			RETURN_NULL_IF(buf == NULL);

			if (r.user_msg_reference.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.user_msg_reference) > bend);
				buf = write(buf, r.user_msg_reference, L);
			}
			if (r.src_port.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.src_port) > bend);
				buf = write(buf, r.src_port, L);
			}
			if (r.src_addr_subunit.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.src_addr_subunit) > bend);
				buf = write(buf, r.src_addr_subunit, L);
			}
			if (r.dst_port.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.dst_port) > bend);
				buf = write(buf, r.dst_port, L);
			}
			if (r.dst_addr_subunit.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.dst_addr_subunit) > bend);
				buf = write(buf, r.dst_addr_subunit, L);
			}
			if (r.sar_msg_ref_num.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.sar_msg_ref_num) > bend);
				buf = write(buf, r.sar_msg_ref_num, L);
			}
			if (r.sar_total_segments.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.sar_total_segments) > bend);
				buf = write(buf, r.sar_total_segments, L);
			}
			if (r.sar_segment_seqnum.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.sar_segment_seqnum) > bend);
				buf = write(buf, r.sar_segment_seqnum, L);
			}
			if (r.more_msgs_to_send.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.more_msgs_to_send) > bend);
				buf = write(buf, r.more_msgs_to_send, L);
			}
			if (r.payload_type.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.payload_type) > bend);
				buf = write(buf, r.payload_type, L);
			}
			/* if (r.msg_payload.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.msg_payload) > bend);
				buf = write(buf, r.msg_payload, L);
			} */
			if (r.privacy_ind.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.privacy_ind) > bend);
				buf = write(buf, r.privacy_ind, L);
			}
			if (r.callback_num.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.callback_num) > bend);
				buf = write(buf, r.callback_num, L);
			}
			if (r.callback_num_pres_ind.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.callback_num_pres_ind) > bend);
				buf = write(buf, r.callback_num_pres_ind, L);
			}
			if (r.callback_num_atag.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.callback_num_atag) > bend);
				buf = write(buf, r.callback_num_atag, L);
			}
			if (r.src_subaddr.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.src_subaddr) > bend);
				buf = write(buf, r.src_subaddr, L);
			}
			if (r.user_resp_code.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.user_resp_code) > bend);
				buf = write(buf, r.user_resp_code, L);
			}
			if (r.display_time.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.display_time) > bend);
				buf = write(buf, r.display_time, L);
			}
			if (r.sms_signal.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.sms_signal) > bend);
				buf = write(buf, r.sms_signal, L);
			}
			if (r.ms_validity.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.ms_validity) > bend);
				buf = write(buf, r.ms_validity, L);
			}
			if (r.ms_msg_wait_fclts.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.ms_msg_wait_fclts) > bend);
				buf = write(buf, r.ms_msg_wait_fclts, L);
			}
			if (r.number_of_msgs.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.number_of_msgs) > bend);
				buf =write(buf, r.number_of_msgs, L);
			}
			if (r.alert_on_msg_delivery.tag!=0) {
				RETURN_NULL_IF(buf + sizeof(r.alert_on_msg_delivery) > bend);
				buf = write(buf, r.alert_on_msg_delivery, L);
			}
			if (r.lang_ind.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.lang_ind) > bend);
				buf = write(buf, r.lang_ind, L);
			}
			if (r.its_reply_type.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.its_reply_type) > bend);
				buf = write(buf, r.its_reply_type, L);
			}
			if (r.its_session_info.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.its_session_info) > bend);
				buf = write(buf, r.its_session_info, L);
			}
			if (r.ussd_serv_op.tag != 0) {
				RETURN_NULL_IF(buf + sizeof(r.ussd_serv_op) > bend);
				buf = write(buf, r.ussd_serv_op, L);
			}
			return buf;
		}

		/* SUBMIT_SM_R P&W */
		template <class LogT>
		void parse(submit_sm_r & r, const bin::u8_t * buf
				, const bin::u8_t * bend, LogT & L) {
			using namespace bin;

			RETURN_NULL_IF(buf + sizeof(r.command) >= bend);
			buf = parse(r.command, buf, L);

			RETURN_NULL_IF(buf + sizeof(r.msg_id) > bend);
			buf = p::scpyl(r.msg_id, buf, bend, sizeof(r.msg_id), r.msg_id_len);
		}

		template <class LogT>
		void write(bin::u8_t * buf, const submit_sm_r & r, LogT & L) {
			using namespace bin;
			buf = write(buf, r.command, L);
			buf = w::scpy(buf, r.msg_id, r.msg_id_len + 1);
		}

		/* SUBMIT_MULTI P&W */
		template <class LogT>
		void parse(submit_multi_sm & r, const bin::u8_t * buf
				, const bin::u8_t * bend, LogT & L) {

			using namespace bin;

			bin::u16_t optid;
			buf = ascbuf(buf);

			RETURN_NULL_IF(buf + sizeof(r.command) >= bend);
			buf = parse(r.command, buf, L);

			RETURN_NULL_IF(buf + sizeof(r.serv_type) >= bend);
			buf = p::scpyl(r.serv_type, buf, bend
					, sizeof(r.serv_type), r.serv_type_len);

			RETURN_NULL_IF(buf + sizeof(r.src_addr_ton) >= bend);
			buf = p::cp_u8(&r.src_addr_ton, buf);

			RETURN_NULL_IF(buf + sizeof(r.src_addr_npi) >= bend);
			buf = p::cp_u8(&r.src_addr_npi, buf);

			RETURN_NULL_IF(buf + sizeof(r.src_addr) >= bend);
			buf = p::scpyl(r.src_addr, buf, bend
					, sizeof(r.src_addr), r.src_addr_len);

			RETURN_NULL_IF(buf + sizeof(r.num_of_dsts) >= bend);
			buf = p::cp_u8(&r.num_of_dsts, buf);

			RETURN_NULL_IF(buf + sizeof(r.esm_class) >= bend);
			buf = p::cp_u8(&r.esm_class, buf);

			RETURN_NULL_IF(buf + sizeof(r.protocol_id) >= bend);
			buf = p::cp_u8(&r.protocol_id, buf);

			RETURN_NULL_IF(buf + sizeof(r.priority_flag) >= bend);
			buf = p::cp_u8(&r.priority_flag, buf);

			RETURN_NULL_IF(buf + sizeof(r.schedule_delivery_time) >= bend);
			buf = p::scpyf(r.schedule_delivery_time, buf, bend
					, sizeof(r.schedule_delivery_time));

			RETURN_NULL_IF(buf + sizeof(r.validity_period) >= bend);
			buf = p::scpyf(r.validity_period, buf, bend
					, sizeof(r.validity_period));

			RETURN_NULL_IF(buf + sizeof(r.registered_delivery) >= bend);
			buf = p::cp_u8(&r.registered_delivery, buf);

			RETURN_NULL_IF(buf + sizeof(r.replace_if_present_flag) >= bend);
			buf = p::cp_u8(&r.replace_if_present_flag, buf);

			RETURN_NULL_IF(buf + sizeof(r.data_coding) >= bend);
			buf = p::cp_u8(&r.data_coding, buf);

			RETURN_NULL_IF(buf + sizeof(r.sm_default_msg_id) >= bend);
			buf = p::cp_u8(&r.sm_default_msg_id, buf);

			RETURN_NULL_IF(buf + sizeof(r.short_msg_len) >= bend);
			buf = p::cp_u8(&r.short_msg_len, buf);

			RETURN_NULL_IF(buf + sizeof(r.short_msg) >= bend);
			buf = p::scpyl(r.short_msg, buf, bend
					, sizeof(r.short_msg), r.short_msg_len);

			buf = p::cp_u16(asbuf(optid), buf);
			/*
			buf += sizeof(bin::u16_t);
			*/

			while (buf < bend) {
				switch (optid) {
					case option::user_msg_reference:
						buf = parse(r.user_msg_reference, buf, L); break;
					case option::src_port:
						buf = parse(r.src_port, buf, L); break;
					case option::src_addr_subunit:
						buf = parse(r.src_addr_subunit, buf, L); break;
					case option::dst_port:
						buf = parse(r.dst_port, buf, L); break;
					case option::dst_addr_subunit:
						buf = parse(r.dst_addr_subunit, buf, L); break;
					case option::sar_msg_ref_num:
						buf = parse(r.sar_msg_ref_num, buf, L); break;
					case option::sar_total_segments:
						buf = parse(r.sar_total_segments, buf, L); break;
					case option::sar_segment_seqnum:
						buf = parse(r.sar_segment_seqnum, buf, L); break;
					case option::payload_type:
						buf = parse(r.payload_type, buf, L); break;
					/* TODO: Parse msg_payload*/
					/*
					case option::msg_payload:
						buf = parse(r.msg_payload, buf, L);
						break;
					*/
					case option::privacy_ind:
						buf = parse(r.privacy_ind, buf, L); break;
					case option::callback_num:
						buf = parse(r.callback_num, buf, L); break;
					case option::callback_num_pres_ind:
						buf = parse(r.callback_num_pres_ind, buf, L); break;
					case option::callback_num_atag:
						buf = parse(r.callback_num_atag, buf, L); break;
					case option::src_subaddr:
						buf = parse(r.src_subaddr, buf, L); break;
					case option::dst_subaddr:
						buf = parse(r.dst_subaddr, buf, L); break;
					case option::display_time:
						buf = parse(r.display_time, buf, L); break;
					case option::sms_signal:
						buf = parse(r.sms_signal, buf, L); break;
					case option::ms_validity:
						buf = parse(r.ms_validity, buf, L); break;
					case option::ms_msg_wait_fclts:
						buf = parse(r.ms_msg_wait_fclts, buf, L); break;
					case option::alert_on_msg_delivery:
						buf = parse(r.alert_on_msg_delivery, buf, L); break;
					case option::lang_ind:
						buf = parse(r.lang_ind, buf, L); break;
					default: return;
				}
				RETURN_NULL_IF(buf + sizeof(optid) > bend);
				buf = p::cp_u16(asbuf(optid), buf);
			}
		}

		template <class LogT>
		void write(bin::u8_t * buf, const submit_multi_sm & r, LogT & L) {
			using namespace bin;
			buf = write(buf, r.command, L);
			buf = w::scpy(buf, r.serv_type, 6);

			buf = w::cp_u8(buf, r.src_addr_ton);
			buf = w::cp_u8(buf, r.src_addr_npi);
			buf = w::scpy(buf, r.src_addr, 21);
			buf = w::cp_u8(buf, r.num_of_dsts);
			/*
			TODO dst_addr(es)
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
			buf = w::cp_u8(buf, r.short_msg_len);
			buf = w::scpy(buf, r.short_msg, 254);

			if (r.user_msg_reference.tag != 0)	buf = write(buf, r.user_msg_reference, L);
			if (r.src_port.tag != 0)				buf = write(buf, r.src_port, L);
			if (r.src_addr_subunit.tag != 0)		buf = write(buf, r.src_addr_subunit, L);
			if (r.dst_port.tag != 0)				buf = write(buf, r.dst_port.tag, L);
			if (r.dst_addr_subunit.tag != 0)		buf = write(buf, r.dst_addr_subunit, L);
			if (r.sar_msg_ref_num.tag != 0)			buf = write(buf, r.sar_msg_ref_num, L);
			if (r.sar_total_segments.tag != 0)		buf = write(buf, r.sar_total_segments, L);
			if (r.sar_segment_seqnum.tag != 0)		buf = write(buf, r.sar_segment_seqnum, L);
			if (r.payload_type.tag != 0)			buf = write(buf, r.payload_type, L);
			if (r.msg_payload.tag != 0)			buf = write(buf, r.msg_payload.tag, L);
			if (r.privacy_ind.tag != 0)		buf = write(buf, r.privacy_ind, L);
			if (r.callback_num.tag != 0)			buf = write(buf, r.callback_num, L);
			if (r.callback_num_pres_ind.tag != 0)	buf = write(buf, r.callback_num_pres_ind, L);
			if (r.callback_num_atag.tag != 0)		buf = write(buf, r.callback_num_atag, L);
			if (r.src_subaddr.tag != 0)		buf = write(buf, r.src_subaddr, L);
			if (r.dst_subaddr.tag != 0)			buf = write(buf, r.dst_subaddr, L);
			if (r.display_time.tag != 0)			buf = write(buf, r.display_time, L);
			if (r.sms_signal.tag != 0)				buf = write(buf, r.sms_signal, L);
			if (r.ms_validity.tag != 0)				buf = write(buf, r.ms_validity, L);
			if (r.ms_msg_wait_fclts.tag != 0)	buf = write(buf, r.ms_msg_wait_fclts, L);
			if (r.alert_on_msg_delivery.tag!=0)	buf = write(buf, r.alert_on_msg_delivery, L);
			if (r.lang_ind.tag != 0)		buf = write(buf, r.lang_ind, L);
		}

		/* DEST_ADDRESS P&W */
		template <class LogT>
		void parse(dst_addr & r, const bin::u8_t * buf
				, const bin::u8_t * bend, LogT & L) {
			using namespace bin;

			RETURN_NULL_IF(buf + sizeof(r.dst_flag) >= bend);
			buf = p::cp_u8(&r.dst_flag, buf);
			/* TODO: 4.5.1.1 SME_Address */
		}

		template <class LogT>
		void write(bin::u8_t * buf, const dst_addr & r, LogT & L) {
			using namespace bin;
			buf = w::cp_u8(buf, &r.dst_flag);
			/* TODO: 4.5.1.1 SME_Address */
		}


		/* SUBMIT_MULTI_R P&W */
		template <class LogT>
		void parse(submit_multi_r & r, const bin::u8_t * buf
				, const bin::u8_t * bend, LogT & L) {
			using namespace bin;
			buf = ascbuf(buf);

			RETURN_NULL_IF(buf + sizeof(r.command) >= bend);
			buf = parse(r.command, buf, L);

			/* scpyl returns NULL if error has occurred */
			buf = p::scpyl(r.msg_id, buf, bend
						, sizeof(r.msg_id), r.msg_id_len);
			RETURN_NULL_IF(buf == NULL);

			RETURN_NULL_IF(buf + sizeof(r.no_unsuccess) >= bend);
			buf = p::cp_u8(&r.no_unsuccess, buf);

			/* TODO unsuccess_sme(s) */
		}
	}

	/* OPERATIONS DUMP */
	namespace {

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<bin::u8_t> & t) {
			L << std::bitset<8>(t.val);
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<bin::u16_t> & t) {
			L << t.val;
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<bin::u32_t> & t) {
			L << t.val;
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<bin::u8_t[256]> & t) {
			L << std::string(t.val, t.val + t.len);
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<bin::u8_t[65]> & t) {
			L << std::string(t.val, t.val + t.len);
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<bin::u8_t[23]> & t) {
			L << std::string(t.val, t.val + t.len);
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<bin::u8_t[19]> & t) {
			L << std::string(t.val, t.val + t.len);
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<bin::u8_t[3]> & t) {
			L << std::string(t.val, t.val + t.len);
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<bin::u8_t[2]> & t) {
			L << std::string(t.val, t.val + t.len);
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<bin::u8_t[1]> & t) {
			L << std::string(t.val, t.val + t.len);
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const tlv<bin::u8_t *> & t) {
			L << t.val;
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const pdu & t) {
			L
			<< "[command.len:"	<< t.len << "]"
			<< "[command.id:"	<< t.id << "]"
			<< "[command.status:"<< t.status << "]"
			<< "[command.seqno:"	<< t.seqno << "]";

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const submit_sm & r) {
			L	<< "submit_sm: "
				<< r.command
				<< "[serv_type:"				<< r.serv_type													<< "]"
				<< "[src_addr_ton:"			<< std::bitset<8>(r.src_addr_ton)								<< "]"
				<< "[src_addr_npi:"			<< std::bitset<8>(r.src_addr_npi)								<< "]"
				<< "[src_addr:"				<< r.src_addr													<< "]"
				<< "[dst_addr_ton:"			<< std::bitset<8>(r.dst_addr_ton)								<< "]"
				<< "[dst_addr_npi:"			<< std::bitset<8>(r.dst_addr_npi)								<< "]"
				<< "[dst_addr:"				<< r.dst_addr													<< "]"
				<< "[esm_class:"				<< std::bitset<8>(r.esm_class)									<< "]"
				<< "[protocol_id:"				<< static_cast<int>(r.protocol_id)								<< "]"
				<< "[priority_flag:"			<< static_cast<int>(r.priority_flag)							<< "]"
				<< "[schedule_delivery_time:"	<< r.schedule_delivery_time										<< "]"
				<< "[validity_period:"			<< r.validity_period											<< "]"
				<< "[registered_delivery:"		<< static_cast<bool>(r.registered_delivery)						<< "]"
				<< "[replace_if_present_flag:"<< static_cast<bool>(r.replace_if_present_flag)					<< "]"
				<< "[data_coding:"				<< std::bitset<8>(r.data_coding)								<< "]"
				<< "[sm_default_msg_id:"		<< static_cast<int>(r.sm_default_msg_id)						<< "]"
				<< "[short_msg_len:"			<< static_cast<int>(r.short_msg_len)							<< "]"
				<< "[short_msg:"				<< std::string(r.short_msg, r.short_msg + r.short_msg_len)		<< "]";
			if (r.user_msg_reference.tag != 0)	{ L << "[user_msg_reference:"		<< r.user_msg_reference		<< "]";	}
			if (r.src_port.tag != 0)			{ L << "[src_port:"				<< r.src_port				<< "]"; }
			if (r.src_addr_subunit.tag != 0)	{ L << "[src_addr_subunit:"		<< r.src_addr_subunit		<< "]"; }
			if (r.dst_port.tag != 0)			{ L << "[dst_port:"				<< r.dst_port				<< "]"; }
			if (r.dst_addr_subunit.tag != 0)	{ L << "[dst_addr_subunit:"		<< r.dst_addr_subunit		<< "]";	}
			if (r.sar_msg_ref_num.tag != 0)		{ L << "[sar_msg_ref_num:"			<< r.sar_msg_ref_num		<< "]";	}
			if (r.sar_total_segments.tag != 0)	{ L << "[sar_total_segments:"		<< r.sar_total_segments		<< "]";	}
			if (r.sar_segment_seqnum.tag != 0)	{ L << "[sar_segment_seqnum:"		<< r.sar_segment_seqnum		<< "]";	}
			if (r.more_msgs_to_send.tag != 0)	{ L << "[more_msgs_to_send:"		<< r.more_msgs_to_send		<< "]"; }
			if (r.payload_type.tag != 0)		{ L << "[payload_type:"			<< r.payload_type			<< "]"; }
			if (r.msg_payload.tag != 0)			{ L << "[msg_payload:"
				<< std::string(r.payload_type.val, r.payload_type.val +	r.payload_type.len) << "]"; }
			if (r.privacy_ind.tag != 0)			{ L << "[privacy_ind:"				<< r.privacy_ind			<< "]"; }
			if (r.callback_num.tag != 0)		{ L << "[callback_num:"			<< r.callback_num			<< "]";	}
			if (r.callback_num_pres_ind.tag!=0) { L << "[callback_num_pres_ind:"	<< r.callback_num_pres_ind 	<< "]"; }
			if (r.callback_num_atag.tag != 0)	{ L << "[callback_num_atag:"		<< r.callback_num_atag		<< "]";	}
			if (r.src_subaddr.tag != 0)			{ L << "[src_subaddr:"				<< r.src_subaddr			<< "]";	}
			if (r.dst_subaddr.tag != 0)		{ L << "[dst_subaddr:"			<< r.dst_subaddr			<< "]";	}
			if (r.user_resp_code.tag != 0)		{ L << "[user_resp_code:"			<< r.user_resp_code			<< "]";	}
			if (r.display_time.tag != 0)		{ L << "[display_time:"			<< r.display_time			<< "]"; }
			if (r.sms_signal.tag != 0)			{ L << "[sms_signal:"				<< r.sms_signal				<< "]";	}
			if (r.ms_validity.tag != 0)			{ L << "[ms_validity:"				<< r.ms_validity			<< "]"; }
			if (r.ms_msg_wait_fclts.tag!=0)		{ L << "[ms_msg_wait_fclts:"		<< r.ms_msg_wait_fclts		<< "]"; }
			if (r.number_of_msgs.tag != 0)		{ L << "[number_of_msgs:"			<< r.number_of_msgs			<< "]";	}
			if (r.alert_on_msg_delivery.tag!=0) { L << "[alert_on_msg_delivery:"	<< r.alert_on_msg_delivery	<< "]"; }
			if (r.lang_ind.tag != 0)			{ L << "[lang_ind:"				<< r.lang_ind				<< "]"; }
			if (r.its_reply_type.tag != 0)		{ L << "[its_reply_type:"			<< r.its_reply_type			<< "]"; }
			if (r.its_session_info.tag != 0)	{ L << "[its_session_info:"		<< r.its_session_info		<< "]"; }
			if (r.ussd_serv_op.tag != 0)		{ L << "[ussd_serv_op:"			<< r.ussd_serv_op			<< "]"; }
			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const data_sm & r) {
			L	<< "data_sm: "
				<< r.command
				<< "[serv_type:"				<< r.serv_type													<< "]"
				<< "[src_addr_ton:"			<< std::bitset<8>(r.src_addr_ton)								<< "]"
				<< "[src_addr_npi:"			<< std::bitset<8>(r.src_addr_npi)								<< "]"
				<< "[src_addr:"				<< r.src_addr													<< "]"
				<< "[dst_addr_ton:"			<< std::bitset<8>(r.dst_addr_ton)								<< "]"
				<< "[dst_addr_npi:"			<< std::bitset<8>(r.dst_addr_npi)								<< "]"
				<< "[dst_addr:"				<< r.dst_addr													<< "]"
				<< "[esm_class:"				<< std::bitset<8>(r.esm_class)									<< "]"
				<< "[registered_delivery:"		<< static_cast<bool>(r.registered_delivery)						<< "]"
				<< "[data_coding:"				<< std::bitset<8>(r.data_coding)								<< "]";
			if (r.user_msg_reference.tag != 0)	{ L << "[user_msg_reference:"		<< r.user_msg_reference		<< "]";	}
			if (r.src_port.tag != 0)			{ L << "[src_port:"				<< r.src_port				<< "]"; }
			if (r.src_addr_subunit.tag != 0)	{ L << "[src_addr_subunit:"		<< r.src_addr_subunit		<< "]"; }
			if (r.dst_port.tag != 0)			{ L << "[dst_port:"				<< r.dst_port				<< "]"; }
			if (r.dst_addr_subunit.tag != 0)	{ L << "[dst_addr_subunit:"		<< r.dst_addr_subunit		<< "]";	}
			if (r.sar_msg_ref_num.tag != 0)		{ L << "[sar_msg_ref_num:"			<< r.sar_msg_ref_num		<< "]";	}
			if (r.sar_total_segments.tag != 0)	{ L << "[sar_total_segments:"		<< r.sar_total_segments		<< "]";	}
			if (r.sar_segment_seqnum.tag != 0)	{ L << "[sar_segment_seqnum:"		<< r.sar_segment_seqnum		<< "]";	}
			if (r.more_msgs_to_send.tag != 0)	{ L << "[more_msgs_to_send:"		<< r.more_msgs_to_send		<< "]"; }
			if (r.payload_type.tag != 0)		{ L << "[payload_type:"			<< r.payload_type			<< "]"; }
			if (r.msg_payload.tag != 0)			{ L << "[msg_payload:"
				<< std::string(r.payload_type.val, r.payload_type.val +	r.payload_type.len) << "]"; }
			if (r.privacy_ind.tag != 0)			{ L << "[privacy_ind:"				<< r.privacy_ind			<< "]"; }
			if (r.callback_num.tag != 0)		{ L << "[callback_num:"			<< r.callback_num			<< "]";	}
			if (r.callback_num_pres_ind.tag!=0) { L << "[callback_num_pres_ind:"	<< r.callback_num_pres_ind 	<< "]"; }
			if (r.callback_num_atag.tag != 0)	{ L << "[callback_num_atag:"		<< r.callback_num_atag		<< "]";	}
			if (r.src_subaddr.tag != 0)			{ L << "[src_subaddr:"				<< r.src_subaddr			<< "]";	}
			if (r.dst_subaddr.tag != 0)		{ L << "[dst_subaddr:"			<< r.dst_subaddr			<< "]";	}
			if (r.user_resp_code.tag != 0)		{ L << "[user_resp_code:"			<< r.user_resp_code			<< "]";	}
			if (r.display_time.tag != 0)		{ L << "[display_time:"			<< r.display_time			<< "]"; }
			if (r.sms_signal.tag != 0)			{ L << "[sms_signal:"				<< r.sms_signal				<< "]";	}
			if (r.ms_validity.tag != 0)			{ L << "[ms_validity:"				<< r.ms_validity			<< "]"; }
			if (r.ms_msg_wait_fclts.tag!=0)		{ L << "[ms_msg_wait_fclts:"		<< r.ms_msg_wait_fclts		<< "]"; }
			if (r.number_of_msgs.tag != 0)		{ L << "[number_of_msgs:"			<< r.number_of_msgs			<< "]";	}
			if (r.alert_on_msg_delivery.tag!=0) { L << "[alert_on_msg_delivery:"	<< r.alert_on_msg_delivery	<< "]"; }
			if (r.lang_ind.tag != 0)			{ L << "[lang_ind:"				<< r.lang_ind				<< "]"; }
			if (r.its_reply_type.tag != 0)		{ L << "[its_reply_type:"			<< r.its_reply_type			<< "]"; }
			if (r.its_session_info.tag != 0)	{ L << "[its_session_info:"		<< r.its_session_info		<< "]"; }
			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const submit_sm_r & r) {
			L
			<< "submit_sm_r:"
			<< r.command
			<< "[msg_id:"<< std::string(r.msg_id, r.msg_id + r.msg_id_len) << "]";

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const deliver_sm_r & r) {
			L
			<< "deliver_sm_r:"
			<< r.command
			<< "[msg_id:"<< std::string(r.msg_id, r.msg_id + r.msg_id_len) << "]";

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const bind_transmitter & r) {
			L<< "bind_transmitter:"
			 << "[command.len: "		<< r.command.len								<< "]"
			 << "[command.id: "		<< r.command.id									<< "]"
			 << "[command.status: "	<< r.command.status								<< "]"
			 << "[command.seqno: "		<< r.command.seqno								<< "]"
			 << "[sys_id: "			<< r.sys_id										<< "]"
			 << "[password: "			<< r.password									<< "]"
			 << "[sys_type: "			<< r.sys_type									<< "]"
			 << "[interface_version: "	<< static_cast<unsigned>(r.interface_version)	<< "]"
			 << "[addr_ton: "			<< static_cast<unsigned>(r.addr_ton)			<< "]"
			 << "[addr_npi: "			<< static_cast<unsigned>(r.addr_npi)			<< "]"
			 << "[addr_range: "		<< r.addr_range									<< "]";

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const bind_transmitter_r & r) {
			L 
			<< "bind_transmitter_r:"
			<< "[command.len: "<< r.command.len << "]"
			<< "[command.id: "<< r.command.id << "]"
			<< "[command.status: "<< r.command.status << "]"
			<< "[command.seqno: "<< r.command.seqno << "]"

			<< "[sys_id: "<< r.sys_id << "]"

			<< "[sc_interface_version.tag: "<< r.sc_interface_version.tag << "]"
			<< "[sc_interface_version.len: "<< r.sc_interface_version.len << "]"
			<< "[sc_interface_version.val: "<< static_cast<unsigned>(r.sc_interface_version.val) << "]";

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const bind_receiver & r) {
			L
			<< "bind_receiver:"
			<< "[command.len: "<< r.command.len << "]"
			<< "[command.id: "<< r.command.id << "]"
			<< "[command.status: "<< r.command.status << "]"
			<< "[command.seqno: "<< r.command.seqno << "]"

			<< "[sys_id: "<< r.sys_id << "]"
			<< "[password: "<< r.password << "]"
			<< "[sys_type: "<< r.sys_type << "]"
			<< "[interface_version: "<< static_cast<unsigned>(r.interface_version) << "]"
			<< "[addr_ton: "<< static_cast<unsigned>(r.addr_ton) << "]"
			<< "[addr_npi: "<< static_cast<unsigned>(r.addr_npi) << "]"
			<< "[addr_range: "<< r.addr_range << "]";

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const bind_receiver_r & r) {
			L
			<< "bind_receiver_r:"
			<< "[command.len: "<< r.command.len << "]"
			<< "[command.id: "<< r.command.id << "]"
			<< "[command.status: "<< r.command.status << "]"
			<< "[command.seqno: "<< r.command.seqno << "]"

			<< "[sys_id: "<< r.sys_id << "]"
			<< "[sc_interface_version.tag: "<< r.sc_interface_version.tag << "]"
			<< "[sc_interface_version.len: "<< r.sc_interface_version.len << "]"
			<< "[sc_interface_version.val: "<< static_cast<unsigned>(r.sc_interface_version.val) << "]";

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const bind_transceiver & r) {
			L
			<< "bind_transceiver:"
			<< "[command.len: "<< r.command.len << "]"
			<< "[command.id: "<< r.command.id << "]"
			<< "[command.status: "<< r.command.status << "]"
			<< "[command.seqno: "<< r.command.seqno << "]"

			<< "[sys_id: "<< r.sys_id << "]"
			<< "[password: "<< r.password << "]"
			<< "[sys_type: "<< r.sys_type << "]"
			<< "[interface_version: "<< static_cast<unsigned>(r.interface_version) << "]"
			<< "[addr_ton: "<< static_cast<unsigned>(r.addr_ton) << "]"
			<< "[addr_npi: "<< static_cast<unsigned>(r.addr_npi) << "]"
			<< "[addr_range: "<< r.addr_range << "]";

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const bind_transceiver_r & r) {

			L
			<< "bind_transceiver_r:"
			<< "[command.len: "<< r.command.len << "]"
			<< "[command.id: "<< r.command.id << "]"
			<< "[command.status: "<< r.command.status << "]"
			<< "[command.seqno: "<< r.command.seqno << "]"
			<< "[sys_id: "<< r.sys_id << "]";

			if (r.sc_interface_version.tag == option::sc_interface_version) {
				L << "[sc_interface_version.val: "<< static_cast<unsigned>(r.sc_interface_version.val) << "]";
			}

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const unbind & r) {

			L
			<< "unbind:"
			<< "[command.len: "<< r.command.len << "]"
			<< "[command.id: "<< r.command.id << "]"
			<< "[command.status: "<< r.command.status << "]"
			<< "[command.seqno: "<< r.command.seqno << "]";

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const unbind_r & r) {

			L
			<< "unbind_r:"
			<< "[command.len: "<< r.command.len << "]"
			<< "[command.id: "<< r.command.id << "]"
			<< "[command.status: "<< r.command.status << "]"
			<< "[command.seqno: "<< r.command.seqno << "]";

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const outbind & r) {

			L
			<< "outbind:"
			<< "[command.len: "<< r.command.len << "]"
			<< "[command.id: "<< r.command.id << "]"
			<< "[command.status: "<< r.command.status << "]"
			<< "[command.seqno: "<< r.command.seqno << "]"
			<< "[sys_id: "<< r.sys_id << "]"
			<< "[sys_password: "<< r.password << "]";

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const generic_nack & r) {

			L
			<< "generic_nack:"
			<< "[command.len: "<< r.command.len << "]"
			<< "[command.id: "<< r.command.id << "]"
			<< "[command.status: "<< r.command.status << "]"
			<< "[command.seqno: "<< r.command.seqno << "]";

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const deliver_sm & r) {
			L	<< "deliver_sm: "
				<< r.command
				<< "[serv_type:"				<< r.serv_type													<< "]"
				<< "[src_addr_ton:"			<< std::bitset<8>(r.src_addr_ton)								<< "]"
				<< "[src_addr_npi:"			<< std::bitset<8>(r.src_addr_npi)								<< "]"
				<< "[src_addr:"				<< r.src_addr													<< "]"
				<< "[dst_addr_ton:"			<< std::bitset<8>(r.dst_addr_ton)								<< "]"
				<< "[dst_addr_npi:"			<< std::bitset<8>(r.dst_addr_npi)								<< "]"
				<< "[dst_addr:"				<< r.dst_addr													<< "]"
				<< "[esm_class:"				<< std::bitset<8>(r.esm_class)									<< "]"
				<< "[protocol_id:"				<< static_cast<int>(r.protocol_id)								<< "]"
				<< "[priority_flag:"			<< static_cast<int>(r.priority_flag)							<< "]"
				<< "[schedule_delivery_time:"	<< r.schedule_delivery_time										<< "]"
				<< "[validity_period:"			<< r.validity_period											<< "]"
				<< "[registered_delivery:"		<< static_cast<bool>(r.registered_delivery)						<< "]"
				<< "[replace_if_present_flag:"<< static_cast<bool>(r.replace_if_present_flag)					<< "]"
				<< "[data_coding:"				<< std::bitset<8>(r.data_coding)								<< "]"
				<< "[sm_default_msg_id:"		<< static_cast<int>(r.sm_default_msg_id)						<< "]"
				<< "[short_msg_len:"			<< static_cast<int>(r.short_msg_len)							<< "]"
				<< "[short_msg:"				<< std::string(r.short_msg, r.short_msg + r.short_msg_len)		<< "]";
			if (r.user_msg_reference.tag != 0)	{ L << "[user_msg_reference:"		<< r.user_msg_reference		<< "]";	}
			if (r.src_port.tag != 0)			{ L << "[src_port:"				<< r.src_port				<< "]"; }
			if (r.dst_port.tag != 0)			{ L << "[dst_port:"				<< r.dst_port				<< "]"; }
			if (r.sar_msg_ref_num.tag != 0)		{ L << "[sar_msg_ref_num:"			<< r.sar_msg_ref_num		<< "]";	}
			if (r.sar_total_segments.tag != 0)	{ L << "[sar_total_segments:"		<< r.sar_total_segments		<< "]";	}
			if (r.sar_segment_seqnum.tag != 0)	{ L << "[sar_segment_seqnum:"		<< r.sar_segment_seqnum		<< "]";	}
			if (r.payload_type.tag != 0)		{ L << "[payload_type:"			<< r.payload_type			<< "]"; }
			if (r.privacy_ind.tag != 0)			{ L << "[privacy_ind:"				<< r.privacy_ind			<< "]"; }
			if (r.callback_num.tag != 0)		{ L << "[callback_num:"			<< r.callback_num			<< "]";	}
			if (r.src_subaddr.tag != 0)			{ L << "[src_subaddr:"				<< r.src_subaddr			<< "]";	}
			if (r.dst_subaddr.tag != 0)		{ L << "[dst_subaddr:"			<< r.dst_subaddr			<< "]";	}
			if (r.user_resp_code.tag != 0)		{ L << "[user_resp_code:"			<< r.user_resp_code			<< "]";	}
			if (r.lang_ind.tag != 0)			{ L << "[lang_ind:"				<< r.lang_ind				<< "]"; }
			if (r.its_session_info.tag != 0)	{ L << "[its_session_info:"		<< r.its_session_info		<< "]"; }
			if (r.network_error_code.tag != 0) 	{ L << "[network_error_code:"	<< r.network_error_code		<< "]"; }
			if (r.msg_state.tag != 0)			{ L << "[msg_state:"			<< r.msg_state				<< "]"; }
			if (r.receipted_msg_id.tag != 0)	{ L << "[receipted_msg_id:"		<< r.receipted_msg_id		<< "]"; }
			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const data_sm_r & r) {
			L
			<< "data_sm_r:"
			<< r.command
			<< "[msg_id:"<< std::string(r.msg_id, r.msg_id + r.msg_id_len) << "]";
			if (r.delivery_failure_reason.tag != 0) {
				L
				<< "[delivery_failure_reason:"
				<< static_cast<unsigned>(r.delivery_failure_reason.val) << "]";
			}
			if (r.network_error_code.tag != 0) {
				L
				<< "[network_error_code:"
				<< std::string(r.network_error_code.val,
						r.network_error_code.val + r.network_error_code.len)
				<< "]";
			}
			if (r.additional_status_info_text.tag != 0) {
				L
				<< "[additional_status_info_text:"
				<< std::string(r.additional_status_info_text.val
					, r.additional_status_info_text.val + r.additional_status_info_text.len)
				<< "]";
			}
			if (r.dpf_result.tag != 0) {
				L << "[dpf_result:"<< static_cast<unsigned>(r.dpf_result.val) << "]";
			}

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const query_sm & r) {

			L
			<< "query_sm:"
			<< r.command
			<< "[msg_id:"
			<< std::string(r.msg_id, r.msg_id+r.msg_id_len)
			<< "]"
			<< "[src_addr_ton:"<< static_cast<unsigned>(r.src_addr_ton) << "]"
			<< "[src_addr_npi:"<< static_cast<unsigned>(r.src_addr_npi) << "]"
			<< "[src_addr:"
			<< std::string(r.msg_id, r.msg_id+r.msg_id_len)
			<< "]";

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const query_sm_r & r) {

			L
			<< "query_sm_r:"
			<< r.command
			<< "[msg_id:"<< std::string(r.msg_id, r.msg_id+r.msg_id_len) << "]"
			<< "[final_date:"<< std::string(r.final_date, r.final_date+r.final_date_len) << "]"
			<< "[msg_state:"<< static_cast<unsigned>(r.msg_state) << "]"
			<< "[error_code:"<< static_cast<unsigned>(r.error_code) << "]";

			return L;
		}
		
		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const cancel_sm & r) {

			L
			<< "cancel_sm:"
			<< r.command
			<< "[serv_type:"<< std::string(r.serv_type, r.serv_type+r.serv_type_len) << "]"
			<< "[msg_id:"<< std::string(r.msg_id, r.msg_id+r.msg_id_len) << "]"
			<< "[src_addr_ton:"<< static_cast<unsigned>(r.src_addr_ton) << "]"
			<< "[src_addr_npi:"<< static_cast<unsigned>(r.src_addr_npi) << "]"
			<< "[src_addr:"<< std::string(r.src_addr, r.src_addr+r.src_addr_len) << "]"
			<< "[dst_addr_ton:"<< static_cast<unsigned>(r.dst_addr_ton) << "]"
			<< "[dst_addr_npi:"<< static_cast<unsigned>(r.dst_addr_npi) << "]"
			<< "[dst_addr:"<< std::string(r.dst_addr, r.dst_addr+r.dst_addr_len) << "]";

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const cancel_sm_r & r) {
			L
			<< "cancel_sm_r:"
			<< r.command;

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const replace_sm & r) {

			L
			<< "replace_sm:"
			<< r.command
			<< "[msg_id:"<< std::string(r.msg_id, r.msg_id+r.msg_id_len) << "]"
			<< "[src_addr_ton:"<< static_cast<unsigned>(r.src_addr_ton) << "]"
			<< "[src_addr_npi:"<< static_cast<unsigned>(r.src_addr_npi) << "]"
			<< "[src_addr:"<< std::string(r.src_addr, r.src_addr+r.src_addr_len) << "]"
			<< "[schedule_delivery_time:"
			<< std::string(r.schedule_delivery_time,
					r.schedule_delivery_time + r.schedule_delivery_time_len)
			<< "]"
			<< "[validity_period:"
			<< std::string(r.validity_period,
					r.validity_period + r.validity_period_len)
			<< "]"
			<< "[registered_delivery:"
			<< static_cast<unsigned>(r.registered_delivery)
			<< "]"
			<< "[sm_default_msg_id:"
			<< static_cast<unsigned>(r.sm_default_msg_id)
			<< "]"
			<< "[short_msg_len:"
			<< static_cast<unsigned>(r.short_msg_len)
			<< "]"
			<< "[short_msg:"
			<< std::string(r.short_msg,
					r.short_msg + r.short_msg_len)
			<< "]";

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const replace_sm_r & r) {

			L
			<< "replace_sm_r:"
			<< r.command;

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const enquire_link & r) {

			L
			<< "enquire_link:"
			<< r.command;

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const enquire_link_r & r) {

			L
			<< "enquire_link_r:"
			<< r.command;

			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const alert_notification & r) {

			L
			<< "alert_notification:"
			<< r.command
			<< "[src_addr_ton:"<< static_cast<unsigned>(r.src_addr_ton) << "]"
			<< "[src_addr_npi:"<< static_cast<unsigned>(r.src_addr_npi) << "]"
			<< "[src_addr:"<< std::string(r.src_addr, r.src_addr+r.src_addr_len) << "]"
			<< "[esme_addr_ton:"<< static_cast<unsigned>(r.esme_addr_ton) << "]"
			<< "[esme_addr_npi:"<< static_cast<unsigned>(r.esme_addr_npi) << "]"
			<< "[esme_addr:"<< std::string(r.esme_addr, r.esme_addr+r.esme_addr_len) << "]";
			if (r.ms_availability_status.tag != 0) {
				L
				<< "[ms_availability_status:"
				<< static_cast<unsigned>(r.ms_availability_status.val) << "]";
			}

			return L;
		}
	}

	const std::size_t bind_pdu_min_len = sizeof(pdu) + sizeof(bin::u8_t) * 7;

	template <class LogT>
	class tlv_writer {
		protected:
			LogT & L;

		public:
			tlv_writer(LogT & l): L(l) {}
			virtual ~tlv_writer() {}

			bin::u8_t * write_tlv_u8(bin::u8_t * buf, const tlv<bin::u8_t> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::cp_u8(buf, ascbuf(t.val));
				return buf;
			}

			bin::u8_t * write_tlv_u16(bin::u8_t * buf, const tlv<bin::u16_t> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::cp_u16(buf, ascbuf(t.val));
				return buf;
			}

			bin::u8_t * write_tlv_u32(bin::u8_t * buf, const tlv<bin::u32_t> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::cp_u32(buf, ascbuf(t.val));
				return buf;
			}

			bin::u8_t * write_tlv_s256(bin::u8_t * buf, const tlv<bin::u8_t[256]> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}

			bin::u8_t * write_tlv_s65(bin::u8_t * buf, const tlv<bin::u8_t[65]> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}

			bin::u8_t * write_tlv_s23(bin::u8_t * buf, const tlv<bin::u8_t[23]> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}

			bin::u8_t * write_tlv_s3(bin::u8_t * buf, const tlv<bin::u8_t[3]> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}

			bin::u8_t * write_tlv_ptr(bin::u8_t * buf, const tlv<bin::u8_t *> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}

			bin::u8_t * write_tlv_s19(bin::u8_t * buf, const tlv<bin::u8_t[19]> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}

			bin::u8_t * write_tlv_s2(bin::u8_t * buf, const tlv<bin::u8_t[2]> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}

			bin::u8_t * write_tlv_s1(bin::u8_t * buf, const tlv<bin::u8_t[1]> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}
	};
	template <class LogT>
	class writer: public tlv_writer<LogT> {
		using tlv_writer<LogT>::L;
		using tlv_writer<LogT>::write_tlv_s256;
		using tlv_writer<LogT>::write_tlv_s65;
		using tlv_writer<LogT>::write_tlv_u32;
		using tlv_writer<LogT>::write_tlv_s23;
		using tlv_writer<LogT>::write_tlv_s19;
		using tlv_writer<LogT>::write_tlv_u16;
		using tlv_writer<LogT>::write_tlv_ptr;
		using tlv_writer<LogT>::write_tlv_u8;
		using tlv_writer<LogT>::write_tlv_s3;
		using tlv_writer<LogT>::write_tlv_s2;
		using tlv_writer<LogT>::write_tlv_s1;

		public:
			writer(LogT & l): tlv_writer<LogT>(l) {}
			virtual ~writer() {}

			bin::u8_t * write(bin::u8_t * buf, bin::u8_t * bend
				, const bind_transmitter & msg) {
				return write_bind(buf, bend, msg);
			}

			bin::u8_t * write(bin::u8_t * buf, const bin::u8_t * bend
				, const bind_transmitter_r & msg) {
				return write_bind_r(buf, bend, msg);
			}

			bin::u8_t * write(bin::u8_t * buf, const bin::u8_t * bend
					, const outbind & msg) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);

				RETURN_NULL_IF(buf + msg.sys_id_len > bend);
				buf = w::scpy(buf, msg.sys_id, msg.sys_id_len);

				RETURN_NULL_IF(buf + msg.password_len > bend);
				buf = w::scpy(buf, msg.password, msg.password_len);

				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, const bin::u8_t * bend
				, const bind_receiver & msg) {
				return write_bind(buf, bend, msg);
			}

			bin::u8_t * write(bin::u8_t * buf, const bin::u8_t * bend
				, const bind_receiver_r & msg) {
				return write_bind_r(buf, bend, msg);
			}

			bin::u8_t * write(bin::u8_t * buf, const bin::u8_t * bend
				, const bind_transceiver & msg) {
				return write_bind(buf, bend, msg);
			}

			bin::u8_t * write(bin::u8_t * buf, const bin::u8_t * bend
				, const bind_transceiver_r & msg) {
				return write_bind_r(buf, bend, msg);
			}

			bin::u8_t * write(bin::u8_t * buf, bin::u8_t * bend
					, const unbind & msg) {
				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);
				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, bin::u8_t * bend
					, const unbind_r & msg) {
				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);
				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, bin::u8_t * bend
				, const generic_nack & msg) {
				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);
				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf , bin::u8_t * bend
					, const submit_sm & msg) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);

				RETURN_NULL_IF(buf + msg.serv_type_len > bend);
				buf = w::scpy(buf, msg.serv_type, msg.serv_type_len);

				RETURN_NULL_IF(buf + sizeof(msg.src_addr_ton) > bend);
				buf = w::cp_u8(buf, &msg.src_addr_ton);

				RETURN_NULL_IF(buf + sizeof(msg.src_addr_npi) > bend);
				buf = w::cp_u8(buf, &msg.src_addr_npi);

				RETURN_NULL_IF(buf + msg.src_addr_len > bend);
				buf = w::scpy(buf, msg.src_addr, msg.src_addr_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 2 > bend);
				buf = w::cp_u8(buf, &msg.dst_addr_ton);
				buf = w::cp_u8(buf, &msg.dst_addr_npi);

				RETURN_NULL_IF(buf + msg.dst_addr_len > bend);
				buf = w::scpy(buf, msg.dst_addr, msg.dst_addr_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 3 > bend);
				buf = w::cp_u8(buf, &msg.esm_class);
				buf = w::cp_u8(buf, &msg.protocol_id);
				buf = w::cp_u8(buf, &msg.priority_flag);

				RETURN_NULL_IF(buf + msg.schedule_delivery_time_len > bend);
				buf = w::scpy(buf, msg.schedule_delivery_time
						, msg.schedule_delivery_time_len);

				RETURN_NULL_IF(buf + msg.validity_period_len > bend);
				buf = w::scpy(buf, msg.validity_period
						, msg.validity_period_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 5 > bend);
				buf = w::cp_u8(buf, &msg.registered_delivery);
				buf = w::cp_u8(buf, &msg.replace_if_present_flag);
				buf = w::cp_u8(buf, &msg.data_coding);
				buf = w::cp_u8(buf, &msg.sm_default_msg_id);
				buf = w::cp_u8(buf, &msg.short_msg_len);

				RETURN_NULL_IF(buf + msg.short_msg_len > bend);
				buf = w::scpy(buf, msg.short_msg, msg.short_msg_len);

				if (msg.user_msg_reference.tag == option::user_msg_reference) {
					RETURN_NULL_IF(buf + msg.user_msg_reference.raw_size() > bend);
					buf = write_tlv_u16(buf, msg.user_msg_reference);
				}
				if (msg.src_port.tag == option::src_port) {
					RETURN_NULL_IF(buf + msg.src_port.raw_size() > bend);
					buf = write_tlv_u16(buf, msg.src_port);
				}
				if (msg.src_addr_subunit.tag == option::src_addr_subunit) {
					RETURN_NULL_IF(buf + msg.src_addr_subunit.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.src_addr_subunit);
				}
				if (msg.dst_port.tag == option::dst_port) {
					RETURN_NULL_IF(buf + msg.dst_port.raw_size() > bend);
					buf = write_tlv_u16(buf, msg.dst_port);
				}
				if (msg.dst_addr_subunit.tag == option::dst_addr_subunit) {
					RETURN_NULL_IF(buf + msg.dst_addr_subunit.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.dst_addr_subunit);
				}
				if (msg.sar_msg_ref_num.tag == option::sar_msg_ref_num) {
					RETURN_NULL_IF(buf + msg.sar_msg_ref_num.raw_size() > bend);
					buf = write_tlv_u16(buf, msg.sar_msg_ref_num);
				}
				if (msg.sar_total_segments.tag == option::sar_total_segments) {
					RETURN_NULL_IF(buf + msg.sar_total_segments.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.sar_total_segments);
				}
				if (msg.sar_segment_seqnum.tag == option::sar_segment_seqnum) {
					RETURN_NULL_IF(buf + msg.sar_segment_seqnum.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.sar_segment_seqnum);
				}
				if (msg.more_msgs_to_send.tag == option::more_msgs_to_send) {
					RETURN_NULL_IF(buf + msg.more_msgs_to_send.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.more_msgs_to_send);
				}
				if (msg.payload_type.tag == option::payload_type) {
					RETURN_NULL_IF(buf + msg.payload_type.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.payload_type);
				}
				if (msg.msg_payload.tag == option::msg_payload) {
					RETURN_NULL_IF(buf + msg.msg_payload.raw_size() > bend);
					buf = write_tlv_ptr(buf, msg.msg_payload);
				}
				if (msg.privacy_ind.tag == option::privacy_ind) {
					RETURN_NULL_IF(buf + msg.privacy_ind.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.privacy_ind);
				}
				if (msg.dst_subaddr.tag == option::dst_subaddr) {
					RETURN_NULL_IF(buf + msg.dst_subaddr.raw_size() > bend);
					buf = write_tlv_s23(buf, msg.dst_subaddr);
				}
				if (msg.callback_num.tag == option::callback_num) {
					RETURN_NULL_IF(buf + msg.callback_num.raw_size() > bend);
					buf = write_tlv_s19(buf, msg.callback_num);
				}
				if (msg.callback_num_pres_ind.tag == option::callback_num_pres_ind) {
					RETURN_NULL_IF(buf + msg.callback_num_pres_ind.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.callback_num_pres_ind);
				}
				if (msg.callback_num_atag.tag == option::callback_num_atag) {
					RETURN_NULL_IF(buf + msg.callback_num_atag.raw_size() > bend);
					buf = write_tlv_s65(buf, msg.callback_num_atag);
				}
				if (msg.src_subaddr.tag == option::src_subaddr) {
					RETURN_NULL_IF(buf + msg.src_subaddr.raw_size() > bend);
					buf = write_tlv_s23(buf, msg.src_subaddr);
				}
				if (msg.user_resp_code.tag == option::user_resp_code) {
					RETURN_NULL_IF(buf + msg.user_resp_code.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.user_resp_code);
				}
				if (msg.display_time.tag == option::display_time) {
					RETURN_NULL_IF(buf + msg.display_time.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.display_time);
				}
				if (msg.sms_signal.tag == option::sms_signal) {
					RETURN_NULL_IF(buf + msg.sms_signal.raw_size() > bend);
					buf = write_tlv_u16(buf, msg.sms_signal);
				}
				if (msg.ms_validity.tag == option::ms_validity) {
					RETURN_NULL_IF(buf + msg.ms_validity.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.ms_validity);
				}
				if (msg.ms_msg_wait_fclts.tag == option::ms_msg_wait_fclts) {
					RETURN_NULL_IF(buf + msg.ms_msg_wait_fclts.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.ms_msg_wait_fclts);
				}
				if (msg.number_of_msgs.tag == option::number_of_msgs) {
					RETURN_NULL_IF(buf + msg.number_of_msgs.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.number_of_msgs);
				}
				if (msg.alert_on_msg_delivery.tag == option::alert_on_msg_delivery) {
					RETURN_NULL_IF(buf + msg.alert_on_msg_delivery.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.alert_on_msg_delivery);
				}
				if (msg.lang_ind.tag == option::lang_ind) {
					RETURN_NULL_IF(buf + msg.lang_ind.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.lang_ind);
				}
				if (msg.its_reply_type.tag == option::its_reply_type) {
					RETURN_NULL_IF(buf + msg.its_reply_type.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.its_reply_type);
				}
				if (msg.its_session_info.tag == option::its_session_info) {
					RETURN_NULL_IF(buf + msg.its_session_info.raw_size() > bend);
					buf = write_tlv_s2(buf, msg.its_session_info);
				}
				if (msg.ussd_serv_op.tag == option::ussd_serv_op) {
					RETURN_NULL_IF(buf + msg.ussd_serv_op.raw_size() > bend);
					buf = write_tlv_s1(buf, msg.ussd_serv_op);
				}
				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, bin::u8_t * bend,
					const submit_sm_r & msg) {
				using namespace bin;
				RETURN_NULL_IF(buf + sizeof(msg.command) > bend)
				buf = write_pdu(buf, msg.command);
				RETURN_NULL_IF(buf + msg.msg_id_len > bend);
				buf = w::scpy(buf, msg.msg_id, msg.msg_id_len);
				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, bin::u8_t * bend
				, const deliver_sm & msg) {

				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);

				RETURN_NULL_IF(buf + msg.serv_type_len > bend);
				buf = w::scpy(buf, msg.serv_type, msg.serv_type_len);

				RETURN_NULL_IF(buf + sizeof(msg.src_addr_ton) > bend);
				buf = w::cp_u8(buf, &msg.src_addr_ton);

				RETURN_NULL_IF(buf + sizeof(msg.src_addr_npi) > bend);
				buf = w::cp_u8(buf, &msg.src_addr_npi);

				RETURN_NULL_IF(buf + msg.src_addr_len > bend);
				buf = w::scpy(buf, msg.src_addr, msg.src_addr_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 2 > bend);
				buf = w::cp_u8(buf, &msg.dst_addr_ton);
				buf = w::cp_u8(buf, &msg.dst_addr_npi);

				RETURN_NULL_IF(buf + msg.dst_addr_len > bend);
				buf = w::scpy(buf, msg.dst_addr, msg.dst_addr_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 3 > bend);
				buf = w::cp_u8(buf, &msg.esm_class);
				buf = w::cp_u8(buf, &msg.protocol_id);
				buf = w::cp_u8(buf, &msg.priority_flag);

				RETURN_NULL_IF(buf + msg.schedule_delivery_time_len > bend);
				buf = w::scpy(buf, msg.schedule_delivery_time
						, msg.schedule_delivery_time_len);

				RETURN_NULL_IF(buf + msg.validity_period_len > bend);
				buf = w::scpy(buf, msg.validity_period
						, msg.validity_period_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 5 > bend);
				buf = w::cp_u8(buf, &msg.registered_delivery);
				buf = w::cp_u8(buf, &msg.replace_if_present_flag);
				buf = w::cp_u8(buf, &msg.data_coding);
				buf = w::cp_u8(buf, &msg.sm_default_msg_id);
				buf = w::cp_u8(buf, &msg.short_msg_len);

				RETURN_NULL_IF(buf + msg.short_msg_len > bend);
				buf = w::scpy(buf, msg.short_msg, msg.short_msg_len);

				if (msg.user_msg_reference.tag == option::user_msg_reference) {
					RETURN_NULL_IF(buf + msg.user_msg_reference.raw_size() > bend);
					buf = write_tlv_u16(buf, msg.user_msg_reference);
				}
				if (msg.src_port.tag == option::src_port) {
					RETURN_NULL_IF(buf + msg.src_port.raw_size() > bend);
					buf = write_tlv_u16(buf, msg.src_port);
				}
				if (msg.dst_port.tag == option::dst_port) {
					RETURN_NULL_IF(buf + msg.dst_port.raw_size() > bend);
					buf = write_tlv_u16(buf, msg.dst_port);
				}
				if (msg.sar_msg_ref_num.tag == option::sar_msg_ref_num) {
					RETURN_NULL_IF(buf + msg.sar_msg_ref_num.raw_size() > bend);
					buf = write_tlv_u16(buf, msg.sar_msg_ref_num);
				}
				if (msg.sar_total_segments.tag == option::sar_total_segments) {
					RETURN_NULL_IF(buf + msg.sar_total_segments.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.sar_total_segments);
				}
				if (msg.sar_segment_seqnum.tag == option::sar_segment_seqnum) {
					RETURN_NULL_IF(buf + msg.sar_segment_seqnum.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.sar_segment_seqnum);
				}
				if (msg.payload_type.tag == option::payload_type) {
					RETURN_NULL_IF(buf + msg.payload_type.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.payload_type);
				}
				if (msg.privacy_ind.tag == option::privacy_ind) {
					RETURN_NULL_IF(buf + msg.privacy_ind.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.privacy_ind);
				}
				if (msg.callback_num.tag == option::callback_num) {
					RETURN_NULL_IF(buf + msg.callback_num.raw_size() > bend);
					buf = write_tlv_s19(buf, msg.callback_num);
				}
				if (msg.src_subaddr.tag == option::src_subaddr) {
					RETURN_NULL_IF(buf + msg.src_subaddr.raw_size() > bend);
					buf = write_tlv_s23(buf, msg.src_subaddr);
				}
				if (msg.user_resp_code.tag == option::user_resp_code) {
					RETURN_NULL_IF(buf + msg.user_resp_code.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.user_resp_code);
				}
				if (msg.lang_ind.tag == option::lang_ind) {
					RETURN_NULL_IF(buf + msg.lang_ind.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.lang_ind);
				}
				if (msg.its_session_info.tag == option::its_session_info) {
					RETURN_NULL_IF(buf + msg.its_session_info.raw_size() > bend);
					buf = write_tlv_s2(buf, msg.its_session_info);
				}
				if (msg.network_error_code.tag == option::network_error_code) {
					RETURN_NULL_IF(buf + msg.network_error_code.raw_size() > bend);
					buf = write_tlv_s3(buf, msg.network_error_code);
				}
				if (msg.msg_state.tag == option::msg_state) {
					RETURN_NULL_IF(buf + msg.msg_state.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.msg_state);
				}
				if (msg.receipted_msg_id.tag == option::receipted_msg_id) {
					RETURN_NULL_IF(buf + msg.receipted_msg_id.raw_size() > bend);
					buf = write_tlv_s65(buf, msg.receipted_msg_id);
				}
				if (msg.dst_subaddr.tag == option::dst_subaddr) {
					RETURN_NULL_IF(buf + msg.dst_subaddr.raw_size() > bend);
					buf = write_tlv_s23(buf, msg.dst_subaddr);
				}
				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, bin::u8_t * bend
					, const deliver_sm_r & msg) {
				using namespace bin;
				RETURN_NULL_IF(buf + sizeof(msg.command) > bend)
				buf = write_pdu(buf, msg.command);
				RETURN_NULL_IF(buf + msg.msg_id_len > bend);
				buf = w::scpy(buf, ascbuf(msg.msg_id), msg.msg_id_len);
				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, bin::u8_t * bend
					, const data_sm & msg) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);

				RETURN_NULL_IF(buf + msg.serv_type_len > bend);
				buf = w::scpy(buf, msg.serv_type, msg.serv_type_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*2 > bend);
				buf = w::cp_u8(buf, &msg.src_addr_ton);
				buf = w::cp_u8(buf, &msg.src_addr_npi);

				RETURN_NULL_IF(buf + msg.src_addr_len > bend);
				buf = w::scpy(buf, msg.src_addr, msg.src_addr_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 2 > bend);
				buf = w::cp_u8(buf, &msg.dst_addr_ton);
				buf = w::cp_u8(buf, &msg.dst_addr_npi);

				RETURN_NULL_IF(buf + msg.dst_addr_len > bend);
				buf = w::scpy(buf, msg.dst_addr, msg.dst_addr_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 3 > bend);
				buf = w::cp_u8(buf, &msg.esm_class);
				buf = w::cp_u8(buf, &msg.registered_delivery);
				buf = w::cp_u8(buf, &msg.data_coding);

				if (msg.src_port.tag == option::src_port) {
					RETURN_NULL_IF(buf + msg.src_port.raw_size() > bend);
					buf = write_tlv_u16(buf, msg.src_port);
				}
				if (msg.src_addr_subunit.tag == option::src_addr_subunit) {
					RETURN_NULL_IF(buf + msg.src_addr_subunit.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.src_addr_subunit);
				}
				if (msg.src_network_type.tag == option::src_network_type) {
					RETURN_NULL_IF(buf + msg.src_network_type.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.src_network_type);
				}
				if (msg.src_bearer_type.tag == option::src_bearer_type) {
					RETURN_NULL_IF(buf + msg.src_bearer_type.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.src_bearer_type);
				}
				if (msg.dst_addr_subunit.tag == option::dst_addr_subunit) {
					RETURN_NULL_IF(buf + msg.dst_addr_subunit.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.dst_addr_subunit);
				}
				if (msg.dst_port.tag == option::dst_port) {
					RETURN_NULL_IF(buf + msg.dst_port.raw_size() > bend);
					buf = write_tlv_u16(buf, msg.dst_port);
				}
				if (msg.sar_msg_ref_num.tag == option::sar_msg_ref_num) {
					RETURN_NULL_IF(buf + msg.sar_msg_ref_num.raw_size() > bend);
					buf = write_tlv_u16(buf, msg.sar_msg_ref_num);
				}
				if (msg.sar_total_segments.tag == option::sar_total_segments) {
					RETURN_NULL_IF(buf + msg.sar_total_segments.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.sar_total_segments);
				}
				if (msg.sar_segment_seqnum.tag == option::sar_segment_seqnum) {
					RETURN_NULL_IF(buf + msg.sar_segment_seqnum.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.sar_segment_seqnum);
				}
				if (msg.more_msgs_to_send.tag == option::more_msgs_to_send) {
					RETURN_NULL_IF(buf + msg.more_msgs_to_send.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.more_msgs_to_send);
				}
				if (msg.payload_type.tag == option::payload_type) {
					RETURN_NULL_IF(buf + msg.payload_type.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.payload_type);
				}
				if (msg.msg_payload.tag == option::msg_payload) {
					RETURN_NULL_IF(buf + msg.msg_payload.raw_size() > bend);
					buf = write_tlv_ptr(buf, msg.msg_payload);
				}
				if (msg.privacy_ind.tag == option::privacy_ind) {
					RETURN_NULL_IF(buf + msg.privacy_ind.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.privacy_ind);
				}
				if (msg.dst_subaddr.tag == option::dst_subaddr) {
					RETURN_NULL_IF(buf + msg.dst_subaddr.raw_size() > bend);
					buf = write_tlv_s23(buf, msg.dst_subaddr);
				}
				if (msg.callback_num.tag == option::callback_num) {
					RETURN_NULL_IF(buf + msg.callback_num.raw_size() > bend);
					buf = write_tlv_s19(buf, msg.callback_num);
				}
				if (msg.callback_num_pres_ind.tag == option::callback_num_pres_ind) {
					RETURN_NULL_IF(buf + msg.callback_num_pres_ind.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.callback_num_pres_ind);
				}
				if (msg.callback_num_atag.tag == option::callback_num_atag) {
					RETURN_NULL_IF(buf + msg.callback_num_atag.raw_size() > bend);
					buf = write_tlv_s65(buf, msg.callback_num_atag);
				}
				if (msg.src_subaddr.tag == option::src_subaddr) {
					RETURN_NULL_IF(buf + msg.src_subaddr.raw_size() > bend);
					buf = write_tlv_s23(buf, msg.src_subaddr);
				}
				if (msg.user_resp_code.tag == option::user_resp_code) {
					RETURN_NULL_IF(buf + msg.user_resp_code.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.user_resp_code);
				}
				if (msg.display_time.tag == option::display_time) {
					RETURN_NULL_IF(buf + msg.display_time.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.display_time);
				}
				if (msg.sms_signal.tag == option::sms_signal) {
					RETURN_NULL_IF(buf + msg.sms_signal.raw_size() > bend);
					buf = write_tlv_u16(buf, msg.sms_signal);
				}
				if (msg.user_msg_reference.tag == option::user_msg_reference) {
					RETURN_NULL_IF(buf + msg.user_msg_reference.raw_size() > bend);
					buf = write_tlv_u16(buf, msg.user_msg_reference);
				}
				if (msg.ms_validity.tag == option::ms_validity) {
					RETURN_NULL_IF(buf + msg.ms_validity.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.ms_validity);
				}
				if (msg.ms_msg_wait_fclts.tag == option::ms_msg_wait_fclts) {
					RETURN_NULL_IF(buf + msg.ms_msg_wait_fclts.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.ms_msg_wait_fclts);
				}
				if (msg.number_of_msgs.tag == option::number_of_msgs) {
					RETURN_NULL_IF(buf + msg.number_of_msgs.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.number_of_msgs);
				}
				if (msg.alert_on_msg_delivery.tag == option::alert_on_msg_delivery) {
					RETURN_NULL_IF(buf + msg.alert_on_msg_delivery.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.alert_on_msg_delivery);
				}
				if (msg.lang_ind.tag == option::lang_ind) {
					RETURN_NULL_IF(buf + msg.lang_ind.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.lang_ind);
				}
				if (msg.its_reply_type.tag == option::its_reply_type) {
					RETURN_NULL_IF(buf + msg.its_reply_type.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.its_reply_type);
				}
				if (msg.its_session_info.tag == option::its_session_info) {
					RETURN_NULL_IF(buf + msg.its_session_info.raw_size() > bend);
					buf = write_tlv_s2(buf, msg.its_session_info);
				}
				if (msg.src_telematics_id.tag == option::src_telematics_id) {
					RETURN_NULL_IF(buf + msg.src_telematics_id.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.src_telematics_id);
				}
				if (msg.qos_time_to_live.tag == option::qos_time_to_live) {
					RETURN_NULL_IF(buf + msg.qos_time_to_live.raw_size() > bend);
					buf = write_tlv_u32(buf, msg.qos_time_to_live);
				}
				if (msg.set_dpf.tag == option::set_dpf) {
					RETURN_NULL_IF(buf + msg.set_dpf.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.set_dpf);
				}
				if (msg.receipted_msg_id.tag == option::receipted_msg_id) {
					RETURN_NULL_IF(buf + msg.receipted_msg_id.raw_size() > bend);
					buf = write_tlv_s65(buf, msg.receipted_msg_id);
				}
				if (msg.msg_state.tag == option::msg_state) {
					RETURN_NULL_IF(buf + msg.msg_state.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.msg_state);
				}
				if (msg.network_error_code.tag == option::network_error_code) {
					RETURN_NULL_IF(buf + msg.network_error_code.raw_size() > bend);
					buf = write_tlv_s3(buf, msg.network_error_code);
				}
				if (msg.dst_network_type.tag == option::dst_network_type) {
					RETURN_NULL_IF(buf + msg.dst_network_type.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.dst_network_type);
				}
				if (msg.dst_bearer_type.tag == option::dst_bearer_type) {
					RETURN_NULL_IF(buf + msg.dst_bearer_type.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.dst_bearer_type);
				}
				if (msg.dst_telematics_id.tag == option::dst_telematics_id) {
					RETURN_NULL_IF(buf + msg.dst_telematics_id.raw_size() > bend);
					buf = write_tlv_u16(buf, msg.dst_telematics_id);
				}
				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, bin::u8_t * bend
					, const data_sm_r & msg) {

				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);

				RETURN_NULL_IF(buf + msg.msg_id_len > bend);
				buf = w::scpy(buf, msg.msg_id, msg.msg_id_len);

				if (msg.delivery_failure_reason.tag == option::delivery_failure_reason) {
					RETURN_NULL_IF(buf + msg.delivery_failure_reason.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.delivery_failure_reason);
				}
				if (msg.network_error_code.tag == option::network_error_code) {
					RETURN_NULL_IF(buf + msg.network_error_code.raw_size() > bend);
					buf = write_tlv_s3(buf, msg.network_error_code);
				}
				if (msg.additional_status_info_text.tag == option::additional_status_info_text) {
					RETURN_NULL_IF(buf + msg.additional_status_info_text.raw_size() > bend);
					buf = write_tlv_s256(buf, msg.additional_status_info_text);
				}
				if (msg.dpf_result.tag == option::dpf_result) {
					RETURN_NULL_IF(buf + msg.dpf_result.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.dpf_result);
				}

				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, const bin::u8_t * bend
					, const query_sm & msg) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);

				RETURN_NULL_IF(buf + msg.msg_id_len > bend);
				buf = w::scpy(buf, msg.msg_id, msg.msg_id_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*2 > bend);
				buf = w::cp_u8(buf, ascbuf(msg.src_addr_ton));
				buf = w::cp_u8(buf, ascbuf(msg.src_addr_npi));

				RETURN_NULL_IF(buf + msg.src_addr_len > bend);
				buf = w::scpy(buf, msg.src_addr, msg.src_addr_len);

				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, const bin::u8_t * bend
					, const query_sm_r & msg) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);
				
				RETURN_NULL_IF(buf + msg.msg_id_len > bend);
				buf = w::scpy(buf, msg.msg_id, msg.msg_id_len);

				RETURN_NULL_IF(buf + msg.final_date_len > bend);
				buf = w::scpy(buf, msg.final_date, msg.final_date_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*2 > bend);
				buf = w::cp_u8(buf, ascbuf(msg.msg_state));
				buf = w::cp_u8(buf, ascbuf(msg.error_code));

				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, const bin::u8_t * bend
					, const cancel_sm & msg) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof (msg.command) > bend);
				buf = write_pdu(buf, msg.command);

				RETURN_NULL_IF(buf + msg.serv_type_len > bend);
				buf = w::scpy(buf, msg.serv_type, msg.serv_type_len);

				RETURN_NULL_IF(buf + msg.msg_id_len > bend);
				buf = w::scpy(buf, msg.msg_id, msg.msg_id_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*2 > bend);
				buf = w::cp_u8(buf, ascbuf(msg.src_addr_ton));
				buf = w::cp_u8(buf, ascbuf(msg.src_addr_npi));
				RETURN_NULL_IF(buf + msg.src_addr_len > bend);
				buf = w::scpy(buf, msg.src_addr, msg.src_addr_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*2 > bend);
				buf = w::cp_u8(buf, ascbuf(msg.dst_addr_ton));
				buf = w::cp_u8(buf, ascbuf(msg.dst_addr_npi));
				RETURN_NULL_IF(buf + msg.dst_addr_len > bend);
				buf = w::scpy(buf, msg.dst_addr, msg.dst_addr_len);

				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, const bin::u8_t * bend
					, const cancel_sm_r & msg) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);

				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, const bin::u8_t * bend
					, const replace_sm & msg) {
				using namespace bin;
				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);

				RETURN_NULL_IF(buf + msg.msg_id_len > bend);
				buf = w::scpy(buf, msg.msg_id, msg.msg_id_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*2 > bend);
				buf = w::cp_u8(buf, ascbuf(msg.src_addr_ton));
				buf = w::cp_u8(buf, ascbuf(msg.src_addr_npi));
				RETURN_NULL_IF(buf + msg.src_addr_len > bend);
				buf = w::scpy(buf, msg.src_addr, msg.src_addr_len);

				RETURN_NULL_IF(buf + msg.schedule_delivery_time_len > bend);
				buf = w::scpy(buf, msg.schedule_delivery_time, msg.schedule_delivery_time_len);

				RETURN_NULL_IF(buf + msg.validity_period_len > bend);
				buf = w::scpy(buf, msg.validity_period, msg.validity_period_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*3 > bend);
				buf = w::cp_u8(buf, ascbuf(msg.registered_delivery));
				buf = w::cp_u8(buf, ascbuf(msg.sm_default_msg_id));
				buf = w::cp_u8(buf, ascbuf(msg.short_msg_len));

				RETURN_NULL_IF(buf + msg.short_msg_len > bend);
				buf = w::scpy(buf, msg.short_msg, msg.short_msg_len);

				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, const bin::u8_t * bend
					, const replace_sm_r & msg) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);

				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, const bin::u8_t * bend
					, const enquire_link & msg) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);

				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, const bin::u8_t * bend
					, const enquire_link_r & msg) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);

				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, const bin::u8_t * bend
					, const alert_notification & msg) {

				using namespace bin;
				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*2 > bend);
				buf = w::cp_u8(buf, ascbuf(msg.src_addr_ton));
				buf = w::cp_u8(buf, ascbuf(msg.src_addr_npi));
				RETURN_NULL_IF(buf + msg.src_addr_len > bend);
				buf = w::scpy(buf, msg.src_addr, msg.src_addr_len);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*2 > bend);
				buf = w::cp_u8(buf, ascbuf(msg.esme_addr_ton));
				buf = w::cp_u8(buf, ascbuf(msg.esme_addr_npi));
				RETURN_NULL_IF(buf + msg.esme_addr_len > bend);
				buf = w::scpy(buf, msg.esme_addr, msg.esme_addr_len);

				if (msg.ms_availability_status.tag == option::ms_availability_status) {
					RETURN_NULL_IF(buf + msg.ms_availability_status.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.ms_availability_status);
				}

				return buf;
			}

			/* TODO */
			void write(bin::u8_t * buf, const submit_multi_sm & r) {
				using namespace bin;
				buf = write(buf, r.command, L);
				buf = w::scpy(buf, r.serv_type, 6);

				buf = w::cp_u8(buf, r.src_addr_ton);
				buf = w::cp_u8(buf, r.src_addr_npi);
				buf = w::scpy(buf, r.src_addr, 21);
				buf = w::cp_u8(buf, r.num_of_dsts);
				/*
				TODO dst_addr(es)
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
				buf = w::cp_u8(buf, r.short_msg_len);
				buf = w::scpy(buf, r.short_msg, 254);

				if (r.user_msg_reference.tag != 0)	buf = write(buf, r.user_msg_reference, L);
				if (r.src_port.tag != 0)				buf = write(buf, r.src_port, L);
				if (r.src_addr_subunit.tag != 0)		buf = write(buf, r.src_addr_subunit, L);
				if (r.dst_port.tag != 0)				buf = write(buf, r.dst_port.tag, L);
				if (r.dst_addr_subunit.tag != 0)		buf = write(buf, r.dst_addr_subunit, L);
				if (r.sar_msg_ref_num.tag != 0)			buf = write(buf, r.sar_msg_ref_num, L);
				if (r.sar_total_segments.tag != 0)		buf = write(buf, r.sar_total_segments, L);
				if (r.sar_segment_seqnum.tag != 0)		buf = write(buf, r.sar_segment_seqnum, L);
				if (r.payload_type.tag != 0)			buf = write(buf, r.payload_type, L);
				if (r.msg_payload.tag != 0)			buf = write(buf, r.msg_payload.tag, L);
				if (r.privacy_ind.tag != 0)		buf = write(buf, r.privacy_ind, L);
				if (r.callback_num.tag != 0)			buf = write(buf, r.callback_num, L);
				if (r.callback_num_pres_ind.tag != 0)	buf = write(buf, r.callback_num_pres_ind, L);
				if (r.callback_num_atag.tag != 0)		buf = write(buf, r.callback_num_atag, L);
				if (r.src_subaddr.tag != 0)		buf = write(buf, r.src_subaddr, L);
				if (r.dst_subaddr.tag != 0)			buf = write(buf, r.dst_subaddr, L);
				if (r.display_time.tag != 0)			buf = write(buf, r.display_time, L);
				if (r.sms_signal.tag != 0)				buf = write(buf, r.sms_signal, L);
				if (r.ms_validity.tag != 0)				buf = write(buf, r.ms_validity, L);
				if (r.ms_msg_wait_fclts.tag != 0)	buf = write(buf, r.ms_msg_wait_fclts, L);
				if (r.alert_on_msg_delivery.tag!=0)	buf = write(buf, r.alert_on_msg_delivery, L);
				if (r.lang_ind.tag != 0)		buf = write(buf, r.lang_ind, L);
			}

		private:
			template <class BindT>
			bin::u8_t * write_bind(bin::u8_t * buf, const bin::u8_t * bend
					, const BindT & msg) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);

				RETURN_NULL_IF(buf + msg.sys_id_len > bend);
				buf = w::scpy(buf, ascbuf(msg.sys_id), msg.sys_id_len);

				RETURN_NULL_IF(buf + msg.password_len > bend);
				buf = w::scpy(buf, ascbuf(msg.password), msg.password_len);

				RETURN_NULL_IF(buf + msg.sys_type_len > bend);
				buf = w::scpy(buf, ascbuf(msg.sys_type), msg.sys_type_len);

				RETURN_NULL_IF(buf
						+ sizeof(bin::u8_t)
						+ sizeof(bin::u8_t)
						+ sizeof(bin::u8_t)
						> bend);
				buf = w::cp_u8(buf, &msg.interface_version);
				buf = w::cp_u8(buf, &msg.addr_ton);
				buf = w::cp_u8(buf, &msg.addr_npi);

				RETURN_NULL_IF(buf + msg.addr_range_len > bend);
				buf = w::scpy(buf, ascbuf(msg.addr_range), msg.addr_range_len);

				return buf;
			}

			template <class BindT>
			bin::u8_t * write_bind_r(bin::u8_t * buf, const bin::u8_t * bend
					, const BindT & msg) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = write_pdu(buf, msg.command);

				RETURN_NULL_IF(buf + msg.sys_id_len > bend);
				buf = w::scpy(buf, msg.sys_id, msg.sys_id_len);

				if (msg.sc_interface_version.tag == option::sc_interface_version) {
					RETURN_NULL_IF(buf + msg.sc_interface_version.raw_size() > bend);
					buf = write_tlv_u8(buf, msg.sc_interface_version);
				} else if (msg.sc_interface_version.tag != 0) {
					return nullptr; /* tag's value is incorrect */
				}

				return buf;
			}

			bin::u8_t * write_pdu(bin::u8_t * buf, const pdu & msg) {
				using namespace bin;
				buf = w::cp_u32(buf, ascbuf(msg.len));
				buf = w::cp_u32(buf, ascbuf(msg.id));
				buf = w::cp_u32(buf, ascbuf(msg.status));
				buf = w::cp_u32(buf, ascbuf(msg.seqno));
				return buf;
			}
	};

	template <class LogT>
	class tlv_parser {
		protected:
			LogT & L;

		public:
			tlv_parser(LogT & l): L(l) {}
			virtual ~tlv_parser() {}

			const bin::u8_t * parse_tlv_u8(tlv<bin::u8_t> & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cp_u8(asbuf(t.val), buf);
				return buf;
			}

			const bin::u8_t * parse_tlv_u16(tlv<bin::u16_t> & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cp_u16(asbuf(t.val), buf);
				return buf;
			}

			const bin::u8_t * parse_tlv_u32(tlv<bin::u32_t> & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cp_u32(asbuf(t.val), buf);
				return buf;
			}

			const bin::u8_t * parse_tlv_s23(tlv<bin::u8_t[23]> & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cpy(t.val, ascbuf(buf), t.len);
				return buf;
			}

			const bin::u8_t * parse_tlv_s19(tlv_callback_num & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cpy(t.val, ascbuf(buf), t.len);

				return buf;
			}

			const bin::u8_t * parse_tlv_s65(tlv<bin::u8_t[65]> & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cpy(t.val, ascbuf(buf), t.len);
				return buf;
			}

			const bin::u8_t * parse_tlv_ptr(tlv<bin::u8_t *> & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf += t.len;
				/* TODO buf = p::cpy(t.val, ascbuf(buf), t.len); */
				return buf;
			}

			const bin::u8_t * parse_tlv_s3(tlv<bin::u8_t[3]> & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cpy(t.val, ascbuf(buf), t.len);
				return buf;
			}

			const bin::u8_t * parse_tlv_s2(tlv_its_session_info & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cpy(t.val, ascbuf(buf), t.len);
				return buf;
			}

			const bin::u8_t * parse_tlv_s1(tlv_ussd_serv_op & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cpy(t.val, ascbuf(buf), t.len);
				return buf;
			}

			const bin::u8_t * parse_tlv_s256(tlv<bin::u8_t[256]> & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cpy(t.val, ascbuf(buf), t.len);
				return buf;
			}
	};
	template <class LogT>
	class parser: public tlv_parser<LogT> {
		using tlv_parser<LogT>::L;
		using tlv_parser<LogT>::parse_tlv_u8;
		using tlv_parser<LogT>::parse_tlv_u16;
		using tlv_parser<LogT>::parse_tlv_u32;
		using tlv_parser<LogT>::parse_tlv_s19;
		using tlv_parser<LogT>::parse_tlv_s23;
		using tlv_parser<LogT>::parse_tlv_s65;
		using tlv_parser<LogT>::parse_tlv_s256;
		using tlv_parser<LogT>::parse_tlv_s2;
		using tlv_parser<LogT>::parse_tlv_s3;
		using tlv_parser<LogT>::parse_tlv_s1;
		using tlv_parser<LogT>::parse_tlv_ptr;

		public:
			parser(LogT & l): tlv_parser<LogT>(l) {}
			virtual ~parser() {}

			enum action {
				stop		/* Stop parsing immediately */
				, resume	/* Resume parsing */
				, skip		/* Skip the value (may be constructor)
							   of an element */
			};

			const bin::u8_t * parse(const bin::u8_t * buf
				, const bin::u8_t * bend) {
				buf = parse_header(buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				return buf;
			}

		protected:
			virtual action on_bind_transmitter(const bind_transmitter & msg) = 0;
			virtual action on_bind_transmitter_r(const bind_transmitter_r & msg) = 0;
			virtual action on_bind_receiver(const bind_receiver & msg) = 0;
			virtual action on_bind_receiver_r(const bind_receiver_r & msg) = 0;
			virtual action on_bind_transceiver(const bind_transceiver & msg) = 0;
			virtual action on_bind_transceiver_r(const bind_transceiver_r & msg) = 0;
			virtual action on_unbind(const unbind & msg) = 0;
			virtual action on_unbind_r(const unbind_r & msg) = 0;
			virtual action on_outbind(const outbind & msg) = 0;
			virtual action on_generic_nack(const generic_nack & msg) = 0;
			virtual action on_submit_sm(const submit_sm & msg) = 0;
			virtual action on_submit_sm_r(const submit_sm_r & msg) = 0;
			virtual action on_parse_error(const bin::u8_t * buf, const bin::u8_t * bend) = 0;
			virtual action on_submit_multi_sm(const submit_multi_sm & msg) = 0;
			virtual action on_deliver_sm(const deliver_sm & msg) = 0;
			virtual action on_deliver_sm_r(const deliver_sm_r & msg) = 0;
			virtual action on_data_sm(const data_sm & msg) = 0;
			virtual action on_data_sm_r(const data_sm_r & msg) = 0;
			virtual action on_query_sm(const query_sm & msg) = 0;
			virtual action on_query_sm_r(const query_sm_r & msg) = 0;
			virtual action on_cancel_sm(const cancel_sm & msg) = 0;
			virtual action on_cancel_sm_r(const cancel_sm_r & msg) = 0;
			virtual action on_replace_sm(const replace_sm & msg) = 0;
			virtual action on_replace_sm_r(const replace_sm_r & msg) = 0;
			virtual action on_enquire_link(const enquire_link & msg) = 0;
			virtual action on_enquire_link_r(const enquire_link_r & msg) = 0;
			virtual action on_alert_notification(const alert_notification & msg) = 0;

		private:

			const bin::u8_t * parse_header(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				bin::u32_t len, id;

				const bin::u8_t * cur = buf;
				const bin::u8_t * curc;
				while (cur < bend) {
					/* Parse PDU len and id
					 * In general buffer may contain several commands */
					RETURN_NULL_IF(cur + sizeof(bin::u32_t) * 2 > bend);
					curc = cur;
					curc = p::cp_u32(asbuf(len), curc);
					curc = p::cp_u32(asbuf(id), curc);

					if (cur + len > bend) {
						return nullptr;
					}

					switch (id) {
						case command::bind_transmitter:
							cur = parse_bind_transmitter(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::bind_receiver:
							cur = parse_bind_receiver(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::bind_transceiver:
							cur = parse_bind_transceiver(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::bind_transmitter_r:
							cur = parse_bind_transmitter_r(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::submit_sm:
							cur = parse_submit_sm(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::bind_receiver_r:
							cur = parse_bind_receiver_r(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::bind_transceiver_r:
							cur = parse_bind_transceiver_r(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::unbind:
							cur = parse_unbind(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::unbind_r:
							cur = parse_unbind_r(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::generic_nack:
							cur = parse_generic_nack(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::submit_sm_r:
							cur = parse_submit_sm_r(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::submit_multi_sm:
							cur = parse_submit_multi_sm(cur, cur + len);
							continue;
						case command::outbind:
							cur = parse_outbind(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::deliver_sm:
							cur = parse_deliver_sm(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::deliver_sm_r:
							cur = parse_deliver_sm_r(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::data_sm:
							cur = parse_data_sm(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::data_sm_r:
							cur = parse_data_sm_r(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::query_sm:
							cur = parse_query_sm(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::query_sm_r:
							cur = parse_query_sm_r(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::cancel_sm:
							cur = parse_cancel_sm(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::cancel_sm_r:
							cur = parse_cancel_sm_r(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::replace_sm:
							cur = parse_replace_sm(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::replace_sm_r:
							cur = parse_replace_sm_r(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::enquire_link:
							cur = parse_enquire_link(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::enquire_link_r:
							cur = parse_enquire_link_r(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::alert_notification:
							cur = parse_alert_notification(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						default:
							return nullptr;
					}
				}
				return cur;
			}

			const bin::u8_t * parse_pdu(pdu & cmd, const bin::u8_t * buf) {
					using namespace bin;
					buf = p::cp_u32(asbuf(cmd.len), buf);
					buf = p::cp_u32(asbuf(cmd.id), buf);
					buf = p::cp_u32(asbuf(cmd.status), buf);
					buf = p::cp_u32(asbuf(cmd.seqno), buf);
					return buf;
			}

			/* BIND_TRANSMITTER, BIND_RECEIVER, BIND_TRANSCEIVER */
			template <class BindT>
			const bin::u8_t * parse_bind(BindT & msg, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				buf = p::scpyl(msg.sys_id, buf, bend
						, sizeof(msg.sys_id), msg.sys_id_len);
				RETURN_NULL_IF(buf == nullptr);

				buf = p::scpyl(msg.password, buf, bend
						, sizeof(msg.password), msg.password_len);
				RETURN_NULL_IF(buf == nullptr);
				buf = p::scpyl(msg.sys_type, buf, bend
							, sizeof(msg.sys_type), msg.sys_type_len);
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(buf
						+ sizeof(bin::u8_t)
						+ sizeof(bin::u8_t)
						+ sizeof(bin::u8_t)
						> bend);
				buf = p::cp_u8(&msg.interface_version, buf);
				buf = p::cp_u8(&msg.addr_ton, buf);
				buf = p::cp_u8(&msg.addr_npi, buf);

				buf = p::scpyl(msg.addr_range, buf, bend
						, sizeof(msg.addr_range), msg.addr_range_len);
				RETURN_NULL_IF(buf == nullptr);

				return buf;
			}

			/* BIND_TRANSMITTER_R, BIND_RECEIVER_R, BIND_TRANSCEIVER_R */
			template <class BindT>
			const bin::u8_t * parse_bind_r(BindT & msg
					, const bin::u8_t * buf, const bin::u8_t * bend) {
				using namespace bin;
				bin::u16_t optid;
				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				buf = p::scpyl(msg.sys_id, buf, bend
						, sizeof(msg.sys_id), msg.sys_id_len);
				RETURN_NULL_IF(buf == NULL);

				if (buf == bend) /* there is no optional parameter */
					return bend;

				p::cp_u16(asbuf(optid), buf);
				RETURN_NULL_IF(optid != option::sc_interface_version);
				buf = parse_tlv_u8(msg.sc_interface_version, buf);

				return buf;
			}

			const bin::u8_t * parse_bind_transmitter(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				bind_transmitter msg;
				buf = parse_bind(msg, buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				if (on_bind_transmitter(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_bind_transmitter_r(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				bind_transmitter_r msg;
				buf = parse_bind_r(msg, buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				if (on_bind_transmitter_r(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_bind_receiver(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				bind_receiver msg;
				buf = parse_bind(msg, buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				if (on_bind_receiver(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_bind_receiver_r(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				bind_receiver_r msg;
				buf = parse_bind_r(msg, buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				if (on_bind_receiver_r(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_bind_transceiver(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				bind_transceiver msg;
				buf = parse_bind(msg, buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				if (on_bind_transceiver(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_bind_transceiver_r(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				bind_transceiver_r msg;
				buf = parse_bind_r(msg, buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				if (on_bind_transceiver_r(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_unbind(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				unbind msg;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				if (on_unbind(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_unbind_r(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				unbind_r msg;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				if (on_unbind_r(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_generic_nack(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				generic_nack msg;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				if (on_generic_nack(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_submit_sm(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				submit_sm msg;
				bin::u16_t optid;
				bin::u16_t len;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				buf = p::scpyl(msg.serv_type, buf, bend
						, sizeof(msg.serv_type), msg.serv_type_len);
				RETURN_NULL_IF(buf == nullptr);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 2 > bend);
				buf = p::cp_u8(&msg.src_addr_ton, buf);
				buf = p::cp_u8(&msg.src_addr_npi, buf);

				buf = p::scpyl(msg.src_addr, buf, bend
						, sizeof(msg.src_addr), msg.src_addr_len);
				RETURN_NULL_IF(buf == nullptr);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 2 > bend);
				buf = p::cp_u8(&msg.dst_addr_ton, buf);
				buf = p::cp_u8(&msg.dst_addr_npi, buf);

				buf = p::scpyl(msg.dst_addr, buf, bend
						, sizeof(msg.dst_addr), msg.dst_addr_len);
				RETURN_NULL_IF(buf == nullptr);
				
				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 3 > bend);
				buf = p::cp_u8(&msg.esm_class, buf);
				buf = p::cp_u8(&msg.protocol_id, buf);
				buf = p::cp_u8(&msg.priority_flag, buf);

				buf = p::scpyl(msg.schedule_delivery_time , buf, bend
						, sizeof(msg.schedule_delivery_time)
						, msg.schedule_delivery_time_len);
				RETURN_NULL_IF(buf == nullptr);

				buf = p::scpyl(msg.validity_period, buf, bend
						, sizeof(msg.validity_period)
						, msg.validity_period_len);
				RETURN_NULL_IF(buf == nullptr);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 5 > bend);
				buf = p::cp_u8(&msg.registered_delivery, buf);
				buf = p::cp_u8(&msg.replace_if_present_flag, buf);
				buf = p::cp_u8(&msg.data_coding, buf);
				buf = p::cp_u8(&msg.sm_default_msg_id, buf);
				buf = p::cp_u8(&msg.short_msg_len, buf);

				buf = p::cpy(msg.short_msg, buf, msg.short_msg_len);
				RETURN_NULL_IF(buf == nullptr);

				const bin::u8_t * cur;
				while (buf + sizeof(bin::u16_t)*2 <= bend) {
					cur = buf;
					cur = p::cp_u16(asbuf(optid), cur);
					cur = p::cp_u16(asbuf(len), cur);

					switch (optid) {
						case option::user_msg_reference:
							buf = parse_tlv_u16(msg.user_msg_reference, buf);
							break;
						case option::src_port:
							buf = parse_tlv_u16(msg.src_port, buf);
							break;
						case option::src_addr_subunit:
							buf = parse_tlv_u8(msg.src_addr_subunit, buf);
							break;
						case option::dst_port:
							buf = parse_tlv_u16(msg.dst_port, buf);
							break;
						case option::dst_addr_subunit:
							buf = parse_tlv_u8(msg.dst_addr_subunit, buf);
							break;
						case option::sar_msg_ref_num:
							buf = parse_tlv_u16(msg.sar_msg_ref_num, buf);
							break;
						case option::sar_total_segments:
							buf = parse_tlv_u8(msg.sar_total_segments, buf);
							break;
						case option::sar_segment_seqnum:
							buf = parse_tlv_u8(msg.sar_segment_seqnum, buf);
							break;
						case option::more_msgs_to_send:
							buf = parse_tlv_u8(msg.more_msgs_to_send, buf);
							break;
						case option::payload_type:
							buf = parse_tlv_u8(msg.payload_type, buf);
							break;
						case option::msg_payload:
							buf = parse_tlv_ptr(msg.msg_payload, buf);
							break;
						case option::privacy_ind:
							buf = parse_tlv_u8(msg.privacy_ind, buf);
							break;
						case option::callback_num:
							buf = parse_tlv_s19(msg.callback_num, buf);
							break;
						case option::callback_num_pres_ind:
							buf = parse_tlv_u8(msg.callback_num_pres_ind, buf);
							break;
						case option::callback_num_atag:
							buf = parse_tlv_s65(msg.callback_num_atag, buf);
							break;
						case option::src_subaddr:
							buf = parse_tlv_s23(msg.src_subaddr, buf);
							break;
						case option::dst_subaddr:
							buf = parse_tlv_s23(msg.dst_subaddr, buf);
							break;
						case option::user_resp_code:
							buf = parse_tlv_u8(msg.user_resp_code, buf);
							break;
						case option::display_time:
							buf = parse_tlv_u8(msg.display_time, buf);
							break;
						case option::sms_signal:
							buf = parse_tlv_u16(msg.sms_signal, buf);
							break;
						case option::ms_validity:
							buf = parse_tlv_u8(msg.ms_validity, buf);
							break;
						case option::ms_msg_wait_fclts:
							buf = parse_tlv_u8(msg.ms_msg_wait_fclts, buf);
							break;
						case option::number_of_msgs:
							buf = parse_tlv_u8(msg.number_of_msgs, buf);
							break;
						case option::alert_on_msg_delivery:
							buf = parse_tlv_u8(msg.alert_on_msg_delivery, buf);
							break;
						case option::lang_ind:
							buf = parse_tlv_u8(msg.lang_ind, buf);
							break;
						case option::its_reply_type:
							buf = parse_tlv_u8(msg.its_reply_type, buf);
							break;
						case option::its_session_info:
							buf = parse_tlv_s2(msg.its_session_info, buf);
							break;
						case option::ussd_serv_op:
							buf = parse_tlv_s1(msg.ussd_serv_op, buf);
							break;
						default:
							return nullptr;
					}
				}

				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(buf != bend);

				if (on_submit_sm(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_submit_sm_r(const bin::u8_t * buf
				, const bin::u8_t * bend) {

				using namespace bin;
				submit_sm_r msg;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);
				buf = p::scpyl(msg.msg_id, buf, bend
						, sizeof(msg.msg_id), msg.msg_id_len);
				RETURN_NULL_IF(buf == NULL);

				if (on_submit_sm_r(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_deliver_sm_r(const bin::u8_t * buf
				, const bin::u8_t * bend) {

				using namespace bin;
				deliver_sm_r msg;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);
				
				buf = p::scpyl(msg.msg_id, buf, bend
						, sizeof(msg.msg_id), msg.msg_id_len);
				RETURN_NULL_IF(buf == NULL);

				if (on_deliver_sm_r(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_deliver_sm(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				deliver_sm msg;
				bin::u16_t optid;
				bin::u16_t len;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				buf = p::scpyl(msg.serv_type, buf, bend
						, sizeof(msg.serv_type), msg.serv_type_len);
				RETURN_NULL_IF(buf == nullptr);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 2 > bend);
				buf = p::cp_u8(&msg.src_addr_ton, buf);
				buf = p::cp_u8(&msg.src_addr_npi, buf);

				buf = p::scpyl(msg.src_addr, buf, bend
						, sizeof(msg.src_addr), msg.src_addr_len);
				RETURN_NULL_IF(buf == nullptr);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 2 > bend);
				buf = p::cp_u8(&msg.dst_addr_ton, buf);
				buf = p::cp_u8(&msg.dst_addr_npi, buf);

				buf = p::scpyl(msg.dst_addr, buf, bend
						, sizeof(msg.dst_addr), msg.dst_addr_len);
				RETURN_NULL_IF(buf == nullptr);
				
				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 3 > bend);
				buf = p::cp_u8(&msg.esm_class, buf);
				buf = p::cp_u8(&msg.protocol_id, buf);
				buf = p::cp_u8(&msg.priority_flag, buf);

				buf = p::scpyl(msg.schedule_delivery_time , buf, bend
						, sizeof(msg.schedule_delivery_time)
						, msg.schedule_delivery_time_len);
				RETURN_NULL_IF(buf == nullptr);

				buf = p::scpyl(msg.validity_period, buf, bend
						, sizeof(msg.validity_period)
						, msg.validity_period_len);
				RETURN_NULL_IF(buf == nullptr);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 5 > bend);
				buf = p::cp_u8(&msg.registered_delivery, buf);
				buf = p::cp_u8(&msg.replace_if_present_flag, buf);
				buf = p::cp_u8(&msg.data_coding, buf);
				buf = p::cp_u8(&msg.sm_default_msg_id, buf);
				buf = p::cp_u8(&msg.short_msg_len, buf);

				buf = p::cpy(msg.short_msg, buf, msg.short_msg_len);
				RETURN_NULL_IF(buf == nullptr);

				const bin::u8_t * cur;
				while (buf + sizeof(bin::u16_t)*2 <= bend) {
					cur = buf;
					cur = p::cp_u16(asbuf(optid), cur);
					cur = p::cp_u16(asbuf(len), cur);

					switch (optid) {
						case option::user_msg_reference:
							buf = parse_tlv_u16(msg.user_msg_reference, buf);
							break;
						case option::src_port:
							buf = parse_tlv_u16(msg.src_port, buf);
							break;
						case option::dst_port:
							buf = parse_tlv_u16(msg.dst_port, buf);
							break;
						case option::sar_msg_ref_num:
							buf = parse_tlv_u16(msg.sar_msg_ref_num, buf);
							break;
						case option::sar_total_segments:
							buf = parse_tlv_u8(msg.sar_total_segments, buf);
							break;
						case option::sar_segment_seqnum:
							buf = parse_tlv_u8(msg.sar_segment_seqnum, buf);
							break;
						case option::payload_type:
							buf = parse_tlv_u8(msg.payload_type, buf);
							break;
						case option::privacy_ind:
							buf = parse_tlv_u8(msg.privacy_ind, buf);
							break;
						case option::callback_num:
							buf = parse_tlv_s19(msg.callback_num, buf);
							break;
						case option::src_subaddr:
							buf = parse_tlv_s23(msg.src_subaddr, buf);
							break;
						case option::dst_subaddr:
							buf = parse_tlv_s23(msg.dst_subaddr, buf);
							break;
						case option::user_resp_code:
							buf = parse_tlv_u8(msg.user_resp_code, buf);
							break;
						case option::lang_ind:
							buf = parse_tlv_u8(msg.lang_ind, buf);
							break;
						case option::its_session_info:
							buf = parse_tlv_s2(msg.its_session_info, buf);
							break;
						case option::network_error_code:
							buf = parse_tlv_s3(msg.network_error_code, buf);
							break;
						case option::msg_state:
							buf = parse_tlv_u8(msg.msg_state, buf);
							break;
						case option::receipted_msg_id:
							buf = parse_tlv_s65(msg.receipted_msg_id, buf);
							break;
						default:
							return nullptr;
					}
				}

				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(buf != bend);

				if (on_deliver_sm(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_submit_multi_sm(const bin::u8_t * buf
					, const bin::u8_t * bend) {

				using namespace bin;
				submit_multi_sm msg;

				u16_t optid;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				buf = p::scpyl(msg.serv_type, buf, bend
						, sizeof(msg.serv_type), msg.serv_type_len);
				RETURN_NULL_IF(buf == NULL);

				RETURN_NULL_IF(buf + sizeof(u8_t)*2 > bend);
				buf = p::cp_u8(&msg.src_addr_ton, buf);
				buf = p::cp_u8(&msg.src_addr_npi, buf);

				buf = p::scpyl(msg.src_addr, buf, bend
						, sizeof(msg.src_addr), msg.src_addr_len);
				RETURN_NULL_IF(buf == NULL);

				RETURN_NULL_IF(buf + sizeof(u8_t) > bend);
				buf = p::cp_u8(&msg.num_of_dsts, buf);

				/* TODO dst_address(es) new struct */

				RETURN_NULL_IF(buf + sizeof(u8_t)*3 > bend);
				buf = p::cp_u8(&msg.esm_class, buf);
				buf = p::cp_u8(&msg.protocol_id, buf);
				buf = p::cp_u8(&msg.priority_flag, buf);

				buf = p::scpyf(msg.schedule_delivery_time, buf, bend
						, sizeof(msg.schedule_delivery_time));
				RETURN_NULL_IF(buf == NULL);

				buf = p::scpyf(msg.validity_period, buf, bend
						, sizeof(msg.validity_period));
				RETURN_NULL_IF(buf == NULL);

				RETURN_NULL_IF(buf + sizeof(u8_t)*5 > bend);
				buf = p::cp_u8(&msg.registered_delivery, buf);
				buf = p::cp_u8(&msg.replace_if_present_flag, buf);
				buf = p::cp_u8(&msg.data_coding, buf);
				buf = p::cp_u8(&msg.sm_default_msg_id, buf);
				buf = p::cp_u8(&msg.short_msg_len, buf);

				buf = p::scpyl(msg.short_msg, buf, bend
						, sizeof(msg.short_msg), msg.short_msg_len);
				RETURN_NULL_IF(buf == NULL);

				buf = p::cp_u16(asbuf(optid), buf);
				/*
				buf += sizeof(bin::u16_t);
				*/

				while ((buf + sizeof (u16_t)) <= bend) {
					p::cp_u16(asbuf(optid), buf);

					switch (optid) {
						case option::user_msg_reference:
							RETURN_NULL_IF(buf +
									msg.user_msg_reference.raw_size() > bend);
							buf = parse_tlv_u16(msg.user_msg_reference, buf);
							break;
						case option::src_port:
							buf = parse_tlv_u16(msg.src_port, buf);
							break;
						case option::src_addr_subunit:
							buf = parse_tlv_u8(msg.src_addr_subunit, buf);
							break;
						case option::dst_port:
							buf = parse_tlv_u16(msg.dst_port, buf);
							break;
						case option::dst_addr_subunit:
							buf = parse_tlv_u8(msg.dst_addr_subunit, buf);
							break;
						case option::sar_msg_ref_num:
							buf = parse_tlv_u16(msg.sar_msg_ref_num, buf);
							break;
						case option::sar_total_segments:
							buf = parse_tlv_u8(msg.sar_total_segments, buf);
							break;
						case option::sar_segment_seqnum:
							buf = parse_tlv_u8(msg.sar_segment_seqnum, buf);
							break;
						case option::payload_type:
							buf = parse_tlv_u8(msg.payload_type, buf);
							break;
						/* TODO: Parse msg_payload*/
						/*
						case option::msg_payload:
							buf = parse(msg.msg_payload, buf, bend);
							break;
						*/
						case option::privacy_ind:
							buf = parse_tlv_u8(msg.privacy_ind, buf);
							break;
						case option::callback_num:
							buf = parse_tlv_s19(msg.callback_num, buf);
							break;
						case option::callback_num_pres_ind:
							buf = parse_tlv_u8(msg.callback_num_pres_ind, buf);
							break;
						case option::callback_num_atag:
							buf = parse_tlv_s65(msg.callback_num_atag, buf);
							break;
						case option::src_subaddr:
							buf = parse_tlv_s23(msg.src_subaddr, buf);
							break;
						case option::dst_subaddr:
							buf = parse_tlv_s23(msg.dst_subaddr, buf);
							break;
						case option::display_time:
							buf = parse_tlv_u8(msg.display_time, buf);
							break;
						case option::sms_signal:
							buf = parse_tlv_u16(msg.sms_signal, buf);
							break;
						case option::ms_validity:
							buf = parse_tlv_u8(msg.ms_validity, buf);
							break;
						case option::ms_msg_wait_fclts:
							buf = parse_tlv_u8(msg.ms_msg_wait_fclts, buf);
							break;
						case option::alert_on_msg_delivery:
							buf = parse_tlv_u8(msg.alert_on_msg_delivery, buf);
							break;
						case option::lang_ind:
							buf = parse_tlv_u8(msg.lang_ind, buf);
							break;
						default:
							buf = nullptr;
							break;
					}

					RETURN_NULL_IF(buf == nullptr);
				}

				if (on_submit_multi_sm(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_outbind(const bin::u8_t * buf
					, const bin::u8_t * bend) {

				using namespace bin;
				outbind msg;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				buf = p::scpyl(msg.sys_id, buf, bend
						, sizeof (msg.sys_id), msg.sys_id_len);
				RETURN_NULL_IF(buf == nullptr);

				buf = p::scpyl(msg.password, buf, bend
						, sizeof (msg.password), msg.password_len);
				RETURN_NULL_IF(buf == nullptr);

				if (on_outbind(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}
			
			const bin::u8_t * parse_data_sm(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				data_sm msg;
				bin::u16_t optid;
				bin::u16_t len;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				buf = p::scpyl(msg.serv_type, buf, bend
						, sizeof(msg.serv_type), msg.serv_type_len);
				RETURN_NULL_IF(buf == nullptr);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 2 > bend);
				buf = p::cp_u8(&msg.src_addr_ton, buf);
				buf = p::cp_u8(&msg.src_addr_npi, buf);

				buf = p::scpyl(msg.src_addr, buf, bend
						, sizeof(msg.src_addr), msg.src_addr_len);
				RETURN_NULL_IF(buf == nullptr);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 2 > bend);
				buf = p::cp_u8(&msg.dst_addr_ton, buf);
				buf = p::cp_u8(&msg.dst_addr_npi, buf);

				buf = p::scpyl(msg.dst_addr, buf, bend
						, sizeof(msg.dst_addr), msg.dst_addr_len);
				RETURN_NULL_IF(buf == nullptr);
				
				RETURN_NULL_IF(buf + sizeof(bin::u8_t) * 3 > bend);
				buf = p::cp_u8(&msg.esm_class, buf);
				buf = p::cp_u8(&msg.registered_delivery, buf);
				buf = p::cp_u8(&msg.data_coding, buf);

				const bin::u8_t * cur;
				while (buf + sizeof(bin::u16_t)*2 <= bend) {
					cur = buf;
					cur = p::cp_u16(asbuf(optid), cur);
					cur = p::cp_u16(asbuf(len), cur);

					switch (optid) {
						case option::user_msg_reference:
							buf = parse_tlv_u16(msg.user_msg_reference, buf);
							break;
						case option::src_port:
							buf = parse_tlv_u16(msg.src_port, buf);
							break;
						case option::src_addr_subunit:
							buf = parse_tlv_u8(msg.src_addr_subunit, buf);
							break;
						case option::dst_port:
							buf = parse_tlv_u16(msg.dst_port, buf);
							break;
						case option::dst_addr_subunit:
							buf = parse_tlv_u8(msg.dst_addr_subunit, buf);
							break;
						case option::sar_msg_ref_num:
							buf = parse_tlv_u16(msg.sar_msg_ref_num, buf);
							break;
						case option::sar_total_segments:
							buf = parse_tlv_u8(msg.sar_total_segments, buf);
							break;
						case option::sar_segment_seqnum:
							buf = parse_tlv_u8(msg.sar_segment_seqnum, buf);
							break;
						case option::more_msgs_to_send:
							buf = parse_tlv_u8(msg.more_msgs_to_send, buf);
							break;
						case option::payload_type:
							buf = parse_tlv_u8(msg.payload_type, buf);
							break;
						case option::msg_payload:
							buf = parse_tlv_ptr(msg.msg_payload, buf);
							break;
						case option::privacy_ind:
							buf = parse_tlv_u8(msg.privacy_ind, buf);
							break;
						case option::callback_num:
							buf = parse_tlv_s19(msg.callback_num, buf);
							break;
						case option::callback_num_pres_ind:
							buf = parse_tlv_u8(msg.callback_num_pres_ind, buf);
							break;
						case option::callback_num_atag:
							buf = parse_tlv_s65(msg.callback_num_atag, buf);
							break;
						case option::src_subaddr:
							buf = parse_tlv_s23(msg.src_subaddr, buf);
							break;
						case option::dst_subaddr:
							buf = parse_tlv_s23(msg.dst_subaddr, buf);
							break;
						case option::user_resp_code:
							buf = parse_tlv_u8(msg.user_resp_code, buf);
							break;
						case option::display_time:
							buf = parse_tlv_u8(msg.display_time, buf);
							break;
						case option::sms_signal:
							buf = parse_tlv_u16(msg.sms_signal, buf);
							break;
						case option::ms_validity:
							buf = parse_tlv_u8(msg.ms_validity, buf);
							break;
						case option::ms_msg_wait_fclts:
							buf = parse_tlv_u8(msg.ms_msg_wait_fclts, buf);
							break;
						case option::number_of_msgs:
							buf = parse_tlv_u8(msg.number_of_msgs, buf);
							break;
						case option::alert_on_msg_delivery:
							buf = parse_tlv_u8(msg.alert_on_msg_delivery, buf);
							break;
						case option::lang_ind:
							buf = parse_tlv_u8(msg.lang_ind, buf);
							break;
						case option::its_reply_type:
							buf = parse_tlv_u8(msg.its_reply_type, buf);
							break;
						case option::its_session_info:
							buf = parse_tlv_s2(msg.its_session_info, buf);
							break;
						default:
							return nullptr;
					}
				}

				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(buf != bend);

				if (on_data_sm(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_data_sm_r(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				data_sm_r msg;
				bin::u16_t optid;
				bin::u16_t len;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				buf = p::scpyl(msg.msg_id, buf, bend
						, sizeof (msg.msg_id), msg.msg_id_len);
				RETURN_NULL_IF(buf == nullptr);

				const bin::u8_t * cur;
				while (buf + sizeof(bin::u16_t)*2 <= bend) {
					cur = buf;
					cur = p::cp_u16(asbuf(optid), cur);
					cur = p::cp_u16(asbuf(len), cur);

					switch (optid) {
						case option::delivery_failure_reason:
							buf = parse_tlv_u8(msg.delivery_failure_reason, buf);
							break;

						case option::network_error_code:
							buf = parse_tlv_s3(msg.network_error_code, buf);
							break;

						case option::additional_status_info_text:
							buf = parse_tlv_s256(msg.additional_status_info_text, buf);
							break;

						case option::dpf_result:
							buf = parse_tlv_u8(msg.dpf_result, buf);
							break;

						default:
							return nullptr;
					}
				}

				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(buf != bend);

				if (on_data_sm_r(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_query_sm(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				query_sm msg;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);
	
				buf = p::scpyl(msg.msg_id, buf, bend
						, sizeof (msg.msg_id), msg.msg_id_len);
				RETURN_NULL_IF(buf == nullptr);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*2 > bend);
				buf = p::cp_u8(asbuf(msg.src_addr_ton), buf);
				buf = p::cp_u8(asbuf(msg.src_addr_npi), buf);

				buf = p::scpyl(msg.src_addr, buf, bend
						, sizeof (msg.src_addr), msg.src_addr_len);
				RETURN_NULL_IF(buf == nullptr);

				if (on_query_sm(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_query_sm_r(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				query_sm_r msg;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);
	
				buf = p::scpyl(msg.msg_id, buf, bend
						, sizeof (msg.msg_id), msg.msg_id_len);
				RETURN_NULL_IF(buf == nullptr);

				buf = p::scpyl(msg.final_date, buf, bend
						, sizeof (msg.final_date), msg.final_date_len);
				RETURN_NULL_IF(buf == nullptr);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*2 > bend);
				buf = p::cp_u8(asbuf(msg.msg_state), buf);
				buf = p::cp_u8(asbuf(msg.error_code), buf);

				if (on_query_sm_r(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_cancel_sm(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				cancel_sm msg;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				buf = p::scpyl(msg.serv_type, buf, bend
						, sizeof (msg.serv_type), msg.serv_type_len);
				RETURN_NULL_IF(buf == nullptr);

				buf = p::scpyl(msg.msg_id, buf, bend
						, sizeof (msg.msg_id), msg.msg_id_len);
				RETURN_NULL_IF(buf == nullptr);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*2 > bend);
				buf = p::cp_u8(asbuf(msg.src_addr_ton), buf);
				buf = p::cp_u8(asbuf(msg.src_addr_npi), buf);
				buf = p::scpyl(msg.src_addr, buf, bend
						, sizeof (msg.src_addr), msg.src_addr_len);
				RETURN_NULL_IF(buf == nullptr);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*2 > bend);
				buf = p::cp_u8(asbuf(msg.dst_addr_ton), buf);
				buf = p::cp_u8(asbuf(msg.dst_addr_npi), buf);
				buf = p::scpyl(msg.dst_addr, buf, bend
						, sizeof (msg.dst_addr), msg.dst_addr_len);
				RETURN_NULL_IF(buf == nullptr);

				if (on_cancel_sm(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_cancel_sm_r(const bin::u8_t * buf
					, const bin::u8_t * bend) {

				using namespace bin;

				cancel_sm_r msg;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				if (on_cancel_sm_r(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_replace_sm(const bin::u8_t * buf
					, const bin::u8_t * bend) {

				using namespace bin;

				replace_sm msg;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				buf = p::scpyl(msg.msg_id, buf, bend
						, sizeof(msg.msg_id), msg.msg_id_len);
				RETURN_NULL_IF(buf == nullptr);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*2 > bend);
				buf = p::cp_u8(asbuf(msg.src_addr_ton), buf);
				buf = p::cp_u8(asbuf(msg.src_addr_npi), buf);
				buf = p::scpyl(msg.src_addr, buf, bend
						, sizeof (msg.src_addr), msg.src_addr_len);

				RETURN_NULL_IF(buf == nullptr);
				buf = p::scpyl(msg.schedule_delivery_time , buf, bend
						, sizeof(msg.schedule_delivery_time)
						, msg.schedule_delivery_time_len);
				RETURN_NULL_IF(buf == nullptr);

				buf = p::scpyl(msg.validity_period, buf, bend
						, sizeof(msg.validity_period)
						, msg.validity_period_len);
				RETURN_NULL_IF(buf == nullptr);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*3 > bend);
				buf = p::cp_u8(asbuf(msg.registered_delivery), buf);
				buf = p::cp_u8(asbuf(msg.sm_default_msg_id), buf);
				buf = p::cp_u8(asbuf(msg.short_msg_len), buf);

				buf = p::scpyl(msg.short_msg, buf, bend
						, sizeof(msg.short_msg), msg.short_msg_len);
				RETURN_NULL_IF(buf == nullptr);

				if (on_replace_sm(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_replace_sm_r(const bin::u8_t * buf
					, const bin::u8_t * bend) {

				using namespace bin;

				replace_sm_r msg;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				if (on_replace_sm_r(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_enquire_link(const bin::u8_t * buf
					, const bin::u8_t * bend) {

				using namespace bin;
				enquire_link msg;
				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				if (on_enquire_link(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_enquire_link_r(const bin::u8_t * buf
					, const bin::u8_t * bend) {

				using namespace bin;
				enquire_link_r msg;
				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);
				if (on_enquire_link_r(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

			const bin::u8_t * parse_alert_notification(const bin::u8_t * buf
					, const bin::u8_t * bend) {

				using namespace bin;
				bin::u16_t optid, len;
				alert_notification msg;

				RETURN_NULL_IF(buf + sizeof(msg.command) > bend);
				buf = parse_pdu(msg.command, buf);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*2 > bend);
				buf = p::cp_u8(asbuf(msg.src_addr_ton), buf);
				buf = p::cp_u8(asbuf(msg.src_addr_npi), buf);
				buf = p::scpyl(msg.src_addr, buf, bend
						, sizeof (msg.src_addr), msg.src_addr_len);
				RETURN_NULL_IF(buf == nullptr);

				RETURN_NULL_IF(buf + sizeof(bin::u8_t)*2 > bend);
				buf = p::cp_u8(asbuf(msg.esme_addr_ton), buf);
				buf = p::cp_u8(asbuf(msg.esme_addr_npi), buf);
				buf = p::scpyl(msg.esme_addr, buf, bend
						, sizeof (msg.esme_addr), msg.esme_addr_len);

				const bin::u8_t * cur;
				while (buf + sizeof(bin::u16_t)*2 <= bend) {
					cur = buf;
					cur = p::cp_u16(asbuf(optid), cur);
					cur = p::cp_u16(asbuf(len), cur);

					switch (optid) {
						case option::ms_availability_status:
							buf = parse_tlv_u8(msg.ms_availability_status, buf);
							break;

						default:
							return nullptr;
					}
				}

				RETURN_NULL_IF(buf == nullptr);

				if (on_alert_notification(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}
	};

} } }

#endif

