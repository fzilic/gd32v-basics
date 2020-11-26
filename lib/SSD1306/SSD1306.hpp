#ifndef __SSD1306_H
#define __SSD1306_H

#define WIRE_MAX 32 // if needed at all

#include "I2C.hpp"
#include "GPIO.hpp"

namespace SSD1306
{

    enum SSD1306Command : uint8_t
    {
        MEMORY_MODE = 0x20,
        DEACTIVATE_SCROLL = 0x2E,
        SET_START_LINE = 0x40,
        SETCONTRAST = 0x81,
        CHARGE_PUMP = 0x8D,
        SEGREMAP = 0xA0,
        DISPLAYALLON_RESUME = 0xA4,
        NORMALDISPLAY = 0xA6,
        SET_MULTIPLEX = 0xA8,
        DISPLAY_OFF = 0xAE,
        DISPLAYON = 0xAF,
        COMSCANDEC = 0xC8,
        SET_DISPLAY_OFFSET = 0xD3,
        SET_DISPLAYCLOCK_DIV = 0xD5,
        SETPRECHARGE = 0xD9,
        SETCOMPINS = 0xDA,
        SETVCOMDETECT = 0xDB
    };

    class SSD1306
    {
    private:
        I2C::I2C _i2c;
        GPIO::GPIO _reset;

        uint8_t _width;
        uint8_t _height;

        uint32_t _address;

        uint32_t _clockDuring; //400000UL
        uint32_t _clockAfter;  //100000UL

        uint8_t *_buffer;

        void command(const uint8_t command);
        void commands(const uint8_t *commands, uint8_t count);

    public:
        bool init();

        void clearDisplay();
        void display();

        // void setTextSize(uint8_t size);
        // void setTextColor(uint16_t color);
        // void setCursor(int16_t x, int16_t y);
        // void cp437();
        // void write();
    };

}; // namespace SSD1306

#endif