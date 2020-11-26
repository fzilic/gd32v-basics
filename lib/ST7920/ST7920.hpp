#ifndef __ST7920_H
#define __ST7920_H

#include "SPI.hpp"
#include "GPIO.hpp"
#include "GPIO_pins.hpp"

#define ST7920_LINE0 0x80
#define ST7920_LINE1 0x90
#define ST7920_LINE2 0x88
#define ST7920_LINE3 0x98

enum ST7920Command : uint8_t
{
    LCD_CLS = 0x01,
    LCD_ADDRINC = 0x06,
    LCD_DISPLAYON = 0x0C,
    LCD_DISPLAYOFF = 0x08,
    LCD_CURSORON = 0x0E,
    LCD_CURSORBLINK = 0x0F,
    LCD_BASIC = 0x30,
};

class ST7920
{
private:
    SPI::SPI _spi;
    GPIO::GPIO _cs;

public:
    ST7920(SPI::SPIPort spiPort,
           SPI::SPISettings spiSettings,
           GPIO::GPIO cs)
        : ST7920(
              SPI::SPI(
                  spiPort,
                  spiSettings),
              cs) {}

    ST7920(SPI::SPIPort spiPort,
           GPIO::GPIO cs)
        : ST7920(
              SPI::SPI(
                  spiPort,
                  SPI::SPISettings(
                      SPI::MSB,
                      SPI::MODE3,
                      SPI::PSC_256)),
              cs) {}

    ST7920(SPI::SPI spi,
           GPIO::GPIO cs)
        : _spi(spi),
          _cs(cs) {}

    void init();

    void send(uint8_t prefix, uint8_t data);
    void send(ST7920Command command);
    void send(uint8_t data);

    void writeText(uint8_t position, char *str);
};

#endif
