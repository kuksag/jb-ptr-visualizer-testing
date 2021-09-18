cmake_minimum_required(VERSION 3.20)
project(racont_test)

set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Default build type from CMakeLists.txt" FORCE)  # Add debug information
endif (NOT CMAKE_BUILD_TYPE)

add_compile_options(-Wall -Wextra -Werror -O2)
if (UNIX AND NOT CMAKE_CXX_FLAGS)
    add_compile_options(-fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
endif (UNIX AND NOT CMAKE_CXX_FLAGS)

add_executable(racont-test tests.cpp)
