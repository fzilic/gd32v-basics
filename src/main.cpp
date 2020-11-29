#include "Configuration.h"

#ifdef BOOT_COUNTER_DEMO
#include "GPIO_pins.hpp"
#include "GPIO.hpp"
#endif // BOOT_COUNTER_DEMO

#ifdef LED_DEMO
#include "LED.hpp"
#endif //LED_DEMO

#ifdef LCD_BUILTIN_DEMO
#include "LCDBuiltin.hpp"
#endif //LCD_BUILTIN_DEMO

#if defined(BOOT_COUNTER_DEMO) || defined(LED_DEMO) || defined(LCD_BUILTIN_DEMO)
#include "Timer.hpp"
#include <stdio.h>

uint64_t micros, deltaMicros;
uint64_t lcdMillis, lcdDeltaMillis;
uint64_t ledMillis, ledDeltaMillis;

char dispBuffer[15];
#endif // ANY

#ifdef BOOT_COUNTER_DEMO
volatile uint8_t cnt = 0;
volatile uint64_t lastBootDebounce;
#endif // BOOT_COUNTER_DEMO

#ifdef LED_DEMO
LED::LEDColor ledColor;
LED::LED led;
#endif

#ifdef LCD_BUILTIN_DEMO
LCDBuiltin::LCDBuiltin lcd(LCDBuiltin::BLACK, LCDBuiltin::HORIZONTAL_FLIPPED);
#endif

#ifdef BOOT_COUNTER_DEMO
GPIO::GPIO boot = GPIO::GPIO(PA8, GPIO::MODE_IPU);

void key_exti_init(void);
extern "C" void EXTI5_9_IRQHandler(void);
#endif

int main(void)
{
    // initialize devices
#ifdef LED_DEMO
    led.init();
#endif

#ifdef LCD_BUILTIN_DEMO
    lcd.init();
#endif

#ifdef BOOT_COUNTER_DEMO
    boot.init();
    key_exti_init();
#endif

#ifdef LED_DEMO
    led.set(LED::RED);
#endif

#ifdef LCD_BUILTIN_DEMO
    lcd.clear();
    lcd.writeString(24, 0, (char *)"Booting...", LCDBuiltin::YELLOW);
#endif

#if defined(LCD_BUILTIN_DEMO) || defined(LED_DEMO)
    Timer::delay(500);
#endif

#ifdef LED_DEMO
    led.set(LED::GREEN);
#endif

#if defined(LCD_BUILTIN_DEMO) || defined(LED_DEMO)
    Timer::delay(500);
#endif

#ifdef LCD_BUILTIN_DEMO
    lcd.writeString(24, 0, (char *)"Running...", LCDBuiltin::GREEN);
#endif

#ifdef LED_DEMO
    ledColor = LED::BLACK;
#endif

    // test delay / millis
#if defined(BOOT_COUNTER_DEMO) || defined(LED_DEMO) || defined(LCD_BUILTIN_DEMO)
    micros = Timer::micros();

    // update intervals
    lcdMillis = Timer::millis();
    ledMillis = Timer::millis();
#endif
    while (1)
    {
#if defined(BOOT_COUNTER_DEMO) || defined(LED_DEMO) || defined(LCD_BUILTIN_DEMO)
        deltaMicros = Timer::micros() - micros;
        lcdDeltaMillis = Timer::millis() - lcdMillis;
        ledDeltaMillis = Timer::millis() - ledMillis;

        // lcd every 50ms
        if (lcdDeltaMillis >= 50)
        {
            micros = Timer::micros();
            lcdMillis = Timer::millis();

#ifdef LCD_BUILTIN_DEMO
#ifdef BOOT_COUNTER_DEMO
            sprintf(dispBuffer, "Cnt:       %03d", cnt);
            lcd.writeString(24, 16, dispBuffer, LCDBuiltin::BLUE);
#endif // BOOT_COUNTER_DEMO

            sprintf(dispBuffer, "us:    %07lu", deltaMicros);
            lcd.writeString(24, 32, dispBuffer, LCDBuiltin::RED);
            sprintf(dispBuffer, "ms:    %07lu", lcdDeltaMillis);
            lcd.writeString(24, 48, dispBuffer, LCDBuiltin::YELLOW);

            sprintf(dispBuffer, "xxxxxxxxxxxxxx");
            lcd.writeString(24, 64, dispBuffer, LCDBuiltin::MAGENTA);
#endif // LCD_BUILTIN_DEMO
        }

#ifdef LED_DEMO
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
#endif // LED_DEMO
#endif
    }
}

#ifdef BOOT_COUNTER_DEMO
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
#endif // BOOT_COUNTER_DEMO