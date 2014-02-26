cmake_minimum_required(VERSION 2.8)

set(pname ss7test)
project(${pname})

set(Boost_USE_STATIC_LIBS		off)
set(Boost_USE_MULTITHREADED		on)
set(Boost_DEBUG					off)

find_package(Boost 1.54.0 COMPONENTS
	date_time
	filesystem
	system
	thread
	unit_test_framework
	log
	log_setup)

if (NOT Boost_FOUND)
	message (FATAL_ERROR "boost not found")
endif()

#set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-ggdb -Wall -Wextra -Werror -pedantic -std=c++11")

find_library(lrt rt)
find_library(lpthread pthread)
find_library(lss7 ss7 ${CMAKE_CURRENT_SOURCE_DIR}/../Lib)

add_definitions(-D_GLIBCXX_USE_NANOSLEEP=1 -DBOOST_LOG_DYN_LINK -DBOOST_TEST_DYN_LINK)
include_directories("../Inc")
aux_source_directory(src SOURCES)
add_executable(${pname} ${SOURCES})
target_link_libraries(${pname}
	${lrt}
	${lpthread}
	${Boost_LIBRARIES}
)
