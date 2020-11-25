#ifndef __LED_H
#define __LED_H

#include "GPIO_pins.hpp"

namespace LED
{
    enum LEDColor : uint8_t
    {
        BLACK = 0x00,
        RED = 0x04,
        YELLOW = 0x06,
        GREEN = 0x02,
        CYAN = 0x03,
        BLUE = 0x01,
        MAGENTA = 0x05,
        WHITE = 0x07
    };

    class LED
    {
    private:
        GPIO _pins[3] = {
            GPIO(PA2, GPIOMode::MODE_OUT_PP),
            GPIO(PA1, GPIOMode::MODE_OUT_PP),
            GPIO(PC13, GPIOMode::MODE_OUT_PP)};

    public:
        LED();
        void init();
        void set(LEDColor color);
    };
}; // namespace LED

#endif