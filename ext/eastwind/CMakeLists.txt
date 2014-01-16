set(pname eastwind)
SET(CMAKE_CXX_COMPILER g++-4.8)
add_definitions(-D_GLIBCXX_USE_NANOSLEEP=1)

project(${pname})
cmake_minimum_required(VERSION 2.8)

add_subdirectory(ewgate)
add_subdirectory(smppd)

