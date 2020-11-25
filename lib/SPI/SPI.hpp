#ifndef __SPH_H
#define __SPH_H

extern "C"
{
#include "gd32vf103_spi.h"
}
#include <stdint.h>

enum SPIEndianess : uint32_t
{
    MSB = SPI_ENDIAN_MSB,
    LSB = SPI_ENDIAN_LSB
};
enum SPIMode : uint32_t
{
    MODE0 = SPI_CK_PL_LOW_PH_1EDGE,
    MODE1 = SPI_CK_PL_HIGH_PH_1EDGE,
    MODE2 = SPI_CK_PL_LOW_PH_2EDGE,
    MODE3 = SPI_CK_PL_HIGH_PH_2EDGE
};

class SPIPort
{
public:
    uint32_t _spi_periph;
    rcu_periph_enum _rcu_gpio_periph;
    rcu_periph_enum _rcu_spi_periph;
    uint32_t _gpio;
    uint32_t _out;
    uint32_t _in;
    SPIPort(uint32_t spi_periph, rcu_periph_enum rcu_gpio_periph, rcu_periph_enum rcu_spi_periph, uint32_t gpio, uint32_t out, uint32_t in)
        : _spi_periph(spi_periph), _rcu_gpio_periph(rcu_gpio_periph), _rcu_spi_periph(rcu_spi_periph), _gpio(gpio), _out(out), _in(in){};
};

#define SPI_0 SPIPort(SPI0, RCU_GPIOA, RCU_SPI0, GPIOA, GPIO_PIN_5 | GPIO_PIN_7, GPIO_PIN_6)
#define SPI_1 SPIPort(SPI1, RCU_GPIOB, RCU_SPI1, GPIOB, GPIO_PIN_13 | GPIO_PIN_15, GPIO_PIN_14)
#define SPI_2 SPIPort(SPI2, RCU_GPIOB, RCU_SPI2, GPIOB, GPIO_PIN_3 | GPIO_PIN_5, GPIO_PIN_4)

class SPISettings
{
public:
    SPIEndianess _endinaess;
    SPIMode _mode;
    SPISettings(SPIEndianess endinaess, SPIMode mode)
        : _endinaess(endinaess), _mode(mode){};
};

class SPI
{
private:
    SPIPort _spi;
    SPISettings _settings;

public:
    SPI(SPIPort spi, SPISettings settings)
        : _spi(spi), _settings(settings)
    {
        init();
    };

    void init();

    uint8_t transmit(const uint8_t data);
};

#endif