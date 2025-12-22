find_package(GTest REQUIRED)

add_executable(unit_tests main.cpp)
target_link_libraries(unit_tests PRIVATE GTest::gtest_main foundation quant_core)

include(GoogleTest)
gtest_discover_tests(unit_tests)
