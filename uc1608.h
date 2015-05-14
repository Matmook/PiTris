#ifndef __UC1608_H__
#define __UC1608_H__

#include <ctype.h>
#include <stdint.h>
#include <errno.h>
#include "utils.h"
#include "iotools.h"
#include "spi.h"

#define UC1608_DATA		1
#define UC1608_CMD    0

int uc1608_send(struct spi_config *config,unsigned char *data, int len,uint8_t mode,uint8_t cmdPin);
void uc1608_setup(struct spi_config *config,uint8_t cmdPin);

#endif

/* EOF */