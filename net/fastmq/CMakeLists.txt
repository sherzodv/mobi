cmake_minimum_required(VERSION 2.8)

set(pname r)
project(${pname})

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

	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -std=c++11")

	find_library(lrt rt)
	find_library(lpthread pthread)

	add_definitions(-D_GLIBCXX_USE_NANOSLEEP=1 -DBOOST_LOG_DYN_LINK)
	include_directories("../Inc")
	aux_source_directory(src SOURCES)
	add_executable(fastmq${pname} ${SOURCES})
	target_link_libraries(fastmq${pname}
		${lrt}
		${lpthread}
		${Boost_LIBRARIES}
	)

endif()

