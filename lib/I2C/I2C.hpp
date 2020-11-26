#ifndef __I2C_H
#define __I2C_H

extern "C"
{
#include "gd32vf103_i2c.h"
}
#include <stdint.h>

namespace I2C
{

#define I2C_0 I2CPort(I2C0, RCU_I2C0, GPIO_PIN_6 | GPIO_PIN_7)
#define I2C_1 I2CPort(I2C1, RCU_I2C0, GPIO_PIN_10 | GPIO_PIN_11)

    enum I2CDutyCycle : uint32_t
    {
        DUTY_1_2 = I2C_DTCY_2,
        DUTY_9_16 = I2C_DTCY_16_9
    };

    enum I2CAddressFormat : uint32_t
    {
        ADDR_7 = I2C_ADDFORMAT_7BITS,
        ADDR_10 = I2C_ADDFORMAT_10BITS
    };

    class I2CPort
    {
    private:
        uint32_t _i2c_periph;
        rcu_periph_enum _rcu_periph;
        uint32_t _pin;

    public:
        I2CPort(uint32_t i2c_periph,
                rcu_periph_enum rcu_periph,
                uint32_t pin)
            : _i2c_periph(i2c_periph),
              _rcu_periph(rcu_periph),
              _pin(pin) {}

        uint32_t i2c_periph() { return _i2c_periph; }
        rcu_periph_enum rcu_periph() { return _rcu_periph; }
        uint32_t pin() { return _pin; }
    };

    class I2CSettings
    {
    private:
        uint32_t _clock;
        I2CDutyCycle _duty_cycle;
        I2CAddressFormat _addr_mode;

    public:
        I2CSettings()
            : I2CSettings(100000, DUTY_1_2, ADDR_7) {}
        I2CSettings(uint32_t clock,
                    I2CDutyCycle duty_cycle,
                    I2CAddressFormat addr_mode)
            : _clock(clock),
              _duty_cycle(duty_cycle),
              _addr_mode(addr_mode) {}

        uint32_t clock() { return _clock; }
        I2CDutyCycle duty_cycle() { return _duty_cycle; }
        I2CAddressFormat addr_mode() { return _addr_mode; }
    };

    // I2C0
    class I2C
    {
    private:
        I2CPort _port;
        I2CSettings _settings;

        void init();

        void startBus(uint32_t address);
        void stopBus();

    public:
        I2C(I2CPort port,
            I2CSettings settings)
            : _port(port),
              _settings(settings)
        {
            init();
        };

        void begin();
        void end();

        void transmit(uint32_t address, uint8_t data);
        void transmit(uint32_t address, uint8_t *data, uint32_t size);
    };

} // namespace I2C
#endif
