#ifndef smpp_proto_pdu_hpp
#define smpp_proto_pdu_hpp

#include <helper/net/pack.hpp>

namespace smpp { namespace pdu {

	namespace command {
		const std::uint32_t generic_nack		= 0x80000000;
		const std::uint32_t bind_receiver		= 0x00000001;
		const std::uint32_t bind_receiver_r		= 0x80000001;
		const std::uint32_t bind_transmitter	= 0x00000002;
		const std::uint32_t bind_transmitter_r	= 0x80000002;
		const std::uint32_t query_sm			= 0x00000003;
		const std::uint32_t query_sm_r			= 0x80000003;
		const std::uint32_t submit_sm			= 0x00000004;
		const std::uint32_t submit_sm_r			= 0x80000004;
		const std::uint32_t deliver_sm			= 0x00000005;
		const std::uint32_t deliver_sm_r		= 0x80000005;
		const std::uint32_t unbind				= 0x00000006;
		const std::uint32_t unbind_r			= 0x80000006;
		const std::uint32_t replace_sm			= 0x00000007;
		const std::uint32_t replace_sm_r		= 0x80000007;
		const std::uint32_t cancel_sm			= 0x00000008;
		const std::uint32_t cancel_sm_r			= 0x80000008;
		const std::uint32_t bind_transceiver	= 0x00000009;
		const std::uint32_t bind_transceiver_r	= 0x80000009;
		const std::uint32_t outbind				= 0x0000000B;
		const std::uint32_t enquire_link		= 0x00000015;
		const std::uint32_t enquire_link_r		= 0x80000015;
		const std::uint32_t submit_multi_sm		= 0x00000021;
		const std::uint32_t submit_multi_sm_r	= 0x80000021;
		const std::uint32_t alert_notification	= 0x00000102;
		const std::uint32_t data_sm				= 0x00000103;
		const std::uint32_t data_sm_r			= 0x80000103;
	}

	declare_packet(
		header
		, declare_field(std::uint32_t, commandLength)
		, declare_field(std::uint32_t, commandId)
		, declare_field(std::uint32_t, commandStatus)
		, declare_field(std::uint32_t, sequenceNumber)
	);

	struct bind_transmitter {
		header header;
		char systemId[16];
		char password[9];
		char systemType[13];
		uint8_t interfaceVersion;
		uint8_t addrTon;
		uint8_t addrNpi;
		char addressRange[41];
	};

	struct bind_receiver {
		header header;
		char systemId[16];
		char password[9];
		char systemType[13];
		uint8_t interfaceVersion;
		uint8_t addrTon;
		uint8_t addrNpi;
		char addressRange[41];
	};
	
	struct bind_transceiver {
		header header;
		char systemId[16];
		char password[9];
		char systemType[13];
		uint8_t interfaceVersion;
		uint8_t addrTon;
		uint8_t addrNpi;
		char addressRange[41];
	};

} }

#endif

