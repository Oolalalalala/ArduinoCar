#include "CarState.h"
#include "Arduino.h"
#include "StateMachine.h"
#include "BasicControl.h"
#include "Bluetooth.h"
#include "RFID.h"
#include "Config.h"

//static void CalculatePathTraceSpeed(float& leftWheelSpeed, float& rightWheelSpeed)
//{
//  float offset = InferredSensorArray::GetNormalizedErrorValue(CAR_PATH_TRACE_INFERRED_WEIGHT) * CAR_SPEED * CAR_PATH_TRACE_ADJUST_STRENGTH;
//
//  if (offset > 0)
//  {
//    leftWheelSpeed = CAR_SPEED;
//    rightWheelSpeed = CAR_SPEED - offset;
//  }
//  else
//  {
//    rightWheelSpeed = CAR_SPEED;
//    leftWheelSpeed = CAR_SPEED + offset;
//  }
//}

void ForwardState::OnStateEnter()
{
  m_DelayTimer = 0.0f;
  m_OnNode = false;
  m_ExitNode = false;
  Serial.println("Forward state begin");
}

void ForwardState::OnStateUpdate(float dt)
{
  float leftWheelSpeed, rightWheelSpeed;
  m_Controller.OnUpdate(dt);
  m_Controller.GetSpeed(leftWheelSpeed, rightWheelSpeed);
  CarMotor::SetSpeed(leftWheelSpeed * CAR_LEFT_WHEEL_SPEED_RATIO, rightWheelSpeed * CAR_RIGHT_WHEEL_SPEED_RATIO);

  // Enters the node
  if (InferredSensorArray::GetDetectionCount() == 5)
  {
    m_OnNode = true;
  }
  if (m_OnNode)
  {
    m_DelayTimer += dt;
    if (m_DelayTimer >= CAR_FORWARD_ENTER_NODE_IMMUNITY_TIME && InferredSensorArray::GetDetectionCount() <= 3)
    {
      m_DelayTimer = 0.0f;
      m_OnNode = false;
      m_ExitNode = true;
    }
  }
  if (m_ExitNode)
  {
    m_DelayTimer += dt;
    if (m_DelayTimer >= CAR_FORWARD_EXIT_STATE_DELAY)
      m_StateMachine->NextState();
  }
}

void ForwardState::OnStateExit()
{
  Serial.println("Forward state end");
}

void TestRFIDState::OnStateEnter()
{
  m_ReturnedNode = false;
  m_RFIDDetected = false;
  m_ExitDelayTimer = 0.0f;
  m_ExitNode = false;
  m_Aborted = false;

  // Abort use variable
  m_AbortLeftNode = false;
  m_AbortEnterNodeImmunityTimer = 0.0f;
  m_AbortExitNode = false;
  m_AbortExitDelayTimer = 0.0f;

  Serial.println("TestRFID state begin");
}

void TestRFIDState::OnStateUpdate(float dt)
{
  if (m_Aborted)
  {
    AbortSequence(dt);
    return;
  }

  if (m_RFIDDetected)
  {
    ReturnSequence(dt);
    return;
  }

  // Main sequence
  float leftWheelSpeed, rightWheelSpeed;
  m_Controller.OnUpdate(dt);
  m_Controller.GetSpeed(leftWheelSpeed, rightWheelSpeed);
  CarMotor::SetSpeed(leftWheelSpeed * CAR_LEFT_WHEEL_SPEED_RATIO, rightWheelSpeed * CAR_RIGHT_WHEEL_SPEED_RATIO);

  // ACCIDENT: RFID not detected
  if (InferredSensorArray::GetDetectionCount() == 5)
  {
    m_Aborted = true;
    m_AbortStage = 0;
    m_Controller.Reset();
    Bluetooth::SendMessage(3, nullptr, 0);
    m_StateMachine->DiscardNextCommand();
    Serial.println("ACCIDENT: RFID not detected.");
  }

  // RFID detection
  if (RFID::HasValue())
  {
    uint32_t uid = RFID::ReadValue();
    Bluetooth::SendMessage(2, &uid, 4);

    m_RFIDDetected = true;
  }
}

void TestRFIDState::ReturnSequence(float dt)
{
  if (InferredSensorArray::GetDetectionCount() == 5)
    m_ReturnedNode = true;
  if (m_ReturnedNode && InferredSensorArray::GetDetectionCount() <= 3)
    m_ExitNode = true;
  if (m_ExitNode)
  {
    m_ExitDelayTimer += dt;
    if (m_ExitDelayTimer >= CAR_RFID_EXIT_STATE_DEALY)
      m_StateMachine->NextState();
  }

  if (m_ReturnedNode)
    CarMotor::SetSpeed(CAR_SPEED * CAR_LEFT_WHEEL_SPEED_RATIO, CAR_SPEED * CAR_RIGHT_WHEEL_SPEED_RATIO);
  else
    CarMotor::SetSpeed(-CAR_SPEED * CAR_REVERSE_LEFT_WHEEL_SPEED_RATIO, -CAR_SPEED * CAR_REVERSE_RIGHT_WHEEL_SPEED_RATIO);
}

void TestRFIDState::AbortSequence(float dt)
{
  switch (m_AbortStage)
  {
    case 0: // Turn RIght
    {
      if (InferredSensorArray::GetDetectionCount() == 0)
        m_AbortLeftNode = true;

      if (m_AbortLeftNode && InferredSensorArray::GetDetectionCount())
        m_AbortStage++;

      CarMotor::SetSpeed(CAR_SPEED, -CAR_SPEED);

      break;
    }

    case 1: // Go forward
    {
      float leftWheelSpeed, rightWheelSpeed;
      m_Controller.OnUpdate(dt);
      m_Controller.GetSpeed(leftWheelSpeed, rightWheelSpeed);
      CarMotor::SetSpeed(leftWheelSpeed * CAR_LEFT_WHEEL_SPEED_RATIO, rightWheelSpeed * CAR_RIGHT_WHEEL_SPEED_RATIO);

      if (InferredSensorArray::GetDetectionCount() == 5)
        m_AbortOnNode = true;

      if (m_AbortOnNode)
      {
        m_AbortEnterNodeImmunityTimer += dt;

        if (m_AbortEnterNodeImmunityTimer >= CAR_FORWARD_ENTER_NODE_IMMUNITY_TIME && InferredSensorArray::GetDetectionCount() <= 3)
        {
          m_AbortOnNode = false;
          m_AbortExitNode = true;
        }
      }

      if (m_AbortExitNode)
      {
        m_AbortExitDelayTimer += dt;
        if (m_AbortExitDelayTimer >= CAR_FORWARD_EXIT_STATE_DELAY)
          m_StateMachine->NextState();
      }

      break;
    }
  }
  
  // RFID detection
  if (RFID::HasValue())
  {
    uint32_t uid = RFID::ReadValue();
    Bluetooth::SendMessage(2, &uid, 4);

    m_RFIDDetected = true;
  }
}

void TestRFIDState::OnStateExit()
{
  Serial.println("TestRFID state end");
}

void RotateLeftState::OnStateEnter()
{
  leftRoute = false;
  Serial.println("Turn left state begin");
}

void RotateLeftState::OnStateUpdate(float dt)
{
  CarMotor::SetSpeed(-CAR_SPEED * CAR_LEFT_WHEEL_SPEED_RATIO, CAR_SPEED * CAR_RIGHT_WHEEL_SPEED_RATIO);

  if (!leftRoute && InferredSensorArray::GetDetectionCount() == 0)
    leftRoute = true;

  if (leftRoute && InferredSensorArray::GetDetectionCount() != 0)
    m_StateMachine->NextState();
}

void RotateLeftState::OnStateExit()
{
  Serial.println("Turn left state end");
}

void RotateRightState::OnStateEnter()
{
  leftRoute = false;
  Serial.println("Turn right state begin");
}

void RotateRightState::OnStateUpdate(float dt)
{
  CarMotor::SetSpeed(CAR_SPEED * CAR_LEFT_WHEEL_SPEED_RATIO, -CAR_SPEED * CAR_RIGHT_WHEEL_SPEED_RATIO);

  if (!leftRoute && InferredSensorArray::GetDetectionCount() == 0)
    leftRoute = true;

  if (leftRoute && InferredSensorArray::GetDetectionCount() != 0)
    m_StateMachine->NextState();
}

void RotateRightState::OnStateExit()
{
  Serial.println("Turn right state end");
}

void SprintState::OnStateEnter()
{
  m_LeftNode = false;
  m_OnNode = false;
  m_ImmunityTimer = 0.0f;
  m_BrakeTimer = 0.0f;
  
  Serial.println("Sprint state begin");
}

void SprintState::OnStateUpdate(float dt)
{
  m_ImmunityTimer += dt;
  
  float leftWheelSpeed, rightWheelSpeed;
  m_Controller.OnUpdate(dt);
  m_Controller.GetSpeed(leftWheelSpeed, rightWheelSpeed);
  CarMotor::SetSpeed(leftWheelSpeed * CAR_LEFT_WHEEL_SPEED_RATIO, rightWheelSpeed * CAR_RIGHT_WHEEL_SPEED_RATIO);

  if (InferredSensorArray::GetDetectionCount() <= 3)
    m_LeftNode = true;

  if (m_LeftNode && m_ImmunityTimer >= CAR_SPRINT_STATE_IMMUNITY_TIME && InferredSensorArray::GetDetectionCount() == 5)
    m_OnNode = true;

  if (m_OnNode)
  {
    m_BrakeTimer += dt;

    CarMotor::SetSpeed(-CAR_SPRINT_STATE_BRAKE_SPEED * CAR_REVERSE_LEFT_WHEEL_SPEED_RATIO, -CAR_SPRINT_STATE_BRAKE_SPEED * CAR_REVERSE_RIGHT_WHEEL_SPEED_RATIO);

    if (m_BrakeTimer >= CAR_SPRINT_STATE_BRAKE_TIME)
      m_StateMachine->NextState();
  }
}

void SprintState::OnStateExit()
{
  Serial.println("Sprint state end");
}

void TurnLeftState::OnStateEnter()
{
  m_ImmunityTimer = 0.0f;
  m_BrakeTimer = 0.0f;
  m_Exit = false;
  m_TwoBlobs = false;
  Serial.println("Turn left state begin");
}

void TurnLeftState::OnStateUpdate(float dt)
{
  m_ImmunityTimer += dt;

  // Special case: Two blobs -> One blob
  if (InferredSensorArray::GetDetectionBlobCount() == 2)
    m_TwoBlobs = true;
  if (m_TwoBlobs && InferredSensorArray::GetDetectionBlobCount() == 1)
    m_Exit = true;

  if (InferredSensorArray::GetDetectionCount() <= 3 && InferredSensorArray::GetState(0) && InferredSensorArray::GetDetectionBlobCount() == 1 && m_ImmunityTimer >= CAR_TURN_STATE_IMMUNITY_TIME)
    m_Exit = true;

  if (m_Exit)
  {
    m_BrakeTimer += dt;
    CarMotor::SetSpeed(CAR_TURN_RIGHT_SPEED);

    if (m_BrakeTimer >= CAR_TURN_BRAKE_TIME)
      m_StateMachine->NextState();
  }
  else
    CarMotor::SetSpeed(CAR_TURN_LEFT_SPEED);
}

void TurnLeftState::OnStateExit()
{
  Serial.println("Turn left state end");
}

void TurnRightState::OnStateEnter()
{
  m_ImmunityTimer = 0.0f;
  m_BrakeTimer = 0.0f;
  m_Exit = false;
  m_TwoBlobs = false;
  Serial.println("Turn right state begin");
}

void TurnRightState::OnStateUpdate(float dt)
{
  m_ImmunityTimer += dt;

  
  if (InferredSensorArray::GetDetectionBlobCount() == 2)
    m_TwoBlobs = true;
  if (m_TwoBlobs && InferredSensorArray::GetDetectionBlobCount() == 1)
    m_Exit = true;

  if (InferredSensorArray::GetDetectionCount() <= 3 && InferredSensorArray::GetState(4) && InferredSensorArray::GetDetectionBlobCount() == 1 && m_ImmunityTimer >= CAR_TURN_STATE_IMMUNITY_TIME)
    m_Exit = true;

  if (m_Exit)
  {
    m_BrakeTimer += dt;
    CarMotor::SetSpeed(CAR_TURN_LEFT_SPEED);

    if (m_BrakeTimer >= CAR_TURN_BRAKE_TIME)
      m_StateMachine->NextState();
  }
  else
    CarMotor::SetSpeed(CAR_TURN_RIGHT_SPEED);
}

void TurnRightState::OnStateExit()
{
  Serial.println("Turn right state end");
}