#include <Arduino.h>
#include <wiring_extras.h>
#include <M0110aKeyboard.hpp>
#include <pins.hpp>

#define DEBUG

#include <debug.hpp>

M0110aKeyboard keyboard;

void setup()
{
    keyboard.begin(ROW_PINS, COL_PINS, MOD_PINS);
#ifdef DEBUG
    Serial.begin(9600);
#endif
}

void loop()
{
    debugf("--------------------\n\r");

    byte key_code = keyboard.getScanCode();

    delay(2000);
}
