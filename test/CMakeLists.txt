add_executable(ex_tests)
target_sources(
  ex_tests
  PRIVATE example/HelloTest.cpp
          utils/Utils.hpp)

target_include_directories(ex_tests PRIVATE example utils)

target_link_libraries(
  ex_tests
  PRIVATE project_options
          project_warnings
          CONAN_PKG::gtest
          Threads::Threads
          # CONAN_PKG::boost
          ex_lib)

add_test(NAME ex_tests COMMAND ex_tests)

add_custom_command(
  TARGET ex_tests
  POST_BUILD
  COMMAND ctest -C Debug -T test --output-on-failuret)
