#ifndef eastwind_ewgate_proto_hpp
#define eastwind_ewgate_proto_hpp

#define RETURN_NULL_IF(a) if ((a)) { return nullptr; }

#include <bitset>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <ostream>
#include <arpa/inet.h>
#include <toolbox/bin.hpp>

namespace eastwind { namespace ewgate {

	using namespace mobi::net::toolbox;

	/* EWGate message delimiter */
	const bin::u32_t message_delimiter	= 0x1abcdef2;

	/* EWGate message types */
	namespace message { enum type: bin::u8_t {
		null				= 0x00
		, heartbeat			= 0xF0
		, client_heartbeat	= 0xFE
		, server_ready		= 0xFF
	}; }

	/* EWGate header */
	struct pdu {
		bin::u32_t		delimiter;
		bin::u32_t		crc;
		bin::u32_t		r1;
		bin::u32_t		counter;
		message::type	type;
		bin::u8_t		r2;
		bin::u16_t		len;

		pdu(): delimiter(message_delimiter)
			, crc(0)
			, r1(0)
			, counter(0)
			, type(message::null)
			, r2(0)
			, len(0)
		{}

		pdu(message::type ct): crc(0)
			, r1(0)
			, counter(0)
			, type(ct)
			, r2(0)
			, len(0)
		{}

		pdu(const pdu& other): delimiter(other.delimiter)
			, crc(other.crc)
			, r1(other.r1)
			, counter(other.counter)
			, type(other.type)
			, r2(other.r2)
			, len(other.len)
		{}

	};

	namespace {

		struct heartbeat {
			enum kind: bin::u8_t { req = 0x00, res = 0x01, null = 0xFF };

			pdu				hdr;
			kind			type;
			bin::u8_t		resend;
			bin::u32_t		counter;

			heartbeat(const pdu & h)
				: hdr(h)
				, type(null)
				, resend(0)
				, counter(0)
			{}
		};

	}

	template <class LogT>
	class parser {

		public:
			parser(LogT & l): L(l) {}
			virtual ~parser() {}

			enum action {
				stop		/* Stop parsing immediately */
				, resume	/* Resume parsing */
				, skip		/* Skip the value (may be constructor)
							   of an element */
			};

			const bin::u8_t * parse(const bin::u8_t * buf
				, const bin::u8_t * bend) {
				buf = parse_header(buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				return buf;
			}

		protected:

			virtual action on_heartbeat(const heartbeat & msg) = 0;

			pdu m_pdu;
			LogT & L;

		private:
			const bin::u8_t * parse_header(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				const bin::u8_t * cur = buf;

				while (cur < bend) {
					/* Parse PDU len and id
					 * In general buffer may contain several commands */
					RETURN_NULL_IF(cur + sizeof(pdu) > bend);
					cur = parse_pdu(m_pdu, cur);
					RETURN_NULL_IF(cur + m_pdu.len > bend);
					switch (m_pdu.type) {
						case message::type::heartbeat:
							cur = parse_heartbeat(cur, cur + m_pdu.len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						default:
							return nullptr;
					}
				}

				return cur;
			}

			const bin::u8_t * parse_pdu(pdu & msg, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u32(asbuf(msg.delimiter), buf);
				buf = p::cp_u32(asbuf(msg.crc), buf);
				buf = p::cp_u32(asbuf(msg.r1), buf);
				buf = p::cp_u32(asbuf(msg.counter), buf);
				buf = p::cp_u8(asbuf(msg.type), buf);
				buf = p::cp_u8(asbuf(msg.r2), buf);
				buf = p::cp_u16(asbuf(msg.len), buf);
				return buf;
			}

			const bin::u8_t * parse_heartbeat(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;
				heartbeat msg(m_pdu);
				RETURN_NULL_IF(buf + 6 > bend);
				buf = p::cp_u8(asbuf(msg.type), buf);
				buf = p::cp_u8(asbuf(msg.resend), buf);
				buf = p::cp_u32(asbuf(msg.counter), buf);
				if (on_heartbeat(msg) == resume) {
					return buf;
				} else {
					return nullptr;
				}
			}

	};

	template <class LogT>
	class writer {

		LogT & L;

		public:
			writer(LogT & l): L(l) {}
			virtual ~writer() {}

			bin::u8_t * write(bin::u8_t * buf , bin::u8_t * bend
					, const heartbeat & msg) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.hdr) > bend);
				buf = write_pdu(buf, msg.hdr);

				RETURN_NULL_IF(buf + 6 > bend);
				buf = w::cp_u8(buf, ascbuf(msg.type));
				buf = w::cp_u8(buf, ascbuf(msg.resend));
				buf = w::cp_u32(buf, ascbuf(msg.counter));

				return buf;
			}

			bin::u8_t * write_pdu(bin::u8_t * buf, const pdu & msg) {
				using namespace bin;
				buf = w::cp_u32(buf, ascbuf(msg.delimiter));
				buf = w::cp_u32(buf, ascbuf(msg.crc));
				buf = w::cp_u32(buf, ascbuf(msg.r1));
				buf = w::cp_u32(buf, ascbuf(msg.counter));
				buf = w::cp_u8(buf, ascbuf(msg.type));
				buf = w::cp_u8(buf, ascbuf(msg.r2));
				buf = w::cp_u16(buf, ascbuf(msg.len));
				return buf;
			}
	};

	namespace {

		template< typename CharT, typename TraitsT >
		std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const message::type & r) {
			L << "[type:";
			switch (r) {
				case message::null				: L << "null"; break;
				case message::heartbeat			: L << "heartbeat"; break;
				case message::client_heartbeat	: L << "client_heartbeat"; break;
				case message::server_ready		: L << "server_ready"; break;
				default					: L << "unknown"; break;
			}
			L << "]";
			return L;
		}

		template <typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT> &
		operator<<(std::basic_ostream<CharT, TraitsT> & L
				, const pdu & t) {
			L	<< "[hdr:"
				<< "[ew.len:"	<< t.crc << "]"
				<< "[ew.id:"	<< t.counter << "]"
				<< t.type
				<< "[ew.seqno:"	<< t.len << "]"
				<< "]";
			return L;
		}

		template<typename CharT, typename TraitsT>
		std::basic_ostream<CharT, TraitsT>&
		operator<<(std::basic_ostream<CharT, TraitsT> &L
				, const heartbeat & r) {
			L << "["
				<< r.hdr
				<< "]";
			return L;
		}

	}

} }

#endif

