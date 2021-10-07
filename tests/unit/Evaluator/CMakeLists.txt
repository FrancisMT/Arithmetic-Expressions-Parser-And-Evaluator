add_executable(ut_Evaluator ut_Evaluator.cpp)
target_link_libraries(ut_Evaluator evaluator GTest::GTest GTest::Main)
gtest_discover_tests(ut_Evaluator)
