#include "LED.hpp"
// #include "SPI.hpp"
// #include "ST7920.hpp"
#include "LCDBuiltin.hpp"
#include "systick.hpp"

int main(void)
{
    LED led;
    led.red();

    // SPI spi(SPI_0, SPISettings(SPIEndianess::MSB, SPIMode::MODE3));
    LCDBuiltin lcd;
    delay_1ms(100);
    lcd.clear(LCDBuiltinColor::WHITE);

    delay_1ms(1000);
    led.green();

    // ST7920 lcd(SPI(SPI_0, SPISettings(SPIEndianess::MSB, SPIMode::MODE3)), GPIO(PA3, GPIOMode::MODE_OUT_PP));

    // delay_1ms(100);
    // lcd.writeText(ST7920_LINE0, (char *)"Hello");
    // lcd.writeText(ST7920_LINE1 + 4, (char *)"world");

    lcd.writeString(24, 0, (char *)"This is a TEST", LCDBuiltinColor::BLACK);
    lcd.writeString(24, 16, (char *)"This is a TEST", LCDBuiltinColor::BLUE);
    lcd.writeString(24, 32, (char *)"This is a TEST", LCDBuiltinColor::RED);
    lcd.writeString(24, 48, (char *)"This is a TEST", LCDBuiltinColor::MAGENTA);
    lcd.writeString(24, 64, (char *)"This is a TEST", LCDBuiltinColor::RED);
    delay_1ms(1000);

    while (1)
    {
        // lcd.writeText(ST7920_LINE0, (char *)"Hello");
        // lcd.writeText(ST7920_LINE1 + 4, (char *)"world");

        led.blue();
        delay_1ms(1000L);
        led.black();
        delay_1ms(1000L);
    }
}