#include "BasicControl.h"
#include "BaseState.h"
#include "ForwardState.h"
#include "BluetoothDispatcher.h"

void setup() {
  InitPins();
  Serial.begin(9600);
}


CarCommandQueue queue;
CarStateMachine stateMachine(new ForwardState(), &queue);
BluetoothDispatcher dispatcher(&queue);

void loop() 
{
  CollectPinValue();

  dispatcher.ParseAllMessage();

  float dt = GetDeltaTime();
  stateMachine.OnUpdate(dt);
}