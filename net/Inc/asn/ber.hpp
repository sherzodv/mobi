#ifndef mobi_net_asn_ber_hpp
#define mobi_net_asn_ber_hpp

#include <bitset>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <ostream>
#include <arpa/inet.h>

#include <toolbox/bin.hpp>

#define RETURN_NULL_IF(expr) if (expr) return nullptr

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

namespace mobi { namespace net { namespace asn { namespace ber {

	using namespace toolbox;

	const bin::u8_t tag_code_ext = 0x1F;

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

	/* Defines the identification octet, the first octet of an element which
	 * identifies tag class, form and the first 5 bits of a tag code */
	/* TODO: ifdef bit order by platform */
	struct raw_tag {
		bin::u8_t code		: 5;
		tag_form	form	: 1;
		tag_class	klass	: 2;
	};

	struct tag {
		tag_class	klass;
		tag_form	form;
		bin::u64_t	code;
		bin::u64_t	len;
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

	template <class LogT>
	class parser {

		public:
			parser(LogT & l): L(l) {}
			virtual ~parser() {}

			virtual const bin::u8_t * parse(const bin::u8_t * buf
				, const bin::u8_t * bend) {
				return parse_next(buf, bend);
			}

		protected:
			LogT & L;

			enum action {
				stop		/* Stop parsing immediately */
				, resume	/* Resume parsing */
				, skip		/* Skip the value (may be constructor)
							   of an element */
			};

			/* TODO: implement skipping */
			virtual action on_primitive(asn::ber::tag tag, const bin::u8_t * data) = 0;
			virtual action on_constructor_start(asn::ber::tag tag, const bin::u8_t * data) = 0;
			virtual action on_constructor_end() = 0;

			const bin::u8_t * parse_boolean(bool & val
					, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;
				tag t;
				buf = parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(buf + t.len > bend);
				return p::cp_u8(asbuf(val), buf);
			}

			template <typename T>
			const bin::u8_t * parse_integer(T & val
					, bin::sz_t len
					, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;
				/* Such a large integers are not supported */
				RETURN_NULL_IF(len > 8 || buf + len > bend);
				RETURN_NULL_IF(sizeof(val) < len);
				switch (len) {
					case 0: return nullptr; /* Null value parsed as integer */
					case 1: return p::cp_u8(asbuf(val), buf);
					case 2: return p::cp_u16(asbuf(val), buf);
					case 4: return p::cp_u32(asbuf(val), buf);
					case 8: return p::cp_u64(asbuf(val), buf);
					default: /* 3, 5, 6, 7 octects */ {
						val = 0;
						while (len) {
							val <<= 8;
							len &= *buf;
							++buf;
							--len;
						}
						return buf;
					}
				}
			}

			template <typename T>
			const bin::u8_t * parse_integer(T & val
					, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				tag t;
				buf = parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				return parse_integer(val, t.len, buf, bend);
			}

			template <class StringT>
			const bin::u8_t * parse_octstring(StringT & val
					, const tag & t
					, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;
				/* Expected StringT interface:
				 * struct { size_t len, std::uint8_t data[] } */
				val.len = t.len;
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(buf + val.len > bend);
				return p::cpy(asbuf(val.data), buf, val.len);
			}

			template <class StringT>
			const bin::u8_t * parse_octstring(StringT & val
					, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;
				tag t;
				buf = parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				return parse_octstring(val, t, buf, bend);
			}

			const bin::u8_t * parse_octstring(bin::u8_t * dst
					, bin::sz_t & len
					, bin::sz_t maxlen
					, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;
				tag t;
				buf = parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(t.len > maxlen);
				len = t.len;
				return p::cpy(dst, buf, t.len);
			}

		protected:
			tag_class get_tag_class(const bin::u8_t ic) {
				return reinterpret_cast<raw_tag *>(bin::asbuf(ic))->klass;
			}

			tag_class get_tag_form(const bin::u8_t ic) {
				return reinterpret_cast<raw_tag *>(bin::asbuf(ic))->form;
			}

			len_type get_len_type(bin::u8_t first) {
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

			const bin::u8_t * parse_len(bin::u64_t & len
					, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				if (buf >= bend)
					return nullptr;

				switch (get_len_type(*buf)) {
					case len_short: {
						len = static_cast<bin::u64_t>(*buf & 0x7F);
						return buf + 1;
					}
					case len_long: {
						bin::u8_t lenlen;
						if (++buf >= bend)
							return nullptr;
						lenlen = *buf & 0x7F;
						if (buf + lenlen > bend || lenlen > sizeof(bin::u64_t))
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
						/* FIXME: implement */
						len = 0;
						return buf + 1;
					}
					default: return nullptr;
				}
			}

			const bin::u8_t * parse_tag(tag & t, const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				bin::sz_t count;
				const raw_tag *rt = reinterpret_cast<const raw_tag *>(buf);

				/* At least 1 octet needed */
				if (buf + 1 > bend)
					return nullptr;

				t.klass = rt->klass;
				t.form = rt->form;

				buf += 1;

				/* Simple tag code */
				if (rt->code < 0x1F) {
					t.code = rt->code;
					return parse_len(t.len, buf, bend);
				}

				/* Extended tag code */
				for (count = 1, t.code = 0; buf < bend; ++count, ++buf) {
					if (count > sizeof(bin::u64_t)) {
						/* Code octet count > sizeof(bin::u64_t) is not supported */
						return nullptr;
					}
					t.code <<= 7;
					t.code |= *buf & 0x7F;
					/* MSB is 0 - last octet */
					if (*buf <= 0x7F) {
						++buf;
						return parse_len(t.len, buf, bend);
					}
				}

				/* Unexpected end of buffer */
				return nullptr;
			}

			const bin::u8_t * parse_next(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;
				asn::ber::tag t;
				while (buf < bend) {
					buf = parse_tag(t, buf, bend);
					RETURN_NULL_IF(buf == nullptr);
					if (t.form == tagform_primitive) {
						RETURN_NULL_IF(on_primitive(t, buf) == stop);
						buf += t.len;
					} else if (t.form == tagform_constructor) {
						RETURN_NULL_IF(on_constructor_start(t, buf) == stop);
						buf = parse_next(buf, buf + t.len);
						RETURN_NULL_IF(on_constructor_end() == stop);
					} else {
						/* Unknown tag form */
						return nullptr;
					}
				}
				return buf;
			}
	};

	template <class LogT>
	class writer {
		public:
			writer(LogT & l): L(l) {}
			~writer() {}

		protected:
			LogT & L;

			bin::u8_t * write_len(bin::u8_t * buf, bin::u8_t * bend
					, bin::u64_t len) {
				using namespace bin;
				/* 0x80 0b10000000 */
				/* 0x7F 0b01111111 */
				if (len < 0x80) {
					/* Use short length form */
					RETURN_NULL_IF(buf >= bend);
					bin::u8_t l = static_cast<bin::u8_t>(len);
					return w::cp(buf, l);
				} else {
					/* Use long length form */
					bin::u8_t octets = sizeof(bin::u64_t);
					bin::u64_t l = bo::to_net(len);
					bin::u8_t *cur = asbuf(l);
					/* Count the number of significant octets and
					 * find the first most significant octet */
					while (*cur == 0) {
						--octets;
						++cur;
					}
					RETURN_NULL_IF(buf + octets + 1 > bend);
					/* Write number of octets with a MSB set */
					*buf = static_cast<bin::u8_t>(0x80 | octets);
					++buf;
					while (octets) {
						*buf++ = *cur++;
						--octets;
					}
					return buf;
				}
			}

			bin::u8_t * write_tag(bin::u8_t * buf, bin::u8_t * bend
					, tag_class klass, tag_form form
					, bin::u64_t code, bin::u64_t len) {
				using namespace bin;

				raw_tag *rt = reinterpret_cast<raw_tag *>(buf);

				RETURN_NULL_IF(buf + 1 > bend);

				rt->klass = klass;
				rt->form = form;

				if (code < 0x1F) {
					/* Simple tag code */
					rt->code = static_cast<bin::u8_t>(code);
					return write_len(buf + 1, bend, len);
				} else {
					/* Extended tag code */
					bin::u8_t octets, bits;
					bin::u8_t *end;

					rt->code = 0x1F;
					++buf;

					/* Find out how many octets do we need to encode tag
					 * code according to BER extended tag code encoding
					 * rules */

					for (octets = 1, bits = 7
							; bits < 8 * sizeof(code); bits += 7) {
						if (code >> bits != 0) {
							++octets;
						} else {
							break;
						}
					}

					RETURN_NULL_IF(buf + octets > bend);

					end = buf + octets - 1;
					for (bits -= 7; buf < end; bits -= 7, ++buf) {
						*buf = 0x80 | ((code >> bits) & 0x7F);
					}
					*buf = code & 0x7F;
					++buf;

					return write_len(buf, bend, len);
				}

				return buf;
			}

			bin::u8_t * write_tag(bin::u8_t * buf, bin::u8_t * bend
					, const tag & t, bin::u64_t len) {
				return write_tag(buf, bend, t.klass, t.form, t.code
						, len);
			}

			template <typename T>
			bin::u8_t * write_integer(bin::u8_t * buf, bin::u8_t * bend
					, tag_class klass, bin::u64_t code, T val) {
				using namespace bin;
				buf = write_tag(buf, bend, klass
						, tagform_primitive, code, sizeof(T));
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(buf + sizeof(T) > bend);
				return w::cp(buf, val);
			}

			template <class StringT>
			bin::u8_t * write_octstring(bin::u8_t * buf, bin::u8_t * bend
					, tag_class klass, bin::u64_t code, const StringT & str) {
				buf = write_tag(buf, bend, klass
						, tagform_primitive, code, str.len);
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(buf + str.len > bend);
				std::memcpy(buf, str.data, str.len);
				return buf + str.len;
			}
	};

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

} } } }

#endif
