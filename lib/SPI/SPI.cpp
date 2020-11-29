#include "SPI.hpp"

extern "C"
{
#include "gd32vf103_rcu.h"
#include "gd32vf103_gpio.h"
}

namespace SPI
{

    void SPI::init()
    {
        rcu_periph_clock_enable(_spi.rcu_gpio_periph());
        rcu_periph_clock_enable(RCU_AF);
        rcu_periph_clock_enable(_spi.rcu_spi_periph());

        gpio_init(_spi.gpio(), GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, _spi.out());
        if (_spi.in())
            gpio_init(_spi.gpio(), GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, _spi.in());

        spi_parameter_struct params;
        spi_struct_para_init(&params);

        params.device_mode = SPI_MASTER;
        params.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
        params.frame_size = SPI_FRAMESIZE_8BIT;
        params.nss = SPI_NSS_SOFT;
        params.endian = _settings.endinaess();
        params.clock_polarity_phase = _settings.mode();
        params.prescale = _settings.prescale();

        spi_init(_spi.spi_periph(), &params);

        spi_crc_polynomial_set(_spi.spi_periph(), 7);
    }

    void SPI::begin()
    {
        spi_enable(_spi.spi_periph());
    }

    void SPI::end()
    {
        spi_disable(_spi.spi_periph());
    }

    const uint8_t SPI::transfer(uint8_t data)
    {
        while (RESET == spi_i2s_flag_get(_spi.spi_periph(), SPI_FLAG_TBE))
            ;

        spi_i2s_data_transmit(_spi.spi_periph(), data);

        while (RESET == spi_i2s_flag_get(_spi.spi_periph(), SPI_FLAG_RBNE))
            ;

        return spi_i2s_data_receive(_spi.spi_periph());
    }

} // namespace SPI
