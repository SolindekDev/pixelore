
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

#ifndef PIXELORE_INPUT
#define PIXELORE_INPUT

#include <pixelore_window.h>
#include <pixelore_types.h>
#include <pixelore_draw.h>

struct input_t;
typedef void (*INPUT_CALLBACK)(window_t* win, struct input_t input);
typedef void (*INPUT_DRAW)(window_t* win, struct input_t input);

typedef struct input_t {
    i16 initialize, x, y, w, h, id;
    bool is_draw;
    bool is_focus;
    str value;
    INPUT_CALLBACK input_callback;
    INPUT_DRAW input_draw;
    u8 max_len;
} input_t;

#define MAX_INPUTS 128

void create_input(window_t* win, i16 x, i16 y, i16 w, i16 h, bool only_numbers, i32 max_len, str start_value, INPUT_CALLBACK input_callback);

void input_draw_event(window_t* win);

void handle_keyboard_input_event(SDL_Event event);

void handle_backspace_input_event(SDL_Event event);

void handle_mouse_input_event(SDL_Event event);

input_t* get_input_by_id(i16 id);

#endif