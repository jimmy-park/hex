CPMAddPackage(
    NAME Catch2
    URL https://github.com/catchorg/Catch2/archive/v3.2.1.tar.gz
    URL_HASH SHA256=4613d3e8142b672159fcae252a4860d72c8cf8e2df5043b1ae3541db9ef5d73c
    OPTIONS
    "CATCH_INSTALL_DOCS OFF"
    "CATCH_INSTALL_EXTRAS OFF"
    "BENCHMARK_INSTALL_DOCS OFF"
)

include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)
add_executable(validate validate.cpp)
target_link_libraries(validate PRIVATE hex Catch2::Catch2WithMain)

catch_discover_tests(validate)
