# cmake_minimum_required(VERSION <specify CMake version here>)
project(Src)

set(CMAKE_CXX_STANDARD 14)

add_executable(Src main.cpp Token.cpp Token.h Error.cpp Error.h TokenStream.cpp TokenStream.h)