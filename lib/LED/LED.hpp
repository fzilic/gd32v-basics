#ifndef __LED_H
#define __LED_H

#include "GPIO_pins.hpp"

class LED
{
private:
    GPIO _red;
    GPIO _green;
    GPIO _blue;

public:
    LED()
        : _red(GPIO(PC13, GPIOMode::MODE_OUT_PP)),
          _green(GPIO(PA1, GPIOMode::MODE_OUT_PP)),
          _blue(GPIO(PA2, GPIOMode::MODE_OUT_PP))
    {
        init();
    };

    void init();
    void red();
    void green();
    void blue();
    void white();
    void black();
};

#endif