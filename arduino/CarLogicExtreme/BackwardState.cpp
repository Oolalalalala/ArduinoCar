#include "BackwardState.h"
#include "../Core/BasicControl.h"
#include "RotateState.h"

  void ForwardState::OnStateEnter()
  {
    exitNode = false;
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
    
    if (!OnNode() && !exitNode)
      exitNode = true;

    // Enters the node
    if (OnNode() && exitNode)
    {
      // TODO: Update position in map
      m_StateMachine->SwitchState(new TurnLeft1State());
    }
    Serial.println("Forward state");
  }
  
  void ForwardState::OnStateExit()
  {
  }
