#include <M0110aKeyboard.hpp>
#include <KeyboardProtocol.hpp>

// #define SERIAL_DEBUG

void KeyboardProtocol::begin(byte data_pin, byte clock_pin)
{
  this->data_pin = data_pin;
  this->clock_pin = clock_pin;
  pinMode(this->clock_pin, OUTPUT);
  pinMode(this->data_pin, INPUT_PULLUP);
}

// reads the command, operates the diagnostic LED and switches pin mode back to OUTPUT
byte KeyboardProtocol::readCmd()
{
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(this->data_pin, INPUT);
  delayMicroseconds(20);

  while (digitalRead(this->data_pin) != LOW)
    ;
  delayMicroseconds(400); // this is apparently required so we don't lose the first digit
  byte cmd = readByte();
  while (digitalRead(this->data_pin) != HIGH)
    ;

  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(this->data_pin, OUTPUT);
  delayMicroseconds(20);
  return cmd;
}

void KeyboardProtocol::sendKey(uint16_t key)
{
  if (key & NUMPAD)
  {
    sendByte(KP_MOD);
    readCmd();
    sendByte(key);
  }
  else if (key & NUMPAD2)
  {
    sendByte(KEY_SHIFT);
    readCmd();
    sendByte(KP_MOD);
    readCmd();
    sendByte(key);
  }
  else if (key == KEY_NONE) {
    sendByte(KB_NULL);
  }
  else
  {
    sendByte(key);
  }
}

byte KeyboardProtocol::readByte()
{
  byte b = 0;
  for (byte i = 0; i < 8; i++)
  {
    digitalWrite(this->clock_pin, LOW);
    delayMicroseconds(180);
    digitalWrite(this->clock_pin, HIGH);
    delayMicroseconds(80);
    b = (b << 1) | digitalRead(this->data_pin);
    delayMicroseconds(140);
  }
#ifdef SERIAL_DEBUG
  Serial.print(b, HEX);
  Serial.print(" -> ");
#endif
  return b;
}

void KeyboardProtocol::sendByte(byte b)
{
#ifdef SERIAL_DEBUG
  Serial.print(b, HEX);
  Serial.println();
#endif
  for (byte m = 128; m > 0; m >>= 1)
  {
    digitalWrite(this->data_pin, !(b & m) ? LOW : HIGH);
    delayMicroseconds(40);
    digitalWrite(this->clock_pin, LOW);
    delayMicroseconds(120);
    digitalWrite(this->clock_pin, HIGH);
    delayMicroseconds(170);
  }
  digitalWrite(this->data_pin, HIGH);
}
