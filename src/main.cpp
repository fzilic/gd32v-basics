#include "LED.hpp"
#include "SPI.hpp"
#include "ST7920.hpp"

extern "C"
{
#include "systick.h"
}

int main(void)
{
    LED led;
    led.red();

    // SPI spi(SPI_0, SPISettings(SPIEndianess::MSB, SPIMode::MODE3));

    delay_1ms(100);
    ST7920 lcd(SPI(SPI_0, SPISettings(SPIEndianess::MSB, SPIMode::MODE3)), GPIO(PA3, GPIOMode::MODE_OUT_PP));

    delay_1ms(100);
    lcd.writeText(ST7920_LINE0, (char *)"Hello");
    lcd.writeText(ST7920_LINE1 + 4, (char *)"world");

    led.green();
    delay_1ms(100);

    while (1)
    {
        lcd.writeText(ST7920_LINE0, (char *)"Hello");
        lcd.writeText(ST7920_LINE1 + 4, (char *)"world");

        led.blue();
        delay_1ms(1000L);
        led.black();
        delay_1ms(1000L);
    }
}