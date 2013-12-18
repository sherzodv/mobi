#ifndef mobi_net_ss7_tcap_hpp
#define mobi_net_ss7_tcap_hpp

#include <bitset>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <ostream>
#include <arpa/inet.h>

#include <asn/ber.hpp>

#define RETURN_NULL_IF(expr) if (expr) return nullptr

namespace ss7 { namespace tcap {

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

	namespace message_type {
		const proto::u8_t uni		= 0x01;
		const proto::u8_t begin		= 0x02;
		const proto::u8_t end		= 0x04;
		const proto::u8_t resume	= 0x05;
		const proto::u8_t abort		= 0x06;
	}

	namespace portion_type {
		const proto::u8_t dialog	= 0x0B;
		const proto::u8_t component	= 0x0C;
	}

	namespace component_type {
		const proto::u8_t invoke		= 0x01;
		const proto::u8_t result_last	= 0x02;
		const proto::u8_t return_error	= 0x03;
		const proto::u8_t reject		= 0x04;
		const proto::u8_t return_result	= 0x07;
	}

	namespace element {

		struct uni {
		};

		struct begin {
			proto::u64_t otid;
		};

		struct end {
			proto::u64_t dtid;
		};

		struct resume {
		};

		struct abort {
		};

		struct invoke {
			proto::s8_t id;
			proto::s8_t linked_id;
			proto::u32_t op_code;
		};

		struct return_result {
			proto::s8_t invokeId;
			proto::u32_t op_code;
		};

	}

	template< typename CharT, typename TraitsT >
	void dump(std::basic_ostream< CharT, TraitsT >& L, const proto::u8_t * buf, std::size_t len);

	template <class LogT>
	class parser {

		public:
			parser(LogT & l): L(l) {}
			virtual ~parser() {}

			const proto::u8_t * parse(const proto::u8_t * buf
				, const proto::u8_t * bend) {
				using namespace asn::ber;
				buf = parse_message_type(buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				return buf;
			}

		protected:
			LogT & L;

			enum action { stop, resume };

			virtual action on_uni(const element::uni & el) = 0;
			virtual action on_begin(const element::begin & el) = 0;
			virtual action on_end(const element::end & el) = 0;
			virtual action on_resume(const element::resume & el) = 0;
			virtual action on_abort(const element::abort & el) = 0;

			virtual action on_invoke(const element::invoke & el) = 0;
			virtual action on_return_result_last(const element::return_result & el) = 0;

			virtual action on_primitive(asn::ber::tag tag, const proto::u8_t * data) = 0;

			virtual action on_constructor_start(asn::ber::tag tag, const proto::u8_t * data) = 0;
			virtual action on_constructor_end() = 0;

		private:
			asn::ber::tag m_mtag; /* Message type tag */

			const proto::u8_t * parse_message_type(const proto::u8_t * buf
					, const proto::u8_t * bend) {
				using namespace asn::ber;

				/* Parse TCAP Message type tag with len */
				buf = parse_tag(m_mtag, buf, bend, L);

				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(m_mtag.klass != tagclass_application);
				RETURN_NULL_IF(m_mtag.form != tagform_constructor);

				switch (m_mtag.code) {
					case message_type::uni:
						/* TODO: implement */
						return nullptr;
					case message_type::begin:
						buf = parse_begin(buf, bend);
						RETURN_NULL_IF(buf == nullptr);
						return buf;
					case message_type::end:
						buf = parse_end(buf, bend);
						RETURN_NULL_IF(buf == nullptr);
						return buf;
					case message_type::resume:
						/* TODO: implement */
						return nullptr;
					case message_type::abort:
						/* TODO: implement */
						return nullptr;
					default: return nullptr; /* Wrong message type tag */
				}
			}

			const proto::u8_t * parse_begin(const proto::u8_t * buf
					, const proto::u8_t * bend) {
				using namespace asn::ber;

				tag t;
				element::begin b;

				/* Parse OTID tag and len */
				buf = parse_tag(t, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);

				/* Parse OTID value */
				buf = parse_integer(b.otid, t.len, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(on_begin(b) == stop);

				/* See what is next: dialog or component portion?*/

				buf = parse_tag(t, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(t.klass != tagclass_application
						|| t.form != tagform_constructor);

				switch (t.code) {
					case portion_type::dialog:
						/* TODO: implement */
						return nullptr;
					case portion_type::component:
						buf = parse_component_portion(buf, bend);
						RETURN_NULL_IF(buf == nullptr);
						return buf;
					default:
						/* Wrong portion tag */
						return nullptr;
				}
			}

			const proto::u8_t * parse_end(const proto::u8_t * buf
					, const proto::u8_t * bend) {
				using namespace asn::ber;

				tag t;
				element::end end;

				/* Parse DTID tag and len */
				buf = parse_tag(t, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);

				/* Parse DTID value */
				buf = parse_integer(end.dtid, t.len, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(on_end(end) == stop);

				/* See what is next: dialog or component portion?*/

				buf = parse_tag(t, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(t.klass != tagclass_application
						|| t.form != tagform_constructor);

				switch (t.code) {
					case portion_type::dialog:
						/* TODO: implement */
						return nullptr;
					case portion_type::component:
						buf = parse_component_portion(buf, bend);
						RETURN_NULL_IF(buf == nullptr);
						return buf;
					default:
						/* Wrong portion tag */
						return nullptr;
				}
			}

			const proto::u8_t * parse_component_portion(const proto::u8_t * buf
					, const proto::u8_t * bend) {
				using namespace asn::ber;

				tag t;
				while (buf < bend) {
					/* Parse TCAP Component tag with len */
					buf = parse_tag(t, buf, bend, L);
					RETURN_NULL_IF(buf == nullptr);
					switch (t.code) {
						case component_type::invoke:
							buf = parse_invoke(buf, buf + t.len);
							RETURN_NULL_IF(buf == nullptr);
							break;
						case component_type::result_last:
							buf = parse_return_result_last(buf, buf + t.len);
							RETURN_NULL_IF(buf == nullptr);
							break;
						case component_type::return_error:
							/* TODO: implement */
							return nullptr;
						case component_type::reject:
							/* TODO: implement */
							return nullptr;
						case component_type::return_result:
							/* TODO: implement */
							return nullptr;
						default:
							/* Wrong component type: skip */
							buf += t.len;
							break;
					}
				}
				return buf;
			}

			const proto::u8_t * parse_invoke(const proto::u8_t * buf
					, const proto::u8_t * bend) {
				using namespace asn::ber;

				/*
				Invoke ::= SEQUENCE {
					invokeID InvokeIdType,
					linkedID [0] IMPLICIT InvokeIdType OPTIONAL,
					operationCode OPERATION,
					parameter ANY DEFINED BY operationCode OPTIONAL }
				*/

				tag t;
				element::invoke inv;

				/* Parse invokeID tag */
				buf = parse_tag(t, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(t != type::integer);

				/* Parse invokeID value */
				buf = parse_integer(inv.id, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);

				inv.linked_id = 0;
				inv.op_code = 0;

				/* Now see what is next: linkedID is present or op_code */
				buf = parse_tag(t, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);

				if (t.klass == tagclass_contextspec
						&& t.form == tagform_primitive
						&& t.code == 0x00) {
					/* linkedID is present, parse it */
					buf = parse_integer(inv.linked_id, buf, bend, L);
					RETURN_NULL_IF(buf == nullptr);

					/* Parse operationCode tag */
					buf = parse_tag(t, buf, bend, L);
					RETURN_NULL_IF(buf == nullptr);
				}

				/* Parse operationCode value */
				buf = parse_integer(inv.op_code, t.len, buf, bend, L);
				RETURN_NULL_IF(on_invoke(inv) == stop);

				return parse_elements(buf, bend);
			}

			const proto::u8_t * parse_return_result(
					element::return_result & rres
					, const proto::u8_t * buf
					, const proto::u8_t * bend) {
				using namespace asn::ber;

				/* ReturnResult ::=
					SEQUENCE {
					invokeID InvokeIdType,
					result SEQUENCE {
							operationCode OPERATION,
							parameter ANY DEFINED BY operationCode
						} OPTIONAL
					}
				*/

				tag t;

				rres.invokeId = 0;
				rres.op_code = 0;

				/* Parse invokeID tag */
				buf = parse_tag(t, buf, bend, L);

				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(t != type::integer);

				/* Parse invokeID value */
				buf = parse_integer(rres.invokeId, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);

				/* Parse SEQUENCE tag */
				buf = parse_tag(t, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(t != type::sequence);

				/* Parse operationCode tag */
				buf = parse_tag(t, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);

				/* Parse operationCode value */
				buf = parse_integer(rres.op_code, t.len, buf, bend, L);
				RETURN_NULL_IF(buf == nullptr);

				return buf;
			}

			const proto::u8_t * parse_return_result_last(
					const proto::u8_t * buf
					, const proto::u8_t * bend) {
				using namespace asn::ber;

				element::return_result rres;

				buf = parse_return_result(rres, buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(on_return_result_last(rres) == stop);

				return parse_elements(buf, bend);
			}

			const proto::u8_t * parse_elements(const proto::u8_t * buf
					, const proto::u8_t * bend) {
				using namespace asn::ber;
				tag t;
				while (buf < bend) {
					/* Parse invokeID tag */
					buf = parse_tag(t, buf, bend, L);
					RETURN_NULL_IF(buf == nullptr);
					if (t.form == tagform_primitive) {
						RETURN_NULL_IF(on_primitive(t, buf) == stop);
						buf += t.len;
					} else {
						/* constructor */
						RETURN_NULL_IF(on_constructor_start(t, buf) == stop);
						buf = parse_elements(buf, buf + t.len);
						RETURN_NULL_IF(on_constructor_end() == stop);
					}
				}
				return buf;
			}

	};

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const element::begin & b) {
		using asn::ber::operator<<;
		L << "[begin:"
			<< "[otid:" << b.otid << "]"
		<< "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const element::end & end) {
		using asn::ber::operator<<;
		L << "[end:"
			<< "[dtid:" << end.dtid << "]"
		<< "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const element::invoke & inv) {
		using asn::ber::operator<<;
		L << "[invoke:"
			<< "[id:" << static_cast<unsigned>(inv.id) << "]"
			<< "[linked_id:" << static_cast<unsigned>(inv.linked_id) << "]"
			<< "[opcode:" << inv.op_code << "]"
		<< "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& L, const element::return_result & rres) {
		using asn::ber::operator<<;
		L << "[return_result:"
			<< "[id:" << static_cast<unsigned>(rres.invokeId) << "]"
			<< "[opcode:" << rres.op_code << "]"
		<< "]";
		return L;
	}

	template< typename CharT, typename TraitsT >
	void dump(std::basic_ostream< CharT, TraitsT >& L, const proto::u8_t * buf, std::size_t len) {
		L << std::hex;
		while (len) {
			L << "0x" << static_cast<unsigned>(*buf);
			if (len != 1)
				L << " ";
			len--;
			buf++;
		}
		L << std::endl;
	}

} }

#endif