#include "GPIO.hpp"

void GPIO::init()
{
    rcu_periph_clock_enable(_pin._rcu_periph);
    gpio_init(_pin._gpio_periph, _mode, _speed, _pin._pin);
}

void GPIO::set()
{
    gpio_bit_set(_pin._gpio_periph, _pin._pin);
}

void GPIO::clear()
{
    gpio_bit_reset(_pin._gpio_periph, _pin._pin);
}