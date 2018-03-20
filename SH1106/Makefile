
CC = gcc
CC_FLAG = -Wall

INC = -Iconfigs -Idrivers -Ifonts -Iui
LIB = -lwiringPi

TARGET = oled_test
OBJS = oled_test.o

OBJS += drivers/oled_drv.o \
		drivers/oled_drv_rpi_i2c.o

OBJS += fonts/fonts.o

OBJS += ui/ui_index.o

$(TARGET):$(OBJS)
	$(CC) -o $@ $(OBJS) $(INC) $(LIB)
	
.SUFFIXES: .c .o
.c.o:
	$(CC) $(CC_FLAG) $(INC) -c $*.c -o $*.o

.PRONY:clean
clean:
	@echo "Removing linked and compiled files......"
	rm -f $(OBJS) $(TARGET)