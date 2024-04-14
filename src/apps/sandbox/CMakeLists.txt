add_executable(sandbox)

target_sources(sandbox PRIVATE main.cpp)

target_link_libraries(sandbox PRIVATE webcrawl)
