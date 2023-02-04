
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

#include <pixelore_input.h>
#include <pixelore_types.h>
#include <pixelore_button.h>
#include <pixelore_config.h>
#include <pixelore_draw.h>
#include <pixelore_app.h>

#include <SDL2/SDL.h>

__GLOBAL__ i16 inputs_len = 0;
__GLOBAL__ input_t inputs[MAX_INPUTS];
__GLOBAL__ window_t* window;

__GLOBAL__ bool mouse_click = false;

char* append(const char* str, char c)
{
    long len = strlen(str);
    char* res = malloc(len + 2);
    strcpy(res, str);
    res[len + 0] = c;
    res[len + 1] = '\0';
    return res;
}

input_t* get_input_by_id(i16 id)
{
    return &inputs[id];
}

void input_draw_default(window_t* win, input_t input)
{
    color_t input_background = { INPUT_BACKGROUND_COLOR(), 255 };
    color_t input_outline    = { 0, 0, 0, 255 };

    if (input.is_focus)
        input_outline = (color_t){ INPUT_BORDER_ACTIVE_COLOR(), 255 };
    else
        input_outline = (color_t){ INPUT_BORDER_COLOR(), 255 };

    draw_rect(win, input.x, input.y, input.w, input.h, input_background);
    draw_rect_outline(win, input.x, input.y, input.w, input.h, input_outline);

    vec2_t text_vec2 = get_size_text(win, 0, 0, input.value);
    i16 text_x = input.x + 10;
    i16 text_y = input.y + (input.h / 2 - text_vec2.y / 2);

    draw_text(win, text_x, text_y, input.value);
}

void handle_mouse_input_event(SDL_Event event)
{
    if (event.button.button != SDL_BUTTON_LEFT)     return;
    if (event.button.type   != SDL_MOUSEBUTTONDOWN) return;

    for (i32 i = 0; i < inputs_len; i++)
    {
        if (check_box_collision(inputs[i].x, inputs[i].y, inputs[i].w, inputs[i].h, event.button.x, event.button.y, 2, 2))
        {
            SDL_StartTextInput();
            for (i32 i = 0; i < inputs_len; i++)
                inputs[i].is_focus = false;
            inputs[i].is_focus = true;
            break;
        }

        if (inputs[i].is_focus == true)
            SDL_StopTextInput();
        inputs[i].is_focus = false;
    }
}

void handle_backspace_input_event(SDL_Event event)
{    
    for (i32 i = 0; i < inputs_len; i++)
        if (inputs[i].is_focus == true)
            if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
            {
                inputs[i].value[strlen(inputs[i].value) - 1] = '\0';
                inputs[i].input_callback(window, inputs[i]);
            }
}

void handle_keyboard_input_event(SDL_Event event)
{
    for (i32 i = 0; i < inputs_len; i++)
        if (inputs[i].is_focus == true)
        {
            if (strlen(inputs[i].value) == inputs[i].max_len)
                return;
            inputs[i].value = append(inputs[i].value, event.text.text[0]);
            inputs[i].input_callback(window, inputs[i]);
        }
}

void input_draw_event(window_t* win)
{
    for (i32 i = 0; i < inputs_len; i++)
        if (inputs[i].is_draw == true)
            inputs[i].input_draw(win, inputs[i]);
}

void create_input(window_t* win, i16 x, i16 y, i16 w, i16 h, bool only_numbers, i32 max_len, str start_value, INPUT_CALLBACK input_callback)
{
    window = win;
    int inputs_len_ = inputs_len;
    input_t input = { 1, x, y, w, h, inputs_len_, true, false, NULL, input_callback, input_draw_default, max_len };
    input.value = malloc(sizeof(char) * max_len);
    inputs[inputs_len] = input;
    inputs_len++;

    strcpy(inputs[inputs_len - 1].value, start_value);

#ifdef __DEBUG
    DEBUG_NN("Successfully registered input with id [");
    printf("%d]\n", inputs[inputs_len - 1].id);
#endif
}