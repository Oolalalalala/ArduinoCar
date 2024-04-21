#include "CarState.h"
#include "Arduino.h"
#include "StateMachine.h"
#include "BasicControl.h"
#include "Bluetooth.h"
#include "RFID.h"
#include "Config.h"

void ForwardState::OnStateEnter()
{
  m_OnNode = false;
  Serial.println("Forward state begin");
}

void ForwardState::OnStateUpdate(float dt)
{
  float offset = InferredSensorArray::GetNormalizedErrorValue(CAR_PATH_TRACE_INFERRED_WEIGHT) * CAR_SPEED * CAR_PATH_TRACE_ADJUST_STRENGTH;
  float leftWheelSpeed, rightWheelSpeed;

  if (offset > 0)
  {
    leftWheelSpeed = CAR_SPEED;
    rightWheelSpeed = CAR_SPEED - offset;
  }
  else
  {
    rightWheelSpeed = CAR_SPEED;
    leftWheelSpeed = CAR_SPEED + offset;
  }

  CarMotor::SetSpeed(leftWheelSpeed * CAR_LEFT_WHEEL_SPEED_RATIO, rightWheelSpeed * CAR_RIGHT_WHEEL_SPEED_RATIO);

  // Enters the node
  if (InferredSensorArray::GetDetectionCount() == 5)
  {
    m_OnNode = true;
  }
  else if (m_OnNode)
  {
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

  Serial.println("TestRFID state begin");
}

void TestRFIDState::OnStateUpdate(float dt)
{
  if (!m_RFIDDetected)
  {
    float offset = InferredSensorArray::GetNormalizedErrorValue(CAR_PATH_TRACE_INFERRED_WEIGHT) * CAR_SPEED * CAR_PATH_TRACE_ADJUST_STRENGTH;
    float leftWheelSpeed, rightWheelSpeed;

    if (offset > 0)
    {
      leftWheelSpeed = CAR_SPEED;
      rightWheelSpeed = CAR_SPEED - offset;
    }
    else
    {
      rightWheelSpeed = CAR_SPEED;
      leftWheelSpeed = CAR_SPEED + offset;
    }

    CarMotor::SetSpeed(leftWheelSpeed * CAR_LEFT_WHEEL_SPEED_RATIO, rightWheelSpeed * CAR_RIGHT_WHEEL_SPEED_RATIO);

    // TODO: Deal with cases when RFID is not found
    // ERROR: RFID not detected
    if (InferredSensorArray::GetDetectionCount() == 5)
    {
      Serial.println("RFID not detected");
      delay(1000);
    }

    // RFID detection
    if (RFID::HasValue())
    {
      uint32_t uid = RFID::ReadValue();
      Bluetooth::SendMessage(2, &uid, 4);

      m_RFIDDetected = true;
    }
  }
  else
  {
    if (InferredSensorArray::GetDetectionCount() == 5)
      m_ReturnedNode = true;
    else if (m_ReturnedNode)
      m_StateMachine->NextState();

    if (m_ReturnedNode)
      CarMotor::SetSpeed(CAR_SPEED * CAR_LEFT_WHEEL_SPEED_RATIO, CAR_SPEED * CAR_RIGHT_WHEEL_SPEED_RATIO);
    else
      CarMotor::SetSpeed(-CAR_SPEED * CAR_LEFT_WHEEL_SPEED_RATIO, -CAR_SPEED * CAR_RIGHT_WHEEL_SPEED_RATIO);
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