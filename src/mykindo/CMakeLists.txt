add_library(mykindo)
target_sources(
  mykindo
  PUBLIC example/Hello.cpp
         example/Hello.hpp)

target_link_libraries(mykindo PRIVATE project_options project_warnings Threads::Threads)

target_include_directories(mykindo PUBLIC example)
