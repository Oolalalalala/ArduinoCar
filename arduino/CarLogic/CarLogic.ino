#include "BasicControl.h"
#include "StateMachine.h"
#include "BasicControl.h"
#include "Bluetooth.h"
#include "RFID.h"


CarStateMachine* stateMachine;

void setup() 
{
  Serial.begin(9600);

  InferredSensorArray::Initialize();
  Bluetooth::Initialize();
  RFID::Initialize();

  stateMachine = new CarStateMachine();
  stateMachine->WaitForInitialCommand();
}

Timer timer;

void loop() 
{
  InferredSensorArray::CollectState();

  float dt = timer.Tick();
  stateMachine->OnUpdate(dt);
}