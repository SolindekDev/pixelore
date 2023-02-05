
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

#ifndef PIXELORE_TOOLS
#define PIXELORE_TOOLS

#include <pixelore_types.h>
#include <pixelore_window.h>
#include <pixelore_button.h>
#include <pixelore_draw.h>

struct tool_t;
typedef void (*TOOL_CALLBACK)(window_t* win, vec2_t mouse_pos, bool mouse_pressed, struct tool_t* tool_structure);

typedef struct tool_t {
    str tool_name;
    TOOL_CALLBACK tool_func;
    window_t* tool_win;
    i16 tool_id;
} tool_t;

#define MAX_TOOLS 128

void create_tool(window_t* win, str tool_name, TOOL_CALLBACK tool_func);

tool_t* get_tool_by_name(str tool_name);

tool_t* get_tool_by_id(i16 tool_id);

i32 tool_selection_callback(window_t* win, button_t btn, vec2_t _unused, bool __unused);

i16 get_selected_tool();

i32 bitmap_tool_callback(window_t* win, button_t btn, vec2_t mouse, bool button_press);

#endif