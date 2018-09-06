#include "../include/time.h"
#include "../include/soc.h"

uint32_t GetTick() {
    return GPIO_TIMER;
}

void DelayMicrosecond(uint32_t interval) {
    uint32_t tick = GPIO_TIMER;
    uint32_t int_tick = interval * CLOCKS_PER_US;
    while (GPIO_TIMER - tick < int_tick);
}

void DelayMillisecond(uint32_t interval) {
    uint32_t tick = GPIO_TIMER;
    uint32_t int_tick = interval * CLOCKS_PER_US * 1000;
    while (GPIO_TIMER - tick < int_tick);
}
