
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

#ifndef PIXELORE_APP
#define PIXELORE_APP

#include <pixelore_window.h>

#include <stdlib.h>

#define __GLOBAL__ static
#define __EXTERN__ extern
#define EXIT(status) exit(status);

void handle_mouse_whell(SDL_Event event);

void app_start(window_t* win);

void app_loop(window_t* win);

#endif
