#include "LED.hpp"
#include "LCDBuiltin.hpp"
#include "Timer.hpp"

#include <stdio.h>

uint8_t cnt = 0;
uint64_t milis, deltaMilis, micros, deltaMicros;

uint8_t bootCnt = 0, lastBootCnt = 0;
bool bootState = false;
bool lastBootState = false;
uint64_t lastBootDebounce;

char dispBuffer[15];

LED::LEDColor ledColor;
LED::LED led;

LCDBuiltin::LCDBuiltin lcd(LCDBuiltin::BLACK, LCDBuiltin::HORIZONTAL_FLIPPED);

GPIO::GPIO boot = GPIO::GPIO(PA8, GPIO::MODE_IPU);

void readBoot();

int main(void)
{
    // initialize devices
    led.init();
    lcd.init();
    boot.init();

    led.set(LED::RED);
    lcd.clear();

    Timer::delay(500);
    led.set(LED::GREEN);

    lcd.writeString(24, 0, (char *)"This is a TEST", LCDBuiltin::WHITE);

    Timer::delay(500);

    ledColor = LED::BLACK;

    // test delay / milis
    micros = Timer::micros();
    milis = Timer::milis();
    while (1)
    {
        deltaMicros = Timer::micros() - micros;
        deltaMilis = Timer::milis() - milis;

        readBoot();
        if (bootState && lastBootCnt == bootCnt)
        {
            bootCnt++;
        }
        else if (!bootState)
        {
            lastBootCnt = bootCnt;
        }

        if (deltaMilis >= 500)
        {
            micros = Timer::micros();
            milis = Timer::milis();

            sprintf(dispBuffer, "boot:      %03d", bootCnt);
            lcd.writeString(24, 16, dispBuffer, LCDBuiltin::BLUE);

            sprintf(dispBuffer, "us:    %07lu", deltaMicros);
            lcd.writeString(24, 32, dispBuffer, LCDBuiltin::RED);
            sprintf(dispBuffer, "ms:    %07lu", deltaMilis);
            lcd.writeString(24, 48, dispBuffer, LCDBuiltin::YELLOW);

            // print and inc counter
            sprintf(dispBuffer, "Cnt:       %03d", cnt);
            lcd.writeString(24, 64, dispBuffer, LCDBuiltin::MAGENTA);
            cnt++;

            // toggle LED
            if (ledColor == LED::BLACK)
                ledColor = LED::MAGENTA;
            else
                ledColor = LED::BLACK;
            led.set(ledColor);
        }
    }
}

void readBoot()
{
    bool state = boot.read();
    if (state != lastBootState)
    {
        lastBootDebounce = Timer::milis();
    }
    if ((Timer::milis() - lastBootDebounce) > 40 && state != bootState)
    {
        bootState = state;
    }

    lastBootState = state;
}