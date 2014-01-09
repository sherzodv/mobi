
#ifndef smpp_proto_hpp
#define smpp_proto_hpp

#define RETURN_NULL_IF(a) if ((a)) { return nullptr; }

#include <bitset>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <ostream>
#include <arpa/inet.h>
#include <toolbox/bin.hpp>

namespace mobi { namespace net { namespace fastds {

	using namespace toolbox;

	/* fastds 1.0 command ids */
	namespace command {
		enum id: bin::u32_t {
			null					= 0x00000000,
			authenticate_req		= 0x00000001,
			authenticate_res		= 0x80000001,
		};
		enum status: bin::u32_t {
			ok						= 0x00000000,
		};
		enum proto: bin::u32_t {
			unknown					= 0x00000000,
			smpp					= 0x00000001,
		};
	}

	/* fastds 1.0 tlv ids */
	namespace option {
		enum tag: bin::u16_t {
			null			= 0x0000
		};
	}

	/* fastds PDU header */
	struct pdu {
		bin::u32_t		len;
		command::id		id;
		command::proto	proto;
		command::status	status;
		bin::u32_t 		seqno;
		pdu(): len(0)
			, id(command::null)
			, proto(command::smpp)
			, status(command::ok)
			, seqno(0)
		{}
		pdu(command::id cid): len(0)
			, id(cid)
			, proto()
			, status(command::ok)
			, seqno(0)
		{}
	};

	/* SMPP 3.4 generic tag-len-value type */
	template <typename ValueT>
	struct tlv {
		option::tag	tag;
		bin::u16_t	len;
		ValueT val;
		tlv(): tag(option::tag::null), len(0) {}
	};

	/* fastds 1.0 TLV specializations */
	namespace {
	}

	/* fastds 1.0 messages */
	namespace {

		struct authenticate_req {
			pdu header;

			bin::u8_t username[16];
			bin::u8_t password[16];

			/* this values will not be written to buffer */
			bin::sz_t username_len;
			bin::sz_t password_len;

			authenticate_req(): header(command::authenticate_req)
				, username_len(0)
				, password_len(0)
			{}

			bin::sz_t raw_size() const {
				return	sizeof(header)
						+ username_len
						+ password_len;
			}
		};

		struct authenticate_res {
			pdu header;
			authenticate_res(): header(command::authenticate_res)
			{}
		};

	}

	/* fastds 1.0 tlv P&W */
	namespace {
	}

	/* fastds 1.0 message P&W */
	namespace {
	}

	template <class LogT>
	class tlv_parser {
		protected:
			LogT & L;

		public:
			tlv_parser(LogT & l): L(l) {}
			virtual ~tlv_parser() {}

			const bin::u8_t * parse_tlv_u8(tlv<bin::u8_t> & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cp_u8(asbuf(t.val), buf);
				return buf;
			}

			const bin::u8_t * parse_tlv_u16(tlv<bin::u16_t> & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cp_u16(asbuf(t.val), buf);
				return buf;
			}

			const bin::u8_t * parse_tlv_u32(tlv<bin::u32_t> & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cp_u32(asbuf(t.val), buf);
				return buf;
			}

			const bin::u8_t * parse_tlv_s23(tlv<bin::u8_t[23]> & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cpy(t.val, ascbuf(buf), t.len);
				return buf;
			}

			const bin::u8_t * parse_tlv_s19(tlv_callback_num & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cpy(t.val, ascbuf(buf), t.len);
				return buf;
			}

			const bin::u8_t * parse_tlv_s65(tlv_callback_num_atag & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cpy(t.val, ascbuf(buf), t.len);
				return buf;
			}

			const bin::u8_t * parse_tlv_ptr(tlv<bin::u8_t *> & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf += t.len;
				/* TODO buf = p::cpy(t.val, ascbuf(buf), t.len); */
				return buf;
			}

			const bin::u8_t * parse_tlv_s2(tlv_its_session_info & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cpy(t.val, ascbuf(buf), t.len);
				return buf;
			}

			const bin::u8_t * parse_tlv_s1(tlv_ussd_serv_op & t
					, const bin::u8_t * buf) {
				using namespace bin;
				buf = p::cp_u16(asbuf(t.tag), buf);
				buf = p::cp_u16(asbuf(t.len), buf);
				buf = p::cpy(t.val, ascbuf(buf), t.len);
				return buf;
			}
	};

	template <class LogT>
	class parser: public tlv_parser<LogT> {

		using tlv_parser<LogT>::L;
		using tlv_parser<LogT>::parse_tlv_u8;
		using tlv_parser<LogT>::parse_tlv_u16;
		using tlv_parser<LogT>::parse_tlv_u32;
		using tlv_parser<LogT>::parse_tlv_s19;
		using tlv_parser<LogT>::parse_tlv_s23;
		using tlv_parser<LogT>::parse_tlv_s65;
		using tlv_parser<LogT>::parse_tlv_s2;
		using tlv_parser<LogT>::parse_tlv_s1;
		using tlv_parser<LogT>::parse_tlv_ptr;

		public:
			parser(LogT & l): tlv_parser<LogT>(l) {}
			virtual ~parser() {}

			enum action {
				stop		/* Stop parsing immediately */
				, resume	/* Resume parsing */
				, skip		/* Skip the value (may be constructor)
							   of an element */
			};

			const bin::u8_t * parse(const bin::u8_t * buf
				, const bin::u8_t * bend) {
				buf = bootstrap(buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				return buf;
			}

		protected:
			virtual action on_authenticate_req(const authenticate_req & msg) = 0;
			virtual action on_authenticate_res(const authenticate_res & msg) = 0;

		private:

			const bin::u8_t * bootstrap(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace bin;

				bin::u32_t len, id;

				const bin::u8_t * cur = buf;
				const bin::u8_t * curc;
				while (cur < bend) {
					/* Parse PDU len and id
					 * In general buffer may contain several commands */
					RETURN_NULL_IF(cur + sizeof(bin::u32_t) * 2 > bend);
					curc = cur;
					curc = p::cp_u32(asbuf(len), curc);
					curc = p::cp_u32(asbuf(id), curc);

					if (cur + len > bend) {
						return nullptr;
					}

					switch (id) {
						case command::authenticate_req:
							cur = parse_authenticate_req(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						case command::authenticate_res:
							cur = parse_bind_receiver(cur, cur + len);
							RETURN_NULL_IF(cur == nullptr);
							continue;
						default:
							/* TODO: implement other parsers */
							return nullptr;
					}
				}

				return cur;
			}

			const bin::u8_t * parse_pdu(pdu & cmd
					, const bin::u8_t * buf) {
					using namespace bin;
					buf = p::cp_u32(asbuf(cmd.len), buf);
					buf = p::cp_u32(asbuf(cmd.id), buf);
					buf = p::cp_u32(asbuf(cmd.proto), buf);
					buf = p::cp_u32(asbuf(cmd.status), buf);
					buf = p::cp_u32(asbuf(cmd.seqno), buf);
					return buf;
			}

			const bin::u8_t * parse_authenticate_req(authenticate_req & msg
					, const bin::u8_t * buf, const bin::u8_t * bend) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.header) > bend);
				buf = parse_pdu(msg.header, buf);

				buf = p::scpyl(msg.username, buf, bend
						, sizeof(msg.username), msg.username_len);
				RETURN_NULL_IF(buf == NULL);

				buf = p::scpyl(msg.password, buf, bend
						, sizeof(msg.password), msg.password_len);
				RETURN_NULL_IF(buf == NULL);

				/* Check for optional parameters */
				if (buf == bend)
					return bend;

				return buf;
			}

			const bin::u8_t * parse_authenticate_res(authenticate_res & msg
					, const bin::u8_t * buf, const bin::u8_t * bend) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.header) > bend);
				buf = parse_pdu(msg.header, buf);

				/* Check for optional parameters */
				if (buf == bend)
					return bend;

				return buf;
			}
	};

	template <class LogT>
	class tlv_writer {
		protected:
			LogT & L;

		public:
			tlv_writer(LogT & l): L(l) {}
			virtual ~tlv_writer() {}

			bin::u8_t * write_tlv_u8(bin::u8_t * buf, const tlv<bin::u8_t> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::cp_u8(buf, ascbuf(t.val));
				return buf;
			}

			bin::u8_t * write_tlv_u16(bin::u8_t * buf, const tlv<bin::u16_t> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::cp_u16(buf, ascbuf(t.val));
				return buf;
			}

			bin::u8_t * write_tlv_u32(bin::u8_t * buf, const tlv<bin::u32_t> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::cp_u32(buf, ascbuf(t.val));
				return buf;
			}

			bin::u8_t * write_tlv_s256(bin::u8_t * buf, const tlv<bin::u8_t[256]> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}

			bin::u8_t * write_tlv_s65(bin::u8_t * buf, const tlv<bin::u8_t[65]> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}

			bin::u8_t * write_tlv_s23(bin::u8_t * buf, const tlv<bin::u8_t[23]> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}

			bin::u8_t * write_tlv_s3(bin::u8_t * buf, const tlv<bin::u8_t[3]> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}

			bin::u8_t * write_tlv_ptr(bin::u8_t * buf, const tlv<bin::u8_t *> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}

			bin::u8_t * write_tlv_s19(bin::u8_t * buf, const tlv<bin::u8_t[19]> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}

			bin::u8_t * write_tlv_s2(bin::u8_t * buf, const tlv<bin::u8_t[2]> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}

			bin::u8_t * write_tlv_s1(bin::u8_t * buf, const tlv<bin::u8_t[1]> & t) {
				using namespace bin;
				buf = w::cp_u16(buf, ascbuf(t.tag));
				buf = w::cp_u16(buf, ascbuf(t.len));
				buf = w::scpy(buf, t.val, t.len);
				return buf;
			}
	};

	template <class LogT>
	class writer: public tlv_writer<LogT> {

		using tlv_writer<LogT>::L;
		using tlv_writer<LogT>::write_tlv_s256;
		using tlv_writer<LogT>::write_tlv_s65;
		using tlv_writer<LogT>::write_tlv_u32;
		using tlv_writer<LogT>::write_tlv_s23;
		using tlv_writer<LogT>::write_tlv_s19;
		using tlv_writer<LogT>::write_tlv_u16;
		using tlv_writer<LogT>::write_tlv_ptr;
		using tlv_writer<LogT>::write_tlv_u8;
		using tlv_writer<LogT>::write_tlv_s3;
		using tlv_writer<LogT>::write_tlv_s2;
		using tlv_writer<LogT>::write_tlv_s1;

		public:
			writer(LogT & l): tlv_writer<LogT>(l) {}
			virtual ~writer() {}

			bin::u8_t * write_pdu(bin::u8_t * buf, const pdu & msg) {
				using namespace bin;
				buf = w::cp_u32(buf, ascbuf(msg.len));
				buf = w::cp_u32(buf, ascbuf(msg.id));
				buf = w::cp_u32(buf, ascbuf(msg.status));
				buf = w::cp_u32(buf, ascbuf(msg.seqno));
				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, bin::u8_t * bend
				, const authenticate_req & msg) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.header) > bend);
				buf = write_pdu(buf, msg.header);

				RETURN_NULL_IF(buf + msg.username_len > bend);
				buf = w::scpy(buf, msg.username, msg.username_len);

				RETURN_NULL_IF(buf + msg.password_len > bend);
				buf = w::scpy(buf, msg.password, msg.password_len);

				return buf;
			}

			bin::u8_t * write(bin::u8_t * buf, bin::u8_t * bend
				, const authenticate_res & msg) {
				using namespace bin;

				RETURN_NULL_IF(buf + sizeof(msg.header) > bend);
				buf = write_pdu(buf, msg.header);

				return buf;
			}
	};

} } }

#endif

