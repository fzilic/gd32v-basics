#include "GPIO.hpp"

namespace GPIO
{
    void GPIO::init()
    {
        rcu_periph_clock_enable(_pin.rcu_periph());
        gpio_init(_pin.gpio_periph(), _mode, _speed, _pin.pin());
    }

    void GPIO::set()
    {
        gpio_bit_set(_pin.gpio_periph(), _pin.pin());
    }

    void GPIO::clear()
    {
        gpio_bit_reset(_pin.gpio_periph(), _pin.pin());
    }

    void GPIO::write(uint8_t value)
    {
        gpio_bit_write(_pin.gpio_periph(), _pin.pin(), status(value));
    }

    bool GPIO::read()
    {
        switch (_mode)
        {
        case MODE_IN_FLOATING:
        case MODE_IPD:
        case MODE_IPU:
            return FlagStatus::SET == gpio_input_bit_get(_pin.gpio_periph(), _pin.pin()) ? true : false;
        case MODE_OUT_OD:
        case MODE_OUT_PP:
            return FlagStatus::SET == gpio_output_bit_get(_pin.gpio_periph(), _pin.pin()) ? true : false;
        default:
            return false;
        }
    }

    FlagStatus GPIO::status(uint8_t value)
    {
        return value > 0 ? FlagStatus::SET : FlagStatus::RESET;
    }

} // namespace GPIO