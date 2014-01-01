
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include <smpp/proto.hpp>

using namespace mobi::net;

/* TOOLS FOR TESTING */
namespace tst_tools {
	int sncmp(const smpp::proto::u8_t * s1
				, const smpp::proto::u8_t * s2
				, std::size_t l)
	{
		while (l--) if (*s1++ != *s2++) return 1;
		return 0;
	}

	/* overloaded operator for comparing tlv<u8_t> */
	inline bool operator==(const smpp::tlv<smpp::proto::u8_t> & a1
		, const smpp::tlv<smpp::proto::u8_t> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (a1.val != a2.val) return false;

		return true;
	}

	/* overloaded operator for comparing tlv<u8_t> */
	inline bool operator==(const smpp::tlv<smpp::proto::u16_t> & a1
		, const smpp::tlv<smpp::proto::u16_t> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (a1.val != a2.val) return false;

		return true;
	}

	/* overloaded operator for comparing tlv<u8_t> */
	inline bool operator==(const smpp::tlv<smpp::proto::u32_t> & a1
		, const smpp::tlv<smpp::proto::u32_t> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (a1.val != a2.val) return false;

		return true;
	}

	/* for value = u8_t[256] */
	inline bool operator==(const smpp::tlv<smpp::proto::u8_t[256]> & a1
		, const smpp::tlv<smpp::proto::u8_t[256]> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}

	inline bool operator==(const smpp::tlv<smpp::proto::u8_t[65]> & a1
		, const smpp::tlv<smpp::proto::u8_t[65]> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}

	inline bool operator==(const smpp::tlv<smpp::proto::u8_t[23]> & a1
		, const smpp::tlv<smpp::proto::u8_t[23]> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}

	inline bool operator==(const smpp::tlv<smpp::proto::u8_t[19]> & a1
		, const smpp::tlv<smpp::proto::u8_t[19]> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}

	inline bool operator==(const smpp::tlv<smpp::proto::u8_t[3]> & a1
		, const smpp::tlv<smpp::proto::u8_t[3]> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}

	inline bool operator==(const smpp::tlv<smpp::proto::u8_t[2]> & a1
		, const smpp::tlv<smpp::proto::u8_t[2]> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}

	inline bool operator==(const smpp::tlv<smpp::proto::u8_t[1]> & a1
		, const smpp::tlv<smpp::proto::u8_t[1]> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}

	inline bool operator==(const smpp::tlv<smpp::proto::u8p_t> & a1
		, const smpp::tlv<smpp::proto::u8p_t> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}
}

using namespace tst_tools;

BOOST_AUTO_TEST_CASE( submit_pw_test_1 )
{
	using namespace smpp;
	using namespace utl;

	submit_sm			r, r2;
	smpp::proto::u8_t	hand_dat[512];
	union {
		uint32_t		len;
		uint8_t			oct[4];
	};

	/* INITIALIZING POCKET WITH RAND VALUES */
	r.src_addr_ton					= 0x01; /* arbitrary value */
	r.src_addr_npi					= 0x02;
	r.dst_addr_ton					= 0x03;
	r.dst_addr_npi					= 0x05;
	r.esm_class						= 0x73;
	r.protocol_id					= 0x90;
	r.priority_flag					= 0x99;
	r.schedule_delivery_time[0]		= 0x00;
	r.validity_period[0]			= 0x00;
	r.registered_delivery			= 0x00;
	r.replace_if_present_flag 		= 0xAD;
	r.data_coding					= 0x80;
	r.sm_default_msg_id				= 0x13;
	r.sm_len						= sizeof ("hello");

	w::cpy(r.short_msg,	ascbuf("hello"), r.sm_len);

	r.serv_type_len = sizeof ("SMPP");
	w::cpy(r.serv_type, ascbuf("SMPP"), r.serv_type_len);

	r.src_addr_len = sizeof ("GOOGLE.RU");
	w::cpy(r.src_addr, ascbuf("GOOGLE.RU"), r.src_addr_len);

	r.dst_addr_len = sizeof ("GOOGLE.COM");
	w::cpy(r.dst_addr, ascbuf("GOOGLE.COM"), r.dst_addr_len);

		/* OPTIONAL FIELDS */

	r.user_msg_reference.tag 	= option::user_msg_reference;
	r.user_msg_reference.len 	= 2;
	r.user_msg_reference.val	= 17;

	r.command.id					= 0x00000004; /* arbitrary value */
	r.command.seqno					= 0x000E0000; /* arbitrary value */
	r.command.status				= 0x0000FFAC; /* arbitrary value */
		/* total pocket length  */
	r.command.len =	sizeof (pdu)
					+12	/* 1 octet fields */
					+2	/* fixed size string fields */
					+r.sm_len
					+r.serv_type_len
					+r.src_addr_len
					+r.dst_addr_len
					+sizeof (r.user_msg_reference);


	/* WRITING r TO BUFFER */
	BOOST_CHECK
	(
		smpp::write(hand_dat, hand_dat+r.command.len, r, std::cout) != nullptr
	);

	/* READING TO r2 FROM BUFFER */
		/*	getting access to first 4 octets from buffer,
			there is length of whole
			data is written */
	for (int i = 0; i < 4; ++i) oct[i] = *(hand_dat+3-i);
	BOOST_CHECK
	(
		smpp::parse(r2, hand_dat, hand_dat+len, std::cout) != nullptr
	);


	/* COMPARING r & r2 */
	BOOST_CHECK(r.command.id		== r2.command.id);
	BOOST_CHECK(r.command.seqno		== r2.command.seqno);
	BOOST_CHECK(r.command.status	== r2.command.status);
	BOOST_CHECK(r.command.len 		== r2.command.len);
	BOOST_CHECK(r.src_addr_ton 		== r2.src_addr_ton);
	BOOST_CHECK(r.src_addr_npi		== r2.src_addr_npi);
	BOOST_CHECK(r.dst_addr_ton		== r2.dst_addr_ton);
	BOOST_CHECK(r.dst_addr_npi		== r2.dst_addr_npi);
	BOOST_CHECK(r.esm_class			== r2.esm_class);
	BOOST_CHECK(r.protocol_id		== r2.protocol_id);
	BOOST_CHECK(r.priority_flag		== r2.priority_flag);

	BOOST_CHECK
	(
		sncmp(r.schedule_delivery_time
			, r2.schedule_delivery_time
			, 0) == 0
	);
	BOOST_CHECK
	(
		sncmp(r.validity_period
			, r2.validity_period
			, 0) == 0
	);
	BOOST_CHECK(r.registered_delivery == r2.registered_delivery);
	BOOST_CHECK(r.replace_if_present_flag == r2.replace_if_present_flag);
	BOOST_CHECK(r.data_coding == r2.data_coding);
	BOOST_CHECK(r.sm_default_msg_id == r2.sm_default_msg_id);

	/* compare string len before */
	BOOST_CHECK(r.sm_len == r2.sm_len);
	BOOST_CHECK
	(
		sncmp(r.short_msg
			, r2.short_msg
			, r.sm_len) == 0
	);

	/* compare string len before */
	BOOST_CHECK(r.serv_type_len == r2.serv_type_len);
	BOOST_CHECK
	(
		sncmp(r.serv_type
			, r2.serv_type
			, r.serv_type_len) == 0
	);

	BOOST_CHECK(r.src_addr_len == r2.src_addr_len);
	BOOST_CHECK
	(
		sncmp(r.src_addr, r2.src_addr, r.src_addr_len) == 0
	);

	BOOST_CHECK(r.dst_addr_len == r2.dst_addr_len);
	BOOST_CHECK
	(
		sncmp(r.dst_addr, r2.dst_addr, r.dst_addr_len) == 0
	);

	BOOST_CHECK(r.user_msg_reference	== r2.user_msg_reference);
	BOOST_CHECK(r.src_port				== r2.src_port);
	BOOST_CHECK(r.src_addr_subunit		== r2.src_addr_subunit);
	BOOST_CHECK(r.dest_port				== r2.dest_port);
	BOOST_CHECK(r.dest_addr_subunit		== r2.dest_addr_subunit);
	BOOST_CHECK(r.sar_msg_ref_num		== r2.sar_msg_ref_num);
	BOOST_CHECK(r.sar_total_segments	== r2.sar_total_segments);
	BOOST_CHECK(r.sar_segment_seqnum	== r2.sar_segment_seqnum);
	BOOST_CHECK(r.more_msgs_to_send		== r2.more_msgs_to_send);
	BOOST_CHECK(r.payload_type			== r2.payload_type);
	BOOST_CHECK(r.msg_payload			== r2.msg_payload);
	BOOST_CHECK(r.privacy_ind			== r2.privacy_ind);
	BOOST_CHECK(r.callback_num			== r2.callback_num);
	BOOST_CHECK(r.callback_num_pres_ind	== r2.callback_num_pres_ind);
	BOOST_CHECK(r.callback_num_atag		== r2.callback_num_atag);
	BOOST_CHECK(r.src_subaddr			== r2.src_subaddr);
	BOOST_CHECK(r.dest_subaddr			== r2.dest_subaddr);
	BOOST_CHECK(r.user_resp_code		== r2.user_resp_code);
	BOOST_CHECK(r.display_time			== r2.display_time);
	BOOST_CHECK(r.sms_signal			== r2.sms_signal);
	BOOST_CHECK(r.ms_validity			== r2.ms_validity);
	BOOST_CHECK(r.ms_msg_wait_fclts		== r2.ms_msg_wait_fclts);
	BOOST_CHECK(r.number_of_msgs		== r2.number_of_msgs);
	BOOST_CHECK(r.alert_on_msg_delivery	== r2.alert_on_msg_delivery);
	BOOST_CHECK(r.lang_ind				== r2.lang_ind);
	BOOST_CHECK(r.its_reply_type		== r2.its_reply_type);
	BOOST_CHECK(r.its_session_info		== r2.its_session_info);
	BOOST_CHECK(r.ussd_serv_op			== r2.ussd_serv_op);
}

