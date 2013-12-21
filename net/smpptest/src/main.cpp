
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include <smpp/proto.hpp>

#if 0
/* TESTING SIGNLE OBJECTS */

BOOST_AUTO_TEST_CASE( test_tlv8 )
{
using namespace smpp;
using namespace local;
proto::u8_t * buf = new proto::u8_t [buf_size];

tlv_dest_addr_subunit ob1, ob2;

ob1.tag = DEST_NUM_SUBUNIT;
ob1.len = 0x0001;
ob1.val = 0x0F;

write(buf, ob1, std::cout);
parse(ob2, buf, std::cout);

BOOST_CHECK(ob1.tag == ob2.tag);
BOOST_CHECK(ob1.len == ob2.len);
BOOST_CHECK(ob1.val == ob2.val);
}

BOOST_AUTO_TEST_CASE( test_tlv16 )
{
using namespace smpp;
using namespace local;
proto::u8_t * buf = new proto::u8_t [buf_size];

tlv_dest_telematics_id ob1, ob2;

ob1.tag = DEST_TELEMATICS_ID;
ob1.len = 0x0002;
ob1.val = 0x0100;

write(buf, ob1, std::cout);
parse(ob2, buf, std::cout);

BOOST_CHECK(ob1.tag == ob2.tag);
BOOST_CHECK(ob1.len == ob2.len);
BOOST_CHECK(ob1.val == ob2.val);
}

BOOST_AUTO_TEST_CASE( test_tlv32 )
{
using namespace smpp;
using namespace local;
proto::u8_t * buf = new proto::u8_t [buf_size];

tlv_qos_time_to_live ob1, ob2;

ob1.tag = QOS_TIME_TO_LIVE;
ob1.len = 0x0004;
ob1.val = 0x00000100;

write(buf, ob1, std::cout);
parse(ob2, buf, std::cout);

BOOST_CHECK(ob1.tag == ob2.tag);
BOOST_CHECK(ob1.len == ob2.len);
BOOST_CHECK(ob1.val == ob2.val);
}

BOOST_AUTO_TEST_CASE( test_pdu )
{
using namespace smpp;
using namespace local;
proto::u8_t * buf = new proto::u8_t [buf_size];

pdu ob1, ob2;

ob1.len		= 0x00000100;
ob1.id		= 0x0000000F;
ob1.status	= 0x0000FF00;
ob1.seqno	= 0x0000113D;

write(buf, ob1, std::cout);
parse(ob2, buf, std::cout);

BOOST_CHECK(ob1.len == ob2.len);
BOOST_CHECK(ob1.id == ob2.id);
BOOST_CHECK(ob1.status == ob2.status);
BOOST_CHECK(ob1.seqno == ob2.seqno);
}

BOOST_AUTO_TEST_CASE( test_bind_transmitter)
{
using namespace smpp;
using namespace local;
proto::u8_t * buf = new proto::u8_t [buf_size];

bind_transmitter ob1, ob2;

ob1.command.len					= 0x00000100;
ob1.command.id					= 0x0000000F;
ob1.command.status				= 0x0000FF00;
ob1.command.seqno				= 0x0000113D;
strcpy((char *) ob1.sys_id, 	"hello, world!");
strcpy((char *) ob1.password,	"password123");
strcpy((char *) ob1.sys_type,	"dos");
ob1.interface_version			= 0x01;
ob1.addr_ton					= 0x0F;
ob1.addr_npi					= 0x0F;
strcpy((char *) ob1.addr_range,	"hello, world!");

write_bind(buf, ob1, std::cout);
parse_bind(ob2, buf, std::cout);

BOOST_CHECK(ob1.command.len == ob2.command.len);
BOOST_CHECK(ob1.command.id == ob2.command.id);
BOOST_CHECK(ob1.command.status == ob2.command.status);
BOOST_CHECK(ob1.command.seqno == ob2.command.seqno);

BOOST_CHECK(!strcmp((char *)ob1.sys_id,		(char *)ob2.sys_id));
BOOST_CHECK(!strcmp((char *)ob1.password,	(char *)ob2.password));
BOOST_CHECK(!strcmp((char *)ob1.sys_type,	(char *)ob2.sys_type));
BOOST_CHECK(!strcmp((char *)ob1.addr_range,	(char *)ob2.addr_range));
}

/* TESTING SET OF OBJECTS */

BOOST_AUTO_TEST_CASE( test_tlv_all )
{
using namespace smpp;
using namespace local;
proto::u8_t * buf = new proto::u8_t [buf_size];

tlv_dest_addr_subunit ob8_1, ob8_2;
tlv_dest_telematics_id ob16_1, ob16_2;
tlv_qos_time_to_live ob32_1, ob32_2;

ob8_1.tag = DEST_NUM_SUBUNIT;
ob8_1.len = 0x0001;
ob8_1.val = 0x0F;

ob16_1.tag = DEST_TELEMATICS_ID;
ob16_1.len = 0x0002;
ob16_1.val = 0x0100;

ob32_1.tag = QOS_TIME_TO_LIVE;
ob32_1.len = 0x0004;
ob32_1.val = 0x00000100;

{
	proto::u8_t * curr = buf;
	curr = write(curr, ob8_1, std::cout);
	curr = write(curr, ob16_1, std::cout);
	curr = write(curr, ob32_1, std::cout);
}

{
	const proto::u8_t * curr = buf;
	curr = parse(ob8_2, curr, std::cout);
	curr = parse(ob16_2, curr, std::cout);
	curr = parse(ob32_2, curr, std::cout);
}

BOOST_CHECK(ob8_1.tag == ob8_2.tag);
BOOST_CHECK(ob8_1.len == ob8_2.len);
BOOST_CHECK(ob8_1.val == ob8_2.val);

BOOST_CHECK(ob16_1.tag == ob16_2.tag);
BOOST_CHECK(ob16_1.len == ob16_2.len);
BOOST_CHECK(ob16_1.val == ob16_2.val);

BOOST_CHECK(ob32_1.tag == ob32_2.tag);
BOOST_CHECK(ob32_1.len == ob32_2.len);
BOOST_CHECK(ob32_1.val == ob32_2.val);
}
*/

#endif

BOOST_AUTO_TEST_CASE( BindT_test_1 )
{
	smpp::proto::u8_t raw_dat[] = {
		"\x00\x00\x00\x28\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x01\x61"
		"\x62\x68\x69\x6b\x00\x70\x61\x73\x73\x77\x6f\x72\x64\x00\x53\x4d\x50"
		"\x50\x00\x01\x00\x00\x00"
	};

	smpp::bind_transmitter fri_dat;
	smpp::parse_bind(fri_dat, raw_dat, raw_dat + sizeof (raw_dat) - 1, std::cout);


	//std::cout << fri_dat.command.len << std::endl;
}

