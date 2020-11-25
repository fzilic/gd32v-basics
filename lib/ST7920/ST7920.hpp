#ifndef __ST7920_H
#define __ST7920_H

#include "SPI.hpp"
#include "GPIO.hpp"
#include "GPIO_pins.hpp"

#define ST7920_LINE0       0x80
#define ST7920_LINE1       0x90
#define ST7920_LINE2       0x88
#define ST7920_LINE3       0x98

enum ST7920Command : uint8_t
{
    LCD_CLS = 0x01,
    LCD_ADDRINC = 0x06,
    LCD_DISPLAYON = 0x0C,
    LCD_DISPLAYOFF = 0x08,
    LCD_BASIC = 0x30,
};

class ST7920
{
private:
    SPI _spi;
    GPIO _cs;

    void init();
    void send(uint8_t prefix, uint8_t data);

public:
    ST7920(SPI spi, GPIO cs)
        : _spi(spi),
          _cs(cs)
    {
        init();
    };

    void send(ST7920Command command);
    void send(uint8_t data);

    void writeText(uint8_t position, char *str);
};

#endif
