
#include <fastmq/log.hpp>

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

namespace format {
	auto simple = ble::stream
		<< ble::attr<std::uint32_t>("RecID")
		<< ": " << ble::format_date_time<boost::posix_time::ptime>(
				"Time", "%d.%m.%Y %H:%M:%S.%f")
		<< " [prc "
			<< ble::attr<bla::current_process_id::value_type>("PrcID") << "]"
		<< " [chan "
			<< ble::attr<std::string>("ChID") << "]"
		<< " [thr "
			<< ble::attr<bla::current_thread_id::value_type>("ThrID") << "]"
		<< " [" << ble::attr<severity>("Severity") << "]"
		<< "\t" << ble::message;
}

namespace file {

	typedef blsi::text_file_backend backend;
	typedef blsi::asynchronous_sink<backend
		, blsi::unbounded_ordering_queue<
			bl::attribute_value_ordering<std::uint32_t, std::less<std::uint32_t>>
		>
	> asink;
	typedef blsi::synchronous_sink<backend> sink;

	source mklogger(const std::string & channel) {
		source src(blk::channel = channel);
		src.add_attribute("RecID", bla::counter<std::uint32_t>(1));
		src.add_attribute("Time", bla::local_clock());
		src.add_attribute("PrcID", bla::current_process_id());
		src.add_attribute("ChID", bla::make_constant(channel));
		src.add_attribute("ThrID", bla::current_thread_id());
		return src;
	}

	void add(const std::string & name, bool autoflush) {

		using boost::shared_ptr;
		using boost::make_shared;

		if (!autoflush) {
			auto b = make_shared<backend>(blk::file_name = name);
			auto s = make_shared<asink>(b
				, blk::order = bl::make_attr_ordering("RecID"
					, std::less<std::uint32_t>())
				, blk::auto_flush = autoflush
			);
			s->set_formatter(format::simple);
			bl::core::get()->add_sink(s);
		} else {
			auto b = make_shared<backend>(blk::file_name = name);
			b->auto_flush(true);
			auto s = make_shared<sink>(b);
			s->set_formatter(format::simple);
			bl::core::get()->add_sink(s);
		}
	}

    template<typename FunT>
	void add(const std::string & nametmpl, std::size_t rotsz
		, const rtime & rt, bool autoflush, const FunT & fmt) {

		using boost::shared_ptr;
		using boost::make_shared;

		auto b = make_shared<backend>(
			blk::file_name = nametmpl
			, blk::rotation_size = rotsz
			, blk::time_based_rotation = rt
			, blk::auto_flush = autoflush
		);

		auto s = make_shared<asink>(b
			, blk::order = bl::make_attr_ordering("RecID"
				, std::less<std::uint32_t>())
		);

		s->set_formatter(fmt);
		bl::core::get()->add_sink(s);
	}
}

} }
