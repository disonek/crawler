add_executable(tests)
target_sources(tests PRIVATE example/HelloTest.cpp)

target_include_directories(tests PRIVATE example)

find_package(GTest REQUIRED)
target_link_libraries(
  tests
  PRIVATE project_options
          project_warnings
          GTest::gtest_main
          GTest::gmock_main
          GTest::gmock
          GTest::gtest
          Threads::Threads
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



add_test(NAME tests COMMAND tests)

add_custom_command(
  TARGET tests
  POST_BUILD
  COMMAND ctest -C Debug -T test --output-on-failuret)
