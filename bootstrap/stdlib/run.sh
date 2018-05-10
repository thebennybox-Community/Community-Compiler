#!/bin/bash

for filename in *.ds; do
   files="$files ./$filename"
done

rm -r bin
mkdir bin

cd ../ilc/
gradle task build
cd ../stdlib/

unzip -d bin ../ilc/build/distributions/duskilc-0.1.zip

clear

../frontend/build/frontend ./bin/out.fil ${files}

./bin/duskilc-0.1/bin/duskilc -v --no-optimization -o ./bin/test.il -e flat_text -p flat_bin ./bin/out.fil
./bin/duskilc-0.1/bin/duskilc -v --no-optimization -o ./bin/test.asm -e nasm -p flat_bin ./bin/out.fil
nasm -f elf -o ./bin/test.o ./bin/test.asm
gcc -m32 -lGL -lGLU -lglut -o stdlib ./bin/test.o
#clear
./stdlib
