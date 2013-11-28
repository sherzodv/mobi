set(pname mobinet)
project(${pname})

cmake_minimum_required(VERSION 2.8)

add_subdirectory(toolbox)
add_subdirectory(vision)
add_subdirectory(fastmq)
add_subdirectory(fastmqr)
add_subdirectory(smpp)
add_subdirectory(smppd)
add_subdirectory(smpptest)
