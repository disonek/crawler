find_package(spdlog REQUIRED)
find_package(cpr REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(EnTT REQUIRED)


add_library(webcrawl
  Crawler.cpp
  Crawler.hpp
  WebCurl.cpp
  WebCurl.hpp
  debug_tools/Timer.hpp
  debug_tools/Trace.hpp
  debug_tools/TraceTimer.hpp
  debug_tools/ScopedTimer.hpp
  debug_tools/ScopedTimer.cpp
)

target_include_directories(webcrawl 
  PUBLIC . debug_tools)

target_link_libraries(
  webcrawl
  PUBLIC project_options 
         spdlog::spdlog
         Threads::Threads
         cxxopts::cxxopts
         cpr::cpr
         LibXml2::LibXml2
         EnTT::EnTT
         )
