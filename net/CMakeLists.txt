set(pname mobinet)
SET(CMAKE_CXX_COMPILER g++-4.8)

project(${pname})
cmake_minimum_required(VERSION 2.8)

add_subdirectory(toolbox)
add_subdirectory(vision)
#add_subdirectory(fastmq)
#add_subdirectory(fastmqr)
add_subdirectory(smpp)
add_subdirectory(smppd)
#add_subdirectory(asn)
#add_subdirectory(ss7)

#add_subdirectory(ss7test)
#add_subdirectory(smpptest)

