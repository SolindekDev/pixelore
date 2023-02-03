
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

#ifndef PIXELORE_FONT_MANAGER
#define PIXELORE_FONT_MANAGER

#include <pixelore_types.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

bool init_fonts();

TTF_Font* get_default_font(u8 font_size);

void font_clean_up();

#endif