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
        AstDefs.h
		Ast.h
        Terminal.h
        ICodeGenerator.h
        ISemanticAnalysis.h
        ISemanticGenerator.h
        ScopeContext.h
		ScopeContext.cpp
        Scope.h
        DuskAssembly.cpp
        DuskAssembly.h
		TokenStream.cpp
		TokenStream.h
		Parser.cpp
		Parser.h
		AstPrettyPrinter.cpp
		AstPrettyPrinter.h
		ILemitter.cpp
		ILemitter.h
		CodeGen.cpp
		SemanticGenerator.cpp
		SemanticsAnalysis.cpp)
