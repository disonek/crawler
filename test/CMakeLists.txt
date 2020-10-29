add_executable(tests)
target_sources(
  tests
  PRIVATE example/HelloTest.cpp
          utils/Utils.hpp)

target_include_directories(tests PRIVATE example utils)

target_link_libraries(
  tests
  PRIVATE project_options
          project_warnings
          CONAN_PKG::gtest
          Threads::Threads
          mykindo)

add_test(NAME tests COMMAND tests)

add_custom_command(
  TARGET tests
  POST_BUILD
  COMMAND ctest -C Debug -T test --output-on-failuret)
