cmake_minimum_required(VERSION 3.20)
project(jb_ptr_visualizer_testing)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(racont)

add_executable(jb_ptr_visualizer_testing main.cpp)
