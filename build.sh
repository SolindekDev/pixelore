#!/bin/bash

C_SOURCES=`find ./ -name "*.c"`

mkdir -p build

for c_file in $C_SOURCES
do
    echo gcc -c -O3 -std=c18 -Wall -I/usr/include/SDL2 -Iinclude $c_file -o ./build/$c_file.o
    gcc -c -O3 -std=c18 -Wall -Wno-unused-variable -I/usr/include/SDL2 -Iinclude $c_file -o ./build/$c_file.o
done

OBJECTS=`find ./build -name "*.c.o"`
echo gcc $OBJECTS -lSDL2 -lSDL2_ttf -o ./pixelore
gcc $OBJECTS -lSDL2 -lSDL2_ttf -lSDL2_image -o ./pixelore