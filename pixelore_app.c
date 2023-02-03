
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

#include <pixelore_app.h>
#include <pixelore_main.h>
#include <pixelore_input.h>
#include <pixelore_types.h>
#include <pixelore_window.h>
#include <pixelore_config.h>
#include <pixelore_button.h>
#include <pixelore_draw.h>
#include <pixelore_scroll.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

__GLOBAL__ i16 selected_tool = 0;  
__GLOBAL__ color_t custom_color = { 0, 0, 0 };

__GLOBAL__ i32 bitmap_w = 0;
__GLOBAL__ i32 bitmap_h = 0;
__GLOBAL__ i32 scroll_bitmap_w = 0;
__GLOBAL__ i32 scroll_bitmap_h = 0;
__GLOBAL__ u8 bitmap_depth = 32;
__GLOBAL__ SDL_Surface* bitmap = NULL;

__GLOBAL__ bool is_project_opened = true;
__GLOBAL__ bool project_saved = true;
__GLOBAL__ str project_name = "defualt_project";
__GLOBAL__ vec2_t project_size = { 32, 32 };

int color_callback_button(window_t* win, button_t btn, vec2_t _unused)
{
    DEBUG_NN("Selected color: ");
    printf("%s (%d, %d, %d)\n", toolkit_colors[btn.id - 1].name, 
           toolkit_colors[btn.id - 1].r, toolkit_colors[btn.id - 1].g, toolkit_colors[btn.id - 1].b);
    custom_color.r = toolkit_colors[btn.id - 1].r;
    custom_color.g = toolkit_colors[btn.id - 1].g;
    custom_color.b = toolkit_colors[btn.id - 1].b;
    SDL_itoa(toolkit_colors[btn.id - 1].r, get_input_by_id(0)->value, 10);
    SDL_itoa(toolkit_colors[btn.id - 1].g, get_input_by_id(1)->value, 10);
    SDL_itoa(toolkit_colors[btn.id - 1].b, get_input_by_id(2)->value, 10);
    return 0;
}

int tool_select_callback(window_t* win, button_t btn, vec2_t _unused)
{
    DEBUG_NN("Selected tool: ");
    printf("%s\n", tools[btn.id - TOOLKIT_COLORS_LENGTH - 1]);
    selected_tool = btn.id - TOOLKIT_COLORS_LENGTH - 1;
    return 1;
}

void eraser(vec2_t bitmap_pos)
{
    color_t color_copy = custom_color;
    custom_color.r = 0;
    custom_color.g = 0;
    custom_color.b = 0;

    write_pixel_into_surface(bitmap_pos);
    custom_color = color_copy;
}

void write_pixel_into_surface(vec2_t bitmap_pos)
{
    u8* pixel = (u8*)bitmap->pixels;
    pixel += (bitmap_pos.y * bitmap->pitch) + (bitmap_pos.x * sizeof(u8) * 4);
    pixel[0] = custom_color.b;
    pixel[1] = custom_color.g;
    pixel[2] = custom_color.r;
}

void erase_pixel_into_surface(vec2_t bitmap_pos)
{
    u8* pixel = (u8*)bitmap->pixels;
    pixel += (bitmap_pos.y * bitmap->pitch) + (bitmap_pos.x * sizeof(u8) * 4);
    pixel[0] = 0;
    pixel[1] = 0;
    pixel[2] = 0;
}
    
void color_picker(vec2_t mouse)
{
    u8* pixel = (u8*)bitmap->pixels;
    pixel += (mouse.y * bitmap->pitch) + (mouse.x * sizeof(u8) * 4);
    
    custom_color.b = pixel[0];
    custom_color.g = pixel[1];
    custom_color.r = pixel[2];
    SDL_itoa(custom_color.r, get_input_by_id(0)->value, 10);
    SDL_itoa(custom_color.g, get_input_by_id(1)->value, 10);
    SDL_itoa(custom_color.b, get_input_by_id(2)->value, 10);

    DEBUG_NN("Picked color ");
    printf("(%d, %d, %d)\n", custom_color.r, custom_color.g, custom_color.b);
}    

void bucket(vec2_t mouse)
{
}

int bitmap_write_pixel_callback(window_t* win, button_t btn, vec2_t mouse)
{
    mouse.x -= btn.x;
    mouse.y -= btn.y;

    mouse.x /= get_scroll();
    mouse.y /= get_scroll();

    if (selected_tool == 0)
        write_pixel_into_surface(mouse);
    else if (selected_tool == 1)
        eraser(mouse);
    else if (selected_tool == 2)
        color_picker(mouse);
    else if (selected_tool == 3)
        bucket(mouse);

    return 0;
}

void value_change_red(window_t* win, input_t input)
{
    custom_color.r = SDL_atoi(input.value);
    printf("Custom color r: %d\n", custom_color.r);
}

void value_change_green(window_t* win, input_t input)
{
    custom_color.g = SDL_atoi(input.value);
    printf("Custom color g: %d\n", custom_color.g);
}

void value_change_blue(window_t* win, input_t input)
{
    custom_color.b = SDL_atoi(input.value);
    printf("Custom color b: %d\n", custom_color.b);
}

void app_start(window_t* win)
{
    bitmap_w = 32; 
    bitmap_h = 32;
    project_size.x = bitmap_w;
    project_size.y = bitmap_h;

    i32 main_container_w, main_container_h;
    main_container_w = (window_get_width(win) / 4) * 3;
    main_container_h = window_get_height(win);

    bitmap = SDL_CreateRGBSurface(0, bitmap_w, bitmap_h, (i32)bitmap_depth, 0, 0, 0, 0);
    create_button(win, main_container_w / 2 - bitmap_w / 2, main_container_h / 2 - bitmap_h / 2,
                  bitmap_w, bitmap_h, bitmap_write_pixel_callback);

    // Toolkit values
    i32 toolkit_x, toolkit_y, toolkit_padding;
    toolkit_padding = 10;
    toolkit_y = 0 + toolkit_padding;
    if (APP_TOOLKIT_ALIGNMENT == TOOLKIT_ALIGNEMT_LEFT)
        toolkit_x = 0;
    else
        toolkit_x = (window_get_width(win) / 4) * 3 + toolkit_padding;

    // Draw toolkit colors
    i32 toolkit_y_newline = 0;
    for (i32 i = 0; i < TOOLKIT_COLORS_LENGTH; i++)
    {
        if (i >= TOOLKIT_NEWLINE_COLOR)
        {
            toolkit_y_newline = 40;
            create_button(win, 10 + toolkit_x + ((i - TOOLKIT_NEWLINE_COLOR) * 40), toolkit_y + 40 + toolkit_y_newline,
                          40, 40, color_callback_button);
        }
        else
            create_button(win, 10 + toolkit_x + (i * 40), toolkit_y + 40 + toolkit_y_newline,
                          40, 40, color_callback_button);
    }

    create_button_with_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 150, 90, 35, "Pencil", tool_select_callback);
    create_button_with_text(win, toolkit_x + toolkit_padding + 100, toolkit_y + toolkit_padding + 150, 90, 35, "Eraser", tool_select_callback);
    create_button_with_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 195, 170, 35, "Color Picker", tool_select_callback);
    create_button_with_text(win, toolkit_x + toolkit_padding + 180, toolkit_y + toolkit_padding + 195, 100, 35, "Bucket", tool_select_callback);

    create_input(win, toolkit_x + toolkit_padding + (0 * 60), toolkit_y + toolkit_padding + 390, 60, 35, true, 3, "0", value_change_red);
    create_input(win, toolkit_x + toolkit_padding + (1 * 60) + 10, toolkit_y + toolkit_padding + 390, 60, 35, true, 3, "0", value_change_green);
    create_input(win, toolkit_x + toolkit_padding + (2 * 60) + 20, toolkit_y + toolkit_padding + 390, 60, 35, true, 3, "0", value_change_blue);

    create_button_with_text(win, toolkit_x + toolkit_padding, window_get_height(win) - toolkit_padding - 35, (window_get_width(win) / 4) - toolkit_padding * 4, 35, "Save", NULL);
}

void app_draw_toolkit(window_t* win)
{
    // Toolkit values
    i32 toolkit_x, toolkit_y, toolkit_padding;
    toolkit_padding = 10;
    toolkit_y = 0 + toolkit_padding;
    if (APP_TOOLKIT_ALIGNMENT == TOOLKIT_ALIGNEMT_LEFT)
        toolkit_x = 0;
    else
        toolkit_x = (window_get_width(win) / 4) * 3 + toolkit_padding;

    // Draw toolkit background
    color_t toolkit_bg_color = { APP_TOOLKIT_BACKGROUND_COLOR(), 255 };
    draw_rect(win, toolkit_x - toolkit_padding,  toolkit_y - toolkit_padding, window_get_width(win) / 4, 
            window_get_height(win), toolkit_bg_color);

    // Draw toolkit colors title
    draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding, "Color pallete");

    // Draw toolkit colors
    i32 toolkit_y_newline = 0;
    for (i32 i = 0; i < TOOLKIT_COLORS_LENGTH; i++)
    {
        if (i >= TOOLKIT_NEWLINE_COLOR)
        {
            toolkit_y_newline = 40;
            draw_rect(win, 10 + toolkit_x + ((i - TOOLKIT_NEWLINE_COLOR) * 40), toolkit_y + 40 + toolkit_y_newline, 
                      40, 40, toolkit_colors[i]);
        }
        else
        {
            draw_rect(win, 10 + toolkit_x + (i * 40), toolkit_y + 40 + toolkit_y_newline, 40, 40, toolkit_colors[i]);
        }
    }

    // Draw toolkit tools title
    draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 120, "Tools");
    // Buttons

    // Draw project info
    i32 offset = 0;

    draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 245, "Project info");
    set_font_size(12);
    if (!is_project_opened)
    {
        draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 270, "No project (CTRL + N)");
        offset = toolkit_padding + 270 + (1 * 18) + 23;
    }
    else
    {
        draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 270, "Name: %s\n", project_name);
        draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 270 + (1 * 18), "Depth: %d\n", bitmap_depth);
        draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 270 + (2 * 18), "Saved: %s\n", project_saved == true ? "Yes" : "No");
        draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 270 + (3 * 18), "Size: %dx%d\n", project_size.x, project_size.y);
        offset = toolkit_padding + 270 + (3 * 18) + 23;
    }
    set_font_size(16);

    draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 360, "Color picker");
    draw_rect(win, toolkit_x + toolkit_padding + (2 * 60) + 20 + 70, toolkit_y + toolkit_padding + 390, 45, 35, (color_t){ custom_color.r, custom_color.g, custom_color.b });
}

void app_draw_main_container(window_t* win)
{
    i32 main_container_w, main_container_h;
    main_container_w = (window_get_width(win) / 4) * 3;
    main_container_h = window_get_height(win);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(win->sdl_renderer, bitmap);
    SDL_Rect rect;

    rect.w = bitmap_w * get_scroll();
    rect.h = bitmap_h * get_scroll();

    rect.x = (main_container_w / 2 - rect.w / 2);
    rect.y = (main_container_h / 2 - rect.h / 2);
    
    scroll_bitmap_w = rect.w;
    scroll_bitmap_w = rect.w;

    SDL_RenderCopy(win->sdl_renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    resize_button(win, 0, rect.x, rect.y, rect.w, rect.h);
}
void app_loop(window_t* win)
{
    app_draw_main_container(win);
    app_draw_toolkit(win);
    button_draw_event(win);
    input_draw_event(win);
}