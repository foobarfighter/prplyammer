cmake_minimum_required(VERSION 2.6)
project(yammer-pidgin)
enable_testing()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_FLAGS "-g -Wall")

add_subdirectory(src)
add_subdirectory(tests)

install(DIRECTORY ca-certs DESTINATION /usr/local/share/purple)
