add_executable(TinyChartTest UnitTest.cpp TinyChartTest.cpp BaseTestCase.cpp)
target_link_libraries(TinyChartTest tinyutil gd gtest)

add_executable(UnitTest UnitTest.cpp server_test.cpp TinyUtilTest.cpp  TinyConfigTest.cpp)
target_link_libraries(UnitTest tinyutil gtest)

add_executable(waltertest waltertest.cpp)
target_link_libraries(waltertest tinyutil)