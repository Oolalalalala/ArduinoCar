#ifndef BLUETOOTH_DISPATCHER_H
#define BLUETOOTH_DISPATCHER_H

#include "CarCommandQueue.h"

class BluetoothDispatcher
{
public:
    BluetoothDispatcher(CarCommandQueue* commandQueue);

    void ParseAllMessage();

private:
    CarCommandQueue* m_CommandQueue;

};

#endif