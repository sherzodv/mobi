#ifndef mobi_net_ss7_decod_hpp
#define mobi_net_ss7_decod_hpp

/* 3GPP TS 23.038 */

/* Alphabets and language specific information */
#include <toolbox/bin.hpp>

namespace mobi { namespace net {

	using namespace toolbox;

namespace gsm {

	inline bin::u16_t decode_7bit_char(bin::u8_t ch) {
		static bin::u16_t map[128] = {
			0x0040		// COMMERCIAL AT
			, 0x00A3	// POUND SIGN
			, 0x0024	// DOLLAR SIGN
			, 0x00A5	// YEN SIGN
			, 0x00E8	// LATIN SMALL LETTER E WITH GRAVE
			, 0x00E9	// LATIN SMALL LETTER E WITH ACUTE
			, 0x00F9	// LATIN SMALL LETTER U WITH GRAVE
			, 0x00EC	// LATIN SMALL LETTER I WITH GRAVE
			, 0x00F2	// LATIN SMALL LETTER O WITH GRAVE
			, 0x00E7	// LATIN SMALL LETTER C WITH CEDILLA
			, 0x000A	// LINE FEED
			, 0x00D8	// LATIN CAPITAL LETTER O WITH STROKE
			, 0x00F8	// LATIN SMALL LETTER O WITH STROKE
			, 0x000D	// CARRIAGE RETURN
			, 0x00C5	// LATIN CAPITAL LETTER A WITH RING ABOVE
			, 0x00E5	// LATIN SMALL LETTER A WITH RING ABOVE
			, 0x0394	// GREEK CAPITAL LETTER DELTA
			, 0x005F	// LOW LINE
			, 0x03A6	// GREEK CAPITAL LETTER PHI
			, 0x0393	// GREEK CAPITAL LETTER GAMMA
			, 0x039B	// GREEK CAPITAL LETTER LAMDA
			, 0x03A9	// GREEK CAPITAL LETTER OMEGA
			, 0x03A0	// GREEK CAPITAL LETTER PI
			, 0x03A8	// GREEK CAPITAL LETTER PSI
			, 0x03A3	// GREEK CAPITAL LETTER SIGMA
			, 0x0398	// GREEK CAPITAL LETTER THETA
			, 0x039E	// GREEK CAPITAL LETTER XI
			, 0x00A0	// ESCAPE TO EXTENSION TABLE (or displayed as NBSP, see note above)
			, 0x00C6	// LATIN CAPITAL LETTER AE
			, 0x00E6	// LATIN SMALL LETTER AE
			, 0x00DF	// LATIN SMALL LETTER SHARP S (German)
			, 0x00C9	// LATIN CAPITAL LETTER E WITH ACUTE
			, 0x0020	// SPACE
			, 0x0021	// EXCLAMATION MARK
			, 0x0022	// QUOTATION MARK
			, 0x0023	// NUMBER SIGN
			, 0x00A4	// CURRENCY SIGN
			, 0x0025	// PERCENT SIGN
			, 0x0026	// AMPERSAND
			, 0x0027	// APOSTROPHE
			, 0x0028	// LEFT PARENTHESIS
			, 0x0029	// RIGHT PARENTHESIS
			, 0x002A	// ASTERISK
			, 0x002B	// PLUS SIGN
			, 0x002C	// COMMA
			, 0x002D	// HYPHEN-MINUS
			, 0x002E	// FULL STOP
			, 0x002F	// SOLIDUS
			, 0x0030	// DIGIT ZERO
			, 0x0031	// DIGIT ONE
			, 0x0032	// DIGIT TWO
			, 0x0033	// DIGIT THREE
			, 0x0034	// DIGIT FOUR
			, 0x0035	// DIGIT FIVE
			, 0x0036	// DIGIT SIX
			, 0x0037	// DIGIT SEVEN
			, 0x0038	// DIGIT EIGHT
			, 0x0039	// DIGIT NINE
			, 0x003A	// COLON
			, 0x003B	// SEMICOLON
			, 0x003C	// LESS-THAN SIGN
			, 0x003D	// EQUALS SIGN
			, 0x003E	// GREATER-THAN SIGN
			, 0x003F	// QUESTION MARK
			, 0x00A1	// INVERTED EXCLAMATION MARK
			, 0x0041	// LATIN CAPITAL LETTER A
			, 0x0042	// LATIN CAPITAL LETTER B
			, 0x0043	// LATIN CAPITAL LETTER C
			, 0x0044	// LATIN CAPITAL LETTER D
			, 0x0045	// LATIN CAPITAL LETTER E
			, 0x0046	// LATIN CAPITAL LETTER F
			, 0x0047	// LATIN CAPITAL LETTER G
			, 0x0048	// LATIN CAPITAL LETTER H
			, 0x0049	// LATIN CAPITAL LETTER I
			, 0x004A	// LATIN CAPITAL LETTER J
			, 0x004B	// LATIN CAPITAL LETTER K
			, 0x004C	// LATIN CAPITAL LETTER L
			, 0x004D	// LATIN CAPITAL LETTER M
			, 0x004E	// LATIN CAPITAL LETTER N
			, 0x004F	// LATIN CAPITAL LETTER O
			, 0x0050	// LATIN CAPITAL LETTER P
			, 0x0051	// LATIN CAPITAL LETTER Q
			, 0x0052	// LATIN CAPITAL LETTER R
			, 0x0053	// LATIN CAPITAL LETTER S
			, 0x0054	// LATIN CAPITAL LETTER T
			, 0x0055	// LATIN CAPITAL LETTER U
			, 0x0056	// LATIN CAPITAL LETTER V
			, 0x0057	// LATIN CAPITAL LETTER W
			, 0x0058	// LATIN CAPITAL LETTER X
			, 0x0059	// LATIN CAPITAL LETTER Y
			, 0x005A	// LATIN CAPITAL LETTER Z
			, 0x00C4	// LATIN CAPITAL LETTER A WITH DIAERESIS
			, 0x00D6	// LATIN CAPITAL LETTER O WITH DIAERESIS
			, 0x00D1	// LATIN CAPITAL LETTER N WITH TILDE
			, 0x00DC	// LATIN CAPITAL LETTER U WITH DIAERESIS
			, 0x00A7	// SECTION SIGN
			, 0x00BF	// INVERTED QUESTION MARK
			, 0x0061	// LATIN SMALL LETTER A
			, 0x0062	// LATIN SMALL LETTER B
			, 0x0063	// LATIN SMALL LETTER C
			, 0x0064	// LATIN SMALL LETTER D
			, 0x0065	// LATIN SMALL LETTER E
			, 0x0066	// LATIN SMALL LETTER F
			, 0x0067	// LATIN SMALL LETTER G
			, 0x0068	// LATIN SMALL LETTER H
			, 0x0069	// LATIN SMALL LETTER I
			, 0x006A	// LATIN SMALL LETTER J
			, 0x006B	// LATIN SMALL LETTER K
			, 0x006C	// LATIN SMALL LETTER L
			, 0x006D	// LATIN SMALL LETTER M
			, 0x006E	// LATIN SMALL LETTER N
			, 0x006F	// LATIN SMALL LETTER O
			, 0x0070	// LATIN SMALL LETTER P
			, 0x0071	// LATIN SMALL LETTER Q
			, 0x0072	// LATIN SMALL LETTER R
			, 0x0073	// LATIN SMALL LETTER S
			, 0x0074	// LATIN SMALL LETTER T
			, 0x0075	// LATIN SMALL LETTER U
			, 0x0076	// LATIN SMALL LETTER V
			, 0x0077	// LATIN SMALL LETTER W
			, 0x0078	// LATIN SMALL LETTER X
			, 0x0079	// LATIN SMALL LETTER Y
			, 0x007A	// LATIN SMALL LETTER Z
			, 0x00E4	// LATIN SMALL LETTER A WITH DIAERESIS
			, 0x00F6	// LATIN SMALL LETTER O WITH DIAERESIS
			, 0x00F1	// LATIN SMALL LETTER N WITH TILDE
			, 0x00FC	// LATIN SMALL LETTER U WITH DIAERESIS
			, 0x00E0	// LATIN SMALL LETTER A WITH GRAVE
		};
		if (ch > 127) {
			return 0x0000;
		}
		return map[ch];
	}

	inline bin::u16_t decode_7bit_ext_char(bin::u8_t ch) {
		switch (ch) {
			case 0x0A: return 0x000C; // FORM FEED
			case 0x14: return 0x005E; // CIRCUMFLEX ACCENT
			case 0x28: return 0x007B; // LEFT CURLY BRACKET
			case 0x29: return 0x007D; // RIGHT CURLY BRACKET
			case 0x2F: return 0x005C; // REVERSE SOLIDUS
			case 0x3C: return 0x005B; // LEFT SQUARE BRACKET
			case 0x3D: return 0x007E; // TILDE
			case 0x3E: return 0x005D; // RIGHT SQUARE BRACKET
			case 0x40: return 0x007C; // VERTICAL LINE
			case 0x65: return 0x20AC; // EURO SIGN
			default: return 0x0000;
		}
	}

	inline bin::u8_t encode_7bit_char(bin::u16_t ch) {
		/* Map ucs2 char to gsm 7 bit char
		 * for extended table return 7 bit code | 0x80
		 * for unknown return 0x80 */
		static bin::u8_t map[256] = {
			0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
			, 0x80, 0x80, 0x0a, 0x80, 0x8a, 0x0d, 0x80, 0x80
			, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
			, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
			, 0x20, 0x21, 0x22, 0x23, 0x02, 0x25, 0x26, 0x27
			, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f
			, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
			, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
			, 0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47
			, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
			, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57
			, 0x58, 0x59, 0x5a, 0xbc, 0xaf, 0xbe, 0x94, 0x11
			, 0x80, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67
			, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f
			, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77
			, 0x78, 0x79, 0x7a, 0xa8, 0xc0, 0xa9, 0xbd, 0x80
			, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
			, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
			, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
			, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
			, 0x1b, 0x40, 0x80, 0x01, 0x24, 0x03, 0x80, 0x5f
			, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
			, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
			, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x60
			, 0x80, 0x80, 0x80, 0x80, 0x5b, 0x0e, 0x1c, 0x80
			, 0x80, 0x1f, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
			, 0x80, 0x5d, 0x80, 0x80, 0x80, 0x80, 0x5c, 0x80
			, 0x0b, 0x80, 0x80, 0x80, 0x5e, 0x80, 0x80, 0x1e
			, 0x7f, 0x80, 0x80, 0x80, 0x7b, 0x0f, 0x1d, 0x09
			, 0x04, 0x05, 0x80, 0x80, 0x07, 0x80, 0x80, 0x80
			, 0x80, 0x7d, 0x08, 0x80, 0x80, 0x80, 0x7c, 0x80
			, 0x0c, 0x06, 0x80, 0x80, 0x7e, 0x80, 0x80, 0x80
		};

		if (ch <= 0xFF) {
			return map[ch];
		}

		switch (ch) {
			case 0x393: return 0x13;
			case 0x394: return 0x10;
			case 0x398: return 0x19;
			case 0x39b: return 0x14;
			case 0x39e: return 0x1a;
			case 0x3a0: return 0x16;
			case 0x3a3: return 0x18;
			case 0x3a6: return 0x12;
			case 0x3a8: return 0x17;
			case 0x3a9: return 0x15;
			case 0x20ac: return 0xe5;
			default: return 0x80;
		}
	}

	/* Convert 7 bit text to 8 bit: fill in leading zeroes */
	template <template <bin::sz_t> class StringT, bin::sz_t MaxLen>
	const bin::u8_t * sept_to_oct(StringT<MaxLen> & r
			, const bin::u8_t * buf, const bin::u8_t * bend, int offs) {

		static_assert(sizeof(*r.data) == sizeof(bin::u8_t)
				, "octet sized string expected");

		bin::u8_t * cur = r.data;
		bin::u8_t * cend = cur + MaxLen;

		enum { head, tail } part =  offs > 0 ? head : tail;

		if (offs == 0) {
			offs = 8;
		}

		while (cur < cend && buf < bend) {
			switch (part) {
				case tail:
					*cur |= (*buf << (8 - offs)) & 0x7F;
					part = head;
					++cur;
					--offs;
					continue;
				case head:
					*cur = *buf >> offs;
					part = tail;
					++buf;
					if (offs == 1) {
						offs = 8;
						++cur;
					}
					continue;
			}
		}

		r.len = cur - r.data;

		return buf;
	}

	/* Convert 8 bit text to 7 bit: Remove leading zeroes */
	template <template <bin::sz_t> class StringT, bin::sz_t MaxLen>
	bin::u8_t * oct_to_sept(bin::u8_t * buf
			, bin::u8_t * bend, int offs
			, const StringT<MaxLen> & r) {

		static_assert(sizeof(*r.data) == sizeof(bin::u8_t)
				, "octet sized string expected");

		const bin::u8_t * cur = r.data;
		const bin::u8_t * cend = cur + r.len;

		enum { head, tail } part =  offs > 0 ? head : tail;

		if (offs == 0) {
			offs = 8;
		}

		while (cur < cend && buf < bend) {
			switch (part) {
				case tail:
					*buf = *cur >> (8 - offs);
					part = head;
					++cur;
					--offs;
					continue;
				case head:
					*buf |= *cur << offs;
					part = tail;
					++buf;
					if (offs == 1) {
						offs = 8;
						++cur;
					}
					continue;
			}
		}

		if (r.len) {
			++buf;
		}

		return buf;
	}

	template <template <bin::sz_t> class UTF16StringT
			 	, template <bin::sz_t> class GSM7BitStringT
				, bin::sz_t UTF16MaxLen, bin::sz_t GSM7BitMaxLen>
	bin::sz_t sbit_to_utf16(UTF16StringT<UTF16MaxLen> & dst
			, const GSM7BitStringT<GSM7BitMaxLen> & src) {

		bool ext = false;
		const bin::u8_t * cur = src.data;
		const bin::u8_t * cend = cur + src.len;

		dst.len = 0;

		while (cur < cend) {
			if (ext) {
				ext = false;
				dst.data[dst.len] = decode_7bit_ext_char(*cur);
				++dst.len;
			} else {
				dst.data[dst.len] = decode_7bit_char(*cur);
				if (dst.data[dst.len] == 0xA0) {
					ext = true;
				} else {
					++dst.len;
				}
			}
			++cur;
		}

		return cur - src.data;
	}

} // namespace gsm

} }

#endif
