#ifndef _OLED_DRV_H
#define _OLED_DRV_H

#include "configs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonts.h"

typedef enum {
	OLED_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
	OLED_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} color_t;

struct oled_drv
{
    unsigned char id;
    char *ic;
    char *name;
    void (*panel_ic_init)(void);
    void (*board_init)(void);
    void (*write_cmd)(unsigned char cmd);
    void (*write_data)(unsigned char data);
};

void oled_drv_init(void);
void oled_drv_update_screen(void);
void oled_drv_fill_screen(color_t color);
void oled_drv_toggle_invert(void);
void oled_drv_draw_pixel(unsigned short x, unsigned short y, color_t color);
void oled_drv_goto_xy(unsigned short x, unsigned short y);
char oled_drv_putc(char ch, font_t *font, color_t color);
char oled_drv_puts(char *str, font_t* font, color_t color);
char oled_drv_put_ch_number(char ch, font_t *font, color_t color);
char oled_drv_put_ch_numbers(char *str, font_t *font, color_t color);

// signal, battery, celsius
char oled_drv_draw_icon(char ch, font_t *font, color_t color);
char oled_drv_draw_icon2(char ch, font_t *font, color_t color);


void oled_drv_draw_line(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, color_t c);
void oled_drv_draw_rectangle(unsigned short x, unsigned short y, unsigned short w, unsigned short h, color_t c);
void oled_drv_draw_filled_rectangle(unsigned short x, unsigned short y, unsigned short w, unsigned short h, color_t c);
void oled_drv_draw_triangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short x3, unsigned short y3, color_t color);
void oled_drv_draw_filled_triangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short x3, unsigned short y3, color_t color);
void oled_drv_draw_circle(unsigned short x0, unsigned short y0, unsigned short r, color_t c);
void oled_drv_draw_filled_circle(unsigned short x0, unsigned short y0, unsigned short r, color_t c);
void oled_drv_display_on(void);
void oled_drv_display_off(void);


#endif // _OLED_DRV_h