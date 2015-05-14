#ifndef __I2C_H__
#define __I2C_H__

#include <fcntl.h>
#include <string.h>
#include <stropts.h>
#include <unistd.h>
#include <linux/i2c-dev.h>

int i2c_open(char *buspath);
int i2c_select_slave(int devicebus, int slave_address);
int i2c_write_data(int devicebus, unsigned char register_number, unsigned char * data, unsigned char length);
int i2c_write_byte(int devicebus, unsigned char register_number, unsigned char data);
unsigned char i2c_read_byte(int devicebus, unsigned char register_number);
char i2c_read_data(int devicebus, unsigned char *data, unsigned char length);

#endif

/* EOF */