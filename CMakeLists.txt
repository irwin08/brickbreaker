cmake_minimum_required(VERSION 3.15)
project(BrickBreaker)

set(CMAKE_CXX_STANDARD "20")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(BrickBreaker src/main.cpp)

target_link_directories(BrickBreaker PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(BrickBreaker sfml-graphics sfml-system sfml-network sfml-audio sfml-window)
target_include_directories(BrickBreaker PUBLIC ${CMAKE_SOURCE_DIR}/include)

