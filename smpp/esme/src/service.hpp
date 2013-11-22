#ifndef smpp_esme_service_hpp
#define smpp_esme_service_hpp

#include <boost/asio.hpp>
#include "session.hpp"

namespace smpp { namespace esme {

	namespace ba = boost::asio::service;

	class service: public smanager<session> {
	};

} }

#endif

