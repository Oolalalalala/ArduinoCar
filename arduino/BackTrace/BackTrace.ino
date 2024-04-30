#include "BasicControl.h"

#define IFA InferredSensorArray

Timer timer;
PIDController m_controller;

void setup(){
    IFA::Initialize();
    Serial.begin(9600);
}

void loop()
{
  IFA::CollectState();
  float leftWheelSpeed, rightWheelSpeed;
  float m_DelayTimer;
  bool m_OnNode, m_ExitNode;

  float dt = timer.Tick();
  m_controller.OnBackUpdate(dt);
  m_controller.GetBackSpeed(leftWheelSpeed, rightWheelSpeed);

  // Enters the node
  if (InferredSensorArray::GetDetectionCount() == 5){
    m_OnNode = true;
  }else if (InferredSensorArray::GetDetectionCount() == 0){
    m_ExitNode = false;
  }
  
  if (m_ExitNode || !m_OnNode){
    CarMotor::SetSpeed(0, 0);
  }else{
    CarMotor::SetSpeed(leftWheelSpeed, rightWheelSpeed);
  }
  
  
  // if (m_OnNode)
  // {
  //   m_DelayTimer += dt;
  //   CarMotor::SetSpeed(0, 0);
  //   while (true){
  //     CarMotor::SetSpeed(50, -50);
  //     delay(250);
  //     CarMotor::SetSpeed(-50, 50);
  //     delay(250);
  //   }
  // }
}