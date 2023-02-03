
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
#include <pixelore_window.h>
#include <pixelore_draw.h>
#include <pixelore_button.h>
#include <pixelore_input.h>
#include <pixelore_app.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>

u32 bitmap[64 * 64] = { 255 };

window_t* create_window(i32 argc, str* argv, i32 win_width, i32 win_height)
{
    // Initialize SDL2 library
    i32 status = SDL_Init(SDL_INIT_EVERYTHING);
    if (status != 0)
    {
        printf("Couldn't initialize SDL");
        return NULL;
    }
    
    // Initialize SDL2 image library
    status = IMG_Init(IMG_INIT_PNG);
    if (status == 0)
    {
        printf("Couldn't initialize SDL Image");
        return NULL;
    }

    // Initialize SDL2 ttf library
    status = TTF_Init();
    if (status != 0)
    {
        printf("Couldn't initialize SDL ttf");
        return NULL;
    }

    // Initialize fonts
    bool bstatus = init_fonts();
    if (bstatus == false)
    {
        printf("Couldn't initialize fonts");
        return NULL;
    }

    // Create window_t structure (that will be returned) and fill 
    // fields of it
    window_t* window = calloc(1, sizeof(window_t));
    window->sdl_window = SDL_CreateWindow("Pixelore", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_width, win_height, 0);
    window->sdl_renderer = SDL_CreateRenderer(window->sdl_window, -1, SDL_RENDERER_ACCELERATED);

    window->close = false;

    return window;
}

void window_clear_buffer(window_t* win)
{
    if (win == NULL) return;

    SDL_SetRenderDrawColor(win->sdl_renderer, win->bg_r, win->bg_g, win->bg_b, 255);
    SDL_RenderClear(win->sdl_renderer);
}

void window_draw(window_t* win)
{
    if (win == NULL) return;

}

void window_clean_up(window_t* win)
{
    font_clean_up();
    SDL_DestroyRenderer(win->sdl_renderer);
    SDL_DestroyWindow(win->sdl_window);
    SDL_Quit();
}

void window_inputs(window_t* win)
{
    if (win == NULL) return;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type)
        {
            case SDL_QUIT:
                win->close = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                handle_mouse_button_event(event);
                handle_mouse_input_event(event);
                break;
            case SDL_MOUSEBUTTONUP:
                handle_mouse_button_event(event);
                break;
            case SDL_MOUSEMOTION:
                handle_mouse_motion_event(event);
                break;
            case SDL_MOUSEWHEEL:
                handle_mouse_whell(event);
                break;
            case SDL_KEYDOWN:
                handle_backspace_input_event(event);
                break;
            case SDL_TEXTINPUT:
                handle_keyboard_input_event(event);
                break;
        }
    }
}

void window_swap_buffers(window_t* win)
{
    if (win == NULL) return;

    SDL_RenderPresent(win->sdl_renderer);
}

void window_set_background(window_t* win, u8 r, u8 g, u8 b)
{
    win->bg_r = r; 
    win->bg_g = g; 
    win->bg_b = b;
}

i32 window_get_width(window_t* win)
{
    i32 width = 0;
    SDL_GetWindowSize(win->sdl_window, &width, NULL);

    return width;
}

i32 window_get_height(window_t* win)
{
    i32 height = 0;
    SDL_GetWindowSize(win->sdl_window, NULL, &height);

    return height;
}
