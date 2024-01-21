#include <Arduino.h>

#ifndef KEYBOARDPROTOCOL_HPP
#define KEYBOARDPROTOCOL_HPP

#define INQUIRY_CMD 0x10
#define INSTANT_CMD 0x14
#define MODEL_CMD 0x16
#define TEST_CMD 0x7D

class KeyboardProtocol
{
private:
    byte data_pin;
    byte clock_pin;

public:
    KeyboardProtocol(){};

    void begin(byte data_pin, byte clock_pin);
    byte readCmd();
    void sendKey(byte key);

private:
    byte readByte();
    void sendByte(byte b);
};
#endif
