cmake_minimum_required(VERSION 3.26)
project(Pong)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 REQUIRED)
include_directories("C:\\msys64\\mingw64\\include")
link_directories("C:\\msys64\\mingw64\\lib")



add_executable(Pong main.cpp)

target_sources(Pong
PRIVATE
    main.cpp
)

target_link_libraries(Pong SDL3::SDL3)