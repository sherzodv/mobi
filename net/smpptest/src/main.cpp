
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

#include <smpp/proto.hpp>

#define BUF_SIZE	0x1000
#define FILE_NAME	"/tmp/.buffer"

#define TLV_ASSERT(a, b)	assert(a.tag == b.tag);	\
							assert(a.len == b.len);	\
							assert(a.val == b.val)	\

namespace boost {

	enum {
		DEST_NUM_SUBUNIT			= 0x0005,
		DEST_NETWORK_TYPE			= 0x0006,
		DEST_BEARER_TYPE			= 0x0007,
		DEST_TELEMATICS_ID			= 0x0008,
		SOURCE_ADDR_SUBUNIT			= 0x000D,
		SOURCE_NETWORK_TYPE			= 0x000E,
		SOURCE_BEARER_TYPE			= 0x000F,
		SOURCE_TELEMATICS_ID		= 0x0010,
		QOS_TIME_TO_LIVE			= 0x0017,
		PAYLOAD_TYPE				= 0x0019,
		ADDITIONAL_STATUS_INFO_TEXT	= 0x001D,
		RECEIPTED_MESSAGE_ID		= 0x001E,
		MS_MSG_WAIT_FACILITIES		= 0x0030,
		PRIVACY_INDICATOR			= 0x0201,
		SOURCE_SUBADDRESS			= 0x0202,
		DEST_SUBADDRESS				= 0x0203,
		USER_MESSAGE_REFERENCE		= 0x0204,
		USER_RESPONSE_CODE			= 0x0205,
		SOURCE_PORT					= 0x020A,
		DESTINATION_PORT			= 0x020B,
		SAR_MSG_REF_NUM				= 0x020C,
		LANGUAGE_INDICATOR			= 0x020D,
		SAR_TOTAL_SEGMENTS			= 0x020E,
		SAR_SEGMENT_SEQNUM			= 0x020F,
		SC_INTERFACE_VERSION		= 0x0210,
		CALLBACK_NUM_PRES_IND		= 0x0302,
		CALLBACK_NUM_ATAG			= 0x0303,
		NUMBER_OF_MESSAGES			= 0x0304,
		CALLBACK_NUM				= 0x0381,
		DPF_RESULT					= 0x0420,
		SET_DPF						= 0x0421,
		MS_AVAILABILITY_STATUS		= 0x0422,
		NETWORK_ERROR_CODE			= 0x0423,
		MESSAGE_PAYLOAD				= 0x0424,
		DELIVERY_FAILURE_REASON		= 0x0425,
		MORE_MESSAGE_TO_SENDA		= 0x0426,
		MESSAGE_STATE				= 0x0427,
		USSD_SERVICE_OP				= 0x0501,
		DISPLAY_TIME				= 0x1201,
		SMS_SIGNAL					= 0x1203,
		MS_VALIDITY					= 0x1204,
		ALERT_ON_MESSAGE_DELIVERY	= 0x130C,
		ITS_REPLY_TYPE				= 0x1380,
		ITS_SESSION_INFO			= 0x1383
	};

	void read_buf_from_file(uint8_t * buf, size_t len) {
		FILE * fin;
		if ((fin = fopen(FILE_NAME, "r")) == NULL) return;
		fread(buf, sizeof(uint8_t), len, fin);
		fclose(fin);
	}
	void write_buf_to_file(uint8_t * buf, size_t len) {
		FILE * fout;
		if ((fout = fopen(FILE_NAME, "w")) == NULL) return;
		fwrite(buf, sizeof(uint8_t), len, fout);
		fclose(fout);
	}

	namespace test {
		using namespace smpp;

		BOOST_AUTO_TEST_CASE( test_tlv_1octet_val )
		{
			proto::u8_t * buf = new proto::u8_t [BUF_SIZE];

			tlv_dest_addr_subunit ob1, ob2;

			ob1.tag = DEST_NUM_SUBUNIT;
			ob1.len = 0x0001;
			ob1.val = 0x0F;

			write(buf, ob1, std::cout);
			write_buf_to_file(buf, BUF_SIZE);
			read_buf_from_file(buf, BUF_SIZE);
			parse(ob2, buf, std::cout);

			assert(ob1.tag == ob2.tag);
			assert(ob1.len == ob2.len);
			assert(ob1.val == ob2.val);
		}

		BOOST_AUTO_TEST_CASE( test_tlv_2octet_val )
		{
			proto::u8_t * buf = new proto::u8_t [BUF_SIZE];

			tlv_dest_telematics_id ob1, ob2;

			ob1.tag = DEST_TELEMATICS_ID;
			ob1.len = 0x0002;
			ob1.val = 0x0100;

			write(buf, ob1, std::cout);

			write_buf_to_file(buf, BUF_SIZE);
			read_buf_from_file(buf, BUF_SIZE);

			parse(ob2, buf, std::cout);

			assert(ob1.tag == ob2.tag);
			assert(ob1.len == ob2.len);
			assert(ob1.val == ob2.val);
		}

		BOOST_AUTO_TEST_CASE( test_tlv_4octet_val )
		{
			proto::u8_t * buf = new proto::u8_t [BUF_SIZE];

			tlv_qos_time_to_live ob1, ob2;

			ob1.tag = QOS_TIME_TO_LIVE;
			ob1.len = 0x0004;
			ob1.val = 0x00000100;

			write(buf, ob1, std::cout);
			write_buf_to_file(buf, BUF_SIZE);
			read_buf_from_file(buf, BUF_SIZE);
			parse(ob2, buf, std::cout);

			assert(ob1.tag == ob2.tag);
			assert(ob1.len == ob2.len);
			assert(ob1.val == ob2.val);
		}

		BOOST_AUTO_TEST_CASE( test_tlv_all_val )
		{
			proto::u8_t * buf = new proto::u8_t [BUF_SIZE];

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

			write_buf_to_file(buf, BUF_SIZE);
			read_buf_from_file(buf, BUF_SIZE);

			{
				const proto::u8_t * curr = buf;
				curr = parse(ob8_2, curr, std::cout);
				curr = parse(ob16_2, curr, std::cout);
				curr = parse(ob32_2, curr, std::cout);
			}

			TLV_ASSERT(ob8_1, ob8_2);
			TLV_ASSERT(ob16_1, ob16_2);
			TLV_ASSERT(ob32_1, ob32_2);
		}
	}
}


