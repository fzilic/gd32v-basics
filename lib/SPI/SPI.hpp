#ifndef __SPH_H
#define __SPH_H

extern "C"
{
#include "gd32vf103_spi.h"
}

#include <stdint.h>

namespace SPI
{

#define SPI_0 SPIPort(SPI0, RCU_GPIOA, RCU_SPI0, GPIOA, GPIO_PIN_5 | GPIO_PIN_7, GPIO_PIN_6)
#define SPI_1 SPIPort(SPI1, RCU_GPIOB, RCU_SPI1, GPIOB, GPIO_PIN_13 | GPIO_PIN_15, GPIO_PIN_14)
#define SPI_2 SPIPort(SPI2, RCU_GPIOB, RCU_SPI2, GPIOB, GPIO_PIN_3 | GPIO_PIN_5, GPIO_PIN_4)

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

    enum SPIPrescale : uint32_t
    {
        PSC_2 = SPI_PSC_2,
        PSC_4 = SPI_PSC_4,
        PSC_8 = SPI_PSC_8,
        PSC_16 = SPI_PSC_16,
        PSC_32 = SPI_PSC_32,
        PSC_64 = SPI_PSC_64,
        PSC_128 = SPI_PSC_128,
        PSC_256 = SPI_PSC_256
    };

    class SPIPort
    {
    private:
        uint32_t _spi_periph;
        rcu_periph_enum _rcu_gpio_periph;
        rcu_periph_enum _rcu_spi_periph;
        uint32_t _gpio;
        uint32_t _out;
        uint32_t _in;

    public:
        SPIPort(uint32_t spi_periph,
                rcu_periph_enum rcu_gpio_periph,
                rcu_periph_enum rcu_spi_periph,
                uint32_t gpio,
                uint32_t out,
                uint32_t in)
            : _spi_periph(spi_periph),
              _rcu_gpio_periph(rcu_gpio_periph),
              _rcu_spi_periph(rcu_spi_periph),
              _gpio(gpio),
              _out(out),
              _in(in) {}

        uint32_t spi_periph() { return _spi_periph; }
        rcu_periph_enum rcu_gpio_periph() { return _rcu_gpio_periph; }
        rcu_periph_enum rcu_spi_periph() { return _rcu_spi_periph; }
        uint32_t gpio() { return _gpio; }
        uint32_t out() { return _out; }
        uint32_t in() { return _in; }
    };

    class SPISettings
    {
    private:
        SPIEndianess _endinaess;
        SPIMode _mode;
        SPIPrescale _prescale;

    public:
        SPISettings(SPIEndianess endinaess,
                    SPIMode mode,
                    SPIPrescale prescale = SPIPrescale::PSC_128)
            : _endinaess(endinaess),
              _mode(mode),
              _prescale(prescale) {}

        SPIEndianess endinaess() { return _endinaess; }
        SPIMode mode() { return _mode; }
        SPIPrescale prescale() { return _prescale; }
    };

    class SPI
    {
    private:
        SPIPort _spi;
        SPISettings _settings;

        spi_parameter_struct _params;

        void init();

    public:
        SPI(SPIPort spi,
            SPISettings settings)
            : _spi(spi),
              _settings(settings)
        {
            init();
        }

        void begin();
        void end();

        uint8_t transfer(const uint8_t data);
    };

}; // namespace SPI

#endif