cmake_minimum_required(VERSION 3.25)

project(
    hex
    VERSION 0.1.0
    DESCRIPTION "Performance comparison of dec-to-hex implementations"
    HOMEPAGE_URL "https://github.com/jimmy-park/hex"
    LANGUAGES CXX
)

option(HEX_BUILD_BENCHMARKS "Build benchmark program" OFF)
option(HEX_BUILD_TESTS "Build test program" OFF)
option(HEX_RUN_BENCHMARKS "Run benchmark program after build" OFF)

if(NOT HEX_TEST_FRAMEWORK)
    set(HEX_TEST_FRAMEWORK doctest)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/MP)
endif()

include(cmake/GetCPM.cmake)

CPMAddPackage(
    NAME fmt
    URL https://github.com/fmtlib/fmt/archive/9.1.0.tar.gz
    URL_HASH SHA256=5dea48d1fcddc3ec571ce2058e13910a0d4a6bab4cc09a809d8b1dd1c88ae6f2
)

add_library(hex INTERFACE)
add_library(hex::hex ALIAS hex)
target_link_libraries(hex INTERFACE fmt::fmt)
target_compile_features(hex INTERFACE cxx_std_23)
target_sources(hex INTERFACE
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/hex.hpp
)

if(HEX_BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

if(HEX_BUILD_TESTS)
    include(CTest)
    add_subdirectory(test)
endif()
