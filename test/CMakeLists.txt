add_executable(tests)
target_sources(tests PRIVATE example/HelloTest.cpp utils/Utils.hpp)

target_include_directories(tests PRIVATE example utils)

find_package(GTest REQUIRED)
target_link_libraries(
  tests
  PRIVATE project_options
          project_warnings
          GTest::gtest_main
          GTest::gmock_main
          GTest::gmock
          GTest::gtest
          Threads::Threads)

add_test(NAME tests COMMAND tests)

add_custom_command(
  TARGET tests
  POST_BUILD
  COMMAND ctest -C Debug -T test --output-on-failuret)
