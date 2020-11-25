#ifndef __GPIO_H
#define __GPIO_H

extern "C"
{
#include "gd32vf103_gpio.h"
#include "gd32vf103_rcu.h"
}

enum GPIOMode : uint32_t
{
    MODE_AIN = GPIO_MODE_AIN,
    MODE_IN_FLOATING = GPIO_MODE_IN_FLOATING,
    MODE_IPD = GPIO_MODE_IPD,
    MODE_IPU = GPIO_MODE_IPU,
    MODE_OUT_OD = GPIO_MODE_OUT_OD,
    MODE_OUT_PP = GPIO_MODE_OUT_PP,
    MODE_AF_OD = GPIO_MODE_AF_OD,
    MODE_AF_PP = GPIO_MODE_AF_PP
};

enum GPIOSpeed : uint32_t
{
    SPEED_10MHZ = GPIO_OSPEED_10MHZ,
    SPEED_2MHZ = GPIO_OSPEED_2MHZ,
    SPEED_50MHZ = GPIO_OSPEED_50MHZ
};

class GPIOPin
{
public:
    GPIOPin(rcu_periph_enum rcu_periph, uint32_t gpio_periph, uint32_t pin)
        : _rcu_periph(rcu_periph),
          _gpio_periph(gpio_periph),
          _pin(pin){};

    rcu_periph_enum _rcu_periph;
    uint32_t _gpio_periph;
    uint32_t _pin;
};

class GPIO
{
private:
    GPIOPin _pin;
    GPIOMode _mode;
    GPIOSpeed _speed;

    void init();

public:
    GPIO(GPIOPin pin, GPIOMode mode, GPIOSpeed speed = GPIOSpeed::SPEED_50MHZ)
        : _pin(pin),
          _mode(mode),
          _speed(speed)
    {
        init();
    };

    void set();
    void clear();
};

#endif