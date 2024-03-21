#include "BasicControl.h"
#include "BaseState.h"
#include "ForwardState.h"

void setup() {
  InitPins();
  Serial.begin(9600);
}


CarStateMachine stateMachine(new ForwardState());

void loop() 
{
  CollectPinValue();

  float dt = GetDeltaTime();
  stateMachine.OnUpdate(dt);
}