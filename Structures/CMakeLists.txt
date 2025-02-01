cmake_minimum_required(VERSION 3.12)

project (
	stuctures
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib)
add_executable(structures main.cpp)
