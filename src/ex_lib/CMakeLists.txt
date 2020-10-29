add_library(ex_lib)
target_sources(
  ex_lib
  PUBLIC example/Hello.cpp
         example/Hello.hpp)

target_link_libraries(ex_lib PRIVATE project_options project_warnings Threads::Threads)

target_include_directories(ex_lib PUBLIC example)
