
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

#include <pixelore_draw.h>
#include <pixelore_window.h>
#include <pixelore_font_manager.h>
#include <pixelore_types.h>
#include <pixelore_config.h>
#include <pixelore_app.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>

__GLOBAL__ i32 font_size = 16;

void set_font_size(i32 size)
{
    font_size = size;
}

vec2_t draw_text(window_t* win, i32 x, i32 y, str format, ...)
{
    va_list arg;
    va_start(arg, format);
    vec2_t vec2 = draw_text_va(win, x, y, format, arg);
    va_end(arg);

    return vec2;
}

vec2_t draw_text_va(window_t* win, i32 x, i32 y, str format, va_list va)
{
    i8 buffer[2048];
    vsprintf(buffer, format, va);

    SDL_Color colorfg = { APP_FONT_COLOR() };
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(get_default_font(font_size), buffer, colorfg, window_get_width(win) - x);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(win->sdl_renderer, surface);

    i32 font_w = 0;
    i32 font_h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &font_w, &font_h);
    SDL_Rect rect = { x, y, font_w, font_h };
    SDL_RenderCopy(win->sdl_renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    vec2_t vec2 = { font_w, font_h };
    return vec2;
}

vec2_t get_size_text(window_t* win, i32 x, i32 y, str format, ...)
{
    va_list arg;
    va_start(arg, format);
    vec2_t vec2 = get_size_text_va(win, x, y, format, arg);
    va_end(arg);

    return vec2;
}

vec2_t get_size_text_va(window_t* win, i32 x, i32 y, str format, va_list va)
{
    i8 buffer[2048];
    vsprintf(buffer, format, va);

    SDL_Color colorfg = { APP_FONT_COLOR() };
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(get_default_font(font_size), buffer, colorfg, window_get_width(win) - x);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(win->sdl_renderer, surface);

    i32 font_w = 0;
    i32 font_h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &font_w, &font_h);
    SDL_Rect rect = { x, y, font_w, font_h };
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    vec2_t vec2 = { font_w, font_h };
    return vec2;
}

SDL_Texture* load_image(window_t* win, str path)
{
    SDL_Surface* surface = IMG_Load(path);
    if (surface == NULL)
    {
        printf("Unable to open %s", path);
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(win->sdl_renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

void draw_image(window_t* win, i32 x, i32 y, SDL_Texture* image)
{
    i32 image_w = 0;
    i32 image_h = 0;
    SDL_QueryTexture(image, NULL, NULL, &image_w, &image_h);
    SDL_Rect rect = { x, y, image_w, image_h };
    SDL_RenderCopy(win->sdl_renderer, image, NULL, &rect);
}

void draw_image_resize(window_t* win, i32 x, i32 y, i32 image_w, i32 image_h, SDL_Texture* image)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = image_w;
    rect.h = image_h;
    SDL_RenderCopy(win->sdl_renderer, image, NULL, &rect);
}

void draw_rect(window_t* win, i32 x, i32 y, i32 w, i32 h, color_t color)
{
    SDL_SetRenderDrawColor(win->sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = { x, y, w, h };
    SDL_RenderFillRect(win->sdl_renderer, &rect);
}

void draw_rect_outline(window_t* win, i32 x, i32 y, i32 w, i32 h, color_t color)
{
    SDL_SetRenderDrawColor(win->sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = { x, y, w, h };
    SDL_RenderDrawRect(win->sdl_renderer, &rect);
}
