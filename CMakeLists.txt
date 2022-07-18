cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)

project(reroute53)

add_subdirectory(src)

install(TARGETS reroute53)
