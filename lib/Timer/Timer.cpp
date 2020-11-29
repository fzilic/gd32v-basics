
#ifndef __TIMER_H
#define __TIMER_H

#include "Timer.hpp"

extern "C"
{
#include "gd32vf103.h"
}

void Timer::delay(uint32_t count)
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

void Timer::delayMicroseconds(uint32_t count)
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
    } while (delta_mtime < (SystemCoreClock / 4000000.0 * count));
}

uint64_t Timer::milis()
{
    return get_timer_value() * 4000 / SystemCoreClock;
}

uint64_t Timer::micros()
{
    return get_timer_value() * 4000000 / SystemCoreClock;
}

#endif