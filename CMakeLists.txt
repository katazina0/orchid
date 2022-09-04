cmake_minimum_required(VERSION 3.24)
include_directories(include/)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")



project(orchid)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../lib)
    file(GLOB_RECURSE orchid_SRC src/*.cpp)
    add_library(orchid STATIC ${orchid_SRC})



project(orchid_example)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build)
    add_executable(orchid_example examples/http_server/main.cpp)
    set_target_properties(orchid_example PROPERTIES OUTPUT_NAME orchid)
    target_link_libraries(orchid_example orchid)