@echo off

if not exist build (
    mkdir build
    cd build
    cmake ../src


    if exist "frontend.sln" (
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
