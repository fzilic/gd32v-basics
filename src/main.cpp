#include "LED.hpp"
#include "LCDBuiltin.hpp"
#include "Timer.hpp"

#include <stdio.h>

uint64_t micros, deltaMicros;
uint64_t lcdMillis, lcdDeltaMillis;
uint64_t ledMillis, ledDeltaMillis;

volatile uint8_t cnt = 0;
volatile uint64_t lastBootDebounce;

char dispBuffer[15];

LED::LEDColor ledColor;
LED::LED led;

LCDBuiltin::LCDBuiltin lcd(LCDBuiltin::BLACK, LCDBuiltin::HORIZONTAL_FLIPPED);

GPIO::GPIO boot = GPIO::GPIO(PA8, GPIO::MODE_IPU);

void key_exti_init(void);
extern "C" void EXTI5_9_IRQHandler(void);

int main(void)
{
    // initialize devices
    led.init();
    lcd.init();
    boot.init();
    key_exti_init();

    led.set(LED::RED);
    lcd.clear();
    lcd.writeString(24, 0, (char *)"Booting...", LCDBuiltin::WHITE);

    Timer::delay(500);
    led.set(LED::GREEN);

    Timer::delay(500);
    lcd.writeString(24, 0, (char *)"Running...", LCDBuiltin::WHITE);

    ledColor = LED::BLACK;

    // test delay / millis
    micros = Timer::micros();

    // update intervals
    lcdMillis = Timer::millis();
    ledMillis = Timer::millis();

    while (1)
    {
        deltaMicros = Timer::micros() - micros;
        lcdDeltaMillis = Timer::millis() - lcdMillis;
        ledDeltaMillis = Timer::millis() - ledMillis;

        // lcd every 50ms
        if (lcdDeltaMillis >= 50)
        {
            micros = Timer::micros();
            lcdMillis = Timer::millis();

            sprintf(dispBuffer, "Cnt:       %03d", cnt);
            lcd.writeString(24, 16, dispBuffer, LCDBuiltin::BLUE);

            sprintf(dispBuffer, "us:    %07lu", deltaMicros);
            lcd.writeString(24, 32, dispBuffer, LCDBuiltin::RED);
            sprintf(dispBuffer, "ms:    %07lu", lcdDeltaMillis);
            lcd.writeString(24, 48, dispBuffer, LCDBuiltin::YELLOW);

            sprintf(dispBuffer, "xxxxxxxxxxxxxx");
            lcd.writeString(24, 64, dispBuffer, LCDBuiltin::MAGENTA);
        }

        if (ledDeltaMillis >= 500)
        {
            ledMillis = Timer::millis();
            // toggle LED
            if (ledColor == LED::BLACK)
                ledColor = LED::MAGENTA;
            else
                ledColor = LED::BLACK;
            led.set(ledColor);
        }
    }
}

void key_exti_init(void)
{
    /* enable the AF clock */
    rcu_periph_clock_enable(RCU_AF);
    /* enable and set key EXTI interrupt to the specified priority */
    eclic_global_interrupt_enable();
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL3_PRIO1);
    eclic_irq_enable(EXTI5_9_IRQn, 1, 1);

    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_8);

    /* configure key EXTI line */
    exti_init(EXTI_8, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_8);
}

extern "C" void EXTI5_9_IRQHandler(void)
{
    if (RESET != exti_interrupt_flag_get(EXTI_8))
    {

        if (!boot.read() && (Timer::millis() - lastBootDebounce) > 40)
        {
            cnt++;
        }

        lastBootDebounce = Timer::millis();

        exti_interrupt_flag_clear(EXTI_8);
    }
}