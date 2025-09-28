cmake_minimum_required(VERSION 4.0)
project(Pong)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL3 REQUIRED)

add_executable(Pong main.cpp)

target_sources(Pong PRIVATE main.cpp)
target_link_libraries(Pong SDL3::SDL3)
