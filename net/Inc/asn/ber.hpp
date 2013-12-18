#ifndef mobi_net_asn_ber_hpp
#define mobi_net_asn_ber_hpp

#include <bitset>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <ostream>
#include <arpa/inet.h>

/* ITU-T Rec Q.773, TCAP */

/*
	4.1.2.1 General Message Structure

	An information element consists of three fields, which always appear
	in the following order. The Tag distinguishes one type from another
	and governs the interpretation of the Contents. The Length specifies
	the length of the Contents. The Contents is the substance of the element,
	containing the primary information the element is intended to convey.


	Information element

	+-----------+    +-----------+
	| Tag       | +--| Value     |
	+-----------+ |  +-----------+
	| Length    | |
	+-----------+-+  +-----------+
	| Contents  | +--| Tag       |
	+-----------+    +-----------+
                     | Length    |
                     +-----------+
                     | Contents  |
                     +-----------+


	Bit labelling scheme

	+---+---+---+---+---+---+---+---+
	| H | G | F | E | D | C | B | A |
	+---+---+---+---+---+---+---+---+


	4.1.2.2.3 Tag code

	Simple code
	+---+---+---+---+---+---+---+---+
	| C |   F   |        code       |
	+---+---+---+---+---+---+---+---+

	Extended code
	+---+---+---+---+---+---+---+---+
	| C |   F   | 1   1   1   1   1 |
	+---+---+---+---+---+---+---+---+
	| 1 |           MSB             |
	+---+---+---+---+---+---+---+---+
	| 0 |           LSB             |
	+---+---+---+---+---+---+---+---+

	C - class, F - form

	The extension mechanism is to code bits A to E of the first octet
	as 11111. Bit H of the following octet serves as an extension indication.
	If bit H of the extension octet is set to 0, then no further octets for
	this tag are used. If bit H is set to 1, the following octet is also used
	for extension of the Tag code. The resultant Tag consists of bits A to G
	of each extension octet, with bit G of the first extension octet being
	most significant and bit A of the last extension octet being least
	significant. Tag code 31 is encoded as 0011111 in bits G to A of a single
	extension octet. Higher tag codes continue from this point using the
	minimum possible number of extension octets.
*/
/*
	4.1.2.3 Length of the Contents

	The length does not include the Tag nor the Length of the Contents octets.

	If the length is less than 128 octets, the short form is used. In the
	short form, bit H is coded 0, and the length is encoded as a binary number
	using bits A to G.

	The long form Length is from 2 to 127 octets long. Bit H of the first
	octet is coded 1, and bits A to G of the first octet encode a number one
	less than the size of the Length in octets as an unsigned binary number
	whose MSB and LSB are bits G and A, respectively. The length itself is
	encoded as an unsigned binary number whose MSB and LSB are bit H of the
	second octet and bit A of the last octet, respectively. This binary number
	should be encoded in the fewest possible octets, with no leading octets
	having the value 0.

	Short form < 128
	+---+---+---+---+---+---+---+---+
	| 0 |          Length           |
	+---+---+---+---+---+---+---+---+

	Long form
	+---+---+---+---+---+---+---+---+
	| 1 |            N              |
	+---+---+---+---+---+---+---+---+
	| MSB                           |   1
	+---+---+---+---+---+---+---+---+
	|                               |   ...
	+---+---+---+---+---+---+---+---+
	|                           LSB |   N
	+---+---+---+---+---+---+---+---+

	Indefinite form
	+---+---+---+---+---+---+---+---+
	| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
	+---+---+---+---+---+---+---+---+
*/

namespace asn { namespace ber {

	namespace proto {
		typedef std::uint8_t	u8_t;
		typedef std::uint16_t	u16_t;
		typedef std::uint32_t	u32_t;
		typedef std::uint32_t	u64_t;

		typedef std::int8_t		s8_t;
		typedef std::int16_t	s16_t;
		typedef std::int32_t	s32_t;
		typedef std::int32_t	s64_t;
	}

	/* MEMORY MANIPULATION UTILITIES */

	namespace utl {
		using namespace proto;
		typedef std::size_t sz_t;

		/* Byte order handling routines */
		namespace bo {
			inline u16_t tohost(u16_t v) { return ntohs(v); }
			inline u32_t tohost(u32_t v) { return ntohl(v); }
			inline u16_t tonet(u16_t v) { return htons(v); }
			inline u32_t tonet(u32_t v) { return htonl(v); }
		}

		template <typename T>
		inline const u8_t * ascbuf(T & src) {
			return reinterpret_cast<const u8_t *>(&src);
		}

		template <typename T>
		inline const u8_t * ascbuf(T * src) {
			return reinterpret_cast<const u8_t *>(src);
		}

		template <typename T>
		inline u8_t * asbuf(T & src) {
			return reinterpret_cast<u8_t *>(&src);
		}

		template <typename T>
		inline u8_t * asbuf(T * src) {
			return reinterpret_cast<u8_t *>(src);
		}

		/* Parse from buffer to struct , move buffer pointer */
		namespace p {

			inline const u8_t * cp_u8(u8_t * dst, const u8_t * src) {
				*dst = *src++;
				return src;
			}

			inline const u8_t * cp_u16(u8_t * dst, const u8_t * src) {
				/* Copy with byte order handling */
				dst += 1;
				*dst-- = *src++;
				*dst-- = *src++;
				return src;
			}

			inline const u8_t * cp_u32(u8_t * dst, const u8_t * src) {
				/* Copy with byte order handling */
				dst += 3;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				return src;
			}

			inline const u8_t * cp_u64(u8_t * dst, const u8_t * src) {
				/* Copy with byte order handling */
				dst += 7;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				*dst-- = *src++;
				return src;
			}

			inline const u8_t * cpy(u8_t * dst, const u8_t * src, sz_t len) {
				while (len) {
					*dst++ = *src++;
					--len;
				}
				return src;
			}

			/* Reverse copy */
			inline const u8_t * ypc(u8_t * dst, const u8_t * src, sz_t len) {
				std::size_t templ = len;
				src += len - 1;
				while (len) {
					*dst++ = *src--;
					--len;
				}
				return src + templ + 1;
			}
		}

		/* Write from struct to buffer, move buffer pointer */
		namespace w {

			inline u8_t * cp_u8(u8_t * dst, const u8_t * src) {
				*dst++ = *src;
				return dst;
			}

			inline u8_t * cp_u16(u8_t * dst, const u8_t * src) {
				src += 1;
				*dst++ = *src--;
				*dst++ = *src;
				return dst;
			}

			inline u8_t * cp_u32(u8_t * dst, const u8_t * src) {
				src += 3;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				return dst;
			}

			inline u8_t * cp_u64(u8_t * dst, const u8_t * src) {
				src += 7;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				*dst++ = *src--;
				return dst;
			}

			inline u8_t * cpy(u8_t * dst, const u8_t * src, sz_t len) {
				while (len) {
					*dst++ = *src++;
					--len;
				}
				return dst;
			}

			/* Reverse copy */
			inline u8_t * ypc(u8_t * dst, const u8_t * src, sz_t len) {
				src += len - 1;
				while (len) {
					*dst++ = *src--;
					--len;
				}
				return dst;
			}
		}
	}

	const proto::u8_t tag_code_ext = 0x1F;

	enum len_type { len_short, len_long, len_infinite };

	/* 2 bits */
	enum tag_class {
		tagclass_universal		= 0x00,
		tagclass_application	= 0x01,
		tagclass_contextspec	= 0x02,
		tagclass_private		= 0x03,
	};

	enum tag_form {
		tagform_primitive	= 0x00,
		tagform_constructor	= 0x01,
	};

	enum tag_code {
		tagcode_boolean		= 0x01,
		tagcode_integer		= 0x02,
		tagcode_bitstring	= 0x03,
		tagcode_octstring	= 0x04,
		tagcode_null		= 0x05,
		tagcode_oid			= 0x06,
		tagcode_obj_descr	= 0x07,
		tagcode_external	= 0x08,
		tagcode_real		= 0x09,
		tagcode_enum		= 0x0A,
		tagcode_embedded	= 0x0B,
		tagcode_utf8string	= 0x0C,
		tagcode_rel_oid		= 0x0D,
		tagcode_time		= 0x0E,
		tagcode_sequence	= 0x10,
		tagcode_set			= 0x11,
	};

	struct raw_tag {
		proto::u8_t code	: 5;
		tag_form	form	: 1;
		tag_class	klass	: 2;
	};

	struct tag {
		tag_class		klass;
		tag_form		form;
		proto::u64_t	code;
		proto::u64_t	len;
	};

	namespace type {
		const tag boolean		= { tagclass_universal, tagform_primitive, tagcode_boolean, 0x00 };
		const tag integer		= { tagclass_universal, tagform_primitive, tagcode_integer, 0x00 };
		const tag bitstring		= { tagclass_universal, tagform_primitive, tagcode_bitstring, 0x00 };
		const tag octstring		= { tagclass_universal, tagform_primitive, tagcode_octstring, 0x00 };
		const tag null			= { tagclass_universal, tagform_primitive, tagcode_null, 0x00 };
		const tag oid			= { tagclass_universal, tagform_primitive, tagcode_oid, 0x00 };
		const tag sequence		= { tagclass_universal, tagform_constructor, tagcode_sequence, 0x00 };
		const tag set			= { tagclass_universal, tagform_constructor, tagcode_set, 0x00 };
	}

	inline bool operator==(const tag & l, const tag & r) {
		/* Use to determine type of element before parsing, that is
		 * why we don't compare length here */
		return l.klass == r.klass && l.form == r.form && l.code == r.code;
	}

	inline bool operator!=(const tag & l, const tag & r) {
		return !(l == r);
	}

	len_type get_len_type(proto::u8_t first) {
		/* 0x80 0b10000000 */
		/* 0x7F 0b01111111 */
		if (first <= 0x7F) {
			return len_short;
		} else if (first == 0x80) {
			return len_infinite;
		} else {
			return len_long;
		}
	}

	proto::u8_t get_tag_code(const proto::u8_t * buf) {
		using namespace utl;
		raw_tag rt;
		buf = p::cp_u8(asbuf(rt), buf);
		return rt.code;
	}

	template <class LogT>
	const proto::u8_t * parse_integer(proto::s8_t & val
			, const proto::u8_t * buf
			, const proto::u8_t * bend, LogT & L) {
		(void)(L);
		using namespace utl;

		if (buf + 1 >= bend) {
			return nullptr;
		}

		return p::cp_u8(asbuf(val), buf);
	}

	template <class LogT>
	const proto::u8_t * parse_integer(proto::u64_t & val
			, proto::u64_t len
			, const proto::u8_t * buf
			, const proto::u8_t * bend, LogT & L) {
		(void)(L);
		using namespace utl;

		/* Assume we parsed tag-len already */
		if (len < 1 || len > sizeof(proto::u64_t)) {
			return nullptr;
		}

		if (buf + len >= bend) {
			return nullptr;
		}

		return p::ypc(asbuf(val), buf, len);
	}

	template <class LogT>
	const proto::u8_t * parse_len(proto::u64_t & len
			, const proto::u8_t * buf
			, const proto::u8_t * bend, LogT & L) {
		(void)(L);
		using namespace utl;

		if (buf >= bend)
			return nullptr;

		switch (get_len_type(*buf)) {
			case len_short: {
				len = static_cast<proto::u64_t>(*buf & 0x7F);
				return buf + 1;
			}
			case len_long: {
				proto::u8_t lenlen;
				if (++buf >= bend)
					return nullptr;
				lenlen = *buf & 0x7F;
				if (buf + lenlen >= bend || lenlen > sizeof(proto::u64_t))
					return nullptr;
				len = 0L;
				while (lenlen) {
					len <<= 8;
					len &= *buf;
					++buf;
					--lenlen;
				}
				return buf;
			}
			case len_infinite: {
				len = 0;
				return buf + 1;
			}
			default: return nullptr;
		}
	}

	template <class LogT>
	const proto::u8_t * parse_tag(tag & t
			, const proto::u8_t * buf
			, const proto::u8_t * bend
			, LogT & L) {
		using namespace utl;

		raw_tag rt;
		std::size_t count;

		/* At least 1 octet needed */
		if (buf + 1 >= bend)
			return nullptr;

		buf = p::cp_u8(asbuf(rt), buf);

		t.klass = rt.klass;
		t.form = rt.form;

		/* Simple tag code */
		if (rt.code < 0x1F) {
			t.code = rt.code;
			return parse_len(t.len, buf, bend, L);
		}

		/* Extended tag code */
		for (count = 1, rt.code = 0; buf < bend; ++count, ++buf) {
			if (count > sizeof(proto::u64_t)) {
				/* Code octet count > sizeof(proto::u64_t) is not supported */
				return nullptr;
			}
			rt.code <<= 8;
			rt.code |= *buf & 0x7F;
			/* MSB is 0 - last octet */
			if (*buf <= 0x7F) {
				++buf;
				return parse_len(t.len, buf, bend, L);
			}
		}

		/* Unexpected end of buffer */
		return nullptr;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, tag_class tclass) {
		L << "[class:";
		switch (tclass) {
			case tagclass_universal		: L << "universal"; break;
			case tagclass_application	: L << "application"; break;
			case tagclass_contextspec	: L << "contextspec"; break;
			case tagclass_private		: L << "private"; break;
			default: L << "wrong:" << static_cast<unsigned>(tclass); break;
		}
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, tag_form tform) {
		L << "[form:";
		switch (tform) {
			case tagform_primitive		: L << "primitive"; break;
			case tagform_constructor	: L << "constructor"; break;
			default: L << "wrong:" << static_cast<unsigned>(tform); break;
		}
		L << "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, tag t) {
		L << "[tag:"
			<< t.klass
			<< t.form
			<< "[code:" << t.code << "]"
			<< "[len:" << t.len << "]"
		<< "]";
		return L;
	}

} }

#endif