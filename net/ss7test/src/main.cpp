
#include <ss7/sccp.hpp>
#include <ss7/m3ua.hpp>
#include <ss7/tcap.hpp>
#include <ss7/map.hpp>
#include <asn/ber.hpp>

#include <iostream>
#include <cstring>

void test_sccp() {

	using namespace ss7;

	sccp::proto::u8_t buf[255];

	const sccp::proto::u8_t sccp_raw1[] =
		"\x01\x01\x0C\x02\x02\x70\x0F\x03\x02\x57"
		"\x0F\x04\x01\x06\x05\x47\x00\x09\x01\x03"
		"\x0E\x19\x0B\x52\x06\x00\x11\x04\x97\x20"
		"\x78\x76\x83\x06\x0B\x12\x08\x00\x11\x04"
		"\x97\x20\x73\x00\x05\x08\x29\x62\x27\x48"
		"\x04\x16\x00\x00\x00\x6C\x1F\xA1\x1D\x02"
		"\x01\x00\x02\x01\x2D\x30\x15\x80\x07\x91"
		"\x97\x20\x78\x76\x83\xF6\x81\x01\x01\x82"
		"\x07\x91\x97\x20\x73\x00\x05\xF8";

	const sccp::proto::u8_t sccp_raw2[] =
		"\x01\x03\x0e\x19\x0b\x12\xee\x00\x11\x04"
		"\x99\x63\x95\x99\x99\x01\x0b\x12\x08\x00"
		"\x11\x04\x99\x63\x95\x99\x99\x09\x0f\x64"
		"\x0d\x49\x04\x2a\x00\x02\xb0\x6c\x05\xa2"
		"\x03\x02\x01\x01";

	const sccp::proto::u8_t sccp_raw3[] =
		"\x01\x03\x0e\x19\x0b\x12\x06\x00\x11\x04"
		"\x99\x63\x65\x52\x57\x08\x0b\x12\xee\x00"
		"\x11\x04\x99\x63\x95\x99\x99\x01\x29\x62"
		"\x27\x48\x04\x29\x00\x32\xce\x6c\x1f\xa1"
		"\x1d\x02\x01\x01\x02\x01\x2d\x30\x15\x80"
		"\x07\x91\x99\x63\x65\x52\x57\xf8\x81\x01"
		"\x01\x82\x07\x91\x99\x63\x95\x99\x99\xf1";

	const sccp::proto::u8_t sccp_raw4[] =
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

	sccp::parse(msg, sccp::utl::ascbuf(sccp_raw1), std::cout);
	std::cout << msg << std::endl;

	sccp::parse(msg, sccp::utl::ascbuf(sccp_raw2), std::cout);
	std::cout << msg << std::endl;

	sccp::parse(msg, sccp::utl::ascbuf(sccp_raw3), std::cout);
	std::cout << msg << std::endl;

	sccp::parse(msg, sccp::utl::ascbuf(sccp_raw4), std::cout);
	std::cout << msg << std::endl;

	std::memset(buf, 0, sizeof(buf));
	sccp::write(buf, msg, std::cout);

	sccp::dump(std::cout, sccp_raw4, sizeof(sccp_raw1));
	sccp::dump(std::cout, buf, sizeof(sccp_raw1));

}

void test_m3ua() {

	using namespace ss7;

	m3ua::proto::u8_t buf[255];

	const m3ua::proto::u8_t m3ua_raw1[] =
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
	m3ua::dump(std::cout
		, msg.protocol_data.val.data
		, msg.protocol_data.val.data_len);

	std::memset(buf, 0, sizeof(buf));
	m3ua::write(buf, msg, std::cout);

	m3ua::dump(std::cout, m3ua_raw1, sizeof(m3ua_raw1));
	m3ua::dump(std::cout, buf, sizeof(m3ua_raw1));
}

void test_tcap() {

	using namespace ss7;
	using namespace asn;

	const tcap::proto::u8_t *cur, *cend;
	tcap::proto::u8_t buf[255];

	const tcap::proto::u8_t tcap_raw1[] =
		"\x62\x27\x48\x04\x29\x00\x32\xce\x6c\x1f\xa1\x1d\x02\x01\x01\x02"
		"\x01\x2d\x30\x15\x80\x07\x91\x99\x63\x65\x52\x57\xf8\x81\x01\x01"
		"\x82\x07\x91\x99\x63\x95\x99\x99\xf1";

	(void)(buf);
	(void)(tcap_raw1);

	cur = tcap_raw1;
	cend = cur + sizeof(tcap_raw1) - 1;

	class parser: public tcap::parser<std::ostream> {
		public:
			parser(std::ostream & out): tcap::parser<std::ostream>(out) {}
			virtual ~parser() {}

		protected:

			virtual action on_uni(const tcap::element::uni & el) {
				(void)(el);
				return stop;
			}

			virtual action on_begin(const tcap::element::begin & el) {
				using tcap::operator<<;
				L << std::hex << el << std::endl;
				return resume;
			}

			virtual action on_end(const tcap::element::uni & el) {
				(void)(el);
				return stop;
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

			virtual action on_primitive(asn::ber::tag el, const tcap::proto::u8_t * data) {
				(void)(data);
				L << el << std::endl;
				tcap::dump(L, data, el.len);
				return resume;
			}

			virtual action on_constructor(asn::ber::tag el, const tcap::proto::u8_t * data) {
				(void)(data);
				L << el << std::endl;
				return resume;
			}

	} p(std::cout);

	cur = p.parse(cur, cend);
	if (cur == nullptr) {
		std::cout << "parse error" << std::endl;
	}
}

int main() {

	(void)(test_sccp);
	(void)(test_m3ua);
	(void)(test_tcap);

	//test_sccp();
	//test_m3ua();
	test_tcap();

	return 0;
}
