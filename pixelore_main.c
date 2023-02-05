
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

#include <pixelore_main.h>
#include <pixelore_types.h>
#include <pixelore_window.h>
#include <pixelore_config.h>
#include <pixelore_app.h>
#include <pixelore_draw.h>

#include <stdio.h>
#include <stdlib.h>

/* Main entry of pixelore */
i32 main(i32 argc, str* argv)
{
    /* Create an window and check is everyhing gone well
     * if not return with EXIT_FAILURE which identicate
     * returning with error*/
    window_t* win = create_window(argc, argv, 1280, 720);    

    if (win == NULL) 
        return EXIT_FAILURE;

    /* Set app background to APP_BACKGROUND_COLOR that can
     * be changed in pixelore_config.h */
    window_set_background(win, APP_BACKGROUND_COLOR());
    
    /* Run the start application function */
    app_start(win);

    /* Loop until win->close is true */
    while (!win->close)
    {
        /* Inputs, clear buffer, app loop and swap the buffers */
        window_inputs(win);
        window_clear_buffer(win);         
        app_loop(win);
        window_swap_buffers(win);
    }
    
    /* Clean up everything and unallocate everything */
    window_clean_up();
}