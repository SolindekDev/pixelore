
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

#ifndef PIXELORE_DRAW
#define PIXELORE_DRAW

#include <pixelore_types.h>
#include <pixelore_window.h>

#include <stdarg.h>

#define IMAGE SDL_Texture

typedef struct color_t { 
    u8 r, g, b, a; 
    str name;
} color_t;

typedef struct hsv_color_t {
    double h, s, v;       
} hsv_color_t;

typedef struct vec2_t {
    i16 x, y;
} vec2_t;

void set_font_size(i32 size);

vec2_t draw_text(window_t* win, i32 x, i32 y, str format, ...);

vec2_t draw_text_va(window_t* win, i32 x, i32 y, str format, va_list va);

vec2_t get_size_text(window_t* win, i32 x, i32 y, str format, ...);

vec2_t get_size_text_va(window_t* win, i32 x, i32 y, str format, va_list va);

void draw_image(window_t* win, i32 x, i32 y, SDL_Texture* image);

void draw_image_resize(window_t* win, i32 x, i32 y, i32 image_w, i32 image_h, SDL_Texture* image);

void draw_rect(window_t* win, i32 x, i32 y, i32 w, i32 h, color_t color);

void draw_rect_outline(window_t* win, i32 x, i32 y, i32 w, i32 h, color_t color);

SDL_Texture* load_image(window_t* win, str path);

void draw_gradient(window_t* win, i32 x, i32 y, i32 w, i32 h, color_t start_color, color_t end_color);

void draw_hsv_gradient(window_t* win, i32 x, i32 y, i32 w, i32 h);

#endif