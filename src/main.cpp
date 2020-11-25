#include "LED.hpp"
#include "ST7920.hpp"
#include "LCDBuiltin.hpp"
#include "systick.hpp"

#include <stdio.h>

int main(void)
{
    uint8_t cnt = 0;
    char dispBuffer[15];

    LED led;
    led.red();

    LCDBuiltin lcd;
    lcd.clear(LCDBuiltinColor::WHITE);

    delay_1ms(200);
    led.green();

    lcd.writeString(24, 0, (char *)"This is a TEST", LCDBuiltinColor::BLACK);
    lcd.writeString(24, 16, (char *)"This is a TEST", LCDBuiltinColor::BLUE);
    lcd.writeString(24, 32, (char *)"This is a TEST", LCDBuiltinColor::RED);
    lcd.writeString(24, 48, (char *)"This is a TEST", LCDBuiltinColor::MAGENTA);
    delay_1ms(200);

    // ST7920 lcdExtern(
    //     SPI(
    //         SPIPort(SPI2, RCU_GPIOB, RCU_SPI2, GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, NULL),
    //         SPISettings(SPIEndianess::MSB, SPIMode::MODE3, SPIPrescale::PSC_256)),
    //     GPIO(PB9, GPIOMode::MODE_OUT_PP));

    // lcdExtern.writeText(ST7920_LINE0, (char *)"Hello");
    // lcdExtern.writeText(ST7920_LINE1 + 4, (char *)"world");

    GPIO tPin(PB9, GPIOMode::MODE_OUT_PP);

    delay_1ms(200);

    while (1)
    {
        led.blue();
        sprintf(dispBuffer, "Cnt:       %03d", cnt);
        lcd.writeString(24, 64, dispBuffer, LCDBuiltinColor::BLACK);
        // lcdExtern.writeText(ST7920_LINE0, dispBuffer);
        cnt++;
        tPin.set();
        delay_1ms(200L);

        led.black();
        sprintf(dispBuffer, "Cnt:       %03d", cnt);
        lcd.writeString(24, 64, dispBuffer, LCDBuiltinColor::BLACK);
        // lcdExtern.writeText(ST7920_LINE0, dispBuffer);
        cnt++;
        tPin.clear();
        delay_1ms(200L);
    }
}