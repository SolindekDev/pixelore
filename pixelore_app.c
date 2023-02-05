
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

/* All pixelore includes */
#include <pixelore_app.h>
#include <pixelore_main.h>
#include <pixelore_input.h>
#include <pixelore_types.h>
#include <pixelore_window.h>
#include <pixelore_config.h>
#include <pixelore_button.h>
#include <pixelore_draw.h>
#include <pixelore_scroll.h>
#include <pixelore_tools.h>

/* STD Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* SDL2 Library */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/* STB Library */
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

/* Some important global variables */

/* Colors */
__GLOBAL__ color_t custom_color = { 255, 255, 255 };

/* Bitmap info */
__GLOBAL__ i32          bitmap_w = 0;
__GLOBAL__ i32          bitmap_h = 0;
__GLOBAL__ u8           bitmap_depth = 32;

/* Why there is two bitmaps? Front bitmap 
 * is bitmap where we are writting colors
 * BGR but in the back we write as RGB because
 * idk why but surface have BGR format instead
 * of RGB, back bitmap is used only for
 * saving is as file */
__GLOBAL__ SDL_Surface* front_bitmap = NULL;
__GLOBAL__ SDL_Surface* back_bitmap  = NULL;

/* Project info */
__GLOBAL__ bool   is_project_opened = false;
__GLOBAL__ bool   project_saved = true;
__GLOBAL__ str    project_name = "default_project";
__GLOBAL__ vec2_t project_size = { 32, 32 };

__GLOBAL__ vec2_t start_line = { -1, -1 };
__GLOBAL__ vec2_t end_line = { -1, -1 };

/* Function for validating project name */
bool is_project_name_valid(str project_name)
{
    if (project_name[0] == '\0') return false;

    ITER(strlen(project_name))
        if (project_name[i] == ' ' || project_name[i] == '-') return false;

    return true;
}

/* Function for saving project */
i32 save_project_callback(window_t* win, button_t btn, vec2_t _unused, bool btn_click)
{
    /* Ignore if the button is realsed instead of pressed */
    if (btn_click == false) return 0;

    /* Check is the project name ok ok lol */
    bool project_name_valid = is_project_name_valid(project_name);
    if (project_name_valid == false)
    {
#ifdef __DEBUG
        DEBUG_NN("Project name is not valid, make sure that it's not empty and don't have any spaces or '-' characters: ");
        printf("%s\n", project_name);
#endif
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Project name is invalid", 
                                 "Project name is not valid, make sure that it's not empty and don't have any spaces or '-' characters", win->sdl_window);
        return 0;
    }

    /* Create image filename so it looks cool lol */
    str image_name_buf = malloc(512 * sizeof(char));
    snprintf(image_name_buf, 512, "./%s.png", project_name);

    /* Quick little debug thing ;) */
#ifdef __DEBUG
    DEBUG_NN("Creating file: ");
    printf("%s\n", image_name_buf);
#endif

    /* Write png to file by using stb_image_write library that i awesome! */
    i32 status = stbi_write_png(image_name_buf, bitmap_w, bitmap_h, 4, (const void*)back_bitmap->pixels, back_bitmap->pitch);
    if (status == 0)
    {
#ifdef __DEBUG
        DEBUG_NN("Something gone wrong, stbi_write_png returned: ");
        printf("%d\n", status);
#endif
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create image file", 
                                 "Something gone wrong while calling stbi_write_png", win->sdl_window);
    }

    /* Free the allocated memory for image_name_buf */
    free(image_name_buf);

    return 0;
}

/* Function for easier setting the custom color value*/
void set_custom_color_value(color_t color)
{
    custom_color.r = color.r;
    custom_color.g = color.g;
    custom_color.b = color.b;
}

/* Function for updating color picker inputs easier */
void update_color_picker_inputs(color_t color)
{
    SDL_itoa(color.r, get_input_by_id(0)->value, 10);
    SDL_itoa(color.g, get_input_by_id(1)->value, 10);
    SDL_itoa(color.b, get_input_by_id(2)->value, 10);
}

/* Function for handling color buttons */
i32 color_callback_button(window_t* win, button_t btn, vec2_t _unused, bool __unused)
{
    /* Quick little debug */
#ifdef __DEBUG
    DEBUG_NN("Selected color: ");
    printf("%s (%d, %d, %d)\n", toolkit_colors[btn.id - 1].name, 
           toolkit_colors[btn.id - 1].r, toolkit_colors[btn.id - 1].g, toolkit_colors[btn.id - 1].b);
#endif

    /* Update color values and inputs */
    set_custom_color_value(toolkit_colors[btn.id - 1]);
    update_color_picker_inputs(toolkit_colors[btn.id - 1]);

    return 0;
}

/* Function for eraser logic */
void tool_eraser(window_t* _win, vec2_t mouse_pos, bool _mouse_pressed, tool_t* _tool_structure)
{
    /* Copy the custom_color variable and set the 
     * real one to zero's */
    color_t color_copy = custom_color;
    custom_color.r = 0;
    custom_color.g = 0;
    custom_color.b = 0;

    /* Write black pixel into bitmap surface */
    write_pixel_into_surface(mouse_pos);
    custom_color = color_copy;
}

/* Function for writting an pixel with specified
 * position and color */
void put_pixel_into_surface(vec2_t bitmap_pos, color_t color)
{
    /* Write to font bitmap */

    /* Create u8 pointer of front bitmap pixels and calculate offset */
    u8* front_pixels = (u8*)front_bitmap->pixels;
    i32 front_offset = (bitmap_pos.y * front_bitmap->pitch) + (bitmap_pos.x * sizeof(u8) * 4);

    /* Set front pixels into the pointer */
    front_pixels[front_offset + 0] = color.b;
    front_pixels[front_offset + 1] = color.g;
    front_pixels[front_offset + 2] = color.r;
    front_pixels[front_offset + 3] = 255;

    /* Write to back bitmap */

    /* Create u8 pointer of back bitmap pixels and calculate offset */
    u8* back_pixels = (u8*)back_bitmap->pixels;
    i32 back_offset = (bitmap_pos.y * back_bitmap->pitch) + (bitmap_pos.x * sizeof(u8) * 4);

    /* Set back pixels into the pointer */
    back_pixels[back_offset + 0] = color.r;
    back_pixels[back_offset + 1] = color.g;
    back_pixels[back_offset + 2] = color.b;
    back_pixels[back_offset + 3] = 255;
}

/* Simple function wrapper for put_pixel_into_surface
 * that will just pass custom_color variable as pixel
 * color */
void write_pixel_into_surface(vec2_t bitmap_pos)
{
    put_pixel_into_surface(bitmap_pos, custom_color);
}

/* Simple function that will return specified 
 * color of pixel position */
color_t get_pixel_bitmap(vec2_t bitmap_pos)
{    
    /* Create u8 pointer of bitmap pixels 
     * and calculate offset */
    u8* pixel = (u8*)front_bitmap->pixels;
    i32 offset = (bitmap_pos.y * front_bitmap->pitch) + (bitmap_pos.x * sizeof(u8) * 4);

    /* Pack this pixel into color_t */
    return (color_t){ pixel[offset + 2], pixel[offset + 1], pixel[offset + 0] };
}

/* Color picker implementation function */
void tool_color_picker(window_t* _win, vec2_t mouse_pos, bool _mouse_pressed, tool_t* _tool_structure)
{
    /* Get the pixel as an color */
    color_t color_pixel = get_pixel_bitmap(mouse_pos);

    /* Update color values and inputs */
    set_custom_color_value(color_pixel);
    update_color_picker_inputs(color_pixel);

    /* Quick little debug thingi */
#ifdef __DEBUG
    DEBUG_NN("Picked color ");
    printf("(%d, %d, %d)\n", custom_color.r, custom_color.g, custom_color.b);
#endif
}    

/* TODO: create button implementation */
void tool_bucket(window_t* _win, vec2_t mouse, bool _mouse_pressed, tool_t* _tool_structure)
{
    /* Loop over all the pixels in all directions and if pixels does not 
     * equals the started pixel end looping in this direction */

    color_t start_pixel = get_pixel_bitmap(mouse);

    /* loop trough pixels from the mouse.x to bitmap_w */
    for (i32 x1 = mouse.x; x1 < bitmap_w; x1++)
    {
        if ((PIXEL_MATCH(get_pixel_bitmap((vec2_t){ x1, mouse.y }), start_pixel)) == true)
        {
            for (i32 y1 = mouse.y; y1 < bitmap_h; y1++)
            {
                if ((PIXEL_MATCH(get_pixel_bitmap((vec2_t){ x1, y1 }), start_pixel)) == true)
                    put_pixel_into_surface((vec2_t){ x1, y1 }, custom_color);
                else
                    break;
            }

            for (i32 y2 = mouse.y - 1; y2 != -1; y2--)
            {
                if ((PIXEL_MATCH(get_pixel_bitmap((vec2_t){ x1, y2 }), start_pixel)) == true)
                    put_pixel_into_surface((vec2_t){ x1, y2 }, custom_color);
                else
                    break;
            }
        }
        else
            break;
    }

    // /* loop trough pixels from the mouse.x to x cord 0 */
    for (i32 x1 = mouse.x; x1 != -1; x1--)
    {
        if ((PIXEL_MATCH(get_pixel_bitmap((vec2_t){ x1 == mouse.x ? x1 - 1 : x1, mouse.y }), start_pixel)) == true)
        {
            for (i32 y1 = mouse.y; y1 < bitmap_h; y1++)
            {
                if ((PIXEL_MATCH(get_pixel_bitmap((vec2_t){ x1, y1 }), start_pixel)) == true)
                    put_pixel_into_surface((vec2_t){ x1, y1 }, custom_color);
                else
                    break;
            }

            for (i32 y2 = mouse.y - 1; y2 != -1; y2--)
            {
                if ((PIXEL_MATCH(get_pixel_bitmap((vec2_t){ x1, y2 }), start_pixel)) == true)
                    put_pixel_into_surface((vec2_t){ x1, y2 }, custom_color);
                else
                    break;
            }
        }
        else
            break;
    }
}

void draw_line_in_bitmap(vec2_t start, vec2_t end, color_t color)
{
    i32 dx = end.x - start.x;
    i32 dy = end.y - start.y;
    i32 d = 2 * dy - dx;
    i32 y = start.y;

    for (i32 x = start.x; x <= end.x; x++) {
        put_pixel_into_surface((vec2_t){ x, y }, color);
        if (d > 0) {
            y = y + 1;
            d = d - 2 * dx;
        }
        d = d + 2 * dy;
    }
}

/* Implementation of line tool */
void tool_line(window_t* _win, vec2_t mouse_pos, bool mouse_pressed, tool_t* _tool_structure)
{
    if (mouse_pressed == 1)
    {
        if (IS_VEC_EMPTY(start_line))
        {
            start_line = mouse_pos;
            return;
        }

        end_line = mouse_pos;
    }
    else
    {
        PRINT_VEC(start_line);
        PRINT_VEC(end_line);

        draw_line_in_bitmap(start_line, end_line, custom_color);
        SET_VEC_EMPTY(start_line);
        SET_VEC_EMPTY(end_line);
    }
}

void tool_pencil(window_t* win, vec2_t mouse_pos, bool mouse_pressed, tool_t* tool_structure)
{
    write_pixel_into_surface(mouse_pos);
}

/* Function that is called always on color picker input
 * modification */
void value_change_color_picker_input(window_t* win, input_t input)
{
    /* Which input is actually modifing? */
    switch (input.id)
    {
        case 0: custom_color.r = SDL_atoi(input.value); break;
        case 1: custom_color.g = SDL_atoi(input.value); break;
        case 2: custom_color.b = SDL_atoi(input.value); break;
    }
}

/* Function for easier setting up bitmap_w & bitmap_h
 * and the project_size */
void set_default_bitmap_project_size(i32 w, i32 h)
{
    bitmap_w = w;       bitmap_h = h;
    project_size.x = w; project_size.y = h;
}

/* Function for setuping bitmap */
void setup_bitmap(window_t* win, i32 main_container_w, i32 main_container_h)
{
    front_bitmap = SDL_CreateRGBSurface(0, bitmap_w, bitmap_h, (i32)bitmap_depth, 0, 0, 0, 0);
    back_bitmap  = SDL_CreateRGBSurface(0, bitmap_w, bitmap_h, (i32)bitmap_depth, 0, 0, 0, 0);
    create_button(win, main_container_w / 2 - bitmap_w / 2, main_container_h / 2 - bitmap_h / 2,
                  bitmap_w, bitmap_h, bitmap_tool_callback, true);
}

/* Function for creating toolkit colors*/
void create_toolkit_colors(window_t* win, i32 toolkit_x, i32 toolkit_y)
{
    i32 toolkit_y_newline = 0;
    for (i32 i = 0; i < TOOLKIT_COLORS_LENGTH; i++)
    {
        if (i >= TOOLKIT_NEWLINE_COLOR)
        {
            toolkit_y_newline = 40;
            create_button(win, 10 + toolkit_x + ((i - TOOLKIT_NEWLINE_COLOR) * 40), toolkit_y + 40 + toolkit_y_newline, 40, 40, color_callback_button, false);
        }
        else
            create_button(win, 10 + toolkit_x + (i * 40), toolkit_y + 40 + toolkit_y_newline,40, 40, color_callback_button, false);
    }
}

/* Function for creating tool buttons*/
void create_button_tools(window_t* win, i32 toolkit_x, i32 toolkit_y, i32 toolkit_padding)
{
    create_button_with_text(win, toolkit_x + toolkit_padding,       toolkit_y + toolkit_padding + 150, 90,  35, "Pencil",       tool_selection_callback, false);
    create_button_with_text(win, toolkit_x + toolkit_padding + 100, toolkit_y + toolkit_padding + 150, 90,  35, "Eraser",       tool_selection_callback, false);
    create_button_with_text(win, toolkit_x + toolkit_padding + 200, toolkit_y + toolkit_padding + 150, 70,  35, "Line",         tool_selection_callback, false);
    create_button_with_text(win, toolkit_x + toolkit_padding,       toolkit_y + toolkit_padding + 195, 170, 35, "Color Picker", tool_selection_callback, false);
    create_button_with_text(win, toolkit_x + toolkit_padding + 180, toolkit_y + toolkit_padding + 195, 100, 35, "Bucket",       tool_selection_callback, false);
    create_tool(win, "Pencil",       tool_pencil);
    create_tool(win, "Eraser",       tool_eraser);
    create_tool(win, "Line",         tool_line);
    create_tool(win, "Color Picker", tool_color_picker);
    create_tool(win, "Bucket",       tool_bucket);
}

/* Function for creating input pickers */
void create_color_input_pickers(window_t* win, i32 toolkit_x, i32 toolkit_y, i32 toolkit_padding)
{
    char buf[32];
    SDL_itoa(custom_color.r, buf, 10);
    create_input(win, toolkit_x + toolkit_padding + (0 * 60),      toolkit_y + toolkit_padding + 390, 60, 35, true, 3, buf, value_change_color_picker_input);

    SDL_itoa(custom_color.g, buf, 10);
    create_input(win, toolkit_x + toolkit_padding + (1 * 60) + 10, toolkit_y + toolkit_padding + 390, 60, 35, true, 3, buf, value_change_color_picker_input);

    SDL_itoa(custom_color.b, buf, 10);
    create_input(win, toolkit_x + toolkit_padding + (2 * 60) + 20, toolkit_y + toolkit_padding + 390, 60, 35, true, 3, buf, value_change_color_picker_input);
}

/* Function that will create some other essential buttons*/
void create_other_buttons(window_t* win, i32 toolkit_x, i32 toolkit_y, i32 toolkit_padding)
{
    create_button_with_text(win, toolkit_x + toolkit_padding, window_get_height(win) - toolkit_padding - 35, 
                            (window_get_width(win) / 4) - toolkit_padding * 4, 35, "Save", save_project_callback, false);
}

/* Project name input callback function */
void project_name_input_callback(window_t* win, input_t input)
{
    /* Just update project_name global variable */
    project_name = input.value;
}

/* Function for creating project name input */
void create_project_name_input(window_t* win, i32 toolkit_x, i32 toolkit_y, i32 toolkit_padding)
{
    create_input(win, toolkit_x + toolkit_padding, window_get_height(win) - toolkit_padding - 35 * 2 - 10, 
                 (window_get_width(win) / 4) - toolkit_padding * 4, 35, false, 100, project_name, project_name_input_callback);
}

/* Function that is called on start */
void app_start(window_t* win)
{
    /* Setup some bitmap things and variables */
    set_default_bitmap_project_size(32, 32);

    SETUP_MAIN_CONTAINER_VARIABLES();
    setup_bitmap(win, main_container_w, main_container_h);

    /* Setup toolkit variables, SETUP_TOOLKIT_VARIBLES macro
     * is an simple macro that just calculate important
     * toolkit variables then create toolkit colors */
    SETUP_TOOLKIT_VARIABLES();
    create_toolkit_colors(win, toolkit_x, toolkit_y);

    /* Create button tools */
    create_button_tools(win, toolkit_x, toolkit_y, toolkit_padding);

    /* Create color input pickers */
    create_color_input_pickers(win, toolkit_x, toolkit_y, toolkit_padding);

    /* Create other buttons */
    create_other_buttons(win, toolkit_x, toolkit_y, toolkit_padding);

    /* Create project name input */
    create_project_name_input(win, toolkit_x, toolkit_y, toolkit_padding);
}

/* Draw toolkit background */
void draw_toolkit_background(window_t* win, i32 toolkit_x, i32 toolkit_y, i32 toolkit_padding)
{
    draw_rect(win, toolkit_x - toolkit_padding, 
              toolkit_y - toolkit_padding, window_get_width(win) / 4, 
              window_get_height(win), (color_t){ APP_TOOLKIT_BACKGROUND_COLOR(), 255 });
}

/* Draw toolkit colors */
void draw_toolkit_colors(window_t* win, i32 toolkit_x, i32 toolkit_y, i32 toolkit_padding)
{
    i32 toolkit_y_newline = 0;
    for (i32 i = 0; i < TOOLKIT_COLORS_LENGTH; i++)
    {
        if (i >= TOOLKIT_NEWLINE_COLOR)
        {
            toolkit_y_newline = 40;
            draw_rect(win, 10 + toolkit_x + ((i - TOOLKIT_NEWLINE_COLOR) * 40), toolkit_y + 40 + toolkit_y_newline, 40, 40, toolkit_colors[i]);
        }
        else
            draw_rect(win, 10 + toolkit_x + (i * 40), toolkit_y + 40 + toolkit_y_newline, 40, 40, toolkit_colors[i]);
    }
}

void draw_toolkit_project_info(window_t* win, i32 toolkit_x, i32 toolkit_y, i32 toolkit_padding)
{
    set_font_size(12);

    draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 270, "Name: %s\n", project_name);
    draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 270 + (1 * 18), "Depth: %d\n", bitmap_depth);
    draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 270 + (2 * 18), "Saved: %s\n", project_saved == true ? "Yes" : "No");
    draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 270 + (3 * 18), "Size: %dx%d\n", project_size.x, project_size.y);

    set_font_size(16);
}

/* Function for drawing toolkit */
void app_draw_toolkit(window_t* win)
{
    /* Setup some variables */
    SETUP_MAIN_CONTAINER_VARIABLES();
    SETUP_TOOLKIT_VARIABLES();

    /* Draw toolkit base */
    draw_toolkit_background(win, toolkit_x, toolkit_y, toolkit_padding);
    draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding, "Color pallete");
    draw_toolkit_colors(win, toolkit_x, toolkit_y, toolkit_padding);

    /* Draw toolkit titles and projet info */
    draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 120, "Tools");
    draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 245, "Project info");
    draw_toolkit_project_info(win, toolkit_x, toolkit_y, toolkit_padding);
    draw_text(win, toolkit_x + toolkit_padding, toolkit_y + toolkit_padding + 360, "Color picker");
    draw_rect(win, toolkit_x + toolkit_padding + (2 * 60) + 20 + 70, toolkit_y + toolkit_padding + 390, 45, 35, (color_t){ custom_color.r, custom_color.g, custom_color.b });
}

/* Function for drawing main container*/
void app_draw_main_container(window_t* win)
{
    /* Setup main container variables */
    SETUP_MAIN_CONTAINER_VARIABLES();

    /* Setup texture and rect and then render it and destroy the texture
     * also resize the bitmap*/
    SDL_Texture* texture = SDL_CreateTextureFromSurface(win->sdl_renderer, front_bitmap);
    SDL_Rect rect = { };
    rect.w = bitmap_w * get_scroll();
    rect.h = bitmap_h * get_scroll();
    rect.x = (main_container_w / 2 - rect.w / 2);
    rect.y = (main_container_h / 2 - rect.h / 2);

    SDL_RenderCopy(win->sdl_renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    resize_button(win, 0, rect.x, rect.y, rect.w, rect.h);

    if (!(start_line.x == -1 || start_line.y == -1))
    {
        SDL_SetRenderDrawColor(win->sdl_renderer, custom_color.r, custom_color.g, custom_color.b, custom_color.a);
        SDL_RenderDrawLine(win->sdl_renderer, rect.x + (start_line.x * get_scroll()), rect.y + (start_line.y * get_scroll()), 
                        rect.x + (end_line.x * get_scroll()), rect.y + (end_line.y * get_scroll()));
    }
}

/* Function that is called in for loop */
void app_loop(window_t* win)
{
    /* Draw main container and toolkit */
    app_draw_main_container(win);
    app_draw_toolkit(win);

    /* Draw buttons and inputs */
    button_draw_event(win);
    input_draw_event(win);
}