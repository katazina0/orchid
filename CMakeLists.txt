project(orchid)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(orchid examples/basic_server/main.cpp)