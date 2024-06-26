#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "stdint.h"
#include "CarState.h"

class Bluetooth
{
public:
    static void Initialize();
    static void ShutDown();

    static void SendMessage(uint8_t type, void* data, uint8_t length);

    static int AvailableStateMessageCount();
    static CarCommand ReadStateMessage();
};


#endif