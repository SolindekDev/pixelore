
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

#ifndef PIXELORE_WINDOWS
#define PIXELORE_WINDOWS

#include <pixelore_types.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct window_t {
    bool close;

    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;

    u8 bg_r, bg_g, bg_b;
} window_t;

window_t* create_window(i32 argc, str* argv, i32 win_width, i32 win_height);

void window_clear_buffer(window_t* win);

void window_draw(window_t* win);

void window_inputs(window_t* win);

void window_swap_buffers(window_t* win);

void window_set_background(window_t* win, u8 r, u8 g, u8 b);

i32 window_get_width(window_t* win);

i32 window_get_height(window_t* win);

void window_clean_up();

#endif