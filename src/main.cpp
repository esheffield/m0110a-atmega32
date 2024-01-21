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

    Serial.println("Initializing...");
    keyboard.begin(ROW_PINS, COL_PINS, MOD_PINS);
    mac.begin(DATA_PIN, CLK_PIN);
}

void loop()
{
    //     byte key_code = keyboard.getScanCode();

    // #ifdef SERIAL_DEBUG
    //     if(key_code) {
    //         Serial.printf("--------------------\n\r");
    //         Serial.printf("KEY_CODE: %02x\n\r", key_code);
    //     }
    // #endif

    byte cmd = mac.readCmd();

    Serial.printf("Command: %02X\n\r", cmd);

    switch (cmd)
    {
    case INQUIRY_CMD:
    {
        Serial.println("Inquiry");
        inquiry();
        break;
    }
    case INSTANT_CMD:
    {
        Serial.println("Instant");
        uint16_t key = keyboard.getScanCode();
        Serial.printf("\tSending: %02X\n\r", key);
        mac.sendKey(key);
        break;
    }
    case MODEL_CMD:
    {
        Serial.println("Model");
        mac.sendByte(MODEL_ID);
        break;
    }
    case TEST_CMD:
    {
        Serial.println("Test");
        mac.sendByte(KB_ACK);
        break;
    }
    }

    // delay(2000);
}

void inquiry()
{
    unsigned long start = millis();
    while ((millis() - start) < 250)
    {
        int key = keyboard.getScanCode();
        if (key != KEY_NONE)
        {
            Serial.printf("\tSending: %02X\n\r", key);
            mac.sendKey(key);
            return;
        }
    }
    Serial.printf("\tSending: %02X\n\r", KEY_NONE);
    mac.sendKey(KEY_NONE);
}
