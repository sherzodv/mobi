#ifndef mobi_net_toolbox_hpp
#define mobi_net_toolbox_hpp

#include <bitset>
#include <signal.h>
#include <boost/asio.hpp>

namespace mobi { namespace net { namespace toolbox { namespace bin {

	typedef std::uint8_t	u8_t;
	typedef std::uint16_t	u16_t;
	typedef std::uint32_t	u32_t;
	typedef std::uint64_t	u64_t;

	typedef std::int8_t		s8_t;
	typedef std::int16_t	s16_t;
	typedef std::int32_t	s32_t;
	typedef std::int32_t	s64_t;

	typedef std::size_t		sz_t;

	struct buffer {
		bin::sz_t len;
		bin::u8_t * data;

		buffer(): len(0), data(nullptr) {}
	};

	struct const_buffer {
		bin::sz_t len;
		const bin::u8_t * data;

		const_buffer(): len(0), data(nullptr) {}
		const_buffer(const buffer & mbuf): len(mbuf.len), data(mbuf.data) {}
	};

	/* Byte order handling routines */
	namespace bo {
		inline u16_t to_host(u16_t v) { return ntohs(v); }
		inline u32_t to_host(u32_t v) { return ntohl(v); }
		inline u16_t to_net(u16_t v) { return htons(v); }
		inline u32_t to_net(u32_t v) { return htonl(v); }
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

	/* Copy from buffer to struct, move buffer pointer
	 * Used in binary protocol parsers */

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

		/* Parse fixed size string. It will contain either exactly
		 * len-1 characters and a terminating zero, or only and only
		 * terminating zero, see SMPP 3.4 spec for details */

		inline const u8_t * scpyf(u8_t * dst, const u8_t * src
				, const u8_t * srcend, sz_t len) {
			if (src == srcend)
				return NULL;

			if (*src == 0) {
				*dst++ = *src++;
				return src;
			}

			while (len) {
				*dst++ = *src++;
				if (src >= srcend)
					return NULL;
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

		/* Parse zero terminated string with specified max length.
		 * l will contain number of characters parsed not counting the
		 * terminating zero. */

		/* Parse zero terminated string with specified max length.
		 * l will contain number of characters parsed not counting the
		 * terminating zero. */

		inline const u8_t * scpyl(u8_t * dst, const u8_t * src
				, const u8_t * srcend, sz_t len, sz_t & l) {
			if (src == srcend)
				return nullptr;

			for (l = 0; (l < len) && (src < srcend);) {
				if (*src == 0) {
					*dst++ = *src++; ++l;
					return src;
				}
				*dst++ = *src++; ++l;
			}
			return src;
		}

		inline const u8_t * scpyl(u8_t * dst, const u8_t * src
				, const u8_t * srcend, sz_t len, bin::u8_t & l) {
			if (src == srcend)
				return NULL;

			for (l = 0; (l < len) && (src < srcend);) {
				if (*src == 0) {
					*dst++ = *src++; ++l;
					return src;
				}
				*dst++ = *src++; ++l;
			}
			return src;
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

		
		inline u8_t * scpyf(u8_t * dst, const u8_t * const dend
				, const u8_t * src, sz_t len) {
			if (dst == dend)
				return NULL;

			if (*src == 0) {
				*dst++ = *src++;
				return dst;
			}

			while (len) {
				*dst++ = *src++;
				if (dst >= dend)
					return NULL;
				--len;
			}
			*dst++ = 0;
			return dst;
		}

		inline u8_t * scpy(u8_t * dst, const u8_t * src, sz_t len) {
			while (len--) {
				*dst++ = *src++;
			}
			return dst;
		}

	}

	class hex_str_ref {

		/* Hexadecimal string, used as a temporary object
		 * for displaying octect string in hex form */

		const u8_t * m_buf;
		sz_t m_len;
		const char * m_delimiter;
		const char * m_prefix;

		public:
			hex_str_ref(const u8_t * buf, sz_t len)
				: m_buf(reinterpret_cast<const u8_t *>(buf))
				, m_len(len)
				, m_delimiter(" ")
				, m_prefix("0x")
			{}

			hex_str_ref(const char * buf, sz_t len)
				: m_buf(reinterpret_cast<const u8_t *>(buf))
				, m_len(len)
				, m_delimiter(" ")
				, m_prefix("0x")
			{}

			hex_str_ref(const buffer & buf)
				: m_buf(reinterpret_cast<const u8_t *>(buf.data))
				, m_len(buf.len)
				, m_delimiter(" ")
				, m_prefix("0x")
			{}

			hex_str_ref(const const_buffer & buf)
				: m_buf(reinterpret_cast<const u8_t *>(buf.data))
				, m_len(buf.len)
				, m_delimiter(" ")
				, m_prefix("0x")
			{}

			hex_str_ref & delimit(const char * delimiter) {
				m_delimiter = delimiter;
				return *this;
			}

			hex_str_ref & prefix(const char * p) {
				m_prefix = p;
				return *this;
			}

			template<typename CharT, typename TraitsT> friend
			std::basic_ostream<CharT, TraitsT> &
			operator<<(std::basic_ostream<CharT, TraitsT> & out, const hex_str_ref & str) {
				const std::ostream::fmtflags flags = out.flags();
				const std::streamsize width = out.width();
				const CharT fill = out.fill();
				const u8_t * buf = str.m_buf;
				sz_t len = str.m_len;
				out.setf(std::ios::hex, std::ios::basefield);
				while (len) {
					if (str.m_prefix != nullptr) {
						out << str.m_prefix;
					}
					out.width(2);
					out.fill('0');
					out << static_cast<unsigned>(*buf);
					out.fill(fill);
					out.width(width);
					if (len != 1 && str.m_delimiter != nullptr) {
						out << str.m_delimiter;
					}
					len--;
					buf++;
				}
				out.flags(flags);
				return out;
			}
	};

	class bin_str_ref {

		/* Hexadecimal string, used as a temporary object
		 * for displaying octect string in hex form */

		const u8_t * m_buf;
		sz_t m_len;
		const char * m_delimiter;
		const char * m_prefix;

		public:
			bin_str_ref(const u8_t * buf, sz_t len)
				: m_buf(reinterpret_cast<const u8_t *>(buf))
				, m_len(len)
				, m_delimiter("")
				, m_prefix("0b")
			{}

			bin_str_ref(const char * buf, sz_t len)
				: m_buf(reinterpret_cast<const u8_t *>(buf))
				, m_len(len)
				, m_delimiter("")
				, m_prefix("0b")
			{}

			bin_str_ref & delimit(const char * delimiter) {
				m_delimiter = delimiter;
				return *this;
			}

			bin_str_ref & prefix(const char * p) {
				m_prefix = p;
				return *this;
			}

			template<typename CharT, typename TraitsT> friend
			std::basic_ostream<CharT, TraitsT> &
			operator<<(std::basic_ostream<CharT, TraitsT> & out, const bin_str_ref & str) {
				const u8_t * buf = str.m_buf;
				sz_t len = str.m_len;
				if (str.m_prefix != nullptr) {
					out << str.m_prefix;
				}
				while (len) {
					out << std::bitset<8>(*buf);
					if (len != 1 && str.m_delimiter != nullptr) {
						out << str.m_delimiter;
					}
					len--;
					buf++;
				}
				return out;
			}
	};

	/* Decode BCD string pointed by src to dst */
	void bcd_decode_z(char * dst, const u8_t * src
			, sz_t len, bool odd = false) {
		while (len) {
			*dst++ = (*src & 0x0F) + 0x30;
			if (odd && len == 1)
				break;
			*dst++ = ((*src & 0xF0) >> 4) + 0x30;
			src++;
			len--;
		}
		*dst = 0;
	}

} } } }

#endif
