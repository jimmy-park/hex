CPMAddPackage(
    NAME benchmark
    URL https://github.com/google/benchmark/archive/v1.7.1.tar.gz
    URL_HASH SHA256=6430e4092653380d9dc4ccb45a1e2dc9259d581f4866dc0759713126056bc1d7
    OPTIONS
    "BENCHMARK_ENABLE_TESTING OFF"
    "BENCHMARK_ENABLE_INSTALL OFF"
    "BENCHMARK_INSTALL_DOCS OFF"
)

add_executable(perf performance.cpp)
target_link_libraries(perf PRIVATE hex benchmark::benchmark)

if(HEX_RUN_BENCHMARKS)
    add_custom_command(TARGET perf POST_BUILD
        COMMAND $<$<CONFIG:Release>:$<TARGET_FILE:perf>>
    )
endif()
