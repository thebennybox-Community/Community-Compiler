#!/usr/bin/env bash

dotnet run -t ./test/test.il

cd test

nasm -f elf -o test.o test.asm 
gcc  -m32 -o main.o -c test.c

gcc -m32 -lGL -lGLU -lglut -o test main.o test.o 

./test

cd ..