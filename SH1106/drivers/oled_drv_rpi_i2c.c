#include "oled_drv_rpi_i2c.h"
#include <wiringPiI2C.h>

static int oled_fd;

void oled_drv_rpi_init(void)
{
    oled_fd = wiringPiI2CSetup(OLED_I2C_ADDR >> 1);
    if(oled_fd == -1)
    {
        printf("RPI i2c failed.\n");
        return;
    }
}

void oled_drv_rpi_write_cmd(unsigned char cmd)
{
    int ret = wiringPiI2CWriteReg8(oled_fd, 0x00, cmd);
    if(ret)
    {
        printf("RPI write cmd failed.\n");
        return;
    }
}

void oled_drv_rpi_write_data(unsigned char data)
{
    int ret = wiringPiI2CWriteReg8(oled_fd, 0x40, data);
    if(ret)
    {
        printf("RPI write data failed.\n");
        return;
    }
}