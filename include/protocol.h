#include<Arduino.h>

#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#define INQUIRY_CMD 0x10
#define INSTANT_CMD 0x14
#define MODEL_CMD 0x16
#define TEST_CMD 0x7D

byte readCmd();
void inquiry();
void sendKey(unsigned int key);

#endif
