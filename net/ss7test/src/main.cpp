
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

			virtual action on_uni(const tcap::element::uni & el) {
				(void)(el);
				return stop;
			}

			virtual action on_begin(const tcap::element::begin & el) {
				using tcap::operator<<;
				L << std::hex << el << std::endl;
				return resume;
			}

			virtual action on_end(const tcap::element::end & el) {
				using tcap::operator<<;
				L << std::hex << el << std::endl;
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

			virtual action on_invoke(const tcap::element::invoke & el) {
				using tcap::operator<<;
				L << el << std::endl;
				return resume;
			}

			virtual action on_return_result_last(const tcap::element::return_result & el) {
				using tcap::operator<<;
				L << el << std::endl;
				return resume;
			}

			virtual action on_primitive(ber::tag el, const bin::u8_t * data) {
				(void)(data);
				indent();
				L << el << std::endl;
				return resume;
			}

			virtual action on_constructor_start(ber::tag el, const bin::u8_t * data) {
				(void)(data);
				indent();
				L << el << std::endl;
				depth++;
				return resume;
			}

			virtual action on_constructor_end() {
				depth--;
				return resume;
			}

	} p(std::cout);

	std::cout << "----------------------------------------" << std::endl;
	cur = p.parse(tcap_raw1, tcap_raw1 + sizeof(tcap_raw1) - 1);
	if (cur == nullptr) {
		std::cout << "parse error" << std::endl;
	}

	std::cout << "----------------------------------------" << std::endl;
	cur = p.parse(tcap_raw2, tcap_raw2 + sizeof(tcap_raw2) - 1);
	if (cur == nullptr) {
		std::cout << "parse error" << std::endl;
	}
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

	class parser: public map::parser<std::ostream> {

		typedef map::parser<std::ostream> base;

		public:
			parser(std::ostream & out): base(out) {}
			virtual ~parser() {}

		protected:
			using base::L;
			using base::stop;
			using base::resume;
			using base::skip;

			virtual action on_routing_info_for_sm_arg(const map::routing_info_for_sm_arg_t & msg) {
				using map::operator<<;
				(void)(msg);
				//L << msg << std::endl;
				return resume;
			}
			virtual action on_routing_info_for_sm_res(const map::routing_info_for_sm_res_t & msg) {
				using map::operator<<;
				(void)(msg);
				//L << msg << std::endl;
				return resume;
			}
			virtual action on_mo_forward_sm_arg(const map::mo_forward_sm_arg_t & msg) {
				using map::operator<<;
				(void)(msg);
				//L << msg << std::endl;
				return resume;
			}
			virtual action on_mo_forward_sm_res(const map::mo_forward_sm_res_t & msg) {
				using map::operator<<;
				(void)(msg);
				//L << msg << std::endl;
				return resume;
			}
	} p(std::cout);

	cur = p.parse(tcap_raw1, tcap_raw1 + sizeof(tcap_raw1) - 1);
	BOOST_CHECK(cur != nullptr);
	BOOST_CHECK(cur == (tcap_raw1 + sizeof(tcap_raw1) -1));

	cur = p.parse(tcap_raw2, tcap_raw2 + sizeof(tcap_raw2) - 1);
	BOOST_CHECK(cur != nullptr);
	BOOST_CHECK(cur == (tcap_raw2 + sizeof(tcap_raw2) -1));

	cur = p.parse(tcap_raw3, tcap_raw3 + sizeof(tcap_raw3) - 1);
	BOOST_CHECK(cur != nullptr);
	BOOST_CHECK(cur == (tcap_raw3 + sizeof(tcap_raw3) -1));
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
				using map::operator<<;
				L << msg << std::endl;
				return resume;
			}

			virtual action on_sms_submit(const sms::submit_t & msg) {
				using map::operator<<;
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

	class writer: public asn::ber::writer<std::ostream>,
		public asn::ber::parser<std::ostream> {

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
			writer(std::ostream & out): base(out), parser_base(out) {}
			~writer() {}

			void test() {
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

	} w(std::cout);

	w.test();
}

BOOST_AUTO_TEST_CASE(test_map_writer) {

	using namespace mobi::net;
	using namespace mobi::net::ss7;
	using namespace mobi::net::toolbox;

	class writer: public map::writer<std::ostream>,
		public map::parser<std::ostream> {

		typedef map::writer<std::ostream> base;
		typedef map::parser<std::ostream> parser_base;

		using base::L;

		action on_routing_info_for_sm_arg(const map::routing_info_for_sm_arg_t & msg) {
			(void)(msg);
			return parser_base::resume;
		}
		action on_routing_info_for_sm_res(const map::routing_info_for_sm_res_t & msg) {
			(void)(msg);
			return parser_base::resume;
		}
		action on_mo_forward_sm_arg(const map::mo_forward_sm_arg_t & msg) {
			(void)(msg);
			return parser_base::resume;
		}
		action on_mo_forward_sm_res(const map::mo_forward_sm_res_t & msg) {
			(void)(msg);
			return parser_base::resume;
		}

		public:
			writer(std::ostream & out): base(out), parser_base(out) {}
			~writer() {}

			void test() {
				const bin::sz_t bsize = 1024;
				bin::u8_t buf[bsize] = { 0 };
				bin::u8_t *bend = buf + bsize;
				bin::u8_t *bcur = buf;

				L << "sizeof(map::routing_info_for_sm_arg_t) == "
					<< sizeof(map::routing_info_for_sm_arg_t) << std::endl;

				map::routing_info_for_sm_arg_t ri;
				bcur = base::write(buf, bend, ri);
				if (bcur == nullptr) {
					L << "write ri error" << std::endl;
				}

				L << bin::hex_str_ref(buf, bsize) << std::endl;
			}

	} w(std::cout);

	w.test();
}
