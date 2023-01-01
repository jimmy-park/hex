cmake_minimum_required(VERSION 3.25)

project(
    hex
    VERSION 0.1.0
    DESCRIPTION "Performance comparison of dec-to-hex implementations"
    HOMEPAGE_URL "https://github.com/jimmy-park/hex"
    LANGUAGES CXX
)

option(HEX_BENCHMARK_AFTER_BUILD "Run benchmark program after build" OFF)

include(CTest)
include(cmake/GetCPM.cmake)

CPMAddPackage(
    NAME fmt
    URL https://github.com/fmtlib/fmt/archive/9.1.0.tar.gz
)
CPMAddPackage(
    NAME benchmark
    URL https://github.com/google/benchmark/archive/v1.7.1.tar.gz
    OPTIONS
    "BENCHMARK_ENABLE_TESTING OFF"
    "BENCHMARK_ENABLE_INSTALL OFF"
    "BENCHMARK_INSTALL_DOCS OFF"
)

add_library(hex INTERFACE)
target_link_libraries(hex INTERFACE fmt::fmt)
target_compile_features(hex INTERFACE cxx_std_23)
target_sources(hex INTERFACE
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/hex.hpp
)

add_executable(perf benchmark/performance.cpp)
target_link_libraries(perf PRIVATE hex benchmark::benchmark)

if(HEX_BENCHMARK_AFTER_BUILD)
    add_custom_command(TARGET perf POST_BUILD
        COMMAND $<$<CONFIG:Release>:$<TARGET_FILE:perf>>
    )
endif()

add_executable(validate test/validate.cpp)
target_link_libraries(validate PRIVATE hex)
add_test(NAME test-validate COMMAND validate)