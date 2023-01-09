if(HEX_TEST_FRAMEWORK STREQUAL "doctest")
    CPMAddPackage(
        NAME doctest
        URL https://github.com/doctest/doctest/archive/v2.4.9.tar.gz
        URL_HASH SHA256=19b2df757f2f3703a5e63cee553d85596875f06d91a3333acd80a969ef210856
    )

    include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
    add_executable(validate validate_doctest.cpp)
    target_link_libraries(validate PRIVATE hex doctest::doctest_with_main)
    target_compile_definitions(validate PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS)

    doctest_discover_tests(validate)
elseif(HEX_TEST_FRAMEWORK STREQUAL "catch")
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
    add_executable(validate validate_catch.cpp)
    target_link_libraries(validate PRIVATE hex Catch2::Catch2WithMain)

    catch_discover_tests(validate)
endif()
