
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include <smpp/proto.hpp>

using namespace mobi::net;
using namespace mobi::net::toolbox;

/* TOOLS FOR TESTING */
namespace tst_tools {
	int sncmp(const bin::u8_t * s1
				, const bin::u8_t * s2
				, std::size_t l)
	{
		while (l--) if (*s1++ != *s2++) return 1;
		return 0;
	}

	/* overloaded operator for comparing tlv<u8_t> */
	inline bool operator==(const smpp::tlv<bin::u8_t> & a1
		, const smpp::tlv<bin::u8_t> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (a1.val != a2.val) return false;

		return true;
	}

	/* overloaded operator for comparing tlv<u8_t> */
	inline bool operator==(const smpp::tlv<bin::u16_t> & a1
		, const smpp::tlv<bin::u16_t> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (a1.val != a2.val) return false;

		return true;
	}

	/* overloaded operator for comparing tlv<u8_t> */
	inline bool operator==(const smpp::tlv<bin::u32_t> & a1
		, const smpp::tlv<bin::u32_t> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (a1.val != a2.val) return false;

		return true;
	}

	/* for value = u8_t[256] */
	inline bool operator==(const smpp::tlv<bin::u8_t[256]> & a1
		, const smpp::tlv<bin::u8_t[256]> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}

	inline bool operator==(const smpp::tlv<bin::u8_t[65]> & a1
		, const smpp::tlv<bin::u8_t[65]> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}

	inline bool operator==(const smpp::tlv<bin::u8_t[23]> & a1
		, const smpp::tlv<bin::u8_t[23]> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}

	inline bool operator==(const smpp::tlv<bin::u8_t[19]> & a1
		, const smpp::tlv<bin::u8_t[19]> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}

	inline bool operator==(const smpp::tlv<bin::u8_t[3]> & a1
		, const smpp::tlv<bin::u8_t[3]> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}

	inline bool operator==(const smpp::tlv<bin::u8_t[2]> & a1
		, const smpp::tlv<bin::u8_t[2]> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}

	inline bool operator==(const smpp::tlv<bin::u8_t[1]> & a1
		, const smpp::tlv<bin::u8_t[1]> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}

	inline bool operator==(const smpp::tlv<bin::u8_t *> & a1
		, const smpp::tlv<bin::u8_t *> & a2)
	{
		if (a1.tag != a2.tag) return false;
		if (a1.len != a2.len) return false;
		if (sncmp(a1.val, a2.val, a1.len) != 0) return false;

		return true;
	}
}

#define SET_STRING(fld, cstr)	do {										\
									fld ## _len = sizeof(cstr); 			\
									w::cpy(fld, ascbuf(cstr), fld ## _len);	\
								} while (0)

using namespace tst_tools;

class smpp_parser : public smpp::parser<std::ostream> {
	public:
		smpp_parser(std::ostream & out): smpp::parser<std::ostream>(out) {}
		virtual ~smpp_parser() {}

	protected:
		virtual action on_bind_transmitter(const smpp::bind_transmitter & msg) {

			std::cout << std::endl;
			std::cout << "bind_transmitter" << std::endl;
			std::cout << "[ command.len: " << msg.command.len << " ]" << std::endl;
			std::cout << "[ command.id: " << msg.command.id << " ]" << std::endl;
			std::cout << "[ command.status: " << msg.command.status << " ]" << std::endl;
			std::cout << "[ command.seqno: " << msg.command.seqno << " ]" << std::endl;

			std::cout << "[ sys_id: " << msg.sys_id << " ]" << std::endl;
			std::cout << "[ password: " << msg.password << " ]" << std::endl;
			std::cout << "[ sys_type: " << msg.sys_type << " ]" << std::endl;
			std::cout << "[ interface_version: " << static_cast<unsigned>(msg.interface_version) << " ]" << std::endl;
			std::cout << "[ addr_ton: " << static_cast<unsigned>(msg.addr_ton) << " ]" << std::endl;
			std::cout << "[ addr_npi: " << static_cast<unsigned>(msg.addr_npi) << " ]" << std::endl;
			std::cout << "[ addr_range: " << msg.addr_range << " ]" << std::endl;

			return resume;
		}

		virtual action on_bind_transmitter_r(const smpp::bind_transmitter_r & msg) {

			std::cout << std::endl;
			std::cout << "bind_transmitter_r" << std::endl;
			std::cout << "[ command.len: " << msg.command.len << " ]" << std::endl;
			std::cout << "[ command.id: " << msg.command.id << " ]" << std::endl;
			std::cout << "[ command.status: " << msg.command.status << " ]" << std::endl;
			std::cout << "[ command.seqno: " << msg.command.seqno << " ]" << std::endl;

			std::cout << "[ sys_id: " << msg.sys_id << " ]" << std::endl;

			std::cout << "[ sc_interface_version.tag: " << msg.sc_interface_version.tag << " ]" << std::endl;
			std::cout << "[ sc_interface_version.len: " << msg.sc_interface_version.len << " ]" << std::endl;
			std::cout << "[ sc_interface_version.val: " << static_cast<unsigned>(msg.sc_interface_version.val) << " ]" << std::endl;

			return resume;
		}

		virtual action on_bind_receiver(const smpp::bind_receiver & msg) {

			std::cout << std::endl;
			std::cout << "bind_receiver" << std::endl;
			std::cout << "[ command.len: " << msg.command.len << " ]" << std::endl;
			std::cout << "[ command.id: " << msg.command.id << " ]" << std::endl;
			std::cout << "[ command.status: " << msg.command.status << " ]" << std::endl;
			std::cout << "[ command.seqno: " << msg.command.seqno << " ]" << std::endl;

			std::cout << "[ sys_id: " << msg.sys_id << " ]" << std::endl;
			std::cout << "[ password: " << msg.password << " ]" << std::endl;
			std::cout << "[ sys_type: " << msg.sys_type << " ]" << std::endl;
			std::cout << "[ interface_version: " << static_cast<unsigned>(msg.interface_version) << " ]" << std::endl;
			std::cout << "[ addr_ton: " << static_cast<unsigned>(msg.addr_ton) << " ]" << std::endl;
			std::cout << "[ addr_npi: " << static_cast<unsigned>(msg.addr_npi) << " ]" << std::endl;
			std::cout << "[ addr_range: " << msg.addr_range << " ]" << std::endl;

			return resume;
		}

		virtual action on_bind_receiver_r(const smpp::bind_receiver_r & msg) {

			std::cout << std::endl;
			std::cout << "bind_receiver_r" << std::endl;
			std::cout << "[ command.len: " << msg.command.len << " ]" << std::endl;
			std::cout << "[ command.id: " << msg.command.id << " ]" << std::endl;
			std::cout << "[ command.status: " << msg.command.status << " ]" << std::endl;
			std::cout << "[ command.seqno: " << msg.command.seqno << " ]" << std::endl;

			std::cout << "[ sys_id: " << msg.sys_id << " ]" << std::endl;
			std::cout << "[ sc_interface_version.tag: " << msg.sc_interface_version.tag << " ]" << std::endl;
			std::cout << "[ sc_interface_version.len: " << msg.sc_interface_version.len << " ]" << std::endl;
			std::cout << "[ sc_interface_version.val: " << static_cast<unsigned>(msg.sc_interface_version.val) << " ]" << std::endl;

			return resume;
		}

		virtual action on_bind_transceiver(const smpp::bind_transceiver & msg) {

			std::cout << std::endl;
			std::cout << "bind_transceiver" << std::endl;
			std::cout << "[ command.len: " << msg.command.len << " ]" << std::endl;
			std::cout << "[ command.id: " << msg.command.id << " ]" << std::endl;
			std::cout << "[ command.status: " << msg.command.status << " ]" << std::endl;
			std::cout << "[ command.seqno: " << msg.command.seqno << " ]" << std::endl;

			std::cout << "[ sys_id: " << msg.sys_id << " ]" << std::endl;
			std::cout << "[ password: " << msg.password << " ]" << std::endl;
			std::cout << "[ sys_type: " << msg.sys_type << " ]" << std::endl;
			std::cout << "[ interface_version: " << static_cast<unsigned>(msg.interface_version) << " ]" << std::endl;
			std::cout << "[ addr_ton: " << static_cast<unsigned>(msg.addr_ton) << " ]" << std::endl;
			std::cout << "[ addr_npi: " << static_cast<unsigned>(msg.addr_npi) << " ]" << std::endl;
			std::cout << "[ addr_range: " << msg.addr_range << " ]" << std::endl;

			return resume;
		}

		virtual action on_bind_transceiver_r(const smpp::bind_transceiver_r & msg) {

			std::cout << std::endl;
			std::cout << "bind_transceiver_r" << std::endl;
			std::cout << "[ command.len: " << msg.command.len << " ]" << std::endl;
			std::cout << "[ command.id: " << msg.command.id << " ]" << std::endl;
			std::cout << "[ command.status: " << msg.command.status << " ]" << std::endl;
			std::cout << "[ command.seqno: " << msg.command.seqno << " ]" << std::endl;

			std::cout << "[ sys_id: " << msg.sys_id << " ]" << std::endl;
			std::cout << "[ sc_interface_version.tag: " << msg.sc_interface_version.tag << " ]" << std::endl;
			std::cout << "[ sc_interface_version.len: " << msg.sc_interface_version.len << " ]" << std::endl;
			std::cout << "[ sc_interface_version.val: " << static_cast<unsigned>(msg.sc_interface_version.val) << " ]" << std::endl;

			return resume;
		}

		virtual action on_unbind(const smpp::unbind & msg) {

			std::cout << std::endl;
			std::cout << "unbind" << std::endl;
			std::cout << "[ command.len: " << msg.command.len << " ]" << std::endl;
			std::cout << "[ command.id: " << msg.command.id << " ]" << std::endl;
			std::cout << "[ command.status: " << msg.command.status << " ]" << std::endl;
			std::cout << "[ command.seqno: " << msg.command.seqno << " ]" << std::endl;

			return resume;
		}

		virtual action on_unbind_r(const smpp::unbind_r & msg) {

			std::cout << std::endl;
			std::cout << "unbind_r" << std::endl;
			std::cout << "[ command.len: " << msg.command.len << " ]" << std::endl;
			std::cout << "[ command.id: " << msg.command.id << " ]" << std::endl;
			std::cout << "[ command.status: " << msg.command.status << " ]" << std::endl;
			std::cout << "[ command.seqno: " << msg.command.seqno << " ]" << std::endl;

			return resume;
		}

		virtual action on_outbind(const smpp::outbind & msg) {

			std::cout << std::endl;
			std::cout << "outbind" << std::endl;
			std::cout << "[ command.len: " << msg.command.len << " ]" << std::endl;
			std::cout << "[ command.id: " << msg.command.id << " ]" << std::endl;
			std::cout << "[ command.status: " << msg.command.status << " ]" << std::endl;
			std::cout << "[ command.seqno: " << msg.command.seqno << " ]" << std::endl;

			std::cout << "[ sys_id: " << msg.sys_id << " ]" << std::endl;
			std::cout << "[ sys_password: " << msg.password << " ]" << std::endl;

			return resume;
		}

		virtual action on_generic_nack(const smpp::generic_nack & msg) {

			std::cout << std::endl;
			std::cout << "generic_nack" << std::endl;
			std::cout << "[ command.len: " << msg.command.len << " ]" << std::endl;
			std::cout << "[ command.id: " << msg.command.id << " ]" << std::endl;
			std::cout << "[ command.status: " << msg.command.status << " ]" << std::endl;
			std::cout << "[ command.seqno: " << msg.command.seqno << " ]" << std::endl;
			return resume;
		}

		virtual action on_submit_sm(const smpp::submit_sm & msg) {

			using namespace smpp;

			std::cout << std::endl;
			std::cout << "submit_sm" << std::endl;
			std::cout << "[ command.len: " << msg.command.len << " ]" << std::endl;
			std::cout << "[ command.id: " << msg.command.id << " ]" << std::endl;
			std::cout << "[ command.status: " << msg.command.status << " ]" << std::endl;
			std::cout << "[ command.seqno: " << msg.command.seqno << " ]" << std::endl;
			std::cout << "[ serv_type: " << msg.serv_type << " ]" << std::endl;
			std::cout << "[ src_addr_ton: " << static_cast<unsigned>(msg.src_addr_ton) << " ]" << std::endl;
			std::cout << "[ src_addr_npi: " << static_cast<unsigned>(msg.src_addr_npi) << " ]" << std::endl;
			std::cout << "[ src_addr: " << msg.src_addr << " ]" << std::endl;
			std::cout << "[ dst_addr_ton: " << static_cast<unsigned>(msg.dst_addr_ton) << " ]" << std::endl;
			std::cout << "[ dst_addr_npi: " << static_cast<unsigned>(msg.dst_addr_npi) << " ]" << std::endl;
			std::cout << "[ dst_addr: " << msg.dst_addr << " ]" << std::endl;
			std::cout << "[ esm_class: " << static_cast<unsigned>(msg.esm_class) << " ]" << std::endl;
			std::cout << "[ protocol_id: " << static_cast<unsigned>(msg.protocol_id) << " ]" << std::endl;
			std::cout << "[ priority_flag: " << static_cast<unsigned>(msg.priority_flag) << " ]" << std::endl;
			std::cout << "[ schedule_delivery_time: " << msg.schedule_delivery_time << " ]" << std::endl;
			std::cout << "[ validity_period: " << msg.validity_period << " ]" << std::endl;
			std::cout << "[ registered_delivery: " << static_cast<unsigned>(msg.registered_delivery) << " ]" << std::endl;
			std::cout << "[ replace_if_present_flag: " << static_cast<unsigned>(msg.replace_if_present_flag) << " ]" << std::endl;
			std::cout << "[ data_coding: " << static_cast<unsigned>(msg.data_coding) << " ]" << std::endl;
			std::cout << "[ sm_default_msg_id: " << static_cast<unsigned>(msg.sm_default_msg_id) << " ]" << std::endl;
			std::cout << "[ short_msg_len: " << static_cast<unsigned>(msg.short_msg_len) << " ]" << std::endl;
			std::cout << "[ short_msg: " << msg.short_msg << " ]" << std::endl;

			if (msg.user_msg_reference.tag == option::user_msg_reference) {
				std::cout << "[ user_msg_reference: " << msg.user_msg_reference.val << " ]" << std::endl;
			}
			if (msg.src_port.tag == option::src_port) {
				std::cout << "[ src_port: " << msg.src_port.val << " ]" << std::endl;
			}
			if (msg.src_addr_subunit.tag == option::src_addr_subunit) {
				std::cout << "[ src_addr_subunit: " << src_addr_subunit.val << " ]" << std::endl;
			}
			if (msg.dest_port.tag == option::dest_port) {
				std::cout << "[ dest_port: " << msg.dest_port.val << " ]" << std::endl;
			}
			if (msg.dest_addr_subunit.tag == option::dest_addr_subunit) {
				std::cout << "[ dest_addr_subunit: " << msg.dest_addr_subunit.val << " ]" << std::endl;
			}
			if (msg.sar_msg_ref_num.tag == option::sar_msg_ref_num) {
				std::cout << "[ sar_msg_ref_num: " << msg.sar_msg_ref_num.val << " ]" << std::endl;
			}
			if (msg.sar_total_segments.tag == option::sar_total_segments) {
				std::cout << "[ sar_total_segments: " << msg.sar_total_segments.val << " ]" << std::endl;
			}
			if (msg.sar_segment_seqnum.tag == option::sar_segment_seqnum) {
				std::cout << "[ sar_segment_seqnum: " << msg.sar_segment_seqnum.val << " ]" << std::endl;
			}
			if (msg.more_msgs_to_send.tag == option::more_msgs_to_send) {
				std::cout << "[ more_msgs_to_send: " << msg.more_msgs_to_send.val << " ]" << std::endl;
			}
			if (msg.payload_type.tag == option::payload_type) {
				std::cout << "[ payload_type: " << msg.payload_type.val << " ]" << std::endl;
			}
			if (msg.msg_payload.tag == option::msg_payload) {
				std::cout << "[ msg_payload: " << msg.msg_payload.val << std::endl;
			}
			if (msg.privacy_ind.tag == option::privacy_ind) {
				std::cout << "[ privacy_ind: " << msg.privacy_ind.val << " ]" << std::endl;
			}
			if (msg.callback_num.tag == option::privacy_ind) {
				std::cout << "[ callback_num: " << msg.callback_num.val << " ]" << std::endl;
			}
			if (msg.callback_num_pres_ind.tag == option::callback_num_pres_ind) {
				std::cout << "[ callback_num_pres_ind: " << msg.callback_num_pres_ind.val << " ]" << std::endl;
			}
			if (msg.callback_num_atag.tag == option::callback_num_atag) {
				std::cout << "[ callback_num_atag: " << msg.callback_num_atag.val << " ]" << std::endl;
			}
			if (msg.src_subaddr.tag == option::src_subaddr) {
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


			return resume;
		}

		virtual action on_submit_sm_r(const smpp::submit_sm_r & msg) {
			(void)(msg);
			return resume;
		}

		virtual action on_parse_error(const smpp::bin::u8_t * buf, const smpp::bin::u8_t * bend) {
			(void)(buf);
			(void)(bend);
			return resume;
		}

		virtual action on_submit_multi_sm(const smpp::submit_multi_sm & msg) {
			(void)(msg);
			return resume;
		}
};

BOOST_AUTO_TEST_CASE( test_pw_bind_transmitter )
{
	using namespace smpp;
	using namespace bin;

	writer<std::ostream> w(std::cout);
	smpp_parser p(std::cout);

	bind_transmitter msg;
	/* mandatory fileds */
	SET_STRING(msg.sys_id,			"MATRIX");
	SET_STRING(msg.password,		"WRABBIT");
	SET_STRING(msg.sys_type,		"THRILLER");
	SET_STRING(msg.addr_range,		"MATRIX_HAS_YOU");
	msg.interface_version			= 0x10;
	msg.addr_ton					= 0x10;
	msg.addr_npi					= 0x10;
	/* setting up pdu */
	msg.command.id					= command::bind_transmitter; /* arbitrary value */
	msg.command.seqno				= 0x00000010; /* arbitrary value */
	msg.command.status				= 0x00000010; /* arbitrary value */
	msg.command.len				= msg.raw_size();

	bin::sz_t _buf[0x100] = {};
	bin::u8_t * buf		= asbuf(_buf);
	bin::u8_t * bend	= asbuf(_buf) + msg.command.len;

	BOOST_CHECK(w.write_bind_transmitter(buf, bend, msg) != nullptr);
	BOOST_CHECK(p.parse(buf, bend) != nullptr);
}

BOOST_AUTO_TEST_CASE( test_pw_bind_transmitter_r )
{
	using namespace smpp;
	using namespace bin;

	writer<std::ostream>	w(std::cout);
	smpp_parser				p(std::cout);

	bind_transmitter_r msg;

	msg.sc_interface_version.tag	= option::sc_interface_version;
	msg.sc_interface_version.len	= 1;
	msg.sc_interface_version.val	= 0x10;

	SET_STRING(msg.sys_id,		"LINUX");
	msg.command.id				= command::bind_transmitter_r;
	msg.command.seqno			= 0x00000010;
	msg.command.status			= 0x00000010;
	msg.command.len				= msg.raw_size();

	bin::sz_t _buf[0x100] = {};
	bin::u8_t * buf = asbuf(_buf);
	bin::u8_t * bend = asbuf(_buf) + msg.command.len;

	BOOST_CHECK(w.write_bind_transmitter_r(buf, bend, msg) != nullptr);
	BOOST_CHECK(p.parse(buf, bend) != nullptr);
}

BOOST_AUTO_TEST_CASE( test_pw_outbind )
{
	using namespace smpp;
	using namespace bin;

	writer<std::ostream>	w(std::cout);
	smpp_parser				p(std::cout);

	outbind msg;
	SET_STRING(msg.password,		"PASSWD");
	SET_STRING(msg.sys_id,		"LINUX");
	msg.command.id				= command::outbind;
	msg.command.seqno			= 0x00000010;
	msg.command.status			= 0x00000010;
	msg.command.len				= msg.raw_size();

	bin::sz_t _buf[0x100] = {};
	bin::u8_t * buf = asbuf(_buf);
	bin::u8_t * bend = asbuf(_buf) + msg.command.len;

	BOOST_CHECK(w.write_outbind(buf, bend, msg) != nullptr);
	BOOST_CHECK(p.parse(buf, bend) != nullptr);
}

BOOST_AUTO_TEST_CASE( test_pw_bind_receiver )
{
	using namespace smpp;
	using namespace bin;

	writer<std::ostream>	w(std::cout);
	smpp_parser				p(std::cout);

	bind_receiver msg;
	SET_STRING(msg.password,	"PASSWD");
	SET_STRING(msg.sys_id,		"BIND_RECEIVER");
	SET_STRING(msg.sys_type,	"BIND_RECV_T");
	msg.interface_version		= 0x10;
	msg.addr_ton				= 0x10;
	msg.addr_npi				= 0x10;
	SET_STRING(msg.addr_range,	"BIND_RECV_RANGE");

	msg.command.id				= command::bind_receiver;
	msg.command.seqno			= 0x00000010;
	msg.command.status			= 0x00000010;
	msg.command.len				= msg.raw_size();

	bin::sz_t _buf[0x100] = {};
	bin::u8_t * buf = asbuf(_buf);
	bin::u8_t * bend = asbuf(_buf) + msg.command.len;

	BOOST_CHECK(w.write_bind_receiver(buf, bend, msg) != nullptr);
	BOOST_CHECK(p.parse(buf, bend) != nullptr);
}

BOOST_AUTO_TEST_CASE( test_pw_bind_receiver_r )
{
	using namespace smpp;
	using namespace bin;

	writer<std::ostream>	w(std::cout);
	smpp_parser				p(std::cout);

	bind_receiver_r msg;
	SET_STRING(msg.sys_id,			"BIND_RECV_R");
	msg.sc_interface_version.tag	= option::sc_interface_version;
	msg.sc_interface_version.len	= 1;
	msg.sc_interface_version.val	= 0x10;

	msg.command.id				= command::bind_receiver_r;
	msg.command.seqno			= 0x00000010;
	msg.command.status			= 0x00000010;
	msg.command.len				= msg.raw_size();

	bin::sz_t _buf[0x100] = {};
	bin::u8_t * buf = asbuf(_buf);
	bin::u8_t * bend = asbuf(_buf) + msg.command.len;

	BOOST_CHECK(w.write_bind_receiver_r(buf, bend, msg) != nullptr);
	BOOST_CHECK(p.parse(buf, bend) != nullptr);
}

BOOST_AUTO_TEST_CASE( test_pw_bind_transceiver )
{
	using namespace smpp;
	using namespace bin;

	writer<std::ostream>	w(std::cout);
	smpp_parser				p(std::cout);

	bind_transceiver msg;
	SET_STRING(msg.password,	"PASSWD");
	SET_STRING(msg.sys_id,		"B_TRANSCEIVER");
	SET_STRING(msg.sys_type,	"B_TRANSC_T");
	msg.interface_version		= 0x10;
	msg.addr_ton				= 0x10;
	msg.addr_npi				= 0x10;
	SET_STRING(msg.addr_range,	"RANGE");

	msg.command.id				= command::bind_transceiver;
	msg.command.seqno			= 0x00000010;
	msg.command.status			= 0x00000010;
	msg.command.len				= msg.raw_size();

	bin::sz_t _buf[0x100] = {};
	bin::u8_t * buf = asbuf(_buf);
	bin::u8_t * bend = asbuf(_buf) + msg.command.len;

	BOOST_CHECK(w.write_bind_transceiver(buf, bend, msg) != nullptr);
	BOOST_CHECK(p.parse(buf, bend) != nullptr);
}

BOOST_AUTO_TEST_CASE( test_pw_bind_transceiver_r )
{
	using namespace smpp;
	using namespace bin;

	writer<std::ostream>	w(std::cout);
	smpp_parser				p(std::cout);

	bind_transceiver_r msg;
	SET_STRING(msg.sys_id,		"B_TRANSCEIVER_R");
	msg.sc_interface_version.tag	= option::sc_interface_version;
	msg.sc_interface_version.len	= 1;
	msg.sc_interface_version.val	= 0x10;

	msg.command.id				= command::bind_transceiver_r;
	msg.command.seqno			= 0x00000010;
	msg.command.status			= 0x00000010;
	msg.command.len				= msg.raw_size();

	bin::sz_t _buf[0x100] = {};
	bin::u8_t * buf = asbuf(_buf);
	bin::u8_t * bend = asbuf(_buf) + msg.command.len;

	BOOST_CHECK(w.write_bind_transceiver_r(buf, bend, msg) != nullptr);
	BOOST_CHECK(p.parse(buf, bend) != nullptr);
}

BOOST_AUTO_TEST_CASE( test_pw_unbind )
{
	using namespace smpp;
	using namespace bin;

	writer<std::ostream>	w(std::cout);
	smpp_parser				p(std::cout);

	unbind msg;
	msg.command.id				= command::unbind;
	msg.command.seqno			= 0x00000010;
	msg.command.status			= 0x00000010;
	msg.command.len				= msg.raw_size();

	bin::sz_t _buf[0x100] = {};
	bin::u8_t * buf = asbuf(_buf);
	bin::u8_t * bend = asbuf(_buf) + msg.command.len;

	BOOST_CHECK(w.write_unbind(buf, bend, msg) != nullptr);
	BOOST_CHECK(p.parse(buf, bend) != nullptr);
}

BOOST_AUTO_TEST_CASE( test_pw_unbind_r )
{
	using namespace smpp;
	using namespace bin;

	writer<std::ostream>	w(std::cout);
	smpp_parser				p(std::cout);

	unbind_r msg;
	msg.command.id				= command::unbind_r;
	msg.command.seqno			= 0x00000010;
	msg.command.status			= 0x00000010;
	msg.command.len				= msg.raw_size();

	bin::sz_t _buf[0x100] = {};
	bin::u8_t * buf = asbuf(_buf);
	bin::u8_t * bend = asbuf(_buf) + msg.command.len;

	BOOST_CHECK(w.write_unbind_r(buf, bend, msg) != nullptr);
	BOOST_CHECK(p.parse(buf, bend) != nullptr);
}

BOOST_AUTO_TEST_CASE( test_pw_generic_nack )
{
	using namespace smpp;
	using namespace bin;

	writer<std::ostream>	w(std::cout);
	smpp_parser				p(std::cout);

	generic_nack msg;
	msg.command.id				= command::generic_nack;
	msg.command.seqno			= 0x00000010;
	msg.command.status			= 0x00000010;
	msg.command.len				= msg.raw_size();

	bin::sz_t _buf[0x100] = {};
	bin::u8_t * buf = asbuf(_buf);
	bin::u8_t * bend = asbuf(_buf) + msg.command.len;

	BOOST_CHECK(w.write_generic_nack(buf, bend, msg) != nullptr);
	BOOST_CHECK(p.parse(buf, bend) != nullptr);
}

BOOST_AUTO_TEST_CASE( test_pw_submit_sm )
{
	using namespace smpp;
	using namespace bin;

	writer<std::ostream>	w(std::cout);
	smpp_parser				p(std::cout);

	submit_sm msg;
	/* mandatory fields */
	SET_STRING(msg.serv_type,	"SUBS");
	msg.src_addr_ton			= 0x10;
	msg.src_addr_npi			= 0x10;
	SET_STRING(msg.src_addr,	"SUBMIT_SM");
	msg.dst_addr_ton			= 0x10;
	msg.dst_addr_npi			= 0x10;
	SET_STRING(msg.dst_addr,	"SUBMIT_SM");
	msg.esm_class				= 0x10;
	msg.protocol_id				= 0x10;
	msg.priority_flag			= 0x10;
	SET_STRING(msg.schedule_delivery_time,	"17_LEN_STR______");
	SET_STRING(msg.validity_period,			"17_len_STR______");
	msg.registered_delivery		= 0x10;
	msg.replace_if_present_flag	= 0x10;
	msg.data_coding				= 0x10;
	msg.sm_default_msg_id		= 0x10;
	SET_STRING(msg.short_msg,	"SUBMIT_SM_SHORT_MESSAGE");
	/* optional fields */

	msg.user_msg_reference.tag	= option::user_msg_reference;
	msg.user_msg_reference.len	= 2;
	msg.user_msg_reference.val	= 0x10;

	msg.command.id				= command::submit_sm;
	msg.command.seqno			= 0x00000010;
	msg.command.status			= 0x00000010;
	msg.command.len				= msg.raw_size();

	bin::sz_t _buf[0x100] = {};
	bin::u8_t * buf = asbuf(_buf);
	bin::u8_t * bend = asbuf(_buf) + msg.command.len;

	BOOST_CHECK(w.write_submit_sm(buf, bend, msg) != nullptr);
	BOOST_CHECK(p.parse(buf, bend) != nullptr);
}


#if 0
BOOST_AUTO_TEST_CASE( parser_class_test )
{
	smpp_parser p(std::cout);
	const bin::u8_t raw_dat[] =
		"\x00\x00\x00\x20\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x01"
	  	"\x74\x65\x73\x74\x00\x74\x65\x73\x74\x31\x00\x00\x33\x00\x00\x00";

	p.parse(raw_dat, raw_dat + sizeof(raw_dat)-1);
}
BOOST_AUTO_TEST_CASE( submit_pw_test_1 )
{
	using namespace smpp;
	using namespace bin;

	submit_sm	r, r2;
	bin::u8_t	buf[512];
	bin::u32_t	len;

	/* INITIALIZING PACKET WITH RAND VALUES */
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
	SET_STRING(r.short_msg,			"GITHUB");
	SET_STRING(r.serv_type,			"LINUX");
	SET_STRING(r.src_addr,			"GNU");
	SET_STRING(r.dst_addr,			"DP IS POWER");
	/* OPTIONAL FIELDS */
	r.user_msg_reference.tag 		= option::user_msg_reference;
	r.user_msg_reference.len 		= 2;
	r.user_msg_reference.val		= 17;
	/* PDU INITIALIZING */
	r.command.id					= command::generic_nack; /* arbitrary value */
	r.command.seqno					= 0x000E0000; /* arbitrary value */
	r.command.status				= 0x0000FFAC; /* arbitrary value */
	/* TOTAL PACKET LENGTH  */
	r.command.len =	sizeof (pdu)
					+12	/* 1 octet fields */
					+2	/* fixed size string fields */
					+r.short_msg_len
					+r.serv_type_len
					+r.src_addr_len
					+r.dst_addr_len
					+sizeof (r.user_msg_reference);
	/* WRITING r TO BUFFER */
	BOOST_CHECK(smpp::write(buf, buf+r.command.len, r, std::cout) != nullptr);
	/* READING TO r2 FROM BUFFER */
	p::cp_u32(asbuf(&len), ascbuf(buf));
	BOOST_CHECK(smpp::parse(r2, buf, buf+len, std::cout) != nullptr);
	/* COMPARING r & r2 */
	BOOST_CHECK(r.command.id						== r2.command.id);
	BOOST_CHECK(r.command.seqno						== r2.command.seqno);
	BOOST_CHECK(r.command.status					== r2.command.status);
	BOOST_CHECK(r.command.len 						== r2.command.len);
	BOOST_CHECK(r.src_addr_ton 						== r2.src_addr_ton);
	BOOST_CHECK(r.src_addr_npi						== r2.src_addr_npi);
	BOOST_CHECK(r.dst_addr_ton						== r2.dst_addr_ton);
	BOOST_CHECK(r.dst_addr_npi						== r2.dst_addr_npi);
	BOOST_CHECK(r.esm_class							== r2.esm_class);
	BOOST_CHECK(r.protocol_id						== r2.protocol_id);
	BOOST_CHECK(r.priority_flag						== r2.priority_flag);
	BOOST_CHECK(sncmp(r.schedule_delivery_time,		r2.schedule_delivery_time, 0)==0);
	BOOST_CHECK(sncmp(r.validity_period,			r2.validity_period, 0)==0);
	BOOST_CHECK(r.registered_delivery				== r2.registered_delivery);
	BOOST_CHECK(r.replace_if_present_flag			== r2.replace_if_present_flag);
	BOOST_CHECK(r.data_coding						== r2.data_coding);
	BOOST_CHECK(r.sm_default_msg_id					== r2.sm_default_msg_id);
	BOOST_CHECK(r.short_msg_len						== r2.short_msg_len);
	BOOST_CHECK(sncmp(r.short_msg,					r2.short_msg, r.short_msg_len)==0);
	BOOST_CHECK(r.serv_type_len						== r2.serv_type_len);
	BOOST_CHECK(sncmp(r.serv_type,					r2.serv_type, r.serv_type_len) == 0);
	BOOST_CHECK(r.src_addr_len						== r2.src_addr_len);
	BOOST_CHECK(sncmp(r.src_addr,					r2.src_addr, r.src_addr_len) == 0);
	BOOST_CHECK(r.dst_addr_len						== r2.dst_addr_len);
	BOOST_CHECK(sncmp(r.dst_addr,					r2.dst_addr, r.dst_addr_len) == 0);
	BOOST_CHECK(r.user_msg_reference				== r2.user_msg_reference);
	BOOST_CHECK(r.src_port							== r2.src_port);
	BOOST_CHECK(r.src_addr_subunit					== r2.src_addr_subunit);
	BOOST_CHECK(r.dest_port							== r2.dest_port);
	BOOST_CHECK(r.dest_addr_subunit					== r2.dest_addr_subunit);
	BOOST_CHECK(r.sar_msg_ref_num					== r2.sar_msg_ref_num);
	BOOST_CHECK(r.sar_total_segments				== r2.sar_total_segments);
	BOOST_CHECK(r.sar_segment_seqnum				== r2.sar_segment_seqnum);
	BOOST_CHECK(r.more_msgs_to_send					== r2.more_msgs_to_send);
	BOOST_CHECK(r.payload_type						== r2.payload_type);
	BOOST_CHECK(r.msg_payload						== r2.msg_payload);
	BOOST_CHECK(r.privacy_ind						== r2.privacy_ind);
	BOOST_CHECK(r.callback_num						== r2.callback_num);
	BOOST_CHECK(r.callback_num_pres_ind				== r2.callback_num_pres_ind);
	BOOST_CHECK(r.callback_num_atag					== r2.callback_num_atag);
	BOOST_CHECK(r.src_subaddr						== r2.src_subaddr);
	BOOST_CHECK(r.dest_subaddr						== r2.dest_subaddr);
	BOOST_CHECK(r.user_resp_code					== r2.user_resp_code);
	BOOST_CHECK(r.display_time						== r2.display_time);
	BOOST_CHECK(r.sms_signal						== r2.sms_signal);
	BOOST_CHECK(r.ms_validity						== r2.ms_validity);
	BOOST_CHECK(r.ms_msg_wait_fclts					== r2.ms_msg_wait_fclts);
	BOOST_CHECK(r.number_of_msgs					== r2.number_of_msgs);
	BOOST_CHECK(r.alert_on_msg_delivery				== r2.alert_on_msg_delivery);
	BOOST_CHECK(r.lang_ind							== r2.lang_ind);
	BOOST_CHECK(r.its_reply_type					== r2.its_reply_type);
	BOOST_CHECK(r.its_session_info					== r2.its_session_info);
	BOOST_CHECK(r.ussd_serv_op						== r2.ussd_serv_op);
}
#endif

