#!/bin/bash

C_SOURCES=`find ./ -name "*.c"`

mkdir -p build

for c_file in $C_SOURCES
do
    echo gcc -c -O3 -std=c18 -Wall -D __DEBUG -Wno-unused-variable $(sdl2-config --cflags) -Iinclude $c_file -o ./build/$c_file.o
    gcc -c -O3 -std=c18 -Wall -D __DEBUG -Wno-unused-variable $(sdl2-config --cflags) -Iinclude $c_file -o ./build/$c_file.o
done

OBJECTS=`find ./build -name "*.c.o"`
echo gcc $OBJECTS $(sdl2-config --libs) -lSDL2_ttf -lSDL2_image -lm -o ./pixelore
gcc $OBJECTS $(sdl2-config --libs) -lSDL2_ttf -lSDL2_image -lm -o ./pixelore
