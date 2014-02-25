
#define BOOST_TEST_MODULE mobi_net_ss7
#include <boost/test/unit_test.hpp>

#include <toolbox/bin.hpp>
#include <asn/ber.hpp>

#include <ss7/sccp.hpp>
#include <ss7/m3ua.hpp>
#include <ss7/tcap.hpp>
#include <ss7/map.hpp>
#include <ss7/sms.hpp>

#include <iostream>
#include <cstring>

class buffer_base {
	protected:
		mobi::net::toolbox::bin::sz_t bsize;
		mobi::net::toolbox::bin::u8_t *buf;
		mobi::net::toolbox::bin::u8_t *bend;
		mobi::net::toolbox::bin::u8_t *bcur;
	public:
		buffer_base(): bsize(1024)
				, buf(new mobi::net::toolbox::bin::u8_t[bsize])
				, bend(buf + bsize), bcur(nullptr) {}
		virtual ~buffer_base() { delete buf; }
};

static inline const char * asstr(const mobi::net::toolbox::bin::u8_t * ptr) {
	return reinterpret_cast<const char *>(ptr);
}

BOOST_AUTO_TEST_CASE(test_sccp_bcd_coders) {

	using namespace mobi::net::ss7;
	using namespace mobi::net::toolbox;

	const bin::sz_t max_sz = 20;

	struct string_t {
		bin::sz_t len;
		bin::u8_t data[max_sz];

		string_t(): len(max_sz), data{0} {}
	};

	const char sccp_s1[] = "1234567890ABCDEF";
	const char sccp_s2[] = "12345678901ABCDEF";

	string_t sccp_num1;
	string_t sccp_num2;

	string_t en;
	string_t de;

	sccp_num1.len = sizeof(sccp_s1) - 1;
	memcpy(sccp_num1.data, sccp_s1, sccp_num1.len);

	sccp_num2.len = sizeof(sccp_s2) - 1;
	memcpy(sccp_num2.data, sccp_s2, sccp_num2.len);

	sccp::bcd_encode(en, sccp_num1);
	BOOST_CHECK(memcmp("\x21\x43\x65\x87\x09\xBA\xDC\xFE"
				, en.data, en.len) == 0);

	sccp::bcd_decode(de, en);
	BOOST_CHECK(std::memcmp(sccp_s1, de.data, de.len) == 0);

	sccp::bcd_encode(en, sccp_num2);
	BOOST_CHECK(std::memcmp("\x21\x43\x65\x87\x09\xA1\xCB\xED\x0F"
				, en.data, en.len) == 0);

	sccp::bcd_decode(de, en, true);
	BOOST_CHECK(std::memcmp(sccp_s2, de.data, de.len) == 0);
}

BOOST_AUTO_TEST_CASE(test_map_tbcd_coders) {

	using namespace mobi::net::ss7;
	using namespace mobi::net::toolbox;

	const bin::sz_t max_sz = 20;

	struct string_t {
		bin::sz_t len;
		bin::u8_t data[max_sz];

		string_t(): len(max_sz), data{0} {}
	};

	const char map_s1[] = "1234567890*#abcd";
	const char map_s2[] = "12345678901*#abcd";

	string_t map_num1;
	string_t map_num2;

	string_t en;
	string_t de;

	map_num1.len = sizeof(map_s1) - 1;
	memcpy(map_num1.data, map_s1, map_num1.len);

	map_num2.len = sizeof(map_s2) - 1;
	memcpy(map_num2.data, map_s2, map_num2.len);

	map::bcd_encode(en, map_num1);
	BOOST_CHECK(std::memcmp("\x21\x43\x65\x87\x09\xBA\xDC\xFE"
				, en.data, en.len) == 0);

	map::bcd_decode(de, en);
	BOOST_CHECK(std::memcmp(map_s1, de.data, de.len) == 0);

	map::bcd_encode(en, map_num2);
	BOOST_CHECK(std::memcmp("\x21\x43\x65\x87\x09\xA1\xCB\xED\xFF"
				, en.data, en.len) == 0);

	map::bcd_decode(de, en, true);
	BOOST_CHECK(std::memcmp(map_s2, de.data, de.len) == 0);
}

BOOST_AUTO_TEST_CASE(test_m3ua) {
	using namespace mobi::net::ss7;
	using namespace mobi::net::toolbox;

	bin::u8_t buf[255];

	const bin::u8_t m3ua_raw1[] =
		"\x01\x00\x01\x01\x00\x00\x00\x68\x00\x06\x00\x08\x00\x00\x00\x01"
		"\x02\x10\x00\x57\x00\x00\x0f\x57\x00\x00\x0f\x31\x03\x03\x00\x0b"
		"\x09\x01\x03\x0e\x19\x0b\x12\x06\x00\x11\x04\x99\x63\x93\x68\x11"
		"\x06\x0b\x12\xee\x00\x11\x04\x99\x63\x95\x99\x99\x01\x29\x62\x27"
		"\x48\x04\x2b\x00\xda\x99\x6c\x1f\xa1\x1d\x02\x01\x01\x02\x01\x2d"
		"\x30\x15\x80\x07\x91\x99\x63\x93\x68\x11\xf6\x81\x01\x01\x82\x07"
		"\x91\x99\x63\x95\x99\x99\xf1\x00";

	(void)(buf);
	(void)(m3ua_raw1);

	m3ua::message::data msg;

	m3ua::parse(msg, m3ua_raw1, m3ua_raw1 + sizeof(m3ua_raw1)-1, std::cout);

	using m3ua::operator<<;

	std::cout << msg << std::endl;
	std::cout << bin::hex_str_ref(msg.protocol_data.val.data
			, msg.protocol_data.val.data_len) << std::endl;

	std::memset(buf, 0, sizeof(buf));
	m3ua::write(buf, msg, std::cout);

	std::cout << bin::hex_str_ref(m3ua_raw1, sizeof(m3ua_raw1)) << std::endl;
	std::cout << bin::hex_str_ref(buf, sizeof(m3ua_raw1)) << std::endl;
}

BOOST_AUTO_TEST_CASE(test_sccp) {
	using namespace mobi::net::ss7;
	using namespace mobi::net::toolbox;

	bin::u8_t buf[255];

	const bin::u8_t sccp_raw1[] =
		"\x01\x01\x0C\x02\x02\x70\x0F\x03\x02\x57"
		"\x0F\x04\x01\x06\x05\x47\x00\x09\x01\x03"
		"\x0E\x19\x0B\x52\x06\x00\x11\x04\x97\x20"
		"\x78\x76\x83\x06\x0B\x12\x08\x00\x11\x04"
		"\x97\x20\x73\x00\x05\x08\x29\x62\x27\x48"
		"\x04\x16\x00\x00\x00\x6C\x1F\xA1\x1D\x02"
		"\x01\x00\x02\x01\x2D\x30\x15\x80\x07\x91"
		"\x97\x20\x78\x76\x83\xF6\x81\x01\x01\x82"
		"\x07\x91\x97\x20\x73\x00\x05\xF8";

	const bin::u8_t sccp_raw2[] =
		"\x01\x03\x0e\x19\x0b\x12\xee\x00\x11\x04"
		"\x99\x63\x95\x99\x99\x01\x0b\x12\x08\x00"
		"\x11\x04\x99\x63\x95\x99\x99\x09\x0f\x64"
		"\x0d\x49\x04\x2a\x00\x02\xb0\x6c\x05\xa2"
		"\x03\x02\x01\x01";

	const bin::u8_t sccp_raw3[] =
		"\x01\x03\x0e\x19\x0b\x12\x06\x00\x11\x04"
		"\x99\x63\x65\x52\x57\x08\x0b\x12\xee\x00"
		"\x11\x04\x99\x63\x95\x99\x99\x01\x29\x62"
		"\x27\x48\x04\x29\x00\x32\xce\x6c\x1f\xa1"
		"\x1d\x02\x01\x01\x02\x01\x2d\x30\x15\x80"
		"\x07\x91\x99\x63\x65\x52\x57\xf8\x81\x01"
		"\x01\x82\x07\x91\x99\x63\x95\x99\x99\xf1";

	const bin::u8_t sccp_raw4[] =
		"\x01\x03\x0e\x19\x0b\x12\x08\x00\x11\x04"
		"\x99\x63\x05\x00\x00\x02\x0b\x12\xee\x00"
		"\x11\x04\x99\x63\x95\x99\x99\x01\x58\x62"
		"\x56\x48\x04\x2e\x00\x1f\x16\x6c\x4e\xa1"
		"\x4c\x02\x01\x01\x02\x01\x2e\x30\x44\x80"
		"\x08\x34\x08\x22\x00\x40\x85\x54\xf0\x84"
		"\x07\x91\x99\x63\x95\x99\x99\xf1\x04\x2f"
		"\x04\x04\x81\x80\x08\x00\x00\x31\x40\x03"
		"\x21\x14\x85\x02\x24\xab\x5c\x6e\x66\xa3"
		"\xcd\x68\x38\xd8\xac\x06\x12\x97\xd9\xe7"
		"\x34\x3b\x0d\x6a\xd7\xe7\xe4\xb2\x3c\x0d"
		"\xaa\xb3\xcf\xe1\x36\x39\x0c";

	(void)(buf);
	(void)(sccp_raw1);
	(void)(sccp_raw2);
	(void)(sccp_raw3);
	(void)(sccp_raw4);

	sccp::unitdata msg;

	sccp::parse(msg, bin::ascbuf(sccp_raw1), std::cout);
	std::cout << msg << std::endl;

	sccp::parse(msg, bin::ascbuf(sccp_raw2), std::cout);
	std::cout << msg << std::endl;

	sccp::parse(msg, bin::ascbuf(sccp_raw3), std::cout);
	std::cout << msg << std::endl;

	sccp::parse(msg, bin::ascbuf(sccp_raw4), std::cout);
	std::cout << msg << std::endl;

	std::memset(buf, 0, sizeof(buf));
	sccp::write(buf, msg, std::cout);

	std::cout << bin::hex_str_ref(sccp_raw4, sizeof(sccp_raw4)) << std::endl;
	std::cout << bin::hex_str_ref(buf, sizeof(sccp_raw4)) << std::endl;
}

BOOST_AUTO_TEST_CASE(test_tcap) {

	using namespace mobi::net::asn;
	using namespace mobi::net::ss7;
	using namespace mobi::net::toolbox;

	const bin::u8_t * cur;

	/* SendRoutingInfoForSM-Arg */
	const bin::u8_t tcap_raw1[] =
		"\x62\x27\x48\x04\x29\x00\x32\xce\x6c\x1f\xa1\x1d\x02\x01\x01\x02"
		"\x01\x2d\x30\x15\x80\x07\x91\x99\x63\x65\x52\x57\xf8\x81\x01\x01"
		"\x82\x07\x91\x99\x63\x95\x99\x99\xf1";

	const bin::u8_t tcap_raw2[] =
		"\x64\x29\x49\x04\x2b\x00\xec\x98\x6c\x21\xa2\x1f\x02\x01\x01\x30"
		"\x1a\x02\x01\x2d\x30\x15\x04\x08\x34\x08\x32\x00\x60\x95\x10\xf9"
		"\xa0\x09\x81\x07\x91\x99\x63\x95\x99\x99\xf9";

	(void)(tcap_raw1);
	(void)(tcap_raw2);

	using tcap::to_string;

	class parser: public tcap::parser<std::ostream> {

		typedef tcap::parser<std::ostream> base;

		public:
			parser(std::ostream & out): base(out), depth(0) {}
			virtual ~parser() {}

		protected:

			using base::L;
			using base::stop;
			using base::resume;
			using base::skip;

			std::size_t depth;

			void indent() {
				for (std::size_t i = 0; i < depth; ++i) L << "\t";
			}

			virtual action on_uni(const tcap::element::uni & r) {
				(void)(r);
				L << std::hex << to_string(r) << std::endl;
				return stop;
			}

			virtual action on_begin(const tcap::element::begin & r) {
				BOOST_CHECK(std::memcmp(&r.otid
							, "\x29\x00\x32\xCE", sizeof(r.otid)) == 0);
				return resume;
			}

			virtual action on_end(const tcap::element::end & r) {
				BOOST_CHECK(std::memcmp(&r.dtid
							, "\x2B\x00\xEC\x98", sizeof(r.dtid)) == 0);
				return resume;
			}

			virtual action on_resume(const tcap::element::resume & el) {
				(void)(el);
				return stop;
			}

			virtual action on_abort(const tcap::element::abort & el) {
				(void)(el);
				return stop;
			}

			virtual action on_invoke(const tcap::element::invoke & r) {
				BOOST_CHECK(r.id == 1);
				BOOST_CHECK(r.has_linked_id == false);
				BOOST_CHECK(r.op_code.type
						== tcap::element::operation_code::local);
				BOOST_CHECK(r.op_code.as.local
						== map::operation::send_routing_info_for_sm);
				return resume;
			}

			virtual action on_return_result_last(const tcap::element::return_result & r) {
				BOOST_CHECK(r.invokeId == 1);
				BOOST_CHECK(r.op_code.type
						== tcap::element::operation_code::local);
				BOOST_CHECK(r.op_code.as.local
						== map::operation::send_routing_info_for_sm);
				return resume;
			}

			virtual action on_primitive(ber::tag r, const bin::u8_t * data) {
				(void)(r);
				(void)(data);
				/*
				indent();
				L << std::hex << el << std::endl;
				*/
				return resume;
			}

			virtual action on_constructor_start(ber::tag r, const bin::u8_t * data) {
				(void)(r);
				(void)(data);
				/*
				indent();
				L << std::hex << el << std::endl;
				*/
				depth++;
				return resume;
			}

			virtual action on_constructor_end() {
				depth--;
				return resume;
			}

	} p(std::cout);

	cur = p.parse(tcap_raw1, tcap_raw1 + sizeof(tcap_raw1) - 1);
	BOOST_CHECK(cur != nullptr);
	BOOST_CHECK(cur == tcap_raw1 + sizeof(tcap_raw1) - 1);

	cur = p.parse(tcap_raw2, tcap_raw2 + sizeof(tcap_raw2) - 1);
	BOOST_CHECK(cur != nullptr);
	BOOST_CHECK(cur == tcap_raw2 + sizeof(tcap_raw2) - 1);
}

BOOST_AUTO_TEST_CASE(test_map) {

	using namespace mobi::net::asn;
	using namespace mobi::net::ss7;
	using namespace mobi::net::toolbox;

	const bin::u8_t * cur;

	/* Invoke RoutingInfoForSM-Arg */
	const bin::u8_t tcap_raw1[] =
		"\x62\x27\x48\x04\x29\x00\x32\xce\x6c\x1f\xa1\x1d\x02\x01\x01\x02"
		"\x01\x2d\x30\x15\x80\x07\x91\x99\x63\x65\x52\x57\xf8\x81\x01\x01"
		"\x82\x07\x91\x99\x63\x95\x99\x99\xf1";

	/* ReturnResultLast RoutingInfoForSM-Res */
	const bin::u8_t tcap_raw2[] =
		"\x64\x29\x49\x04\x2b\x00\xec\x98\x6c\x21\xa2\x1f\x02\x01\x01\x30"
		"\x1a\x02\x01\x2d\x30\x15\x04\x08\x34\x08\x32\x00\x60\x95\x10\xf9"
		"\xa0\x09\x81\x07\x91\x99\x63\x95\x99\x99\xf9";

	/* Invoke MO-ForwardSM */
	const bin::u8_t tcap_raw3[] =
		"\x62\x56\x48\x04\x2e\x00\x1f\x16\x6c\x4e\xa1\x4c\x02\x01\x01\x02"
		"\x01\x2e\x30\x44\x80\x08\x34\x08\x22\x00\x40\x85\x54\xf0\x84\x07"
		"\x91\x99\x63\x95\x99\x99\xf1\x04\x2f\x04\x04\x81\x80\x08\x00\x00"
		"\x31\x40\x03\x21\x14\x85\x02\x24\xab\x5c\x6e\x66\xa3\xcd\x68\x38"
		"\xd8\xac\x06\x12\x97\xd9\xe7\x34\x3b\x0d\x6a\xd7\xe7\xe4\xb2\x3c"
		"\x0d\xaa\xb3\xcf\xe1\x36\x39\x0c";

	class mock: public map::parser<std::ostream>
		, public map::writer<std::ostream>, public buffer_base {

		typedef map::writer<std::ostream> wbase;
		typedef map::parser<std::ostream> pbase;

		using pbase::L;
		using pbase::stop;
		using pbase::resume;
		using pbase::skip;

		action on_routing_info_for_sm_arg(const map::routing_info_for_sm_arg_t & msg) {
			BOOST_CHECK(msg.msisdn.na == map::na_international);
			BOOST_CHECK(msg.msisdn.np == map::np_isdn_telephony);
			BOOST_CHECK(msg.msisdn.digits() == "99365625758");
			BOOST_CHECK(msg.sc_address.na == map::na_international);
			BOOST_CHECK(msg.sc_address.np == map::np_isdn_telephony);
			BOOST_CHECK(msg.sc_address.digits() == "99365999991");
			return resume;
		}
		action on_routing_info_for_sm_res(const map::routing_info_for_sm_res_t & msg) {
			BOOST_CHECK(msg.imsi.digits() == "438023000659019");
			BOOST_CHECK(msg.location_info_with_lmsi.lmsi.len == 0);
			BOOST_CHECK(msg.location_info_with_lmsi.network_node_number.na == map::na_international);
			BOOST_CHECK(msg.location_info_with_lmsi.network_node_number.np == map::np_isdn_telephony);
			BOOST_CHECK(msg.location_info_with_lmsi.network_node_number.digits() == "99365999999");
			return resume;
		}
		action on_mo_forward_sm_arg(const map::mo_forward_sm_arg_t & msg) {
			BOOST_CHECK(msg.sm_rp_da.has == map::sm_rp_da_t::has_imsi);
			BOOST_CHECK(msg.sm_rp_da.as.imsi.digits() == "438022000458450");
			BOOST_CHECK(msg.sm_rp_oa.has == map::sm_rp_oa_t::has_sc);
			BOOST_CHECK(msg.sm_rp_oa.as.sc_address_oa.na == map::na_international);
			BOOST_CHECK(msg.sm_rp_oa.as.sc_address_oa.np == map::np_isdn_telephony);
			BOOST_CHECK(msg.sm_rp_oa.as.sc_address_oa.digits() == "99365999991");
			/* TODO: test sm_rp_ui */
			return resume;
		}
		action on_mo_forward_sm_res(const map::mo_forward_sm_res_t & msg) {
			/* TODO: test mo_forward_sm_res_t parser */
			(void)(msg);
			// L << msg << std::endl;
			return resume;
		}

		public:
			mock(std::ostream & out)
				: pbase(out), wbase(out), buffer_base() {}
			~mock() {
			}

			void test_write() {
				test_address_string();
				test_location_info_with_lmsi();
				test_routing_info_for_sm_arg();
				test_routing_info_for_sm_res();
			}

			void test_routing_info_for_sm_arg() {

				const bin::u8_t raw1[] =
					"\x30\x15\x80\x07\x91\x99\x63\x65\x52\x57\xf8\x81\x01\x01"
					"\x82\x07\x91\x99\x63\x95\x99\x99\xf1";

				map::routing_info_for_sm_arg_t r;

				r.msisdn.ext = 1;
				r.msisdn.na = map::na_international;
				r.msisdn.np = map::np_isdn_telephony;
				r.msisdn.set_digits("99365625758");

				r.sc_address.ext = 1;
				r.sc_address.na = map::na_international;
				r.sc_address.np = map::np_isdn_telephony;
				r.sc_address.set_digits("99365999991");

				r.sm_rp_pri = true;

				bcur = wbase::write_routing_info_for_sm_arg(buf, bend, r);
				BOOST_CHECK(bcur != nullptr);
				BOOST_CHECK(static_cast<bin::sz_t>(bcur-buf) == r.size(0));
				BOOST_CHECK(std::memcmp(raw1, buf, sizeof(raw1)-1) == 0);
			}

			void test_routing_info_for_sm_res() {
				const bin::u8_t raw1[] = "\x30\x15\x04\x08\x34\x08\x32\x00\x60"
					"\x95\x10\xf9\xa0\x09\x81\x07\x91\x99\x63\x95\x99\x99\xf9";

				map::routing_info_for_sm_res_t r;

				r.imsi.set_digits("438023000659019");
				r.location_info_with_lmsi.lmsi.len = 0;
				r.location_info_with_lmsi.network_node_number.na = map::na_international;
				r.location_info_with_lmsi.network_node_number.np = map::np_isdn_telephony;
				r.location_info_with_lmsi.network_node_number.set_digits("99365999999");

				bcur = wbase::write_routing_info_for_sm_res(buf, bend, r);
				BOOST_CHECK(bcur != nullptr);
				BOOST_CHECK(static_cast<bin::sz_t>(bcur-buf) == r.size(0));
				BOOST_CHECK(std::memcmp(raw1, buf, sizeof(raw1)-1) == 0);
			}

			void test_location_info_with_lmsi() {
				const bin::u8_t raw1[]
					= "\xa0\x09\x81\x07\x91\x99\x63\x95\x99\x99\xf9";

				map::location_info_with_lmsi_t r;

				r.lmsi.len = 0;
				r.network_node_number.na = map::na_international;
				r.network_node_number.np = map::np_isdn_telephony;
				r.network_node_number.set_digits("99365999999");

				bcur = wbase::write_location_info_with_lmsi(buf, bend, r);
				BOOST_CHECK(bcur != nullptr);
				BOOST_CHECK(static_cast<bin::sz_t>(bcur-buf) == r.size(0));
				BOOST_CHECK(std::memcmp(raw1, buf, sizeof(raw1)-1) == 0);
			}

			void test_address_string() {
				const bin::u8_t addr_raw1[]
					= "\x80\x07\x91\x99\x63\x65\x52\x57\xF8";

				map::isdn_address_string_t r;

				r.ext = 1;
				r.na = map::na_international;
				r.np = map::np_isdn_telephony;
				r.set_digits("99365625758");

				bcur = wbase::write_address_string(buf, bend
						, ber::tagclass_contextspec
						, ber::tagform_primitive, 0, r);
				BOOST_CHECK(bcur != nullptr);
				BOOST_CHECK(bcur - buf == sizeof(addr_raw1) - 1);
				BOOST_CHECK(std::memcmp(addr_raw1
							, buf, sizeof(addr_raw1)-1) == 0);
			}

	} m(std::cout);

	cur = m.parse(tcap_raw1, tcap_raw1 + sizeof(tcap_raw1) - 1);
	BOOST_CHECK(cur != nullptr);
	BOOST_CHECK(cur == (tcap_raw1 + sizeof(tcap_raw1) - 1));

	cur = m.parse(tcap_raw2, tcap_raw2 + sizeof(tcap_raw2) - 1);
	BOOST_CHECK(cur != nullptr);
	BOOST_CHECK(cur == (tcap_raw2 + sizeof(tcap_raw2) - 1));

	cur = m.parse(tcap_raw3, tcap_raw3 + sizeof(tcap_raw3) - 1);
	BOOST_CHECK(cur != nullptr);
	BOOST_CHECK(cur == (tcap_raw3 + sizeof(tcap_raw3) - 1));

	m.test_write();
}

BOOST_AUTO_TEST_CASE(test_sms) {
	using namespace mobi::net;
	using namespace mobi::net::asn;
	using namespace mobi::net::ss7;
	using namespace mobi::net::toolbox;

	const bin::u8_t * cur;

	/* SMS-SUBMIT */
	const bin::u8_t tcap_raw1[] =
		"\x31\x12\x0b\x91\x99\x63\x93\x68\x11\xf6\x00\x08\xff\x08\x04\x22"
		"\x04\x35\x04\x41\x04\x42";

	class sms_parser: public sms::parser<std::ostream> {
		public:
			sms_parser(std::ostream & out): sms::parser<std::ostream>(out) {}
			virtual ~sms_parser() {}

		protected:
			virtual action on_sms_deliver(const sms::deliver_t & msg) {
				L << msg << std::endl;
				return resume;
			}

			virtual action on_sms_submit(const sms::submit_t & msg) {
				L << msg << std::endl;
				return resume;
			}

			virtual action on_sms_command(const sms::command_t & msg) {
				(void)(msg);
				return stop;
			}

			virtual action on_sms_status_report(const sms::status_report_t & msg) {
				(void)(msg);
				return stop;
			}

			virtual action on_sms_deliver_report_neg(const sms::deliver_report_neg_t & msg) {
				(void)(msg);
				return stop;
			}

			virtual action on_sms_deliver_report_pos(const sms::deliver_report_pos_t & msg) {
				(void)(msg);
				return stop;
			}

			virtual action on_sms_submit_report_neg(const sms::submit_report_neg_t & msg) {
				(void)(msg);
				return stop;
			}

			virtual action on_sms_submit_report_pos(const sms::submit_report_pos_t & msg) {
				(void)(msg);
				return stop;
			}
	} p(std::cout);

	cur = p.parse_in(tcap_raw1
		, tcap_raw1 + sizeof(tcap_raw1)-1);
	if (cur == nullptr) {
		std::cout << "parse error" << std::endl;
	}
}

BOOST_AUTO_TEST_CASE(test_ber_writer) {

	using namespace mobi::net;
	using namespace mobi::net::toolbox;

	class mock: public asn::ber::writer<std::ostream>
		, public asn::ber::parser<std::ostream>
		, public buffer_base {

		typedef asn::ber::writer<std::ostream> base;
		typedef asn::ber::parser<std::ostream> parser_base;

		using base::L;

		action on_primitive(asn::ber::tag tag, const bin::u8_t * data) {
			(void)(tag);
			(void)(data);
			return parser_base::resume;
		}

		action on_constructor_start(asn::ber::tag tag, const bin::u8_t * data) {
			(void)(tag);
			(void)(data);
			return parser_base::resume;
		}

		action on_constructor_end() {
			return parser_base::resume;
		}

		public:
			mock(std::ostream & out)
				: base(out), parser_base(out), buffer_base() {}
			~mock() {}

			void debug() {
				using asn::ber::operator<<;

				const bin::sz_t bsize = sizeof(bin::u64_t) + 1;

				bin::u64_t len = 0xFF2347623480;
				bin::u64_t netlen = bin::bo::to_net(len);
				bin::u8_t buf[bsize];
				bin::u8_t *bend = buf + bsize;
				const bin::u8_t *cur = nullptr;

				std::memset(buf, 0, bsize);

				L << bin::hex_str_ref(bin::asbuf(len), sizeof(len)) << std::endl;
				L << bin::hex_str_ref(bin::asbuf(netlen), sizeof(netlen)) << std::endl;

				cur = base::write_len(buf, bend, len);
				if (cur == nullptr) {
					L << "write_len error" << std::endl;
					return;
				}
				L << bin::hex_str_ref(buf, bsize) << std::endl;

				std::memset(buf, 0, bsize);
				cur = base::write_tag(buf, bend, asn::ber::tagclass_private
						, asn::ber::tagform_primitive, 0x20C3, 0x55);
				if (cur == nullptr) {
					L << "write_tag error" << std::endl;
					return;
				}
				L << bin::bin_str_ref(buf, bsize).delimit(" ") << std::endl;

				asn::ber::tag t;
				cur = parser_base::parse_tag(t, buf, bend);
				if (cur == nullptr) {
					L << "parse_tag error" << std::endl;
					return;
				}
				L << t << std::endl;

				L << std::hex << (0x4143 >> 8) << std::endl;
			}

			void test() {
				bcur = base::write_boolean(buf, bend
						, asn::ber::tagclass_contextspec, 1, true);
				BOOST_CHECK(std::memcmp(buf, "\x81\x01\x01", 3) == 0);
			}

	} m(std::cout);

	m.test();
}
