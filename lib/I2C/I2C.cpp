#include "I2C.hpp"

extern "C"
{
#include "gd32vf103_rcu.h"
}
namespace I2C
{
    void I2C::init()
    {
        _transmitting = false;
        rcu_periph_clock_enable(RCU_GPIOB); // I2C buses are on PortB
        rcu_periph_clock_enable(_port.rcu_periph());
        gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, _port.pin()); // I2C buses are on PortB

        i2c_clock_config(_port.i2c_periph(), _settings.clock(), _settings.duty_cycle());
    }

    void I2C::startBus(uint32_t address)
    {
        // wait until I2C bus is idle
        // while (i2c_flag_get(_port.i2c_periph(), I2C_FLAG_I2CBSY))
        //     ;

        // send a start condition to I2C bus and wait for it
        i2c_start_on_bus(_port.i2c_periph());
        while (!i2c_flag_get(_port.i2c_periph(), I2C_FLAG_SBSEND))
            ;

        // send slave address to I2C bus and wait for it
        i2c_master_addressing(_port.i2c_periph(), address << 1, I2C_TRANSMITTER);
        while (!i2c_flag_get(_port.i2c_periph(), I2C_FLAG_ADDSEND))
            ;
        i2c_flag_clear(_port.i2c_periph(), I2C_FLAG_ADDSEND);

        //wait until the transmit data buffer is empty
        // while (!i2c_flag_get(_port.i2c_periph(), I2C_FLAG_TBE))
        //     ;

        i2c_ack_config(_port.i2c_periph(), I2C_ACK_ENABLE);
    }

    void I2C::stopBus()
    {
        while (!i2c_flag_get(_port.i2c_periph(), I2C_FLAG_BTC))
            ;

        // send a stop condition to I2C bus
        i2c_stop_on_bus(_port.i2c_periph());
        /* wait until stop condition generate */
        while (I2C_CTL0(_port.i2c_periph()) & I2C_CTL0_STOP)
            ;
    }

    void I2C::begin()
    {
        _transmitting = true;
        i2c_enable(_port.i2c_periph());
    }

    void I2C::end()
    {
        _transmitting = false;
        i2c_disable(_port.i2c_periph());
    }

    void I2C::transmit(uint32_t address, uint8_t data)
    {
        startBus(address);

        // data transmission and wait for empty
        i2c_data_transmit(_port.i2c_periph(), data);
        while (!i2c_flag_get(_port.i2c_periph(), I2C_FLAG_TBE))
            ;

        stopBus();
    }

    void I2C::transmit(uint32_t address, uint8_t *data, uint32_t size)
    {
        startBus(address);

        while (size--)
        {
            // data transmission and wait for empty
            i2c_data_transmit(_port.i2c_periph(), *(data++));
            while (!i2c_flag_get(_port.i2c_periph(), I2C_FLAG_TBE))
                ;
        }

        stopBus();
    }

} // namespace I2C