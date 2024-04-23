#include "Bluetooth.h"
#include "Arduino.h"

static uint8_t s_WriteMessageBuffer[32];

void Bluetooth::Initialize()
{
  Serial1.begin(9600);
}

void Bluetooth::ShutDown()
{
  Serial1.end();
}

void Bluetooth::SendMessage(uint8_t type, void *data, uint8_t length)
{
  s_WriteMessageBuffer[0] = '<';
  s_WriteMessageBuffer[1] = type;
  s_WriteMessageBuffer[2] = length;

  if (data && length)
    memcpy(s_WriteMessageBuffer + 3, data, length);

  s_WriteMessageBuffer[length + 3] = '>';

  Serial1.write(s_WriteMessageBuffer, length + 4);
}

int Bluetooth::AvailableStateMessageCount()
{
  return Serial1.available();
}

CarCommand Bluetooth::ReadStateMessage()
{
  char command = Serial1.read();
  Serial.println(command);
  switch (command)
  {
    case 'f': return CarCommand::Forward;
    case 'l': return CarCommand::RotateLeft;
    case 'r': return CarCommand::RotateRight;
    case 'i': return CarCommand::TestRFID;
  }
  Serial.println("Invalid CarCommand");
  return CarCommand::None;
}