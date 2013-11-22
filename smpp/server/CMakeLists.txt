cmake_minimum_required(VERSION 2.8)
set(ProjectName smppserver)
project(${ProjectName})

set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories("../")
include_directories("../../../helper")

aux_source_directory(src sources)
add_executable(${ProjectName} ${sources})

find_library(bsystem boost_system)
find_library(bthread pthread)

target_link_libraries(${ProjectName} ${bsystem} ${bthread})

