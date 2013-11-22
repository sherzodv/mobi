cmake_minimum_required(VERSION 2.8)

set(pname logger)

project(${pname})

set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
set(HEADERS_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Inc/fastmq/${pname})

set(LIBRARY_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Lib)
set(LIBRARY_NAME libfastmq${pname}.*)

set(CMAKE_CXX_FLAGS "-ggdb -Wall -Wextra -Werror -ansi -pedantic -std=c++11 ")
add_definitions(-D_GLIBCXX_USE_NANOSLEEP=1 -DBOOST_LOG_DYN_LINK)
aux_source_directory(src SOURCES)
add_library(fastmq${pname} SHARED ${SOURCES})

add_custom_command(TARGET fastmq${pname} POST_BUILD
	COMMAND cp ${HEADERS} ${HEADERS_OUTPUT_DIR})
add_custom_command(TARGET fastmq${pname} POST_BUILD
	COMMAND cp ${LIBRARY_SRC_DIR}/${LIBRARY_NAME} ${LIBRARY_OUTPUT_DIR}/)
