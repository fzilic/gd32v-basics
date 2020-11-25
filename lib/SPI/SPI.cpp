#include "SPI.hpp"

extern "C"
{
#include "gd32vf103_rcu.h"
#include "gd32vf103_gpio.h"
}

// SPIPort(SPI0, RCU_GPIOA, RCU_SPI0, GPIOA, GPIO_PIN_5 | GPIO_PIN_7, GPIO_PIN_6)
void SPI::init()
{
    rcu_periph_clock_enable(_spi._rcu_gpio_periph);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(_spi._rcu_spi_periph);

    gpio_init(_spi._gpio, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, _spi._out);
    if (_spi._in)
        gpio_init(_spi._gpio, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, _spi._in);

    spi_struct_para_init(&_params);

    _params.device_mode = SPI_MASTER;
    _params.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    _params.frame_size = SPI_FRAMESIZE_8BIT;
    _params.nss = SPI_NSS_SOFT;
    _params.endian = _settings._endinaess;
    _params.clock_polarity_phase = _settings._mode;
    _params.prescale = _settings._prescale;

    spi_init(_spi._spi_periph, &_params);

    spi_crc_polynomial_set(_spi._spi_periph, 7);
}

uint8_t SPI::transmit(uint8_t data)
{
    while (RESET == spi_i2s_flag_get(_spi._spi_periph, SPI_FLAG_TBE))
        ;

    spi_i2s_data_transmit(_spi._spi_periph, data);

    while (RESET == spi_i2s_flag_get(_spi._spi_periph, SPI_FLAG_RBNE))
        ;

    return spi_i2s_data_receive(_spi._spi_periph);
}

void SPI::begin()
{
    spi_enable(_spi._spi_periph);
}

void SPI::end()
{
    spi_disable(_spi._spi_periph);
}