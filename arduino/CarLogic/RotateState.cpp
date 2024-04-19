#include "RotateState.h"
#include "ForwardState.h"
#include "BasicControl.h"
#include "StateMachine.h"

void TurnLeft1State::OnStateEnter()
  {
    leftRoute = false;
  }

void TurnLeft1State::OnStateUpdate(float dt)
  {
    MoveWheel(-255, 255);

    if (!leftRoute && !OnRoute())
      leftRoute = true;

    if (leftRoute && OnRoute())
      m_StateMachine->NextState();

      
  Serial.println("Turn left state");
  }
  
void TurnLeft1State::OnStateExit()
{

}

void TurnRight1State::OnStateEnter() 
{
  leftRoute = false;
}

void TurnRight1State::OnStateUpdate(float dt)
{
  MoveWheel(255, -255);

  if (!leftRoute && !OnRoute())
    leftRoute = true;

  if (leftRoute && OnRoute())
      m_StateMachine->NextState();

    
  Serial.println("Turn right state");
}
  
void TurnRight1State::OnStateExit()
{
}


void TurnLeft2State::OnStateEnter()
{
  leftRoute = false;
  routeCount = 0;
}

void TurnLeft2State::OnStateUpdate(float dt)
{
  MoveWheel(-255, 255);

  if (!leftRoute && !OnRoute())
    leftRoute = true;

  if (leftRoute && OnRoute())
  {
    leftRoute = false;
    routeCount++;
    if (routeCount == 2)
      m_StateMachine->NextState();
  }
}
  
void TurnLeft2State::OnStateExit()
{
}