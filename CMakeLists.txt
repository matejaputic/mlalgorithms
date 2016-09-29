cmake_minimum_required (VERSION 2.8.11)
project (mlalgorithms)

find_package(Boost)

SET(CMAKE_CXX_COMPILER "/usr/bin/g++-5")
# SET(CMAKE_CXX_FLAGS "-O3 -std=c++14 -fno-exceptions -fno-rtti -Wall -Werror -fno-omit-frame-pointer")
SET(CMAKE_CXX_FLAGS "-O3 -std=c++14 -fno-rtti -Wall -Werror -fno-omit-frame-pointer")

include_directories(/if21/mp3t/.local/include)
link_directories(/if21/mp3t/.local/lib)

include_directories(${Boost_INCLUDE_DIRS}})
add_executable(boost_tokenizer src/boost_tokenizer.cpp)
target_link_libraries(boost_tokenizer ${Boost_LIBRARIES})

add_executable(helloworld src/helloworld.cpp)
target_link_libraries(helloworld benchmark)
target_link_libraries(helloworld pthread)
