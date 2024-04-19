#include "ForwardState.h"
#include "BasicControl.h"
#include "RotateState.h"
#include "RFID.h"
#include "Bluetooth.h"
#include "StateMachine.h"

  void ForwardState::OnStateEnter()
  {
    m_OnNode = false;
  }

  void ForwardState::OnStateUpdate(float dt)
  {
    float adjustConstant = speed / 5;
    offset = GetOffsetValue() * adjustConstant;

    if (offset > 0)
    {
      leftWheelSpeed = speed;
      rightWheelSpeed = speed - offset;
    }
    else
    {
      rightWheelSpeed = speed;
      leftWheelSpeed = speed + offset;
    }

    leftWheelSpeed *= 0.85;
    MoveWheel(leftWheelSpeed, rightWheelSpeed);

    // Enters the node
    if (OnNode())
    {
      m_OnNode = true;
    }
    else if (m_OnNode)
    {
      m_StateMachine->NextState();
    }

    Serial.println("Forward state");
  }
  
  void ForwardState::OnStateExit()
  {
  }

void TestRFIDState::OnStateEnter()
{
  m_ReturnedNode = false;
  m_RFIDDetected = false;
}


void TestRFIDState::OnStateUpdate(float dt)
{
  if (!m_RFIDDetected)
  {
    float adjustConstant = speed / 5;
    offset = GetOffsetValue() * adjustConstant;

    if (offset > 0)
    {
      leftWheelSpeed = speed;
      rightWheelSpeed = speed - offset;
    }
    else
    {
      rightWheelSpeed = speed;
      leftWheelSpeed = speed + offset;
    }

    leftWheelSpeed *= 0.85;
    MoveWheel(leftWheelSpeed, rightWheelSpeed);

    // ERROR: RFID not detected
    if (OnNode())
    {
      Serial.println("RFID not detected");
      delay(5000);
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
    if (OnNode())
      m_ReturnedNode = true;
    else if (m_ReturnedNode)
      m_StateMachine->NextState();

    if (m_ReturnedNode)
      MoveWheel(255, 255);
    else
      MoveWheel(-255, -255);
  }


  Serial.println("TestRFID state");
}

void TestRFIDState::OnStateExit()
{

}