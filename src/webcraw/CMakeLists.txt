find_package(spdlog REQUIRED)
find_package(cpr REQUIRED)
find_package(LibXml2 REQUIRED)


add_library(webcrawl
  Crawler.cpp
  Crawler.hpp
  Timer.hpp
  Trace.hpp
  WebCurl.cpp
  WebCurl.hpp)

target_include_directories(webcrawl PUBLIC .)

target_link_libraries(
  webcrawl
  PUBLIC project_options 
         spdlog::spdlog
         Threads::Threads
         cxxopts::cxxopts
         cpr::cpr
         LibXml2::LibXml2
         )
