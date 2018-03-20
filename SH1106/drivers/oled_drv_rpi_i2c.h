#ifndef _OLED_DRV_RPI_I2C_H
#define _OLED_DRV_RPI_I2C_H

#include "configs.h"

void oled_drv_rpi_init(void);
void oled_drv_rpi_write_cmd(unsigned char cmd);
void oled_drv_rpi_write_data(unsigned char data);

#endif // _OLED_DRV_RPI_I2C_H