#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "oled_drv.h"
#include "fonts.h"

void ui_index(void)
{
    char temp[] = {0};
    char number = 0;
    
    // draw battery
    oled_drv_goto_xy(0, 0);
    oled_drv_draw_icon2(1, &icon_battery_24x16, OLED_COLOR_WHITE);

    // display temperature value
    oled_drv_goto_xy(27, 5);
    number = 88;
    sprintf(temp, "%3d", number);
    oled_drv_puts(temp, &font_7x10, OLED_COLOR_WHITE);

    // draw celsius
    oled_drv_goto_xy(49, 0);
    oled_drv_draw_icon(0, &icon_celsius_16x16, OLED_COLOR_WHITE);

    // draw fan
    oled_drv_goto_xy(75, 0);
    oled_drv_draw_icon(0, &icon_fan_16x16, OLED_COLOR_WHITE);
    oled_drv_goto_xy(75, 0);
    oled_drv_draw_icon(1, &icon_fan_16x16, OLED_COLOR_WHITE);
    
    // draw signal
    oled_drv_goto_xy(103, 0);
    oled_drv_draw_icon2(0, &icon_signal_24x16, OLED_COLOR_WHITE);

    // draw line
    oled_drv_draw_line(0, 18, 127, 18, OLED_COLOR_WHITE);
    
    // display channel "CH"
    oled_drv_goto_xy(5, 30);
    oled_drv_puts("CH", &font_7x10, OLED_COLOR_WHITE);
    
    // display channel number
    oled_drv_goto_xy(30, 23);
    number = 5;
    sprintf(temp, "%d", number);
    oled_drv_put_ch_numbers(temp, &font_40x16, OLED_COLOR_WHITE);

    // display "VIDEO"
    oled_drv_goto_xy(70, 30);
    oled_drv_puts("VIDEO", &font_7x10, OLED_COLOR_WHITE);

    // display VIDEO FORMAT
    oled_drv_goto_xy(70, 50);
    oled_drv_puts("1080P60", &font_7x10, OLED_COLOR_WHITE);

    oled_drv_update_screen();

    //sleep(5);

#if 1
    while(1)
    {
        oled_drv_goto_xy(75, 0);
        oled_drv_draw_icon(0, &icon_fan_16x16, OLED_COLOR_WHITE);
        oled_drv_update_screen();
        usleep(20000);
        oled_drv_goto_xy(75, 0);
        oled_drv_draw_icon(1, &icon_fan_16x16, OLED_COLOR_WHITE);
        oled_drv_update_screen();
        usleep(20000);
    }
#endif
    while(1)
    {
#if 1
        oled_drv_goto_xy(30, 23);
        oled_drv_put_ch_numbers("0", &font_40x16, OLED_COLOR_WHITE);
        oled_drv_update_screen();
        usleep(200);
        oled_drv_goto_xy(30, 23);
        oled_drv_put_ch_numbers("1", &font_40x16, OLED_COLOR_WHITE);
        oled_drv_update_screen();
        usleep(200);
        oled_drv_goto_xy(30, 23);
        oled_drv_put_ch_numbers("2", &font_40x16, OLED_COLOR_WHITE);
        oled_drv_update_screen();
        usleep(200);
        oled_drv_goto_xy(30, 23);
        oled_drv_put_ch_numbers("3", &font_40x16, OLED_COLOR_WHITE);
        oled_drv_update_screen();
        usleep(200);
        oled_drv_goto_xy(30, 23);
        oled_drv_put_ch_numbers("4", &font_40x16, OLED_COLOR_WHITE);
        oled_drv_update_screen();
        usleep(200);
        oled_drv_goto_xy(30, 23);
        oled_drv_put_ch_numbers("5", &font_40x16, OLED_COLOR_WHITE);
        oled_drv_update_screen();
        usleep(200);
        oled_drv_goto_xy(30, 23);
        oled_drv_put_ch_numbers("6", &font_40x16, OLED_COLOR_WHITE);
        oled_drv_update_screen();
        usleep(200);
        oled_drv_goto_xy(30, 23);
        oled_drv_put_ch_numbers("7", &font_40x16, OLED_COLOR_WHITE);
        oled_drv_update_screen();
        usleep(200);
        oled_drv_goto_xy(30, 23);
        oled_drv_put_ch_numbers("8", &font_40x16, OLED_COLOR_WHITE);
        oled_drv_update_screen();
        usleep(200);
        oled_drv_goto_xy(30, 23);
        oled_drv_put_ch_numbers("9", &font_40x16, OLED_COLOR_WHITE);
        oled_drv_update_screen();
        usleep(200);
#endif
    }    
}
