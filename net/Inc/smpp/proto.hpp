#ifndef smpp_proto_hpp
#define smpp_proto_hpp

#include <cstdint>
#include <cstring>

namespace smpp {

	namespace proto {
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
	}

	namespace command {

		const proto::u32_t generic_nack			= 0x80000000;

		const proto::u32_t bind_receiver		= 0x00000001;
		const proto::u32_t bind_receiver_r		= 0x80000001;

		const proto::u32_t bind_transmitter		= 0x00000002;
		const proto::u32_t bind_transmitter_r	= 0x80000002;

		const proto::u32_t query_sm				= 0x00000003;
		const proto::u32_t query_sm_r			= 0x80000003;

		const proto::u32_t submit_sm			= 0x00000004;
		const proto::u32_t submit_sm_r			= 0x80000004;

		const proto::u32_t deliver_sm			= 0x00000005;
		const proto::u32_t deliver_sm_r			= 0x80000005;

		const proto::u32_t unbind				= 0x00000006;
		const proto::u32_t unbind_r				= 0x80000006;

		const proto::u32_t replace_sm			= 0x00000007;
		const proto::u32_t replace_sm_r			= 0x80000007;

		const proto::u32_t cancel_sm			= 0x00000008;
		const proto::u32_t cancel_sm_r			= 0x80000008;

		const proto::u32_t bind_transceiver		= 0x00000009;
		const proto::u32_t bind_transceiver_r	= 0x80000009;

		const proto::u32_t outbind				= 0x0000000B;

		const proto::u32_t enquire_link			= 0x00000015;
		const proto::u32_t enquire_link_r		= 0x80000015;

		const proto::u32_t submit_multi_sm		= 0x00000021;
		const proto::u32_t submit_multi_sm_r	= 0x80000021;

		const proto::u32_t alert_notification	= 0x00000102;

		const proto::u32_t data_sm				= 0x00000103;
		const proto::u32_t data_sm_r			= 0x80000103;
	}

	struct pdu {
		proto::u32_t len;
		proto::u32_t id;
		proto::u32_t status;
		proto::u32_t seqno;
	};

	template <typename ValueT>
	struct tlv {
		proto::u16_t tag;
		proto::u16_t len;
		ValueT value;
	};

	namespace utl {
		using namespace proto;
		typedef std::size_t sz_t;

		/* Parse from buffer to struct , move buffer pointer */
		namespace p {
			template <typename T>
			inline u8_t * asbuf(T & src) {
				return reinterpret_cast<u8_t *>(&src);
			}

			template <typename T>
			inline u8_t * asbuf(T * src) {
				return reinterpret_cast<u8_t *>(src);
			}

			inline const u8_t * cp1(u8_t * dst, const u8_t * src) {
				*dst = *src++;
				return src;
			}

			inline const u8_t * cpy(u8_t * dst, const u8_t * src, sz_t len) {
				while (len) {
					*dst++ = *src++;
					--len;
				}
				return src;
			}

			inline const u8_t * scpy(u8_t * dst, const u8_t * src, sz_t len) {
				while (len) {
					if (*src == 0) {
						*dst++ = *src++;
						return src;
					}
					*dst++ = *src++;
					--len;
				}
				return src;
			}

			inline const u8_t * scpyl(u8_t * dst, const u8_t * src
					, sz_t len, sz_t & l) {
				l = 0;
				while (len) {
					if (*src == 0) {
						*dst++ = *src++;
						return src;
					}
					*dst++ = *src++;
					--len;
					++l;
				}
				return src;
			}
		}

		/* Write from struct to buffer, move buffer pointer */
		namespace w {
			template <typename T>
			inline const u8_t * asbuf(T & src) {
				return reinterpret_cast<u8_t *>(&src);
			}

			template <typename T>
			inline const u8_t * asbuf(T * src) {
				return reinterpret_cast<u8_t *>(src);
			}

			inline u8_t * cp1(u8_t * dst, const u8_t * src) {
				*dst = *src++;
				return dst;
			}

			inline u8_t * cpy(u8_t * dst, const u8_t * src, sz_t len) {
				while (len) {
					*dst++ = *src++;
					--len;
				}
				return dst;
			}

			inline u8_t * scpy(u8_t * dst, const u8_t * src, sz_t len) {
				while (len) {
					if (*src == 0) {
						*dst++ = *src++;
						return dst;
					}
					*dst++ = *src++;
					--len;
				}
				return dst;
			}
		}
	}

	template <class LogT>
	const proto::u8_t * parse_pdu(pdu & cmd
			, const proto::u8_t * buf, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = p::cpy(p::asbuf(cmd.len), buf, sizeof(cmd.len));
		buf = p::cpy(p::asbuf(cmd.id), buf, sizeof(cmd.id));
		buf = p::cpy(p::asbuf(cmd.status), buf, sizeof(cmd.status));
		buf = p::cpy(p::asbuf(cmd.seqno), buf, sizeof(cmd.seqno));
		return buf;
	}

	template <class LogT>
	proto::u8_t * write_pdu(proto::u8_t * buf, const pdu & cmd, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = w::cpy(buf, w::asbuf(cmd.len), sizeof(cmd.len));
		buf = w::cpy(buf, w::asbuf(cmd.id), sizeof(cmd.id));
		buf = w::cpy(buf, w::asbuf(cmd.status), sizeof(cmd.status));
		buf = w::cpy(buf, w::asbuf(cmd.seqno), sizeof(cmd.seqno));
		return buf;
	}

	template <class LogT, typename ValueT>
	const proto::u8_t * parse_tlv(tlv<ValueT> & t
			, const proto::u8_t * buf, LogT & L);

	template <class LogT, typename ValueT>
	proto::u8_t * write_tlv(proto::u8_t * buf
			, const tlv<ValueT> & t, LogT & L);

	template <class LogT>
	const proto::u8_t * parse_tlv(tlv<proto::u8_t> & t
			, const proto::u8_t * buf, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = p::cpy(p::asbuf(t.tag), buf, sizeof(t.tag));
		buf = p::cpy(p::asbuf(t.len), buf, sizeof(t.len));
		buf = p::cp1(p::asbuf(t.value), buf);
		return buf;
	}

	template <class LogT>
	proto::u8_t * write_tlv(proto::u8_t * buf
			, const tlv<proto::u8_t> & t, LogT & L) {
		(void)(L);
		using namespace utl;
		buf = w::cpy(buf, w::asbuf(t.tag), sizeof(t.tag));
		buf = w::cpy(buf, w::asbuf(t.len), sizeof(t.len));
		buf = w::cp1(buf, w::asbuf(t.value));
		return buf;
	}

	typedef tlv<proto::u8_t> tlv_sc_interface_version;

	struct bind_transmitter {
		pdu command;
		proto::u8_t sys_id[16];
		proto::u8_t password[9];
		proto::u8_t sys_type[13];
		proto::u8_t interface_version;
		proto::u8_t addr_ton;
		proto::u8_t addr_npi;
		proto::u8_t addr_range[41];

		std::size_t sys_id_len;
		std::size_t password_len;
		std::size_t sys_type_len;
		std::size_t addr_range_len;
	};

	struct bind_transmitter_r {
		pdu command;
		proto::u8_t sys_id[16];
		tlv_sc_interface_version sc_interface_version;

		std::size_t sys_id_len;
	};

	struct bind_receiver {
		pdu command;
		proto::u8_t sys_id[16];
		proto::u8_t password[9];
		proto::u8_t sys_type[13];
		proto::u8_t interface_version;
		proto::u8_t addr_ton;
		proto::u8_t addr_npi;
		proto::u8_t addr_range[41];

		std::size_t sys_id_len;
		std::size_t password_len;
		std::size_t sys_type_len;
		std::size_t addr_range_len;
	};

	struct bind_receiver_r {
		pdu command;
		proto::u8_t sys_id[16];
		tlv_sc_interface_version sc_interface_version;

		std::size_t sys_id_len;
	};

	struct bind_transceiver {
		pdu command;
		proto::u8_t sys_id[16];
		proto::u8_t password[9];
		proto::u8_t sys_type[13];
		proto::u8_t interface_version;
		proto::u8_t addr_ton;
		proto::u8_t addr_npi;
		proto::u8_t addr_range[41];

		std::size_t sys_id_len;
		std::size_t password_len;
		std::size_t sys_type_len;
		std::size_t addr_range_len;
	};

	struct bind_transceiver_r {
		pdu command;
		proto::u8_t sys_id[16];
		tlv_sc_interface_version sc_interface_version;

		std::size_t sys_id_len;
	};

	const std::size_t bind_pdu_min_len = sizeof(pdu) + sizeof(proto::u8_t) * 7;

	/* BIND TRANSMITTER */

	template <class LogT>
	void parse_bind_transmitter(bind_transmitter & r
			, const proto::u8_t * buf, LogT & L) {
		using namespace utl;
		buf = parse_pdu(r.command, buf, L);
		buf = p::scpyl(r.sys_id, buf, sizeof(r.sys_id), r.sys_id_len);
		buf = p::scpyl(r.password, buf , sizeof(r.password), r.password_len);
		buf = p::scpyl(r.sys_type, buf, sizeof(r.sys_type), r.sys_id_len);
		buf = p::cp1(&r.interface_version, buf);
		buf = p::cp1(&r.addr_ton, buf);
		buf = p::cp1(&r.addr_npi, buf);
		buf = p::scpyl(r.addr_range, buf
				, sizeof(r.addr_range), r.addr_range_len);
	}

	template <class LogT>
	void write_bind_transmitter(proto::u8_t * buf
			, const bind_transmitter & r, LogT & L) {
		using namespace utl;
		buf = write_pdu(buf, r.command, L);
		buf = w::scpy(buf, r.sys_id, r.sys_id_len + 1);
		buf = w::scpy(buf, r.password, r.password_len + 1);
		buf = w::scpy(buf, r.sys_type, r.sys_id_len + 1);
		buf = w::cp1(buf, &r.interface_version);
		buf = w::cp1(buf, &r.addr_ton);
		buf = w::cp1(buf, &r.addr_npi);
		buf = w::scpy(buf, r.addr_range, r.addr_range_len + 1);
	}

	template <class LogT>
	void parse_bind_transmitter_r(bind_transmitter_r & r
			, const proto::u8_t * buf, LogT & L) {
		using namespace utl;
		buf = parse_pdu(r.command, buf, L);
		buf = parse_tlv(r.sc_interface_version, buf, L);
	}

	template <class LogT>
	void write_bind_transmitter_r(proto::u8_t * buf
			, const bind_transmitter_r & r, LogT & L) {
		using namespace utl;
		buf = write_pdu(buf, r.command, L);
	}

	/* BIND RECEIVER */

	template <class LogT>
	void parse_bind_receiver(bind_receiver & r
			, const proto::u8_t * buf, LogT & L) {
		using namespace utl;
		buf = parse_pdu(r.command, buf, L);
		buf = p::scpyl(r.sys_id, buf, sizeof(r.sys_id), r.sys_id_len);
		buf = p::scpyl(r.password, buf , sizeof(r.password), r.password_len);
		buf = p::scpyl(r.sys_type, buf, sizeof(r.sys_type), r.sys_id_len);
		buf = p::cp1(&r.interface_version, buf);
		buf = p::cp1(&r.addr_ton, buf);
		buf = p::cp1(&r.addr_npi, buf);
		buf = p::scpyl(r.addr_range, buf
				, sizeof(r.addr_range), r.addr_range_len);
	}

	template <class LogT>
	void write_bind_receiver(proto::u8_t * buf
			, const bind_receiver & r, LogT & L) {
		using namespace utl;
		buf = write_pdu(buf, r.command, L);
		buf = w::scpy(buf, r.sys_id, r.sys_id_len + 1);
		buf = w::scpy(buf, r.password, r.password_len + 1);
		buf = w::scpy(buf, r.sys_type, r.sys_id_len + 1);
		buf = w::cp1(buf, &r.interface_version);
		buf = w::cp1(buf, &r.addr_ton);
		buf = w::cp1(buf, &r.addr_npi);
		buf = w::scpy(buf, r.addr_range, r.addr_range_len + 1);
	}

	template <class LogT>
	void parse_bind_receiver_r(bind_receiver_r & r
			, const proto::u8_t * buf, LogT & L) {
		using namespace utl;
		buf = parse_pdu(r.command, buf, L);
		buf = parse_tlv(r.sc_interface_version, buf, L);
	}

	template <class LogT>
	void write_bind_receiver_r(proto::u8_t * buf
			, const bind_receiver_r & r, LogT & L) {
		using namespace utl;
		buf = write_pdu(buf, r.command, L);
	}

	/* BIND TRANSCEIVER */

	template <class LogT>
	void parse_bind_transceiver(bind_transceiver & r
			, const proto::u8_t * buf, LogT & L) {
		using namespace utl;
		buf = parse_pdu(r.command, buf, L);
		buf = p::scpyl(r.sys_id, buf, sizeof(r.sys_id), r.sys_id_len);
		buf = p::scpyl(r.password, buf , sizeof(r.password), r.password_len);
		buf = p::scpyl(r.sys_type, buf, sizeof(r.sys_type), r.sys_id_len);
		buf = p::cp1(&r.interface_version, buf);
		buf = p::cp1(&r.addr_ton, buf);
		buf = p::cp1(&r.addr_npi, buf);
		buf = p::scpyl(r.addr_range, buf
				, sizeof(r.addr_range), r.addr_range_len);
	}

	template <class LogT>
	void write_bind_transceiver(proto::u8_t * buf
			, const bind_transceiver & r, LogT & L) {
		using namespace utl;
		buf = write_pdu(buf, r.command, L);
		buf = w::scpy(buf, r.sys_id, r.sys_id_len + 1);
		buf = w::scpy(buf, r.password, r.password_len + 1);
		buf = w::scpy(buf, r.sys_type, r.sys_id_len + 1);
		buf = w::cp1(buf, &r.interface_version);
		buf = w::cp1(buf, &r.addr_ton);
		buf = w::cp1(buf, &r.addr_npi);
		buf = w::scpy(buf, r.addr_range, r.addr_range_len + 1);
	}

	template <class LogT>
	void parse_bind_transceiver_r(bind_transceiver_r & r
			, const proto::u8_t * buf, LogT & L) {
		using namespace utl;
		buf = parse_pdu(r.command, buf, L);
		buf = parse_tlv(r.sc_interface_version, buf, L);
	}

	template <class LogT>
	void write_bind_transceiver_r(proto::u8_t * buf
			, const bind_transceiver_r & r, LogT & L) {
		using namespace utl;
		buf = write_pdu(buf, r.command, L);
	}

}

#endif
