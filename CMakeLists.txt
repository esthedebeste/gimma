cmake_minimum_required(VERSION 4.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(gimma)

add_subdirectory(deps/simple-2d)
file(GLOB_RECURSE gimmasrc src/*.cpp)
add_executable(gimma ${gimmasrc})
target_link_libraries(gimma PRIVATE simple-2d)