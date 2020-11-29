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

#if defined(BOOT_COUNTER_DEMO) || defined(LED_DEMO) || defined(LCD_BUILTIN_DEMO) || defined(EXT_SSD1306_DEMO)
#include "Timer.hpp"
#include <stdio.h>

uint64_t micros, deltaMicros;
uint64_t lcdMillis, lcdDeltaMillis;
uint64_t ledMillis, ledDeltaMillis;

char dispBuffer[15];
#endif // ANY

#ifdef EXT_SSD1306_DEMO
#include "I2C.hpp"
#include "u8g2.h"
#include "u8x8.h"
#endif // EXT_SSD1306_DEMO

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

#ifdef EXT_SSD1306_DEMO
u8x8_t u8x8;
uint8_t tile[8] = {0x0f, 0x0f, 0x0f, 0x0f, 0xf0, 0xf0, 0xf0, 0xf0};

I2C::I2C i2c0(I2C_0, I2C::I2CSettings(100000UL));

extern "C" uint8_t u8x8_gpio_and_delay_gd32v(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
extern "C" uint8_t u8x8_byte_hw_i2c_gd32v(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

void ssd1306Init(void);
#endif // EXT_SSD1306_DEMO

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

    lcd.writeString(24, 0, (char *)"Booting...    ", LCDBuiltin::RED);
    Timer::delay(100);
#endif

#ifdef EXT_SSD1306_DEMO
    ssd1306Init();
#endif

#if defined(LCD_BUILTIN_DEMO) || defined(LED_DEMO)
    Timer::delay(500);
#endif

#ifdef LED_DEMO
    led.set(LED::GREEN);
#endif

#ifdef EXT_SSD1306_DEMO
    u8x8_SetFont(&u8x8, u8x8_font_chroma48medium8_r);
    u8x8_DrawString(&u8x8, 0, 0, "Hello World");
    u8x8_DrawString(&u8x8, 3, 1, "ABCdefg");
    u8x8_DrawTile(&u8x8, 1, 1, 1, tile);
    u8x8_DrawTile(&u8x8, 2, 2, 1, tile);
    u8x8_DrawTile(&u8x8, 3, 3, 1, tile);
    u8x8_DrawTile(&u8x8, 4, 4, 1, tile);
    u8x8_DrawTile(&u8x8, 5, 5, 1, tile);
    u8x8_DrawTile(&u8x8, 6, 6, 1, tile);
#endif

#if defined(LCD_BUILTIN_DEMO) || defined(LED_DEMO)
    Timer::delay(500);
#endif

#ifdef LCD_BUILTIN_DEMO
    lcd.writeString(24, 0, (char *)"Running...    ", LCDBuiltin::GREEN);
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

#ifdef EXT_SSD1306_DEMO
void ssd1306Init(void)
{
#ifdef LCD_BUILTIN_DEMO
    lcd.writeString(24, 0, "u8x8 Setup    ", LCDBuiltin::YELLOW);
#endif
    u8x8_Setup(&u8x8, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_fast_i2c, u8x8_byte_hw_i2c_gd32v, u8x8_gpio_and_delay_gd32v);

#ifdef LCD_BUILTIN_DEMO
    lcd.writeString(24, 0, "u8x8 Init     ", LCDBuiltin::YELLOW);
#endif
    u8x8_InitDisplay(&u8x8);

#ifdef LCD_BUILTIN_DEMO
    lcd.writeString(24, 0, "u8x8 CD       ", LCDBuiltin::YELLOW);
#endif
    u8x8_ClearDisplay(&u8x8);

#ifdef LCD_BUILTIN_DEMO
    lcd.writeString(24, 0, "u8x8 Set PS    ", LCDBuiltin::YELLOW);
#endif
    u8x8_SetPowerSave(&u8x8, 0);
}

extern "C" uint8_t u8x8_gpio_and_delay_gd32v(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        // called once during init phase of u8g2/u8x8
        // can be used to setup pins
        i2c0.init();
        // i2c0.begin();
        break;
    case U8X8_MSG_DELAY_NANO:
        // delay arg_int * 1 nano second
        Timer::delayMicroseconds(arg_int == 0 ? 0 : 1);
        break;
    case U8X8_MSG_DELAY_100NANO:
        // delay arg_int * 100 nano seconds
        /* not used at the moment */
        Timer::delayMicroseconds(arg_int == 0 ? 0 : 1);
        break;
    case U8X8_MSG_DELAY_10MICRO:
        // delay arg_int * 10 micro seconds
        /* not used at the moment */
        Timer::delayMicroseconds(arg_int * 10);
        break;
    case U8X8_MSG_DELAY_MILLI:
        // delay arg_int * 1 milli second
        Timer::delay(arg_int);
        break;
    case U8X8_MSG_DELAY_I2C:
        // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
        // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
        Timer::delayMicroseconds(arg_int <= 2 ? 5 : 2);
        break;
    case U8X8_MSG_GPIO_D0:
        // D0 or SPI clock pin: Output level in arg_int
        //case U8X8_MSG_GPIO_SPI_CLOCK:
        break;
    case U8X8_MSG_GPIO_D1:
        // D1 or SPI data pin: Output level in arg_int
        //case U8X8_MSG_GPIO_SPI_DATA:
        break;
    case U8X8_MSG_GPIO_D2:
        // D2 pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_D3:
        // D3 pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_D4:
        // D4 pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_D5:
        // D5 pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_D6:
        // D6 pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_D7:
        // D7 pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_E:
        // E/WR pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_CS:
        // CS (chip select) pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_DC:
        // DC (data/cmd, A0, register select) pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_RESET:
        // Reset pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_CS1:
        // CS1 (chip select) pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_CS2:
        // CS2 (chip select) pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_I2C_CLOCK:
        // arg_int=0: Output low at I2C clock pin
        // arg_int=1: Input dir with pullup high for I2C clock pin
        break;
    case U8X8_MSG_GPIO_I2C_DATA:
        // arg_int=0: Output low at I2C data pin
        // arg_int=1: Input dir with pullup high for I2C data pin
        break;
    case U8X8_MSG_GPIO_MENU_SELECT:
        u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_NEXT:
        u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_PREV:
        u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_HOME:
        u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
        break;
    default:
        u8x8_SetGPIOResult(u8x8, 1); // default return value
        break;
    }
    return 1;
}

extern "C" uint8_t u8x8_byte_hw_i2c_gd32v(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    uint32_t address;
    uint32_t address_modified;
    // char buffer[20];

    switch (msg)
    {
    case U8X8_MSG_BYTE_SEND:
        // Wire.write((uint8_t *)arg_ptr, (int)arg_int);
        // lcd.writeString(24, 0, "I2C Send      ", LCDBuiltin::WHITE);

        if (!i2c0.isTransmitting())
            i2c0.begin();

        address = u8x8_GetI2CAddress(u8x8);
        address_modified = address >> 1;
        // sprintf(buffer, "ADR: %02X %02X %03d", address, address_modified, arg_int);
        // lcd.writeString(24, 16, buffer, LCDBuiltin::WHITE);
        i2c0.transmit(address_modified, (uint8_t *)arg_ptr, arg_int);
        break;
    case U8X8_MSG_BYTE_INIT:
        // Wire.begin();
        // lcd.writeString(24, 0, "I2C Begin     ", LCDBuiltin::WHITE);
        i2c0.begin();
        break;
    case U8X8_MSG_BYTE_SET_DC:
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
        // if (u8x8->display_info->i2c_bus_clock_100kHz >= 4)
        // {
        // Wire.setClock(400000L);
        // }
        // Wire.beginTransmission(u8x8_GetI2CAddress(u8x8) >> 1);
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        // Wire.endTransmission();
        // lcd.writeString(24, 0, "I2C End       ", LCDBuiltin::WHITE);
        i2c0.end();
        break;
    default:
        return 0;
    }
    return 1;
}

#endif
