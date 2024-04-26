#include "BasicControl.h"

#define IFA InferredSensorArray


void loop()
{
  Timer timer;
  PIDController m_controller;

  int a = IFA::GetState(0);
  int b = IFA::GetState(1);
  int c = IFA::GetState(2); // The back one
  int d = IFA::GetState(3);
  int e = IFA::GetState(4);

  float leftWheelSpeed, rightWheelSpeed;
  float m_DelayTimer;
  bool m_OnNode, m_ExitNode;

  float dt = timer.Tick();
  m_controller.OnUpdate(dt);
  m_controller.GetBackSpeed(leftWheelSpeed, rightWheelSpeed);
  CarMotor::SetSpeed(leftWheelSpeed, rightWheelSpeed);

  // Enters the node
  if (InferredSensorArray::GetDetectionCount() == 5)
  {
    m_OnNode = true;
  }
  if (m_OnNode)
  {
    m_DelayTimer += dt;
    CarMotor::SetSpeed(0, 0);
    while (true){
      CarMotor::SetSpeed(50, -50);
      delay(250);
      CarMotor::SetSpeed(-50, 50);
      delay(250);
    }
  }
}