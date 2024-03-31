#include "ForwardState.h"
#include "BasicControl.h"
#include "RotateState.h"

  void ForwardState::OnStateEnter()
  {
    inNode = false;
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
      // TODO: Update position in map
      inNode = true;
    }
    // Leaving the node
    else if (inNode)
    {
      // TODO: Make decision here (from the map)
      // Probably transition to other state here
      // if map says go left


      //if (OnRoute()) // There is still route in front
        //m_StateMachine->SwitchState(new ForwardState());
      if (true)
        m_StateMachine->SwitchState(new TurnLeft1State());
      else if (true)// if map says go right
        m_StateMachine->SwitchState(new TurnRight1State());
      else if (true)// if map says go backward
        m_StateMachine->SwitchState(new TurnLeft2State());

    }
    Serial.println("Forward state");
  }
  
  void ForwardState::OnStateExit()
  {
  }
