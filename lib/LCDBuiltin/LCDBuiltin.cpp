#include "LCDBuiltin.hpp"
#include "LCDBuiltin_font.hpp"
#include "Timer.hpp"

namespace LCDBuiltin
{

    void LCDBuiltin::init()
    {
        // initialize devices
        _spi.init();
        _rst.init();
        _cs.init();
        _dc.init();

        // initialize display
        _spi.begin();
        _rst.clear();
        Timer::delay(200);
        _rst.set();
        Timer::delay(20);

        // optional BLK?

        writeCommandRaw(0x11); // turn off sleep mode
        Timer::delay(100);

        writeCommandRaw(0x21); // display inversion mode

        writeCommandRaw(0xB1); // Set the frame frequency of the full colors normal mode
                               // Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPA + BPA +2))
                               // fosc = 850kHz
        writeData8(0x05);      // RTNA
        writeData8(0x3A);      // FPA
        writeData8(0x3A);      // BPA

        writeCommandRaw(0xB2); // Set the frame frequency of the Idle mode
                               // Frame rate=fosc/((RTNB x 2 + 40) x (LINE + FPB + BPB +2))
                               // fosc = 850kHz
        writeData8(0x05);      // RTNB
        writeData8(0x3A);      // FPB
        writeData8(0x3A);      // BPB

        writeCommandRaw(0xB3); // Set the frame frequency of the Partial mode/ full colors
        writeData8(0x05);
        writeData8(0x3A);
        writeData8(0x3A);
        writeData8(0x05);
        writeData8(0x3A);
        writeData8(0x3A);

        writeCommandRaw(0xB4);
        writeData8(0x03);

        writeCommandRaw(0xC0);
        writeData8(0x62);
        writeData8(0x02);
        writeData8(0x04);

        writeCommandRaw(0xC1);
        writeData8(0xC0);

        writeCommandRaw(0xC2);
        writeData8(0x0D);
        writeData8(0x00);

        writeCommandRaw(0xC3);
        writeData8(0x8D);
        writeData8(0x6A);

        writeCommandRaw(0xC4);
        writeData8(0x8D);
        writeData8(0xEE);

        writeCommandRaw(0xC5); /*VCOM*/
        writeData8(0x0E);

        writeCommandRaw(0xE0);
        writeData8(0x10);
        writeData8(0x0E);
        writeData8(0x02);
        writeData8(0x03);
        writeData8(0x0E);
        writeData8(0x07);
        writeData8(0x02);
        writeData8(0x07);
        writeData8(0x0A);
        writeData8(0x12);
        writeData8(0x27);
        writeData8(0x37);
        writeData8(0x00);
        writeData8(0x0D);
        writeData8(0x0E);
        writeData8(0x10);

        writeCommandRaw(0xE1);
        writeData8(0x10);
        writeData8(0x0E);
        writeData8(0x03);
        writeData8(0x03);
        writeData8(0x0F);
        writeData8(0x06);
        writeData8(0x02);
        writeData8(0x08);
        writeData8(0x0A);
        writeData8(0x13);
        writeData8(0x26);
        writeData8(0x36);
        writeData8(0x00);
        writeData8(0x0D);
        writeData8(0x0E);
        writeData8(0x10);

        writeCommandRaw(0x3A); // define the format of RGB picture data
        writeData8(0x05);      // 16-bit/pixel

        writeCommandRaw(0x36);
        // Orientation

        switch (_orientation)
        {
        case VERTICAL:
            writeData8(0x08);
            break;
        case VERTICAL_FLIPPED:
            writeData8(0xC8);
            break;
        case HORIZONTAL:
            writeData8(0x78);
            break;
        default:
            writeData8(0xA8);
            break;
        }

        writeCommandRaw(0x29); // Display On
    }

    void LCDBuiltin::writeCommandRaw(uint8_t data)
    {
        _dc.clear();
        write(data);
    }

    void LCDBuiltin::write(uint8_t data)
    {
        _cs.clear();
        _spi.transfer(data);
        _cs.set();
    }

    void LCDBuiltin::writeData8(uint8_t data)
    {
        _dc.set();
        write(data);
    }

    void LCDBuiltin::writeData16(uint16_t data)
    {
        _dc.set();
        write(data >> 8);
        write(data);
    }

    void LCDBuiltin::clear()
    {
        clear(_backColor);
    }

    void LCDBuiltin::clear(LCDBuiltinColor color)
    {
        uint16_t i, j;
        setAddress(0, 0, _dispWidth - 1, _dispHeight - 1);
        for (i = 0; i < _dispWidth; i++)
        {
            for (j = 0; j < _dispHeight; j++)
            {
                writeData16(color);
            }
        }
    }

    void LCDBuiltin::writeString(uint16_t x, uint16_t y, const char *data, LCDBuiltinColor color)
    {
        while (*data != '\0')
        {
            if (x > _dispWidth - 16)
            {
                x = 0;
                y += 16;
            }
            if (y > _dispHeight - 16)
            {
                y = x = 0;
                clear(RED);
            }
            writeChar(x, y, *data, color);
            x += 8;
            data++;
        }
    }

    void LCDBuiltin::writeChar(uint16_t x, uint16_t y, uint8_t value, LCDBuiltinColor color, LCDBuiltinMode mode)
    {
        uint8_t temp;
        uint8_t pos, t;
        uint16_t x0 = x;

        if (x > _dispWidth - 16 || y > _dispHeight - 16)
            return;          //Settings window
        value = value - ' '; //Get offset value

        setAddress(x, y, x + 8 - 1, y + 16 - 1); //Set cursor position

        if (!mode) //Non-overlapping
        {
            for (pos = 0; pos < 16; pos++)
            {
                temp = asc2_1608[(uint16_t)value * 16 + pos]; //Call 1608 font
                for (t = 0; t < 8; t++)
                {
                    if (temp & 0x01)
                        writeData16(color);
                    else
                        writeData16(_backColor);
                    temp >>= 1;
                    x++;
                }
                x = x0;
                y++;
            }
        }
        else //overlapping mode
        {
            for (pos = 0; pos < 16; pos++)
            {
                temp = asc2_1608[(uint16_t)value * 16 + pos]; //Call 1608 font
                for (t = 0; t < 8; t++)
                {
                    if (temp & 0x01)
                        // LCD_DrawPoint(x + t, y + pos, color); //Draw a dot
                        temp >>= 1;
                }
            }
        }
    }

    void LCDBuiltin::setAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
    {
        switch (_orientation)
        {
        case VERTICAL:
            writeCommandRaw(0x2a); //Column address settings
            writeData16(x1 + 26);
            writeData16(x2 + 26);
            writeCommandRaw(0x2b); //Row address setting
            writeData16(y1 + 1);
            writeData16(y2 + 1);
            writeCommandRaw(0x2c); //Memory write
            break;
        case VERTICAL_FLIPPED:
            writeCommandRaw(0x2a); //Column address settings
            writeData16(x1 + 26);
            writeData16(x2 + 26);
            writeCommandRaw(0x2b); //Row address setting
            writeData16(y1 + 1);
            writeData16(y2 + 1);
            writeCommandRaw(0x2c); //Memory write
            break;
        case HORIZONTAL:
            writeCommandRaw(0x2a); //Column address settings
            writeData16(x1 + 1);
            writeData16(x2 + 1);
            writeCommandRaw(0x2b); //Row address setting
            writeData16(y1 + 26);
            writeData16(y2 + 26);
            writeCommandRaw(0x2c); //Memory write
            break;
        default:
            writeCommandRaw(0x2a); //Column address settings
            writeData16(x1 + 1);
            writeData16(x2 + 1);
            writeCommandRaw(0x2b); //Row address setting
            writeData16(y1 + 26);
            writeData16(y2 + 26);
            writeCommandRaw(0x2c); //Memory write
            break;
        }
    }

} // namespace LCDBuiltin
