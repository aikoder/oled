#include "oled_drv.h"
#include "oled_drv_rpi_i2c.h"
#include "fonts.h"

#define ABS(x)   ((x) > 0 ? (x) : -(x))

typedef struct 
{
	unsigned short current_x;
	unsigned short current_y;
	unsigned char inverted;
	unsigned char initialized;
} oled_t;

// create an instance
static oled_t sh1106;

// size: 128x8 bytes
static unsigned char oled_data_buffer[OLED_WIDTH * OLED_HEIGHT / 8];

static void sh1106_init(void);

// driver instance
struct oled_drv oled_drv = {
    .id             = BOARD_ID,
    .ic             = PANEL_IC,
    .name           = BOARD_NAME,
    .panel_ic_init  = sh1106_init, // customer
    .board_init     = oled_drv_rpi_init, // customer
    .write_cmd      = oled_drv_rpi_write_cmd, // customer
    .write_data     = oled_drv_rpi_write_data, // customer
};

// init board driver
void oled_drv_init(void)
{
    printf("\nBoard Info: %s @%s, 128x64 \n\n", oled_drv.name, oled_drv.ic);
    oled_drv.board_init();
    oled_drv.panel_ic_init();
    printf("oled driver init OK.\n");
}

void oled_drv_update_screen(void)
{
    unsigned char i, j;

    for(i = 0; i < 8; i++)
    {
        // page
        oled_drv.write_cmd(0xB0 + i);
        // cols LSB
        oled_drv.write_cmd(0x02);
        // cols MSB
        oled_drv.write_cmd(0x10);

        for(j = 0; j < 128; j++)
            oled_drv.write_data(oled_data_buffer[OLED_WIDTH * i + j]);
    }
}

void oled_drv_fill_screen(color_t color)
{
    memset(oled_data_buffer, (color == OLED_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(oled_data_buffer));
}

void oled_drv_toggle_invert(void)
{
	unsigned short i;
	
	// Toggle invert
	sh1106.inverted = !sh1106.inverted;
	
	// Do memory toggle
	for (i = 0; i < sizeof(oled_data_buffer); i++) {
		oled_data_buffer[i] = ~oled_data_buffer[i];
	}
}

void oled_drv_draw_pixel(unsigned short x, unsigned short y, color_t color)
{
    if(x >= OLED_WIDTH || y >= OLED_HEIGHT)
    {
        return;
    }

    // Check if pixels are inverted
    if(sh1106.inverted)
    {
        color = (color_t)!color;
    }  

	// Set color
	if(color == OLED_COLOR_WHITE)
    {
		oled_data_buffer[x + (y / 8) * OLED_WIDTH] |= 1 << (y % 8);
	}
    else 
    {
		oled_data_buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y % 8));
	}
}

void oled_drv_goto_xy(unsigned short x, unsigned short y)
{
	// Set write pointers
	sh1106.current_x = x;
	sh1106.current_y = y;
}

char oled_drv_putc(char ch, font_t *font, color_t color)
{
	unsigned int i, b, j;
	
	/* Check available space in LCD */
	if(OLED_WIDTH <= (sh1106.current_x + font->font_width) ||
		OLED_HEIGHT <= (sh1106.current_y + font->font_height))
    {
		/* Error */
		return 0;
	}
    
	/* Go through font */
	for (i = 0; i < font->font_height; i++)
    {
		b = font->data[(ch - ' ') * font->font_height + i];
		for (j = 0; j < font->font_width; j++)
        {
			if ((b << j) & 0x8000) 
            {
				oled_drv_draw_pixel(sh1106.current_x + j, (sh1106.current_y + i), (color_t) color);
			} 
            else
            {
				oled_drv_draw_pixel(sh1106.current_x + j, (sh1106.current_y + i), (color_t)!color);
			}
		}
	}
	
	/* Increase pointer */
	sh1106.current_x += font->font_width;
	
	/* Return character written */
	return ch;    
}

char oled_drv_puts(char *str, font_t* font, color_t color)
{
	/* Write characters */
	while (*str) {
		/* Write character by character */
		if (oled_drv_putc(*str, font, color) != *str) {
			/* Return error */
			return *str;
		}
		
		/* Increase string pointer */
		str++;
	}
	
	/* Everything OK, zero should be returned */
	return *str;
}

char oled_drv_put_ch_number(char ch, font_t *font, color_t color)
{
	unsigned int i, b, j;
	
	/* Check available space in LCD */
	if(OLED_WIDTH <= (sh1106.current_x + font->font_width) ||
		OLED_HEIGHT <= (sh1106.current_y + font->font_height))
    {
		/* Error */
		return 0;
	}
    
	/* Go through font */
	for (i = 0; i < font->font_height; i++)
    {
		b = font->data[(ch - '0') * font->font_height + i];
		for (j = 0; j < font->font_width; j++)
        {
			if ((b << j) & 0x8000) 
            {
				oled_drv_draw_pixel(sh1106.current_x + j, (sh1106.current_y + i), (color_t) color);
			} 
            else
            {
				oled_drv_draw_pixel(sh1106.current_x + j, (sh1106.current_y + i), (color_t)!color);
			}
		}
	}
	
	/* Increase pointer */
	sh1106.current_x += font->font_width;
	
	/* Return character written */
	return ch; 
}

char oled_drv_put_ch_numbers(char *str, font_t *font, color_t color)
{
	/* Write characters */
	while (*str) {
		/* Write character by character */
		if (oled_drv_put_ch_number(*str, font, color) != *str) {
			/* Return error */
			return *str;
		}
		
		/* Increase string pointer */
		str++;
	}
	
	/* Everything OK, zero should be returned */
	return *str;
}

char oled_drv_draw_icon(char ch, font_t *font, color_t color)
{
	unsigned int i, b, j;
	
	/* Check available space in LCD */
	if(OLED_WIDTH <= (sh1106.current_x + font->font_width) ||
		OLED_HEIGHT <= (sh1106.current_y + font->font_height))
    {
		/* Error */
		return 0;
	}
    
	/* Go through font */
	for (i = 0; i < font->font_height; i++)
    {
		b = font->data[ch * font->font_height + i];
		for (j = 0; j < font->font_width; j++)
        {
			if ((b << j) & 0x8000) 
            {
				oled_drv_draw_pixel(sh1106.current_x + j, (sh1106.current_y + i), (color_t) color);
			} 
            else
            {
				oled_drv_draw_pixel(sh1106.current_x + j, (sh1106.current_y + i), (color_t)!color);
			}
		}
	}
	
	/* Increase pointer */
	sh1106.current_x += font->font_width;
	
	/* Return character written */
	return ch;
}

char oled_drv_draw_icon2(char ch, font_t *font, color_t color)
{
    unsigned char x = sh1106.current_x;
    unsigned char y = sh1106.current_y;
    unsigned char i, j, temp, y0 = y;

    for(i = 0; i < font->font_width * 2; i++)
    {
        temp = font->data[ch * font->font_width * 2 + i];
        for(j = 0; j < 8; j++)
        {
            if(temp & 0x01)
                oled_drv_draw_pixel(x, y, color);
            else
                oled_drv_draw_pixel(x, y, !color);

            temp >>= 1;
            y++;

            if((y - y0) == font->font_height)
            {
                y = y0;
                x++;
                break;
            }
        }
    }

	/* Increase pointer */
	sh1106.current_x += 24;
	
	/* Return character written */
	return ch;    
}

void oled_drv_draw_line(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, color_t c)
{
	unsigned short dx, dy, sx, sy, err, e2, i, tmp; 
	
	/* Check for overflow */
	if (x0 >= OLED_WIDTH)
    {
		x0 = OLED_WIDTH - 1;
	}

	if (x1 >= OLED_WIDTH)
    {
		x1 = OLED_WIDTH - 1;
	}

	if (y0 >= OLED_HEIGHT)
    {
		y0 = OLED_HEIGHT - 1;
	}

	if (y1 >= OLED_HEIGHT)
    {
		y1 = OLED_HEIGHT - 1;
	}
	
	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
	sx = (x0 < x1) ? 1 : -1; 
	sy = (y0 < y1) ? 1 : -1; 
	err = ((dx > dy) ? dx : -dy) / 2; 

	if (dx == 0)
    {
		if (y1 < y0)
        {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0)
        {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Vertical line */
		for (i = y0; i <= y1; i++)
        {
			oled_drv_draw_pixel(x0, i, c);
		}
		
		/* Return from function */
		return;
	}
	
	if (dy == 0)
    {
		if (y1 < y0)
        {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0)
        {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Horizontal line */
		for (i = x0; i <= x1; i++)
        {
			oled_drv_draw_pixel(i, y0, c);
		}
		
		/* Return from function */
		return;
	}
	
	while (1)
    {
		oled_drv_draw_pixel(x0, y0, c); 
		if (x0 == x1 && y0 == y1)
        {
			break;
		}
		e2 = err; 
		if (e2 > -dx)
        {
			err -= dy;
			x0 += sx;
		} 
		if (e2 < dy)
        {
			err += dx;
			y0 += sy;
		} 
	}
}

void oled_drv_draw_rectangle(unsigned short x, unsigned short y, unsigned short w, unsigned short h, color_t c)
{
	/* Check input parameters */
	if(x >= OLED_WIDTH || y >= OLED_HEIGHT)
    {
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= OLED_WIDTH) {
		w = OLED_WIDTH - x;
	}
	if((y + h) >= OLED_HEIGHT)
    {
		h = OLED_HEIGHT - y;
	}
	
	/* Draw 4 lines */
	oled_drv_draw_line(x, y, x + w, y, c);         /* Top line */
	oled_drv_draw_line(x, y + h, x + w, y + h, c); /* Bottom line */
	oled_drv_draw_line(x, y, x, y + h, c);         /* Left line */
	oled_drv_draw_line(x + w, y, x + w, y + h, c); /* Right line */
}

void oled_drv_draw_filled_rectangle(unsigned short x, unsigned short y, unsigned short w, unsigned short h, color_t c)
{
	unsigned char i;
	
	/* Check input parameters */
	if(x >= OLED_WIDTH || y >= OLED_HEIGHT)
    {
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if((x + w) >= OLED_WIDTH)
    {
		w = OLED_WIDTH - x;
	}

	if((y + h) >= OLED_HEIGHT)
    {
		h = OLED_HEIGHT - y;
	}
	
	/* Draw lines */
	for (i = 0; i <= h; i++)
    {
		/* Draw lines */
		oled_drv_draw_line(x, y + i, x + w, y + i, c);
	}
}

void oled_drv_draw_triangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short x3, unsigned short y3, color_t color)
{
	/* Draw lines */
	oled_drv_draw_line(x1, y1, x2, y2, color);
	oled_drv_draw_line(x2, y2, x3, y3, color);
	oled_drv_draw_line(x3, y3, x1, y1, color);
}

void oled_drv_draw_filled_triangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short x3, unsigned short y3, color_t color)
{
	unsigned short deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;
	
	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if(x2 >= x1)
    {
		xinc1 = 1;
		xinc2 = 1;
	}
    else
    {
		xinc1 = -1;
		xinc2 = -1;
	}

	if(y2 >= y1)
    {
		yinc1 = 1;
		yinc2 = 1;
	}
    else 
    {
		yinc1 = -1;
		yinc2 = -1;
	}

	if(deltax >= deltay)
    {
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	}
    else
    {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for(curpixel = 0; curpixel <= numpixels; curpixel++)
    {
		oled_drv_draw_line(x, y, x3, y3, color);

		num += numadd;
		if(num >= den)
        {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void oled_drv_draw_circle(unsigned short x0, unsigned short y0, unsigned short r, color_t c) 
{
	unsigned short f = 1 - r;
	unsigned short ddF_x = 1;
	unsigned short ddF_y = -2 * r;
	unsigned short x = 0;
	unsigned short y = r;

    oled_drv_draw_pixel(x0, y0 + r, c);
    oled_drv_draw_pixel(x0, y0 - r, c);
    oled_drv_draw_pixel(x0 + r, y0, c);
    oled_drv_draw_pixel(x0 - r, y0, c);

    while(x < y)
    {
        if(f >= 0) 
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        oled_drv_draw_pixel(x0 + x, y0 + y, c);
        oled_drv_draw_pixel(x0 - x, y0 + y, c);
        oled_drv_draw_pixel(x0 + x, y0 - y, c);
        oled_drv_draw_pixel(x0 - x, y0 - y, c);

        oled_drv_draw_pixel(x0 + y, y0 + x, c);
        oled_drv_draw_pixel(x0 - y, y0 + x, c);
        oled_drv_draw_pixel(x0 + y, y0 - x, c);
        oled_drv_draw_pixel(x0 - y, y0 - x, c);
    }
}

void oled_drv_draw_filled_circle(unsigned short x0, unsigned short y0, unsigned short r, color_t c) 
{
	unsigned short f = 1 - r;
	unsigned short ddF_x = 1;
	unsigned short ddF_y = -2 * r;
	unsigned short x = 0;
	unsigned short y = r;

    oled_drv_draw_pixel(x0, y0 + r, c);
    oled_drv_draw_pixel(x0, y0 - r, c);
    oled_drv_draw_pixel(x0 + r, y0, c);
    oled_drv_draw_pixel(x0 - r, y0, c);
    oled_drv_draw_line(x0 - r, y0, x0 + r, y0, c);

    while(x < y) 
    {
        if(f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        oled_drv_draw_line(x0 - x, y0 + y, x0 + x, y0 + y, c);
        oled_drv_draw_line(x0 + x, y0 - y, x0 - x, y0 - y, c);

        oled_drv_draw_line(x0 + y, y0 + x, x0 - y, y0 + x, c);
        oled_drv_draw_line(x0 + y, y0 - x, x0 - y, y0 - x, c);
    }
}
 
void oled_drv_display_on(void)
{
	oled_drv.write_cmd(0x8D);  
	oled_drv.write_cmd(0x14);  
	oled_drv.write_cmd(0xAF);  
}
void oled_drv_display_off(void)
{
	oled_drv.write_cmd(0x8D);  
	oled_drv.write_cmd(0x10);
	oled_drv.write_cmd(0xAE);  
}

// static functions
//////////////////////////
///// SH1106
static void sh1106_init(void)
{
    oled_drv.write_cmd(0xAE);//--display off
    oled_drv.write_cmd(0x02);//---set low column address
    oled_drv.write_cmd(0x10);//---set high column address
    oled_drv.write_cmd(0x40);//--set start line address  
    oled_drv.write_cmd(0xB0);//--set page address
    oled_drv.write_cmd(0x81); // contract control
    oled_drv.write_cmd(0xFF);//--128   
    oled_drv.write_cmd(0xA1);//set segment remap 
    oled_drv.write_cmd(0xA6);//--normal / reverse
    oled_drv.write_cmd(0xA8);//--set multiplex ratio(1 to 64)
    oled_drv.write_cmd(0x3F);//--1/64 duty
    oled_drv.write_cmd(0xAD);//set charge pump enable
    oled_drv.write_cmd(0x8B);//-0x8B 内供 VCC
    oled_drv.write_cmd(0x33);//-0X30---0X33 set VPP 9V
    oled_drv.write_cmd(0xC8);//Com scan direction
    oled_drv.write_cmd(0xD3);//-set display offset
    oled_drv.write_cmd(0x00);//
    
    oled_drv.write_cmd(0xD5);//set osc division
    oled_drv.write_cmd(0x80);//
    
    oled_drv.write_cmd(0xD8);//set area color mode off
    oled_drv.write_cmd(0x05);//
    
    oled_drv.write_cmd(0xD9);//Set Pre-Charge Period
    oled_drv.write_cmd(0x1F);//
    
    oled_drv.write_cmd(0xDA);//set com pin configuartion
    oled_drv.write_cmd(0x12);//
    
    oled_drv.write_cmd(0xDB);//set Vcomh
    oled_drv.write_cmd(0x40);//

    oled_drv.write_cmd(0xAF);//--turn on oled panel

	// Clear screen
	oled_drv_fill_screen(OLED_COLOR_BLACK);
	
	// Update screen
	oled_drv_update_screen();
	
	/* Set default values */
	sh1106.current_x = 0;
	sh1106.current_y = 0;
	
	/* Initialized OK */
	sh1106.initialized = 1;

    printf("SH1106 init OK.\n");
}
