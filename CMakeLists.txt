project(orchid)
set(CMAKE_CXX_STANDARD 20)
include_directories(include/)
file(GLOB_RECURSE orchid_SRC src/*.cpp)
list(APPEND orchid_SRC examples/http_server/main.cpp)
add_executable(orchid ${orchid_SRC})

set_target_properties(orchid PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../build)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")