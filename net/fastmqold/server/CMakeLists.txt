cmake_minimum_required(VERSION 2.8)

set(pname r)

project(${pname})

add_definitions(-D_GLIBCXX_USE_NANOSLEEP=1 -DBOOST_LOG_DYN_LINK)

set(Boost_USE_STATIC_LIBS		off)
set(Boost_USE_MULTITHREADED		on)
set(Boost_DEBUG					off)

find_package(Boost 1.54.0 COMPONENTS
	date_time
	filesystem
	system
	thread
	log
	log_setup)

if (Boost_FOUND)

	find_library(lrt rt)
	find_library(lpthread pthread)
	find_library(lfastmqlogger fastmqlogger ${CMAKE_CURRENT_SOURCE_DIR}/../Lib)
	find_library(lfastmqrouter fastmqrouter ${CMAKE_CURRENT_SOURCE_DIR}/../Lib)

	include_directories("../Inc")

	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -ansi -pedantic -std=c++11")
	aux_source_directory(src SOURCES)
	add_executable(fastmq${pname} ${SOURCES})
	target_link_libraries(fastmq${pname}
		${lrt}
		${lpthread}
		${lfastmqlogger}
		${lfastmqrouter}
		${Boost_LIBRARIES}
	)

endif()

