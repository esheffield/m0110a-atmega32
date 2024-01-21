#include <Arduino.h>
#include <debug.hpp>

int16_t debugf(const char *format, ...)
{
    va_list ap;
#ifdef DEBUG
    return Serial.printf(format, ap);
#else
    return 0;
#endif
}