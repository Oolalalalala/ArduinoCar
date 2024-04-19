#include "BasicControl.h"
#include "BaseState.h"
#include "ForwardState.h"
#include "RFID.h"
#include "StateMachine.h"
#include "Bluetooth.h"

void setup() {
  InitPins();
  Serial.begin(9600);
  Bluetooth::Initialize();
  RFID::Initialize();
}


CarStateMachine stateMachine;

void loop() 
{
  CollectPinValue();

  float dt = GetDeltaTime();
  stateMachine.OnUpdate(dt);
}