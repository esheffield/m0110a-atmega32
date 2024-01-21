#include <Arduino.h>

#define SERIAL_DEBUG

#include <M0110aKeyboard.hpp>
#include <KeyboardProtocol.hpp>
#include <pins.hpp>

void inquiry();

M0110aKeyboard keyboard;
KeyboardProtocol mac;

void setup()
{
#ifdef SERIAL_DEBUG
    Serial.begin(9600);
#endif

    keyboard.begin(ROW_PINS, COL_PINS, MOD_PINS);
    mac.begin(DATA_PIN, CLK_PIN);
}

void loop()
{
#ifdef SERIAL_DEBUG
    Serial.printf(".");
#endif
    byte key_code = keyboard.getScanCode();

#ifdef SERIAL_DEBUG
    if(key_code) {
        Serial.printf("--------------------\n\r");
        Serial.printf("KEY_CODE: %02x\n\r", key_code);
    }
#endif
//   switch (readCmd()) {
//     case 0x10:
//       inquiry();
//       break;

//     case 0x14: // instant
//       sendKey(getKeyTransition());
//       break;

//     case 0x16: // model number
//       sendByte(0x0b);
//       break;

//     case 0x36: // test
//       sendByte(0x7d);
//       break;
//   }

    // delay(2000);
}

void inquiry() {
  unsigned long start = millis();
  while ((millis() - start) < 250) {
    int key = keyboard.getScanCode();
    if (key != KEY_NULL) {
      mac.sendKey(key);
      return;
    }
  }
  mac.sendKey(KEY_NULL);
}
