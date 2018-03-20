#ifndef _CONFIGS_H
#define _CONFIGS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// customer config
#define BOARD_ID    0 // 0:RPI; 1:STM32F10X; 2:STM32F40X
#define BOARD_NAME  "RPI" // RPI; STM32F10X; STM32F40X

#define PANEL_IC    "SH1106" // SSD1306; SH1106

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_I2C_ADDR 0x78

#endif // _CONFIGS_H