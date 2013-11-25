#ifndef itcp_proto_hpp
#define itcp_proto_hpp

#include <queue>
#include <vector>
#include <cstdint>
#include <boost/asio.hpp>

namespace fastmq {

typedef std::uint8_t	u8_t;
typedef std::uint16_t	u16_t;
typedef std::uint32_t	u32_t;

typedef const std::uint8_t	cu8_t;
typedef const std::uint16_t	cu16_t;
typedef const std::uint32_t	cu32_t;

typedef std::uint8_t	*	u8p_t;
typedef std::uint16_t	*	u16p_t;
typedef std::uint32_t	*	u32p_t;

typedef const std::uint8_t	*	cu8p_t;
typedef const std::uint16_t	*	cu16p_t;
typedef const std::uint32_t	*	cu32p_t;

typedef void * vp_t;
typedef const void * cvp_t;

struct msgu {		/* Generic message unit */
	u32_t len;		/* Total message length, including len field */
	u16_t type;		/* Proto type */
	u16_t id;		/* Module Id */
	u8_t data[1];	/* Pointer to data */
};

/* All messages with
 * len field <= sizeof(msgu) are service messages.
 * Messages with len > sizeof(msgu) are data messages.
 * Service messages all fixed size, with size equal to sizeof(msgu).
 *
 * len field for service messages should not be treated as a length
 * of a message, it is used to carry some service information instead.
 */

const u32_t min_data_msgu_length = sizeof(msgu);

typedef msgu * msgup;
typedef const msgu * cmsgup;

vp_t as_vp(msgu * msg) {
	return static_cast<vp_t>(msg);
}

cvp_t as_cvp(const msgu * msg) {
	return static_cast<cvp_t>(msg);
}

u8p_t as_u8p(msgu *msg) {
	return static_cast<u8p_t>(as_vp(msg));
}

cu8p_t as_cu8p(const msgu *msg) {
	return static_cast<cu8p_t>(as_cvp(msg));
}

msgup as_msgup(uint8_t * msg) {
	return static_cast<msgup>(static_cast<vp_t>(msg));
}

cmsgup as_cmsgup(uint8_t * msg) {
	return static_cast<cmsgup>(static_cast<cvp_t>(msg));
}

}

#endif
