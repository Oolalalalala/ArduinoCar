#include "Bluetooth.h"
#include "Arduino.h"

void Bluetooth::Initialize()
{
    Serial1.begin(9600);
    Serial1.print("Connected to CarCar");
}

void Bluetooth::ShutDown()
{
    Serial1.end();
}

void Bluetooth::SendChar(char value)
{
    Serial1.write(value);
}

void Bluetooth::SendString(const char* value)
{
    Serial1.write(value);
}

void Bluetooth::SendInt16(int16_t value)
{
    Serial1.write((char*)&value, 2);
}

void Bluetooth::SendInt32(int32_t value)
{
    Serial1.write((char*)&value, 4);
}

bool Bluetooth::ReadAvailable()
{
    return Serial1.available();
}

char Bluetooth::ReadChar()
{
    return Serial1.read();
}