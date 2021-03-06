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
        const uint32_t _spi_periph;
        const rcu_periph_enum _rcu_gpio_periph;
        const rcu_periph_enum _rcu_spi_periph;
        const uint32_t _gpio;
        const uint32_t _out;
        const uint32_t _in;

    public:
        SPIPort(const uint32_t spi_periph,
                const rcu_periph_enum rcu_gpio_periph,
                const rcu_periph_enum rcu_spi_periph,
                const uint32_t gpio,
                const uint32_t out,
                const uint32_t in)
            : _spi_periph(spi_periph),
              _rcu_gpio_periph(rcu_gpio_periph),
              _rcu_spi_periph(rcu_spi_periph),
              _gpio(gpio),
              _out(out),
              _in(in) {}

        constexpr uint32_t spi_periph() const { return _spi_periph; }
        constexpr rcu_periph_enum rcu_gpio_periph() const { return _rcu_gpio_periph; }
        constexpr rcu_periph_enum rcu_spi_periph() const { return _rcu_spi_periph; }
        constexpr uint32_t gpio() const { return _gpio; }
        constexpr uint32_t out() const { return _out; }
        constexpr uint32_t in() const { return _in; }
    };

    class SPISettings
    {
    private:
        const SPIEndianess _endinaess;
        const SPIMode _mode;
        const SPIPrescale _prescale;

    public:
        SPISettings(const SPIEndianess endinaess,
                    const SPIMode mode,
                    const SPIPrescale prescale = SPIPrescale::PSC_128)
            : _endinaess(endinaess),
              _mode(mode),
              _prescale(prescale) {}

        constexpr SPIEndianess endinaess() const { return _endinaess; }
        constexpr SPIMode mode() const { return _mode; }
        constexpr SPIPrescale prescale() const { return _prescale; }
    };

    class SPI
    {
    private:
        const SPIPort _spi;
        const SPISettings _settings;

    public:
        SPI(const SPIPort spi,
            const SPISettings settings)
            : _spi(spi),
              _settings(settings) {}

        void init();

        void begin();
        void end();

        const uint8_t transfer(const uint8_t data);
    };

}; // namespace SPI

#endif