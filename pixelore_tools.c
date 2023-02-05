
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
#include <pixelore_tools.h>
#include <pixelore_app.h>
#include <pixelore_main.h>
#include <pixelore_input.h>
#include <pixelore_types.h>
#include <pixelore_window.h>
#include <pixelore_config.h>
#include <pixelore_button.h>
#include <pixelore_draw.h>
#include <pixelore_scroll.h>

/* STD Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* SDL2 Library */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

__GLOBAL__ i16 selected_tool = 0;

__GLOBAL__ tool_t tools_list[MAX_TOOLS];
__GLOBAL__ i16 tools_len = 0;

/* Function for creating tools */
void create_tool(window_t* win, str tool_name, TOOL_CALLBACK tool_func)
{
    /* Copy tools len and append our new tool to the tools array */
    tool_t tool = { tool_name, tool_func, win, tools_len };
    tools_list[tools_len] = tool;
    tools_len++;

#ifdef __DEBUG
    DEBUG_NN("Successfully registered tool with name [");
    printf("%s] and id [%d]\n", tools_list[tools_len - 1].tool_name
                              , tools_list[tools_len - 1].tool_id);
#endif
}

/* Function for getting tools by their name */
tool_t* get_tool_by_name(str tool_name)
{
    /* Iterate by the tools_list array and if an element tool_name
     * equals tool_name return this element */
    ITER(tools_len)
        if (tools_list[i].tool_name == tool_name) return &tools_list[i];

    return NULL;
}

/* Function for getting tools by their id */
tool_t* get_tool_by_id(i16 tool_id)
{
    /* Iterate by the tools_list array and if an element tool_id
     * equals tool_id return this element */
    ITER(tools_len)
        if (tools_list[i].tool_id == tool_id) return &tools_list[i];

    return NULL;
}

/* Function that is called when any tools is selected */
i32 tool_selection_callback(window_t* win, button_t btn, vec2_t _unused, bool __unused)
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

/* This function return selected tool, just an easy wrapper */
i16 get_selected_tool()
{
    return selected_tool;
}

/* This function is called when user is clicking on the 
 * bitmap */
i32 bitmap_tool_callback(window_t* win, button_t btn, vec2_t mouse, bool button_press)
{
    /* Calculate the bitmap position from the mouse position lol
     * Just substract mouse.x with btn.x and in the other way then
     * get the scroll number and divide it by mouse.x and mouse.y*/
    mouse.x = ((mouse.x - btn.x) / get_scroll());
    mouse.y = ((mouse.y - btn.y) / get_scroll());

    /* Get the selected tool by id, and then call the
     * tool_func tool function */
    tool_t* selected_tool = get_tool_by_id(get_selected_tool());
    selected_tool->tool_func(win, mouse, button_press, 
                             selected_tool);

    /* Recognize which tool is actually selected
     * and call the main function of it */
//     switch (get_selected_tool())
//     {
//         case 0:  write_pixel_into_surface(mouse);       break;
//         case 1:  eraser(mouse);                         break;
//         case 2:  tool_line(win, mouse, button_press);   break;
//         case 3:  color_picker(mouse);                   break;
//         case 4:  bucket(mouse, custom_color);           break;   
// #ifdef __DEBUG
//         default: DEBUG("Unimplemented tool");           break;
// #else  
//         default:                                        break;
// #endif
//     }

    return 0;
}