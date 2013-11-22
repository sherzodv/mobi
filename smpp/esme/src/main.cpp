
#include <iostream>
#include <helper/net/pack.hpp>

namespace smpp { namespace pdu {

	declare_packet(
		header
		, declare_field(std::uint32_t, commandLength)
		, declare_field(std::uint32_t, commandId)
		, declare_field(std::uint32_t, commandStatus)
		, decalre_field(std::uint32_t, sequenceNumber)
	);

} }

int main(int argc, char ** argv)
{
	std::cout << "Hello, world!!!" << std::endl;
	return 0;
}

