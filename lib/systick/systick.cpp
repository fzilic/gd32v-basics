
#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "systick.hpp"

extern "C"
{
#include "gd32vf103.h"
}

void delay(uint32_t count)
{
    uint64_t start_mtime, delta_mtime;

    // Don't start measuruing until we see an mtime tick
    uint64_t tmp = get_timer_value();
    do
    {
        start_mtime = get_timer_value();
    } while (start_mtime == tmp);

    do
    {
        delta_mtime = get_timer_value() - start_mtime;
    } while (delta_mtime < (SystemCoreClock / 4000.0 * count));
}

void delayMicroseconds(uint32_t count)
{
    uint64_t start_mtime, delta_mtime;

    // Don't start measuruing until we see an mtime tick
    uint64_t tmp = get_timer_value();
    do
    {
        start_mtime = get_timer_value();
    } while (start_mtime == tmp);

    do
    {
        delta_mtime = get_timer_value() - start_mtime;
    } while (delta_mtime < (SystemCoreClock / 4.0 * count));
}

#endif