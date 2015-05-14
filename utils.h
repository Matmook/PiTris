#ifndef __UTILS_H
#define __UTILS_H

#include <time.h>
#include <sys/time.h>
#include <unistd.h>

void waitMS(unsigned int howLong);
void delay (unsigned int howLong);
void delayMicrosecondsHard (unsigned int howLong);
void delayMicroseconds (unsigned int howLong);
#endif
/* end of file */