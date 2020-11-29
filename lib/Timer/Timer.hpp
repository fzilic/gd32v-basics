#ifndef __SYS_TICK_H
#define __SYS_TICK_H

#include <stdint.h>

class Timer
{
public:
    static void delay(uint32_t count);
    static void delayMicroseconds(uint32_t count);
    
    static uint64_t milis();
    static uint64_t micros();
};

#endif