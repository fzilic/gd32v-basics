#ifndef __OLED_BUILTIN_H
#define __OLED_BUILTIN_H

#include "SPI.hpp"
#include "GPIO.hpp"
#include "GPIO_pins.hpp"

namespace LCDBuiltin
{
    enum LCDBuiltinOrientation
    {
        VERTICAL,
        VERTICAL_FLIPPED,
        HORIZONTAL,
        HORIZONTAL_FLIPPED
    };

    enum LCDBuiltinMode : uint8_t
    {
        NON_OVERLAPPING = 0,
        OVERLAPPING = 1
    };

    enum LCDBuiltinColor : uint16_t
    {
        WHITE = 0xFFFF,
        BLACK = 0x0000,
        BLUE = 0x001F,
        BRED = 0XF81F,
        GRED = 0XFFE0,
        GBLUE = 0X07FF,
        RED = 0xF800,
        MAGENTA = 0xF81F,
        GREEN = 0x07E0,
        CYAN = 0x7FFF,
        YELLOW = 0xFFE0,
        BROWN = 0XBC40,
        BRRED = 0XFC07,
        GRAY = 0X8430,
        DARKBLUE = 0X01CF,
        LIGHTBLUE = 0X7D7C,
        GRAYBLUE = 0X5458,
        LIGHTGREEN = 0X841F,
        LGRAY = 0XC618,
        LGRAYBLUE = 0XA651,
        LBBLUE = 0X2B12
    };

    class LCDBuiltin
    {
    private:
        SPI::SPI _spi;
        GPIO::GPIO _rst;
        GPIO::GPIO _cs;
        GPIO::GPIO _dc;
        LCDBuiltinOrientation _orientation;
        uint16_t _dispHeight;
        uint16_t _dispWidth;

        void writeCommandRaw(uint8_t data);
        void write(uint8_t data);
        void writeChar(uint16_t x, uint16_t y, uint8_t value, LCDBuiltinColor color, LCDBuiltinMode mode = NON_OVERLAPPING);

    public:
    public:
        LCDBuiltinColor _backColor;
        LCDBuiltin()
            : LCDBuiltin(WHITE, HORIZONTAL) {}
        LCDBuiltin(LCDBuiltinColor backColor,
                   LCDBuiltinOrientation orientation)
            : LCDBuiltin(backColor,
                         orientation,
                         SPI::SPIPrescale::PSC_2) {}
        LCDBuiltin(LCDBuiltinColor backColor,
                   LCDBuiltinOrientation orientation,
                   SPI::SPIPrescale prescale)
            : _spi(
                  SPI::SPI(
                      SPI::SPIPort(SPI0, RCU_GPIOA, RCU_SPI0, GPIOA, GPIO_PIN_5 | GPIO_PIN_7, GPIO_PIN_6),
                      SPI::SPISettings(SPI::SPIEndianess::MSB, SPI::SPIMode::MODE3, prescale))),
              _rst(GPIO::GPIO(PB1, GPIO::MODE_OUT_PP)),
              _cs(GPIO::GPIO(PB2, GPIO::MODE_OUT_PP)),
              _dc(GPIO::GPIO(PB0, GPIO::MODE_OUT_PP)),
              _orientation(orientation),
              _backColor(backColor) {}

        void init();

        void writeData8(uint8_t data);
        void writeData16(uint16_t data);

        void clear();
        void clear(LCDBuiltinColor color);

        void writeString(uint16_t x, uint16_t y, const char *data, LCDBuiltinColor color);
        void setAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    };

}; // namespace LCDBuiltin

#endif