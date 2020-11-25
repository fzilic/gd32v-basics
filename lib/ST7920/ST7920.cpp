#include "ST7920.hpp"

// extern "C"
// {
// #include "systick.h"
// }
void ST7920::init()
{
    _cs.clear();

    send(LCD_BASIC);
    send(LCD_BASIC);
    send(LCD_CLS);
    // delay_1ms(2);
    send(LCD_ADDRINC);
    send(LCD_DISPLAYON);
}

void ST7920::send(uint8_t prefix, uint8_t data)
{
    _cs.set();
    _spi.transmit(prefix);
    _spi.transmit(data & 0xF0);
    _spi.transmit(data << 4);
    _cs.clear();
}

void ST7920::send(ST7920Command command)
{
    send(0xF8, command);
}

void ST7920::send(uint8_t data)
{
    send(0xFA, data);
}

void ST7920::writeText(uint8_t position, char *str)
{
    send(LCD_BASIC);
    send(position);
    while (*str)
        send(*str++);
}