#!/bin/bash

mkdir -p bin

if [ ! -d bin/duskilc-0.1 ]; then
	cd ../ilc/
	gradle task build
	cd ../stdlib/

	unzip -d bin ../ilc/build/distributions/duskilc-0.1.zip
	clear
fi

../frontend/build/frontend ./bin/out.fil *.ds

./bin/duskilc-0.1/bin/duskilc -v --no-optimization -o ./bin/test.il -e text -p bin ./bin/out.fil
./bin/duskilc-0.1/bin/duskilc -v --no-optimization -o ./bin/test.asm -e nasm -p bin ./bin/out.fil
nasm -f elf -o ./bin/test.o ./bin/test.asm
gcc -m32 -lGL -lGLU -lglut -o stdlib ./bin/test.o
#clear
./stdlib
