
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

/* Some important global variables */

/* Tools & colors */
__GLOBAL__ i16 selected_tool = 0;  
__GLOBAL__ color_t custom_color = { 255, 255, 255 };

/* Bitmap info */
__GLOBAL__ i32          bitmap_w = 0;
__GLOBAL__ i32          bitmap_h = 0;
__GLOBAL__ u8           bitmap_depth = 32;
__GLOBAL__ SDL_Surface* bitmap = NULL;

/* Project info */
__GLOBAL__ bool   is_project_opened = false;
__GLOBAL__ bool   project_saved = true;
__GLOBAL__ str    project_name = "defualt_project";
__GLOBAL__ vec2_t project_size = { 32, 32 };

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
i32 color_callback_button(window_t* win, button_t btn, vec2_t _unused)
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

/* Function for handling tool selection buttons */
i32 tool_select_callback(window_t* win, button_t btn, vec2_t _unused)
{
    /* Quick little debug */
#ifdef __DEBUG
    DEBUG_NN("Selected tool: ");
    printf("%s\n", tools[btn.id - TOOLKIT_COLORS_LENGTH - 1]);
#endif

    /* Set selecteed tool to button id - TOOLKIT_COLORS_LENGTH - 1
     * It's not that complicated math lmao */
    selected_tool = btn.id - TOOLKIT_COLORS_LENGTH - 1;
    return 1;
}

/* Function for eraser logic */
void eraser(vec2_t bitmap_pos)
{
    /* Copy the custom_color variable and set the 
     * real one to zero's */
    color_t color_copy = custom_color;
    custom_color.r = 0;
    custom_color.g = 0;
    custom_color.b = 0;

    /* Write black pixel into bitmap surface */
    write_pixel_into_surface(bitmap_pos);
    custom_color = color_copy;
}

/* Function for writting an pixel with specified
 * position and color */
void put_pixel_into_surface(vec2_t bitmap_pos, color_t color)
{
    /* Create u8 pointer of bitmap pixels 
     * and calculate offset */
    u8* pixel = (u8*)bitmap->pixels;
    i32 offset = (bitmap_pos.y * bitmap->pitch) + (bitmap_pos.x * sizeof(u8) * 4);

    /* Set pixels into the pointer */
    pixel[offset + 0] = color.b;
    pixel[offset + 1] = color.g;
    pixel[offset + 2] = color.r;
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
    u8* pixel = (u8*)bitmap->pixels;
    i32 offset = (bitmap_pos.y * bitmap->pitch) + (bitmap_pos.x * sizeof(u8) * 4);

    /* Pack this pixel into color_t */
    return (color_t){ pixel[offset + 2], pixel[offset + 1], pixel[offset + 0] };
}

/* Color picker implementation function */
void color_picker(vec2_t mouse)
{
    /* Get the pixel as an color */
    color_t color_pixel = get_pixel_bitmap(mouse);

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
void bucket(vec2_t mouse, color_t bucket_color)
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
                    put_pixel_into_surface((vec2_t){ x1, y1 }, bucket_color);
                else
                    break;
            }

            for (i32 y2 = mouse.y - 1; y2 != -1; y2--)
            {
                if ((PIXEL_MATCH(get_pixel_bitmap((vec2_t){ x1, y2 }), start_pixel)) == true)
                    put_pixel_into_surface((vec2_t){ x1, y2 }, bucket_color);
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
                    put_pixel_into_surface((vec2_t){ x1, y1 }, bucket_color);
                else
                    break;
            }

            for (i32 y2 = mouse.y - 1; y2 != -1; y2--)
            {
                if ((PIXEL_MATCH(get_pixel_bitmap((vec2_t){ x1, y2 }), start_pixel)) == true)
                    put_pixel_into_surface((vec2_t){ x1, y2 }, bucket_color);
                else
                    break;
            }
        }
        else
            break;
    }

   
    
}

/* This function is called when user is clicking on the 
 * bitmap */
i32 bitmap_write_pixel_callback(window_t* win, button_t btn, vec2_t mouse)
{
    /* Calculate the bitmap position from the mouse position lol
     * Just substract mouse.x with btn.x and in the other way then
     * get the scroll number and divide it by mouse.x and mouse.y*/
    mouse.x = ((mouse.x - btn.x) / get_scroll());
    mouse.y = ((mouse.y - btn.y) / get_scroll());

    /* Recognize which tool is actually selected
     * and call the main function of it */
    switch (selected_tool)
    {
        case 0:  write_pixel_into_surface(mouse); break;
        case 1:  eraser(mouse);                   break;
        case 2:  color_picker(mouse);             break;
        case 3:  bucket(mouse, custom_color);     break;   
#ifdef __DEBUG
        default: DEBUG("Unimplemented tool");     break;
#else  
        default:                                  break;
#endif
    }

    return 0;
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
    bitmap = SDL_CreateRGBSurface(0, bitmap_w, bitmap_h, (i32)bitmap_depth, 0, 0, 0, 0);
    create_button(win, main_container_w / 2 - bitmap_w / 2, main_container_h / 2 - bitmap_h / 2,
                  bitmap_w, bitmap_h, bitmap_write_pixel_callback);
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
            create_button(win, 10 + toolkit_x + ((i - TOOLKIT_NEWLINE_COLOR) * 40), toolkit_y + 40 + toolkit_y_newline, 40, 40, color_callback_button);
        }
        else
            create_button(win, 10 + toolkit_x + (i * 40), toolkit_y + 40 + toolkit_y_newline,40, 40, color_callback_button);
    }
}

/* Function for creating tool buttons*/
void create_button_tools(window_t* win, i32 toolkit_x, i32 toolkit_y, i32 toolkit_padding)
{
    create_button_with_text(win, toolkit_x + toolkit_padding,       toolkit_y + toolkit_padding + 150, 90,  35, "Pencil",       tool_select_callback);
    create_button_with_text(win, toolkit_x + toolkit_padding + 100, toolkit_y + toolkit_padding + 150, 90,  35, "Eraser",       tool_select_callback);
    create_button_with_text(win, toolkit_x + toolkit_padding,       toolkit_y + toolkit_padding + 195, 170, 35, "Color Picker", tool_select_callback);
    create_button_with_text(win, toolkit_x + toolkit_padding + 180, toolkit_y + toolkit_padding + 195, 100, 35, "Bucket",       tool_select_callback);
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
    create_button_with_text(win, toolkit_x + toolkit_padding, window_get_height(win) - toolkit_padding - 35, (window_get_width(win) / 4) - toolkit_padding * 4, 35, "Save", NULL);
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
    SDL_Texture* texture = SDL_CreateTextureFromSurface(win->sdl_renderer, bitmap);
    SDL_Rect rect = { };
    rect.w = bitmap_w * get_scroll();
    rect.h = bitmap_h * get_scroll();
    rect.x = (main_container_w / 2 - rect.w / 2);
    rect.y = (main_container_h / 2 - rect.h / 2);

    SDL_RenderCopy(win->sdl_renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    resize_button(win, 0, rect.x, rect.y, rect.w, rect.h);
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