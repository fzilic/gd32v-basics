#ifndef __GPIO_H
#define __GPIO_H

extern "C"
{
#include "gd32vf103_gpio.h"
#include "gd32vf103_rcu.h"
}

namespace GPIO
{
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
    private:
        const rcu_periph_enum _rcu_periph;
        const uint32_t _gpio_periph;
        const uint32_t _pin;

    public:
        GPIOPin(const rcu_periph_enum rcu_periph,
                const uint32_t gpio_periph,
                const uint32_t pin)
            : _rcu_periph(rcu_periph),
              _gpio_periph(gpio_periph),
              _pin(pin){};

        constexpr rcu_periph_enum rcu_periph() const { return _rcu_periph; }
        constexpr uint32_t gpio_periph() const { return _gpio_periph; }
        constexpr uint32_t pin() const { return _pin; }
    };

    class GPIO
    {
    private:
        const GPIOPin _pin;
        const GPIOMode _mode;
        const GPIOSpeed _speed;

        constexpr FlagStatus status(const uint8_t value);

    public:
        GPIO(const GPIOPin pin,
             const GPIOMode mode,
             const GPIOSpeed speed = GPIOSpeed::SPEED_50MHZ)
            : _pin(pin),
              _mode(mode),
              _speed(speed) {}

        void init();

        void set();
        void clear();

        bool read();

        void write(uint8_t value);
    };

}; // namespace GPIO

#endif