
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include <smpp/proto.hpp>

int sncmp(const smpp::proto::u8_t * s1
			, const smpp::proto::u8_t * s2
			, std::size_t l)
{
	while (l-- && *s1++ == *s2++);
	return *s1-*s2;
}

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

	{
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
		r.schedule_delivery_time_len	= 0x00;
		r.validity_period_len			= 0x00;

		r.service_type_len = sizeof ("SMPP");
		w::cpy(r.service_type, ascbuf("SMPP"), r.service_type_len);

		r.src_addr_len = sizeof ("GOOGLE.RU");
		w::cpy(r.src_addr, ascbuf("GOOGLE.RU"), r.src_addr_len);

		r.dst_addr_len = sizeof ("GOOGLE.COM");
		w::cpy(r.dst_addr, ascbuf("GOOGLE.COM"), r.dst_addr_len);

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
						+r.service_type_len
						+r.src_addr_len
						+r.dst_addr_len
						+sizeof (r.user_msg_reference);
	}

	/* parse & write shall return nullptr if error has occurred */
	BOOST_CHECK
	(
		smpp::write(hand_dat, hand_dat+r.command.len, r, std::cout) != nullptr
	);

	/* getting access to first 4 octets from buffer, there is length of whole
	 * data is written */
	for (int i = 0; i < 4; ++i) oct[i] = *(hand_dat+3-i);

	BOOST_CHECK
	(
		smpp::parse(r2, hand_dat, hand_dat+len, std::cout) != nullptr
	);


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
		r.schedule_delivery_time_len == r2.schedule_delivery_time_len
	);
	BOOST_CHECK
	(
		sncmp(r.schedule_delivery_time
			, r2.schedule_delivery_time, r.schedule_delivery_time_len) == 0
	);
	BOOST_CHECK
	(
		sncmp(r.validity_period
			, r2.validity_period, r.validity_period_len) == 0
	);
}

