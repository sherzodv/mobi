#ifndef fastmq_logger_hpp
#define fastmq_logger_hpp

#include <iostream>
#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace fastmq { namespace log {

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

namespace file {

	typedef boost::log::sources::severity_channel_logger_mt<severity> source;
	typedef boost::log::sinks::file::rotation_at_time_point rtime;

	source mklogger(const std::string & channel);

	void add(const std::string & name, bool autoflush = false);

    template<typename FunT>
	void add(const std::string & nametmpl, std::size_t rotsz
		, const rtime & rt, bool autoflush
		, const FunT & fmt);
}

#define ltrace(lg)		BOOST_LOG_SEV(lg, fastmq::log::trace)
#define ldebug(lg)		BOOST_LOG_SEV(lg, fastmq::log::debug)
#define linfo(lg)		BOOST_LOG_SEV(lg, fastmq::log::info)
#define lwarning(lg)	BOOST_LOG_SEV(lg, fastmq::log::warning)
#define lerror(lg)		BOOST_LOG_SEV(lg, fastmq::log::error)
#define lcritical(lg)	BOOST_LOG_SEV(lg, fastmq::log::critical)
#define lflush(lg)		do { boost::log::core::get()->flush(); } while(0)

} }

#endif
