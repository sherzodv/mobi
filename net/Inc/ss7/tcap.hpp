#ifndef mobi_net_ss7_tcap_hpp
#define mobi_net_ss7_tcap_hpp

#include <bitset>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <ostream>
#include <arpa/inet.h>

#include <asn/ber.hpp>
#include <toolbox/bin.hpp>

namespace mobi { namespace net { namespace ss7 { namespace tcap {

	using namespace toolbox;

	namespace message_type {
		const bin::u8_t uni		= 0x01;
		const bin::u8_t begin	= 0x02;
		const bin::u8_t end		= 0x04;
		const bin::u8_t resume	= 0x05;
		const bin::u8_t abort	= 0x06;
	}

	namespace portion_type {
		const bin::u8_t dialog		= 0x0B;
		const bin::u8_t component	= 0x0C;
	}

	namespace component_type {
		const bin::u8_t invoke			= 0x01;
		const bin::u8_t result_last		= 0x02;
		const bin::u8_t return_error	= 0x03;
		const bin::u8_t reject			= 0x04;
		const bin::u8_t return_result	= 0x07;
	}

	namespace element {

		struct operation_code {
			enum { local, global, unknown } type;
			union {
				bin::u32_t local;
				bin::u8_t global[64];/* TODO: implement oid */
			} as;

			operation_code(): type(unknown) {}
		};

		struct uni {
		};

		struct begin {
			bin::u32_t otid;
		};

		struct end {
			bin::u32_t dtid;
		};

		struct resume {
		};

		struct abort {
		};

		struct invoke {
			bin::s8_t id;
			bin::s8_t linked_id;
			operation_code op_code;

			bool has_linked_id;

			const bin::u8_t *data;
			const bin::u8_t *dend;

			enum: bin::u64_t { tag_code = 0x01L };

			invoke()
				: id(0), linked_id(0), op_code(), has_linked_id(false)
				, data(nullptr), dend(nullptr) {}
		};

		struct return_result {
			bin::s8_t invokeId;
			operation_code op_code;
			const bin::u8_t *data;
			const bin::u8_t *dend;

			return_result()
				: invokeId(0), op_code(), data(nullptr), dend(nullptr) {}
		};

	}

	template< typename CharT, typename TraitsT >
	void dump(std::basic_ostream< CharT, TraitsT >& L, const bin::u8_t * buf, bin::sz_t len);

	template <class LogT>
	class parser: public asn::ber::parser<LogT> {

		typedef asn::ber::parser<LogT> base;

		public:
			parser(LogT & l): base(l) {}
			virtual ~parser() {}

			virtual const bin::u8_t * parse(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;

				tag t;

				/* Parse message type tag */
				buf = base::parse_tag(t, buf, bend);

				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(t.klass != tagclass_application);
				RETURN_NULL_IF(t.form != tagform_constructor);

				switch (t.code) {
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

		protected:
			using base::L;
			using base::stop;
			using base::resume;
			using base::skip;
			typedef typename base::action action;

			/* TODO: implement skipping */

			virtual action on_uni(const element::uni & el) = 0;
			virtual action on_begin(const element::begin & el) = 0;
			virtual action on_end(const element::end & el) = 0;
			virtual action on_resume(const element::resume & el) = 0;
			virtual action on_abort(const element::abort & el) = 0;

			virtual action on_invoke(const element::invoke & el) = 0;
			virtual action on_return_result_last(const element::return_result & el) = 0;

			virtual action on_primitive(asn::ber::tag tag, const bin::u8_t * data) = 0;

			virtual action on_constructor_start(asn::ber::tag tag, const bin::u8_t * data) = 0;
			virtual action on_constructor_end() = 0;

		protected:

			const bin::u8_t * parse_begin(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;

				tag t;
				element::begin b;

				/* Parse OTID */
				buf = base::parse_octstring(bin::asbuf(b.otid), sizeof(b.otid)
						, buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(on_begin(b) == stop);

				/* See what is next: dialog or component portion?*/

				buf = base::parse_tag(t, buf, bend);
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

			const bin::u8_t * parse_end(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;

				tag t;
				element::end r;

				/* Parse DTID */
				buf = base::parse_octstring(bin::asbuf(r.dtid), sizeof(r.dtid)
						, buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(on_end(r) == stop);

				/* See what is next: dialog or component portion?*/

				buf = base::parse_tag(t, buf, bend);
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

			const bin::u8_t * parse_component_portion(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;

				tag t;
				while (buf < bend) {
					/* Parse TCAP Component tag with len */
					buf = base::parse_tag(t, buf, bend);
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

			const bin::u8_t * parse_invoke(const bin::u8_t * buf
					, const bin::u8_t * bend) {
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

				/* Parse invokeID */
				buf = base::parse_integer(inv.id, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				/* Now see what is next: linkedID is present or op_code */
				buf = base::parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				if (t == tag {tagclass_contextspec, tagform_primitive, 0x00}) {
					/* linkedID is present, parse it */
					buf = base::parse_integer(inv.linked_id, t.len, buf, bend);
					RETURN_NULL_IF(buf == nullptr);

					/* Parse operationCode tag */
					buf = base::parse_tag(t, buf, bend);
					RETURN_NULL_IF(buf == nullptr);
				}

				if (t == type::integer) {
					inv.op_code.type = element::operation_code::local;
					buf = base::parse_integer(inv.op_code.as.local
							, t.len, buf, bend);
				} else if (t == type::oid) {
					/* TODO: implement oid parser */
					return nullptr;
				} else {
					return nullptr;
				}

				inv.data = buf;
				inv.dend = bend;

				switch (on_invoke(inv)) {
					case stop: return nullptr;
					case skip: return bend;
					default: return parse_elements(buf, bend);
				}
			}

			const bin::u8_t * parse_return_result(
					element::return_result & r
					, const bin::u8_t * buf
					, const bin::u8_t * bend) {
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

				/* Parse invokeID */
				buf = base::parse_integer(r.invokeId, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				/* Parse SEQUENCE tag */
				buf = base::parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr);
				RETURN_NULL_IF(t != type::sequence);

				/* Parse operationCode tag */
				buf = base::parse_tag(t, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				if (t == type::integer) {
					r.op_code.type = element::operation_code::local;
					buf = base::parse_integer(r.op_code.as.local
							, t.len, buf, bend);
				} else if (t == type::oid) {
					/* TODO: implement oid parser */
					return nullptr;
				} else {
					return nullptr;
				}

				return buf;
			}

			const bin::u8_t * parse_return_result_last(
					const bin::u8_t * buf
					, const bin::u8_t * bend) {

				using namespace asn::ber;

				element::return_result r;

				buf = parse_return_result(r, buf, bend);
				RETURN_NULL_IF(buf == nullptr);

				r.data = buf;
				r.dend = bend;

				switch (on_return_result_last(r)) {
					case stop: return nullptr;
					case skip: return bend;
					default: return parse_elements(buf, bend);
				}
			}

			const bin::u8_t * parse_elements(const bin::u8_t * buf
					, const bin::u8_t * bend) {
				using namespace asn::ber;
				tag t;
				while (buf < bend) {
					buf = base::parse_tag(t, buf, bend);
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

	template <class LogT>
	class writer: public asn::ber::writer<LogT> {

		typedef asn::ber::writer<LogT> base;

		public:
			writer(LogT & l): base(l) {}
			virtual ~writer() {}

		protected:
			using base::L;

			bin::u8_t * write_begin(bin::u8_t * buf, bin::u8_t * bend
					, const element::begin & r, bin::sz_t len) {

				using namespace asn::ber;

				RETURN_NULL_IF(buf
						+ element_size(message_type::begin, len) > bend);

				buf = base::write_tag(buf, bend, tagclass_application
						, tagform_constructor, message_type::begin, len);
				RETURN_NULL_IF(buf == nullptr);

				buf = base::write_integer(buf, bend
						, tagclass_application, tagcode_octstring
						, r.otid);

				return buf;
			}

			bin::u8_t * write_component_portion(bin::u8_t * buf
					, bin::u8_t * bend, bin::sz_t len) {

				using namespace asn::ber;

				RETURN_NULL_IF(buf
						+ element_size(portion_type::component, len) > bend);

				buf = base::write_tag(buf, bend, tagclass_application
						, tagform_constructor, portion_type::component, len);

				return buf;
			}

			bin::u8_t * write_invoke(bin::u8_t * buf, bin::u8_t * bend
					, const element::invoke & r, bin::sz_t len) {
				using namespace asn::ber;

				RETURN_NULL_IF(buf
						+ element_size(component_type::invoke, len) > bend);

				buf = base::write_tag(buf, bend, tagclass_contextspec
						, tagform_constructor, component_type::invoke, len);
				RETURN_NULL_IF(buf == nullptr);

				buf = base::write_integer(buf, bend, tagclass_universal
						, tagform_primitive, r.id);

				if (r.has_linked_id) {
					buf = base::write_integer(buf, bend, tagclass_contextspec
						, tagform_primitive, 0x00);
				}

				if (r.op_code.type == element::operation_code::local) {
					buf = base::write_integer(buf, bend, tagclass_universal
						, tagform_primitive, r.op_code.as.local);
				} else {
					/* TODO: implement working with oids */
					return nullptr;
				}

				return buf;
			}
	};

	std::string to_string(const element::operation_code & r) {
		std::stringstream out;
		out << "[op_code:";
		switch (r.type) {
			case element::operation_code::local:
				out << "local:" << r.as.local;
				break;
			case element::operation_code::global:
				out << "global:not implemented";
				break;
			default:
				out << "unknown";
				break;
		}
		out << "]";
		return out.str();
	}

	std::string to_string(const element::uni & r) {
		std::stringstream out;
		(void)(r);
		out << "[uni:]";
		return out.str();
	}

	std::string to_string(const element::begin & r) {
		std::stringstream out;
		out << "[begin:[otid:"
			<< bin::hex_str_ref(bin::ascbuf(r.otid), sizeof(r.otid))
				.delimit("").prefix("") << "]]";
		return out.str();
	}

	std::string to_string(const element::end & r) {
		std::stringstream out;
		out << "[end:[dtid:"
			<< bin::hex_str_ref(bin::ascbuf(r.dtid), sizeof(r.dtid))
				.delimit("").prefix("") << "]]";
		return out.str();
	}

	std::string to_string(const element::invoke & r) {
		std::stringstream out;
		out << "[invoke:[id:" << bin::as<unsigned>(r.id) << "]"
			<< "[linked_id:" << bin::as<unsigned>(r.linked_id) << "]"
			<< to_string(r.op_code) << "]";
		return out.str();
	}

	std::string to_string(const element::return_result & r) {
		std::stringstream out;
		out << "[return_result:"
			<< "[id:" << bin::as<unsigned>(r.invokeId) << "]"
			<< to_string(r.op_code) << "]";
		return out.str();
	}

} } } }

#endif
