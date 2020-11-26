#include "LED.hpp"

namespace LED
{

    void LED::init()
    {
        for (uint8_t i = 0; i < 3; i++)
        {
            // initialize device
            _pins[i].init();
            _pins[i].set();
        }
    }

    void LED::set(LEDColor color)
    {
        for (uint8_t i = 0; i < 3; i++)
        {
            _pins[i].write(
                !((color & (0x01 << i))));
        }
    }

} // namespace LED