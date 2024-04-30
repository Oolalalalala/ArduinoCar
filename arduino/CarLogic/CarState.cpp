#include "CarState.h"
#include "Arduino.h"
#include "StateMachine.h"
#include "BasicControl.h"
#include "Bluetooth.h"
#include "RFID.h"
#include "Config.h"

// For RFID
static void CalculatePathTraceSpeed(int maxSpeed, float& leftWheelSpeed, float& rightWheelSpeed)
{
  float offset = InferredSensorArray::GetNormalizedErrorValue(CAR_PATH_TRACE_ADJUST_WEIGHT) * maxSpeed * CAR_PATH_TRACE_ADJUST_STRENGTH;

  if (offset > 0)
  {
    leftWheelSpeed = maxSpeed;
    rightWheelSpeed = maxSpeed - offset;
  }
  else
  {
    rightWheelSpeed = maxSpeed;
    leftWheelSpeed = maxSpeed + offset;
  }
}

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
  m_Controller.GetSpeed(CAR_SPEED, leftWheelSpeed, rightWheelSpeed);
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
  m_SlowDownTimer = 0;
  m_LeftNode = false;
  m_ReturnedNode = false;
  m_RFIDDetected = false;
  m_ExitDelayTimer = 0.0f;
  m_ExitNode = false;
  m_Retested = false;

  m_ReturnCorrection = 0.0f;

  // Retest use variable
  m_RetestLeftNode = false;
  m_RetestBrakeTimer = 0.0f;
  m_RetestExitNode = false;
  m_RetestForwardTimer = 0.0f;
  m_RetestRotateTimer = 0.0f;
  m_Correction = 0;

  
  m_RetestRotateBrakeTimer = 0.0f;
  m_RetestExitRotate = false;

  Serial.println("TestRFID state begin");
}

void TestRFIDState::OnStateUpdate(float dt)
{
  if (m_Retested)
  {
    RetestSequence(dt);
    return;
  }

  if (m_RFIDDetected)
  {
    ReturnSequence(dt);
    return;
  }

  // Main sequence
  if (m_LeftNode)
    m_SlowDownTimer += dt;
    
  int maxSpeed;
  if (m_SlowDownTimer < CAR_RFID_SLOW_DOWN_DELAY || !m_LeftNode)
    maxSpeed = CAR_SPEED;
  else
    maxSpeed = CAR_RFID_SLOW_DOWN_SPEED;
  float leftWheelSpeed, rightWheelSpeed;
  CalculatePathTraceSpeed(maxSpeed, leftWheelSpeed, rightWheelSpeed);
  CarMotor::SetSpeed(leftWheelSpeed * CAR_LEFT_WHEEL_SPEED_RATIO, rightWheelSpeed * CAR_RIGHT_WHEEL_SPEED_RATIO);

  if (InferredSensorArray::GetDetectionCount() <= 3)
    m_LeftNode = true;

  // ACCIDENT: RFID not detected
  if (m_LeftNode && InferredSensorArray::GetDetectionCount() == 5)
  {
    m_Retested = true;
    m_RetestStage = 0;
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
  Serial.println();
  if (InferredSensorArray::GetDetectionCount() >= 4)
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
    CarMotor::SetSpeed(150 * CAR_LEFT_WHEEL_SPEED_RATIO, 150 * CAR_RIGHT_WHEEL_SPEED_RATIO);
  else
  {
    if (m_Correction == 0 && InferredSensorArray::GetState(0))
      m_Correction = 2;
    if (m_Correction == 0 && InferredSensorArray::GetState(4))
      m_Correction = -2;
    
    if (m_Correction == 0)
      CarMotor::SetSpeed(CAR_RFID_REVERSE_SPEED * CAR_REVERSE_LEFT_WHEEL_SPEED_RATIO, CAR_RFID_REVERSE_SPEED * CAR_REVERSE_RIGHT_WHEEL_SPEED_RATIO);
    else if (m_Correction > 0)
      CarMotor::SetSpeed(CAR_RFID_REVERSE_SPEED * CAR_REVERSE_LEFT_WHEEL_SPEED_RATIO * 0.9f, CAR_RFID_REVERSE_SPEED * CAR_REVERSE_RIGHT_WHEEL_SPEED_RATIO);
    else
      CarMotor::SetSpeed(CAR_RFID_REVERSE_SPEED * CAR_REVERSE_LEFT_WHEEL_SPEED_RATIO, CAR_RFID_REVERSE_SPEED * CAR_REVERSE_RIGHT_WHEEL_SPEED_RATIO * 0.9f);

  }
}

void TestRFIDState::RetestSequence(float dt)
{
  switch (m_RetestStage)
  {
    case 0: // Brake
    {
      m_RetestBrakeTimer += dt;
      if (m_RetestBrakeTimer >= CAR_RFID_RETEST_BRAKE_TIME)
      {
        m_RetestStage++;
        m_RetestBrakeTimer = 0.0f;
      }

      CarMotor::SetSpeed(CAR_RFID_RETEST_BRAKE_SPEED, CAR_RFID_RETEST_BRAKE_SPEED);

      break;
    }

    case 1: // Turn Left
    {
      m_RetestRotateTimer += dt;

      if (InferredSensorArray::GetDetectionCount() == 0)
        m_RetestLeftNode = true;

      InferredSensorArray::DebugOutput();

      if (m_RetestLeftNode && InferredSensorArray::GetDetectionCount() && m_RetestRotateTimer >= CAR_RFID_RETEST_ROTATE_IMMUNITY_TIME)
        m_RetestExitRotate = true;

      if (m_RetestExitRotate)
      {
        m_RetestRotateBrakeTimer += dt;
        if (m_RetestRotateBrakeTimer >= CAR_RFID_RETEST_ROTATE_BRAKE_TIME)
          m_RetestStage++;

        CarMotor::SetSpeed(255, -255);
      }
      else
        CarMotor::SetSpeed(CAR_RFID_RETEST_ROTATE_SPEED);

      break;
    }

    case 2: // Go forward
    {
      m_RetestForwardTimer += dt;

      int maxSpeed;
      if (m_RFIDDetected || m_RetestForwardTimer >= CAR_RFID_RETEST_GIVE_UP_TIME)
        maxSpeed = CAR_SPEED;
      else
        maxSpeed = CAR_RFID_RETEST_SPEED;

      float leftWheelSpeed, rightWheelSpeed;
      CalculatePathTraceSpeed(maxSpeed, leftWheelSpeed, rightWheelSpeed);
      CarMotor::SetSpeed(leftWheelSpeed * CAR_LEFT_WHEEL_SPEED_RATIO, rightWheelSpeed * CAR_RIGHT_WHEEL_SPEED_RATIO);

      if (InferredSensorArray::GetDetectionCount() == 5)
          m_StateMachine->NextState();

      if (m_RFIDDetected || m_RetestForwardTimer >= CAR_RFID_RETEST_GIVE_UP_TIME)
        break;

      // RFID detection
      if (RFID::HasValue())
      {
        uint32_t uid = RFID::ReadValue();
        Bluetooth::SendMessage(2, &uid, 4);

        m_RFIDDetected = true;
      }
      
      break;
    }
  }
}

void TestRFIDState::OnStateExit()
{
  Serial.println("TestRFID state end");
}

void RotateLeftState::OnStateEnter()
{
  m_BrakeTimer = 0.0f;
  m_ExitState = false;
  leftRoute = false;
  m_Timer = 0.0f;
  
  Serial.println("Turn left state begin");
}

void RotateLeftState::OnStateUpdate(float dt)
{
  m_Timer += dt;

  CarMotor::SetSpeed(CAR_ROTATE_LEFT_SPEED);

  if (!leftRoute && InferredSensorArray::GetDetectionCount() == 0 && m_Timer >= CAR_ROATATION_EXIT_STATE_IMMUNITY_TIME)
    leftRoute = true;

  if (leftRoute && InferredSensorArray::GetDetectionCount() > 0)
    m_ExitState = true;

  if (m_ExitState)
  {
    m_BrakeTimer += dt;
    CarMotor::SetSpeed(255, -255);
    if (m_BrakeTimer >= CAR_ROTATE_STATE_BRAKE_TIME)
      m_StateMachine->NextState();
  }
}

void RotateLeftState::OnStateExit()
{
  Serial.println("Turn left state end");
}

void RotateRightState::OnStateEnter()
{
  m_BrakeTimer = 0.0f;
  m_ExitState = false;
  leftRoute = false;
  m_Timer = 0.0f;
  Serial.println("Turn right state begin");
}

void RotateRightState::OnStateUpdate(float dt)
{
  m_Timer += dt;

  CarMotor::SetSpeed(CAR_ROTATE_RIGHT_SPEED);

  if (!leftRoute && InferredSensorArray::GetDetectionCount() == 0 && m_Timer >= CAR_ROATATION_EXIT_STATE_IMMUNITY_TIME)
    leftRoute = true;

  if (leftRoute && InferredSensorArray::GetDetectionCount() != 0)
    m_ExitState = true;

  if (m_ExitState)
  {
    m_BrakeTimer += dt;
    CarMotor::SetSpeed(-255, 255);
    if (m_BrakeTimer >= CAR_ROTATE_STATE_BRAKE_TIME)
      m_StateMachine->NextState();
  }
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
  m_Controller.GetSpeed(CAR_SPEED, leftWheelSpeed, rightWheelSpeed);
  CarMotor::SetSpeed(leftWheelSpeed * CAR_LEFT_WHEEL_SPEED_RATIO, rightWheelSpeed * CAR_RIGHT_WHEEL_SPEED_RATIO);

  if (InferredSensorArray::GetDetectionCount() <= 3)
    m_LeftNode = true;

  if (m_LeftNode && m_ImmunityTimer >= CAR_SPRINT_STATE_IMMUNITY_TIME && InferredSensorArray::GetDetectionCount() == 5)
      m_StateMachine->NextState();
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

  // Brake
  if (m_ImmunityTimer < CAR_SPRINT_STATE_BRAKE_TIME)
  {
    CarMotor::SetSpeed(CAR_SPRINT_STATE_BRAKE_SPEED, CAR_SPRINT_STATE_BRAKE_SPEED);
    return;
  }

  // Special case: Two blobs -> One blob
  if (InferredSensorArray::GetDetectionBlobCount() == 2)
    m_TwoBlobs = true;
  if (m_TwoBlobs && InferredSensorArray::GetDetectionBlobCount() == 1 && InferredSensorArray::GetState(3) == 0 && InferredSensorArray::GetState(4) == 0)
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
  
  // Brake
  if (m_ImmunityTimer < CAR_SPRINT_STATE_BRAKE_TIME)
  {
    CarMotor::SetSpeed(CAR_SPRINT_STATE_BRAKE_SPEED, CAR_SPRINT_STATE_BRAKE_SPEED);
    return;
  }

  
  if (InferredSensorArray::GetDetectionBlobCount() == 2)
    m_TwoBlobs = true;
  if (m_TwoBlobs && InferredSensorArray::GetDetectionBlobCount() == 1 && InferredSensorArray::GetState(0) == 0 && InferredSensorArray::GetState(1) == 0)
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