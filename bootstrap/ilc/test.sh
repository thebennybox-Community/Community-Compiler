#!/usr/bin/env bash

dotnet run -t ./test/test.il

cd test

nasm -f elf -o test.o test.asm 
gcc -m32 -lGL -lGLU -lglut -o test test.o 

./test

cd ..