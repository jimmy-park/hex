add_executable(validate validate.cpp)
target_link_libraries(validate PRIVATE hex)
add_test(NAME test-validate COMMAND validate CONFIGURATIONS Release)
