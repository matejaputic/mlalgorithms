cmake_minimum_required (VERSION 2.8.11)
project (mlalgorithms)

SET(CMAKE_CXX_COMPILER "/usr/bin/g++-5")
SET(CMAKE_CXX_FLAGS "-O3 -std=c++14 -fno-exceptions -fno-rtti -Wall -Werror -fno-omit-frame-pointer")

include_directories(/if21/mp3t/.local/include)
link_directories(/if21/mp3t/.local/lib)

add_executable(helloworld src/helloworld.cpp)
target_link_libraries(helloworld benchmark)
target_link_libraries(helloworld pthread)
