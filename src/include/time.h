#ifndef UBW_TIME_H_
#define UBW_TIME_H_

#include "type.h"

#define CLOCKS_PER_US  100
#define CLOCKS_PER_SEC 100000000

uint32_t GetTick();
void DelayMicrosecond(uint32_t interval);
void DelayMillisecond(uint32_t interval);

#endif // UBW_TIME_H_
