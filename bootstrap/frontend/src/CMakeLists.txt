cmake_minimum_required(VERSION 2.6)

project(frontend)

set(CMAKE_CXX_STANDARD 14)

add_executable(
	frontend
		main.cpp
		Token.cpp
		Token.h
		Error.cpp
		Error.h
		TokenStream.cpp
		TokenStream.h
		Parser.cpp
		Parser.h
		Ast.h
)
