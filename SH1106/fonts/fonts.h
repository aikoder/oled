
#ifndef FONTS_H
#define FONTS_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

typedef struct
{
	unsigned char font_width;    /*!< Font width in pixels */
	unsigned char font_height;   /*!< Font height in pixels */
	const unsigned short *data; /*!< Pointer to data font data array */
} font_t;

typedef struct
{
	unsigned short length;      /*!< String length in units of pixels */
	unsigned short height;      /*!< String height in units of pixels */
} font_size_t;

extern font_t font_7x10;

extern font_t font_11x18;

extern font_t font_16x26;

extern font_t font_40x16;

extern font_t icon_fan_16x16;

extern font_t icon_celsius_16x16;

extern font_t icon_battery_24x16;

extern font_t icon_signal_24x16;

char *font_get_string_size(char *str, font_size_t* font_size, font_t *font);

/* C++ detection */
#ifdef __cplusplus
}
#endif

 
#endif // FONTS_H

