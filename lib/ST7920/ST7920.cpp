#include "ST7920.hpp"
#include "systick.hpp"

void ST7920::init()
{
    // initialize devices
    _spi.init();
    _cs.init();

    // initialize display
    _cs.clear();

    send(LCD_BASIC);
    send(LCD_BASIC);
    send(LCD_CLS);
    delay_1ms(5);
    send(LCD_ADDRINC);
    send(LCD_DISPLAYON);
}

void ST7920::send(uint8_t prefix, uint8_t data)
{
    _spi.begin();
    _cs.set();
    _spi.transfer(prefix);
    _spi.transfer(data & 0xF0);
    _spi.transfer(data << 4);
    _cs.clear();
    _spi.end();
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
    while (*str != '\0')
        send(*str++);
}