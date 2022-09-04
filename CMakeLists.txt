project(orchid)

set(CMAKE_CXX_STANDARD 20)

include_directories(include/)

file(GLOB_RECURSE orchid_SRC "src/*.cpp")
add_executable(orchid ${orchid_SRC})

add_executable(orchid examples/basic_server/main.cpp)