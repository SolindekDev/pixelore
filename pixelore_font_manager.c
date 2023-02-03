
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

#include <pixelore_font_manager.h>
#include <pixelore_config.h>
#include <pixelore_types.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

TTF_Font* default_font_16;
TTF_Font* default_font_12;
TTF_Font* default_font_24;

bool init_fonts()
{
    default_font_12 = TTF_OpenFont(APP_FONT_PATH(), 12);
    default_font_16 = TTF_OpenFont(APP_FONT_PATH(), 16);
    default_font_24 = TTF_OpenFont(APP_FONT_PATH(), 24);

    if (default_font_12 == NULL || default_font_16 == NULL || default_font_24 == NULL)
    {
        printf("Couldn't initalize fonts");
        return false;
    }
    
    return true;
}

TTF_Font* get_default_font(u8 font_size) 
{
    switch (font_size)
    {
        case 12: return default_font_12;
        case 16: return default_font_16;
        case 24: return default_font_24;
    }

    return NULL;
}

void font_clean_up()
{
    TTF_CloseFont(default_font_12);
    TTF_CloseFont(default_font_16);
    TTF_CloseFont(default_font_24);
}