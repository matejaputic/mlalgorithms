cmake_minimum_required (VERSION 2.8.11)
project (mlalgorithms)

find_package(Boost)

# For apple, let's use the brew installed g++-6
if(APPLE)
	SET(CMAKE_CXX_COMPILER "/usr/local/bin/g++-6")
	MESSAGE( STATUS "You're an apple, we're using clang++!")

# For everyone else, let's use g++-5
else()
	SET(CMAKE_CXX_COMPILER "/usr/bin/g++-5")
	MESSAGE( STATUS "You're not an apple, so we're going to use g++-5")
endif()

# SET(CMAKE_CXX_FLAGS "-O3 -std=c++14 -fno-exceptions -fno-rtti -Wall -Werror -fno-omit-frame-pointer")
SET(CMAKE_CXX_FLAGS "-O3 -ggdb -std=c++14 -fno-rtti -Wall -Werror -Wno-misleading-indentation -fno-omit-frame-pointer -march=native -fopenmp")

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS}})

# Install to bin directory
#set(EXECUTABLE_OUTPUT_PATH "../bin")
add_executable(boost_tokenizer src/boost_tokenizer.cpp)
