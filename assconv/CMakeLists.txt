cmake_minimum_required(VERSION 3.9)
project(assconv)

set(CMAKE_CXX_STANDARD 17)

include_directories("C:/libs/include")

add_executable(assconv main.cpp)

target_link_libraries(assconv -lstdc++fs)
