#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "ui_index.h"

int main(int argc, char *argv[])
{
    oled_drv_init();

    ui_index();

    while(1);

    return 0;
}

