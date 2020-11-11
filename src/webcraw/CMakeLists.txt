find_package(spdlog REQUIRED)
find_package(cpr REQUIRED)
find_package(LibXml2 REQUIRED)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(webcrawl ${SRC} ${HEADERS})

install(TARGETS webcrawl DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)


target_link_libraries(
  webcrawl
  PUBLIC project_options 
         spdlog::spdlog
         Threads::Threads
         cxxopts::cxxopts
         cpr::cpr
         LibXml2::LibXml2
         )
