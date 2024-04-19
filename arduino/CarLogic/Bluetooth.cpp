#include "Bluetooth.h"
#include "Arduino.h"

Buffer s_BluetoothReadBuffer(256);
Buffer s_AvailableMessage; // Subbuffer of readbuffer
static Buffer s_WriteMessageBuffer(256);
uint8_t* s_ReadBufferBeginPtr;
uint8_t* s_ReadBufferEndPtr;

void Bluetooth::Initialize()
{
    Serial1.begin(9600);
    Serial1.print("Connected to CarCar");
}

void Bluetooth::ShutDown()
{
    Serial1.end();
}


void Bluetooth::SendMessage(uint8_t type, void* data, uint8_t length)
{
    s_WriteMessageBuffer.As<char>()[0] = '<';
    s_WriteMessageBuffer.As<uint8_t>()[1] = type;
    s_WriteMessageBuffer.As<uint8_t>()[2] = length;
    
    if (data && length)
        memcpy(s_WriteMessageBuffer.As<char>() + 3, data, length);

    s_WriteMessageBuffer.As<char>()[length + 3] = '>';

    Serial1.write(s_WriteMessageBuffer.As<char>(), length + 4);
}

int Bluetooth::AvailableStateMessageCount()
{
    return Serial1.available();
}

CarCommand Bluetooth::ReadStateMessage()
{
    uint8_t value = Serial1.read();
    
    return (CarCommand)value;
}