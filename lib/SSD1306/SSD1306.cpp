#include "SSD1306.hpp"
#include "Timer.hpp"
#include <cstdlib>
#include <cstring>

namespace SSD1306
{
    void SSD1306::command(const uint8_t command)
    {
        uint8_t *data = (uint8_t *)std::malloc(2);
        *data = 0x00;
        *(data + 1) = command;

        _i2c.transmit(_address, data, 2);

        std::free(data);
    }

    void SSD1306::commands(const uint8_t *commands, uint8_t count)
    {
        uint8_t *data = (uint8_t *)std::malloc(count + 1);
        *data = 0x00;
        std::memcpy(data + 1, commands, count);

        _i2c.transmit(_address, data, count + 1);

        std::free(data);
    }

    bool SSD1306::init()
    {
        _i2c.init();
        if (&_reset)
        {
            _reset.init();

            // reset display
            _reset.set();
            Timer::delay(1);
            _reset.clear();
            Timer::delay(10);
            _reset.set();
        }

        if (!_buffer)
        {
            _buffer = (uint8_t *)std::malloc(_width * ((_height + 7) / 8));
            return !_buffer;
        }

        clearDisplay();

        // TODO this might be too verbose, after getting display working, try by just sending one big array
        const uint8_t init1[] = {
            DISPLAY_OFF,
            SET_DISPLAYCLOCK_DIV,
            0x80, // suggested ratio
            SET_MULTIPLEX};

        commands(init1, sizeof(init1));

        command(_height - 1);

        const uint8_t init2[] = {
            SET_DISPLAY_OFFSET,
            0x00,
            SET_START_LINE | 0x00,
            CHARGE_PUMP};
        commands(init2, sizeof(init2));

        command(0x14); // TODO (vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0x14

        const uint8_t init3[] = {
            MEMORY_MODE,
            0x00,
            SEGREMAP | 0x1,
            COMSCANDEC};
        commands(init3, sizeof(init3));

        // TODO
        // uint8_t comPins = 0x02;
        // contrast = 0x8F;

        // if ((WIDTH == 128) && (HEIGHT == 32)) {
        //     comPins = 0x02;
        //     contrast = 0x8F;
        // } else if ((WIDTH == 128) && (HEIGHT == 64)) {
        //     comPins = 0x12;
        //     contrast = (vccstate == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF;
        // } else if ((WIDTH == 96) && (HEIGHT == 16)) {
        //     comPins = 0x2; // ada x12
        //     contrast = (vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0xAF;
        // } else {
        //     // Other screen varieties -- TBD
        // }

        command(SETCOMPINS);
        command(0x12);
        command(SETCONTRAST);
        command(0xCF);

        command(SETPRECHARGE);
        command(0xF1); // todo ssd1306_command1((vccstate == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);

        const uint8_t init5[] = {
            SETVCOMDETECT,
            0x40,
            DISPLAYALLON_RESUME,
            NORMALDISPLAY,
            DEACTIVATE_SCROLL,
            DISPLAYON};
        commands(init5, sizeof(init5));

        return true;
    }

    void SSD1306::clearDisplay()
    {
        std::memset(_buffer, 0, _width * ((_height + 7) / 8));
    }

    void SSD1306::display()
    {
    }

} // namespace SSD1306