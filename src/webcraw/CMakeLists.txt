find_package(CURL REQUIRED)
find_package(spdlog REQUIRED)
find_package(MySQL REQUIRED)
find_package(gumbo-parser REQUIRED)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(webcrawl ${SRC} ${HEADERS})

install(TARGETS webcrawl DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

target_link_libraries(
  webcrawl
  PUBLIC project_options 
         CURL::CURL
         MySQL::MySQL
         spdlog::spdlog
         Threads::Threads
         gumbo-parser::gumbo-parser
         cxxopts)
