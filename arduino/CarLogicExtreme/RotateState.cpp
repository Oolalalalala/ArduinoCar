#include "RotateState.h"
#include "ForwardState.h"
#include "../Core/BasicControl.h"

void TurnLeft1State::OnStateEnter()
{
  exitRoute = false;
}

void TurnLeft1State::OnStateUpdate(float dt)
{
  MoveWheel(50, 255);
  
  if (GetDetectorCount() <= 3)
    m_StateMachine->SwitchState(new ForwardState());
    
  Serial.println("Turn left state");
}
  
void TurnLeft1State::OnStateExit()
{

}

void TurnRight1State::OnStateEnter() 
{
  exitRoute = false;
}

void TurnRight1State::OnStateUpdate(float dt)
{
  MoveWheel(255, -255);

  if (!exitRoute && !OnRoute())
    exitRoute = true;

  if (exitRoute && OnRoute())
    m_StateMachine->SwitchState(new ForwardState());

    
  Serial.println("Turn right state");
}
  
void TurnRight1State::OnStateExit()
{
}


void TurnLeft2State::OnStateEnter()
{
  exitRoute = false;
  routeCount = 0;
}

void TurnLeft2State::OnStateUpdate(float dt)
{
  MoveWheel(-255, 255);

  if (!exitRoute && !OnRoute())
    exitRoute = true;

  if (exitRoute && OnRoute())
  {
    exitRoute = false;
    routeCount++;
    if (routeCount == 2)
      m_StateMachine->SwitchState(new ForwardState());
  }
}
  
void TurnLeft2State::OnStateExit()
{
}


void UTurnState::OnStateEnter() 
{
  exitRoute = false;
}

void UTurnState::OnStateUpdate(float dt)
{
  MoveWheel(255, -255);

  if (!exitRoute && !OnRoute())
    exitRoute = true;

  if (exitRoute && OnRoute())
    m_StateMachine->SwitchState(new ForwardState());

    
  Serial.println("U-Turn state");
}
  
void UTurnState::OnStateExit()
{
}