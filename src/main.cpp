#include <Arduino.h>

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
    Serial.println("Initializing...");
#endif

    keyboard.begin(ROW_PINS, COL_PINS, MOD_PINS);
    mac.begin(DATA_PIN, CLK_PIN);
}

void loop()
{
    byte cmd = mac.readCmd();

#ifdef SERIAL_DEBUG
    Serial.printf("Command: %02X\n\r", cmd);
#endif

    switch (cmd)
    {
    case INQUIRY_CMD:
    {
#ifdef SERIAL_DEBUG
        Serial.println("Inquiry");
#endif
        inquiry();
        break;
    }
    case INSTANT_CMD:
    {
#ifdef SERIAL_DEBUG
        Serial.println("Instant");
#endif
        uint16_t key = keyboard.getScanCode();
#ifdef SERIAL_DEBUG
        Serial.printf("\tSending: %02X\n\r", key);
#endif
        mac.sendKey(key);
        break;
    }
    case MODEL_CMD:
    {
#ifdef SERIAL_DEBUG
        Serial.println("Model");
#endif
        mac.sendByte(MODEL_ID);
        break;
    }
    case TEST_CMD:
    {
#ifdef SERIAL_DEBUG
        Serial.println("Test");
#endif
        mac.sendByte(KB_ACK);
        break;
    }
    }
}

void inquiry()
{
    unsigned long start = millis();
    while ((millis() - start) < 250)
    {
        int key = keyboard.getScanCode();
        if (key != KEY_NONE)
        {
#ifdef SERIAL_DEBUG
            Serial.printf("\tSending: %02X\n\r", key);
#endif
            mac.sendKey(key);
            return;
        }
    }
#ifdef SERIAL_DEBUG
    Serial.printf("\tSending: %02X\n\r", KEY_NONE);
#endif
    mac.sendKey(KEY_NONE);
}
