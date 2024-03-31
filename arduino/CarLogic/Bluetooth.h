#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <stdint.h>

class Bluetooth
{
public:
    static void Initialize();
    static void ShutDown();

    static void SendChar(char value);
    static void SendString(const char* value);
    static void SendInt16(int16_t value);
    static void SendInt32(int32_t value);

    static bool ReadAvailable();
    static char ReadChar();
};


#endif