#ifndef __SPI_H__
#define __SPI_H__

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stropts.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

struct spi_config {
    int fd;
    uint8_t channel;
    uint32_t speed;
    uint8_t mode;
    uint8_t bpw;
};

int spi_open(struct spi_config *config);
int spi_write_data(struct spi_config *config, unsigned char *data, int len);

#endif

/* EOF */