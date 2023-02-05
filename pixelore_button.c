
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

#include <pixelore_button.h>
#include <pixelore_types.h>
#include <pixelore_config.h>
#include <pixelore_draw.h>
#include <pixelore_app.h>

#include <SDL2/SDL.h>

__GLOBAL__ i16 buttons_len = 0;
__GLOBAL__ button_t buttons[MAX_BUTTONS];
__GLOBAL__ window_t* window;

__GLOBAL__ bool mouse_click = false;

void create_button(window_t* win, i16 x, i16 y, i16 w, i16 h, BTN_CALLBACK btn_callback, bool is_motion)
{
    window = win;
    i32 buttons_len_ = buttons_len;
    button_t button = { 1, x, y, w, h, buttons_len_, false, false, is_motion, NULL, btn_callback, NULL, };
    buttons[buttons_len] = button;
    buttons_len++;

#ifdef __DEBUG
    DEBUG_NN("Successfully registered button with id [");
    printf("%d]\n", buttons[buttons_len - 1].id);
#endif
}

void resize_button(window_t* win, i32 id, i16 x, i16 y, i16 w, i16 h)
{
    buttons[id].x = x;
    buttons[id].y = y;
    buttons[id].w = w;
    buttons[id].h = h;
}

void create_button_with_text(window_t* win, i16 x, i16 y, i16 w, i16 h, str value, BTN_CALLBACK btn_callback, bool is_motion)
{
    window = win;
    i32 buttons_len_ = buttons_len;
    button_t button = { 1, x, y, w, h, buttons_len_, true, false, is_motion, value, btn_callback, button_draw_handle };
    buttons[buttons_len] = button;
    buttons_len++;

#ifdef __DEBUG
    DEBUG_NN("Successfully registered button with id [");
    printf("%d]\n", buttons[buttons_len - 1].id);
#endif
}

bool check_box_collision(i16 x1, i16 y1, i16 w1, i16 h1, i16 x2, i16 y2, i16 w2, i16 h2) {
    return x1 + w1 > x2 && x1 < x2 + w2 && y1 + h1 > y2 && y1 < y2 + h2;
}

void button_draw_event(window_t* win)
{
    for (i32 i = 0; i < buttons_len; i++)
        if (buttons[i].is_draw == true)
            buttons[i].btn_draw(win, buttons[i]);
}

button_t* get_button_by_id(i16 id)
{
    return &buttons[id];
}

void button_draw_handle(window_t* win, struct button_t btn)
{
    color_t btn_background = { BUTTON_BACKGROUND_COLOR(), 255 };
    color_t btn_outline;
    if (btn.is_active)  
        btn_outline = (color_t){ INPUT_BORDER_ACTIVE_COLOR(), 255 };
    else
        btn_outline = (color_t){ BUTTON_BORDER_COLOR(), 255 };
    draw_rect(win, btn.x, btn.y, btn.w, btn.h, btn_background);
    draw_rect_outline(win, btn.x, btn.y, btn.w, btn.h, btn_outline);

    vec2_t text_vec2 = get_size_text(win, 0, 0, btn.value);
    i16 text_x = btn.x + (btn.w / 2 - text_vec2.x / 2);
    i16 text_y = btn.y + (btn.h / 2 - text_vec2.y / 2);

    draw_text(win, text_x, text_y, btn.value);
}

void handle_mouse_button_event(SDL_Event event)
{
    if (event.button.button != SDL_BUTTON_LEFT) return;

    if (event.button.type == SDL_MOUSEBUTTONDOWN) 
        mouse_click = true;
    else
        mouse_click = false;

    for (i32 i = 0; i < buttons_len; i++)
    {
        button_t current_button = buttons[i];
        if (check_box_collision(current_button.x, current_button.y, current_button.w, current_button.h, event.button.x, event.button.y, 2, 2))
        {
            if (current_button.btn_callback != NULL)
            {
                vec2_t mouse_vec = { event.button.x, event.button.y };
                for (i32 i = 0; i < buttons_len; i++)
                    if (buttons[i].btn_draw != NULL)
                        buttons[i].is_active = false;
                buttons[i].is_active = current_button.btn_callback(window, current_button, mouse_vec, mouse_click);
            }
            break;
        }
    }
}

void handle_mouse_motion_event(SDL_Event event)
{
    if (mouse_click != true) return;

    for (i32 i = 0; i < buttons_len; i++)
    {
        button_t current_button = buttons[i];
        
        if (current_button.is_motion == false) break;

        if (check_box_collision(current_button.x, current_button.y, current_button.w, current_button.h, event.button.x, event.button.y, 2, 2))
        {
            if (current_button.btn_callback != NULL)
            {
                vec2_t mouse_vec = { event.button.x, event.button.y };
                current_button.btn_callback(window, current_button, mouse_vec, mouse_click);
            }
            break;
        }
    }
}