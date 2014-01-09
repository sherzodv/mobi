cmake_minimum_required(VERSION 2.8)

set(pname fastds)
project(${pname})

set(Boost_USE_STATIC_LIBS		off)
set(Boost_USE_MULTITHREADED		on)
set(Boost_DEBUG					off)

find_package(Boost 1.54.0 COMPONENTS
	date_time
	filesystem
	system
	thread
	log-mt
	log_setup-mt)

if (NOT Boost_FOUND)
	message (FATAL_ERROR "boost not found")
endif()

#set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-ggdb -Wall -Wextra -Werror -pedantic -std=c++11")

find_library(lrt rt)
find_library(lpthread pthread)

add_definitions(-D_GLIBCXX_USE_NANOSLEEP=1 -DBOOST_LOG_DYN_LINK)
include_directories("../Inc")
aux_source_directory(src SOURCES)
add_library(${pname} SHARED ${SOURCES})
target_link_libraries(${pname}
	${lrt}
	${lpthread}
	${Boost_LIBRARIES}
)

file(GLOB headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
foreach(header ${headers})
	set(HOUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Inc/${pname})
	add_custom_command(OUTPUT ${HOUT_DIR}/${header}
		COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/src/${header}
			${HOUT_DIR}/${header}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/${header}
	)
	list(APPEND HEADERS ${HOUT_DIR}/${header})
endforeach(header)

add_custom_target(copy_${pname}_headers ALL DEPENDS ${HEADERS})
add_dependencies(${pname} copy_${pname}_headers)

install(TARGETS ${pname} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../Lib)
