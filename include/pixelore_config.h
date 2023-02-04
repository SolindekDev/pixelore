
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

#ifndef PIXELORE_CONFIG
#define PIXELORE_CONFIG

#include <pixelore_draw.h>

/*  This file is for you! the users of pixelore if you really want an own theme
    of our app modifie the values below! */

/* Colors values are in format R (red), G (green), B (blue) */

/* Color of the app background */
#define APP_BACKGROUND_COLOR() 39, 41, 43 

/* Color of the app font color */
#define APP_FONT_COLOR() 232, 235, 237

/* Color of button background color */
#define BUTTON_BACKGROUND_COLOR() 26, 30, 34

/* Color of button border */
#define BUTTON_BORDER_COLOR() 70, 111, 179

/* Color of input background color */
#define INPUT_BACKGROUND_COLOR() 26, 30, 34

/* Color of input border */
#define INPUT_BORDER_COLOR() 70, 111, 179

/* Color of input border */
#define INPUT_BORDER_ACTIVE_COLOR() 50, 81, 133

/* Default font of app */
#define APP_FONT_PATH() "./assets/fonts/MartianMono-Regular.ttf"

/* Don't change these values please start */
#define TOOLKIT_ALIGNEMT_LEFT 1
#define TOOLKIT_ALIGNEMT_RIGHT 2

/* Don't change these values zone end */

/* Color of the app toolkit background */
#define APP_TOOLKIT_BACKGROUND_COLOR() 32, 35, 38 

/* Toolkit alignment (left - 1, right - 2)*/
#define APP_TOOLKIT_ALIGNMENT TOOLKIT_ALIGNEMT_RIGHT

/* Toolkit colors */
#define TOOLKIT_COLORS_LENGTH 11

#define TOOLKIT_NEWLINE_COLOR 7

static color_t toolkit_colors[] = { 
    { 255, 255, 255, 255, "White"  },  /* 1.  White    */ 
    { 255, 000, 000, 255, "Red"    },  /* 2.  Red    */ 
    { 000, 255, 000, 255, "Green"  },  /* 3.  Green  */ 
    { 000, 000, 255, 255, "Blue"   },  /* 4.  Blue   */ 
    {  85, 000, 128, 255, "Purple" },  /* 5.  Purple */ 
    { 000, 255, 255, 255, "Aqua"   },  /* 6.  Aqua   */ 
    { 164, 245, 137, 255, "Lime"   },  /* 7.  Lime   */
    { 255, 136, 000, 255, "Orange" },  /* 8.  Orange */
    { 255, 000, 217, 255, "Pink"   },  /* 9.  Pink   */
    { 251, 255, 000, 255, "Yellow" },  /* 10. Yellow */
    { 000, 000, 000, 255, "Black"  },  /* 11. Black */
};

static str tools[5] = {
    [0] = "Pencil",
    [1] = "Eraser",
    [2] = "Line",
    [3] = "Color picker",
    [4] = "Bucket",
};

#endif