add_executable(ut_Parser ut_Parser.cpp)
target_link_libraries(ut_Parser parser gtest_main)
gtest_discover_tests(ut_Parser)
