add_executable(perf performance.cpp)
target_link_libraries(perf PRIVATE hex benchmark::benchmark)

if(HEX_BENCHMARK_AFTER_BUILD)
    add_custom_command(TARGET perf POST_BUILD
        COMMAND $<$<CONFIG:Release>:$<TARGET_FILE:perf>>
    )
endif()
