cmake_minimum_required(VERSION 3.14)
project(endian-test VERSION 0.1 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# build with c++20 standard
set(CMAKE_CXX_STANDARD 11)

set(PROJECT_SOURCES
    src/main.c)

add_executable(big-endian-test ${PROJECT_SOURCES})
