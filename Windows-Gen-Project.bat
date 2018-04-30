@echo off

if not exist build (
    mkdir build
    cd build
    cmake ../src

    rem If src.sln was found, it's likely that the project was created successfully.
    if exist "compiler.sln" (
    	color a
    	echo Finished generating project.
    ) else (
    	color c
    	echo Error generating project.
    )

) else (
	color e
    echo build folder already exists.
)

PAUSE
