#include "SPI.hpp"

extern "C"
{
#include "gd32vf103_rcu.h"
#include "gd32vf103_gpio.h"
}

void SPI::init()
{
    rcu_periph_clock_enable(_spi._rcu_gpio_periph);
    rcu_periph_clock_enable(_spi._rcu_spi_periph);
 	rcu_periph_clock_enable(RCU_AF);

    gpio_init(_spi._gpio, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, _spi._out);
    gpio_init(_spi._gpio, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, _spi._in);

    spi_init(_spi._spi_periph, &(spi_parameter_struct){
                                   .device_mode = SPI_MASTER,
                                   .trans_mode = SPI_TRANSMODE_FULLDUPLEX,
                                   .frame_size = SPI_FRAMESIZE_8BIT,
                                   .nss = SPI_NSS_SOFT,
                                   .endian = _settings._endinaess,
                                   .clock_polarity_phase = _settings._mode,
                                   .prescale = SPI_PSC_256});

    spi_crc_polynomial_set(_spi._spi_periph, 7);

    spi_enable(_spi._spi_periph);
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