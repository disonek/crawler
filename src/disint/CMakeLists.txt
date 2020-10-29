add_library(disint)
target_sources(
  disint
  PUBLIC example/Hello.cpp
         example/Hello.hpp)

target_link_libraries(disint PRIVATE project_options project_warnings Threads::Threads)

target_include_directories(disint PUBLIC example)
