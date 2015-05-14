#ifndef __IOTOOLS_H
#define __IOTOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

extern volatile unsigned *iotoolsUart;
extern volatile unsigned *iotoolsGpio;

#define BCM2708_PERI_BASE        0x20000000
#define UART_BASE                (BCM2708_PERI_BASE + 0x201000)
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000)

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

#define DR      (0x00 >> 2)
#define RSRECR  (0x04 >> 2)
#define FR      (0x18 >> 2)
#define ILPR    (0x20 >> 2)
#define IBRD    (0x24 >> 2)
#define FBRD    (0x28 >> 2)
#define LCRH    (0x2C >> 2)
#define CR      (0x30 >> 2)
#define IFLS    (0x34 >> 2)
#define IMSC    (0x38 >> 2)
#define RIS     (0x3C >> 2)
#define MIS     (0x40 >> 2)
#define ICR     (0x44 >> 2)
#define DMACR   (0x48 >> 2)
#define ITCR    (0x80 >> 2)
#define ITIP    (0x84 >> 2)
#define ITOP    (0x88 >> 2)
#define TDR     (0x8C >> 2)

#define FR_TXFE 0x80
#define FR_RXFF 0x40
#define FR_TXFF 0x20
#define FR_RXFE 0x10
#define FR_BUSY 0x08
#define FR_DCD  0x04
#define FR_DSR  0x02
#define FR_CTS  0x01

#define INP_GPIO(g) *(iotoolsGpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(iotoolsGpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(iotoolsGpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(iotoolsGpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(iotoolsGpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(iotoolsGpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH
 
#define GPIO_PULL *(iotoolsGpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(iotoolsGpio+38) // Pull up/pull down clock

void iotoolsSetupMaps(void);
volatile unsigned *iotoolsMapPeripheral(unsigned int address);
void iotoolsWaitEndOfTX(void);
int iotoolsGetRPiRevision(void);
#endif
/* end of file */
