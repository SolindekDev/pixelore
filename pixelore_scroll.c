
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

#include <pixelore_scroll.h>
#include <pixelore_main.h>
#include <pixelore_types.h>
#include <pixelore_window.h>
#include <pixelore_config.h>
#include <pixelore_app.h>
#include <pixelore_draw.h>

#include <SDL2/SDL.h>

__GLOBAL__ i32 project_scroll = 1;

/* Function for handling mouse whell event */
void handle_mouse_whell(SDL_Event event)
{
    project_scroll += event.wheel.y;
    
    if (project_scroll == 0)
        project_scroll = 1;

    if (project_scroll == 25)
        project_scroll = 24;
}

/* Function that return project_scroll variable */
i32 get_scroll()
{
    return project_scroll;
}