#ifndef smpp_proto_session_hpp
#define smpp_proto_session_hpp

#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <proto/smanager.hpp>
#include <proto/pdu.hpp>

namespace smpp { namespace proto {

	namespace ba = boost::asio;
	namespace bs = boost::system;
	namespace bip = boost::asio::ip;
	typedef boost::asio::ip::tcp btcp;

	class session: boost::enable_shared_from_this<session> {

		public:
			typedef boost::shared_ptr<session> shared_ptr;
			typedef proto::session_manager<shared_ptr> manager_type;

		private:
			session(ba::io_service & io, manager_type & mgr)
				: m_socket(io)
				, m_manager(mgr)
			{
			}

			void validate(const bs::error_code & errorCode, std::size_t bytes
					, std::size_t needed, const char * msg) {
				if (errorCode)
					throw std::runtime_error(errorCode.message());
				if (bytes < needed)
					throw std::runtime_error(std::string("smpp::proto::session::") + msg);
			}

			template <typename Handler>
			void async_read_n(std::size_t n, Handler h) {
				if (in.buf.size() < n)
					in.buf.resize(n);
				ba::async_read(m_socket, ba::buffer(&in.buf[0], n)
					, boost::bind(h, shared_from_this()
						, ba::placeholders::error, ba::placeholders::bytes_transferred));
			}

			void async_read_header() {
				async_read_n(pdu::header_size, &session::on_header);
				m_manager.add(shared_from_this());
			}

			void on_header(const bs::error_code & errorCode, size_t bytes) {
				validate(errorCode, bytes, pdu::header_size, "on_header: Unexpected end of stream");
				in.header.read(&in.buf[0]);
				switch (in.header.get.commandId) {
					case pdu::command::bind_transmitter:
						async_read_n(in.header.get.commandLength - pdu::header_size
							, &session::parse_bind_transmitter);
						break;
					case pdu::command::bind_receiver:
						async_read_n(in.header.get.commandLength - pdu::header_size
							, &session::parse_bind_receiver);
						break;
					case pdu::command::bind_transceiver:
						async_read_n(in.header.get.commandLength - pdu::header_size
							, &session::parse_bind_transceiver);
						break;
				}
			}

			void parse_bind_transmitter(const bs::error_code & errorCode, size_t bytes) {
				validate(errorCode, bytes, in.header.get.commandLength - pdu::header_size
					, "on_bind::transmitter: Unexpected end of stream");
				pdu::bind_transmitter bt;
				bt.header = in.header;

				uint8_t * src = &in.buf[0];

				std::strncpy(src, bt.systemId, sizeof(bt.systemId));
			}

			void parse_bind_receiver(const bs::error_code & errorCode, size_t bytes) {
				validate(errorCode, bytes, in.header.get.commandLength - pdu::header_size
					, "on_bind::transmitter: Unexpected end of stream");
			}

			void parse_bind_transceiver(const bs::error_code & errorCode, size_t bytes) {
				validate(errorCode, bytes, in.header.get.commandLength - pdu::header_size
					, "on_bind::transmitter: Unexpected end of stream");
			}

		private:
			btcp::socket   m_socket;
			manager_type & m_manager;

			struct in_s {
				pdu::header header;
				std::vector<uint8_t> buf;
			} in;

			struct out_s {
				std::vector<uint8_t> buf;
			} out;
	};

} }

#endif

