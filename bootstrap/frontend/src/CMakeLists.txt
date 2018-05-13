cmake_minimum_required(VERSION 2.6)

project(frontend)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
    add_definitions(/W2)
endif()

add_executable(
	frontend
		main.cpp
		Token.h
		Error.h
        Terminal.h
		TokenStream.cpp
		TokenStream.h
		Parser.cpp
		Parser.h
		Ast.cpp
		Ast.h
        AstDefs.h
		AstPrettyPrinter.cpp
		AstPrettyPrinter.h
		Semantics.cpp
		Semantics.h
		CodeGen.cpp
		CodeGen.h
		ILemitter.cpp
		ILemitter.h)
