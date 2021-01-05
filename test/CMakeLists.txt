add_executable(disint_tests)
target_sources(disint_tests PRIVATE webcrawl/HighLevelTests.cpp)

target_include_directories(disint_tests PRIVATE example)

find_package(GTest REQUIRED)
target_link_libraries(
  disint_tests
  PRIVATE project_options
          project_warnings
          GTest::gtest_main
          GTest::gmock_main
          GTest::gmock
          GTest::gtest
          Threads::Threads
          webcrawl
          )

find_package(benchmark REQUIRED)
add_executable(bench)
target_sources(bench PRIVATE "benchmark/BasicCrawl.cpp")

target_link_libraries(bench
  PRIVATE project_options
          project_warnings
          benchmark::benchmark
          Threads::Threads
          webcrawl)



add_test(NAME disint_tests COMMAND disint_tests)

add_custom_command(
  TARGET disint_tests
  POST_BUILD
  COMMAND ctest -C Debug -T test --output-on-failuret)
