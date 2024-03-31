#include "BluetoothDispatcher.h"
#include "Bluetooth.h"

BluetoothDispatcher::BluetoothDispatcher(CarCommandQueue* commandQueue)
{
    m_CommandQueue = commandQueue;
}

void BluetoothDispatcher::ParseAllMessage()
{
    if (!Bluetooth::ReadAvailable())
        return;
}