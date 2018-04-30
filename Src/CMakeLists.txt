cmake_minimum_required(VERSION 2.6)

project(compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(
	compiler
		main.cpp
		Token.cpp
		Token.h
		Error.cpp
		Error.h
		TokenStream.cpp
		TokenStream.h
)
