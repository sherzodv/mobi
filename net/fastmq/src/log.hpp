#ifndef fastmq_logger_hpp
#define fastmq_logger_hpp

#include <iostream>
#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <thread>
#include <string>
#include <fstream>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <functional>

#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/log/utility/empty_deleter.hpp>
#include <boost/log/utility/record_ordering.hpp>
#include <boost/log/support/date_time.hpp>

namespace fastmq { namespace log {

	namespace bl = boost::log;
	namespace blk = boost::log::keywords;
	namespace bla = boost::log::attributes;
	namespace ble = boost::log::expressions;
	namespace blsi = boost::log::sinks;
	namespace blsr = boost::log::sources;

	/* Define severity levels */
    enum severity {
        trace, debug,
        info, warning,
        error, critical
    };

	/* Define how severity level displayed in logs */
    template<typename CharT, typename TraitsT>
    inline std::basic_ostream<CharT, TraitsT>&
        operator<<(std::basic_ostream<CharT, TraitsT>& strm, severity lvl) {

        static const char* const names[] = {
			"trace", "debug"
			, "info", "warning"
			, "error", "critical"
        };

        if (static_cast<std::size_t>(lvl) < (sizeof(names) / sizeof(*names)))
            strm << names[lvl];
        else
            strm << static_cast<int>(lvl);

        return strm;
    }

	typedef boost::log::sources::severity_channel_logger_mt<severity> source;

	source channel(const std::string & channel) {
		source src(blk::channel = channel);
		src.add_attribute("RecID", bla::counter<std::uint32_t>(1));
		src.add_attribute("Time", bla::local_clock());
		src.add_attribute("PrcID", bla::current_process_id());
		src.add_attribute("ChID", bla::make_constant(channel));
		src.add_attribute("ThrID", bla::current_thread_id());
		return src;
	}

	namespace file {

		typedef boost::log::sinks::file::rotation_at_time_point rtime;
		typedef blsi::text_file_backend backend;
		typedef blsi::asynchronous_sink<backend
			, blsi::unbounded_ordering_queue<
				bl::attribute_value_ordering<std::uint32_t
					, std::less<std::uint32_t>>
			>
		> asink;
		typedef blsi::synchronous_sink<backend> sink;

		void add(const std::string & name, bool autoflush) {
			using boost::shared_ptr;
			using boost::make_shared;

			auto simple = ble::stream
				<< ble::attr<std::uint32_t>("RecID")
				<< "\t"
				<< ": " << ble::format_date_time<boost::posix_time::ptime>(
						"Time", "%d.%m.%Y %H:%M:%S.%f")
				<< " [prc "
					<< ble::attr<bla::current_process_id::value_type>("PrcID") << "]"
				<< " [thr "
					<< ble::attr<bla::current_thread_id::value_type>("ThrID") << "]"
				<< " [chan "
					<< ble::attr<std::string>("ChID") << "]"
				<< "\t[" << ble::attr<severity>("Severity") << "]"
				<< "\t"
				<< ble::message;

			if (!autoflush) {
				auto b = make_shared<backend>(blk::file_name = name);
				auto s = make_shared<asink>(b
					, blk::order = bl::make_attr_ordering("RecID"
						, std::less<std::uint32_t>())
					, blk::auto_flush = autoflush
				);
				s->set_formatter(simple);
				bl::core::get()->add_sink(s);
			} else {
				auto b = make_shared<backend>(blk::file_name = name);
				b->auto_flush(true);
				auto s = make_shared<sink>(b);
				s->set_formatter(simple);
				bl::core::get()->add_sink(s);
			}
		}
	}

} }

#define ltrace(lg)		BOOST_LOG_SEV(lg, fastmq::log::trace)
#define ldebug(lg)		BOOST_LOG_SEV(lg, fastmq::log::debug)
#define linfo(lg)		BOOST_LOG_SEV(lg, fastmq::log::info)
#define lwarning(lg)	BOOST_LOG_SEV(lg, fastmq::log::warning)
#define lerror(lg)		BOOST_LOG_SEV(lg, fastmq::log::error)
#define lcritical(lg)	BOOST_LOG_SEV(lg, fastmq::log::critical)
#define lflush(lg)		do { boost::log::core::get()->flush(); } while(0)

#endif
