
#ifndef __PROJECT_H__
#define __PROJECT_H__

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

/* specific modules */
#include "i2c.h"
#include "wii.h"
#include "spi.h"
#include "uc1608.h"
#include "iotools.h"
#include "utils.h"

#define PRODUCT_NAME	"SpiTris - matthieu@barreteau.org"
#define PRODUCT_VERSION	"1.0"

/* SPI specific */
#define SPI_FREQUENCY 	10000000
#define SPI_CHANNEL		  0

/* LCD command/data IO (BCM value) */
#define LCD_CD		      25

/* command/data values for LCD */
#define DATA		        1
#define CMD			        0

/* i2c bus specific */
#define I2C_BUS_PATH					    "/dev/i2c-1"
#define I2C_WII_CLASSIC_ADDRESS		0x52

/* screen dimension (portrait) */
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   240

/* origin of the game board on the screen */
#define GAME_BOARD_X    24
#define GAME_BOARD_Y    30

/* origin of next piece on the screen */
#define NEXT_PIECE_X		71
#define NEXT_PIECE_Y		199

/* origin of the score on the screen */
#define SCORE_X         20
#define SCORE_Y         206

/* prototypes */
void *inputThread();
void *updateScreenThread();

#endif
