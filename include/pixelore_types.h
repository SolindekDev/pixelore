
/*
    Pixelore Easy and light pixel art editor
    Copyright (C) 2022-2023 SolindekDev

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef PIXELORE_TYPES
#define PIXELORE_TYPES

#include <stdbool.h>
#include <stdio.h>

#define DEBUG(info) printf("\e[1;36m%s\e[0;0m: %s\n", __FILE__, info)
#define DEBUG_NN(info) printf("\e[1;36m%s\e[0;0m: %s", __FILE__, info)

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;

typedef char  i8;
typedef short i16;
typedef int   i32;
typedef long  i64;

typedef float  f32;
typedef double f64;

typedef char* str;

#endif