#include "LED.hpp"
#include "ST7920.hpp"
#include "SSD1306.hpp"
#include "LCDBuiltin.hpp"
#include "systick.hpp"

#include <stdio.h>

uint8_t cnt = 0;
char dispBuffer[15];

LED::LED led;
LCDBuiltin::LCDBuiltin lcd(LCDBuiltin::BLACK, LCDBuiltin::HORIZONTAL_FLIPPED);

int main(void)
{
    // initialize devices
    led.init();
    lcd.init();

    led.set(LED::RED);
    lcd.clear();

    delay(500);
    led.set(LED::GREEN);

    lcd.writeString(24, 0, (char *)"This is a TEST", LCDBuiltin::WHITE);
    lcd.writeString(24, 16, (char *)"This is a TEST", LCDBuiltin::BLUE);
    lcd.writeString(24, 32, (char *)"This is a TEST", LCDBuiltin::RED);
    lcd.writeString(24, 48, (char *)"This is a TEST", LCDBuiltin::MAGENTA);

    // ST7920 lcdExtern(
    //     SPI(
    //         SPIPort(SPI2, RCU_GPIOB, RCU_SPI2, GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, NULL),
    //         SPISettings(SPIEndianess::MSB, SPIMode::MODE3, SPIPrescale::PSC_256)),
    //     GPIO(PB9, GPIOMode::MODE_OUT_PP));

    // lcdExtern.writeText(ST7920_LINE0, (char *)"Hello");
    // lcdExtern.writeText(ST7920_LINE1 + 4, (char *)"world");


    delay(500);

    while (1)
    {
        led.set(LED::MAGENTA);
        sprintf(dispBuffer, "Cnt:       %03d", cnt);
        lcd.writeString(24, 64, dispBuffer, LCDBuiltin::WHITE);
        // lcdExtern.writeText(ST7920_LINE0, dispBuffer);
        cnt++;
        delay(200);

        led.set(LED::BLACK);
        sprintf(dispBuffer, "Cnt:       %03d", cnt);
        lcd.writeString(24, 64, dispBuffer, LCDBuiltin::WHITE);
        // lcdExtern.writeText(ST7920_LINE0, dispBuffer);
        cnt++;
        delay(200);
    }
}