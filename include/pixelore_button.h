
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

#ifndef PIXELORE_BUTTON
#define PIXELORE_BUTTON

#include <pixelore_window.h>
#include <pixelore_types.h>
#include <pixelore_draw.h>

struct button_t;
typedef int (*BTN_CALLBACK)(window_t* win, struct button_t btn, vec2_t vec, bool mouse_pressed);
typedef void (*BTN_DRAW)(window_t* win, struct button_t btn);

typedef struct button_t {
    i16 initialize, x, y, w, h, id;
    bool is_draw, is_active;
    str value;
    BTN_CALLBACK btn_callback;
    BTN_DRAW btn_draw;
} button_t;

#define MAX_BUTTONS 128

button_t* get_button_by_id(i16 id);

void handle_mouse_button_event(SDL_Event event);

void handle_mouse_motion_event(SDL_Event event);

bool check_box_collision(i16 x1, i16 y1, i16 w1, i16 h1, i16 x2, i16 y2, i16 w2, i16 h2);

void create_button(window_t* win, i16 x, i16 y, i16 w, i16 h, BTN_CALLBACK btn_callback);

void resize_button(window_t* win, i32 id, i16 x, i16 y, i16 w, i16 h);

void button_draw_handle(window_t* win, struct button_t btn);

void create_button_with_text(window_t* win, i16 x, i16 y, i16 w, i16 h, str value, BTN_CALLBACK btn_callback);

void handle_mouse_event(SDL_Event event);

void button_draw_event(window_t* win);

#endif
