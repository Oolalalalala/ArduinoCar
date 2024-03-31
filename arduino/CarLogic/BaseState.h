#ifndef BASE_STATE_H
#define BASE_STATE_H

#include "CarCommandQueue.h"

// Forward delcaration
class CarStateMachine;

class CarState
{
public:
  CarState() {};
  virtual ~CarState() {};

  virtual void OnStateEnter() = 0;
  virtual void OnStateUpdate(float dt) = 0; // dt in seconds
  virtual void OnStateExit() = 0;

protected:
  CarStateMachine* m_StateMachine = nullptr;
  CarCommandQueue* m_CommandQueue = nullptr;

  friend class CarStateMachine;
};

class CarStateMachine
{
public:
  CarStateMachine(CarState* beginState, CarCommandQueue* commandQueue)
  {
    m_OldState = nullptr;
    m_CommandQueue = commandQueue;

    m_State = beginState;
    m_State->m_StateMachine = this;
    m_State->m_CommandQueue = commandQueue;
    m_State->OnStateEnter();
  }

  ~CarStateMachine()
  {
    m_State->OnStateExit();
    delete m_State;
  }

  void SwitchState(CarState* newState)
  {
    m_OldState = m_State;
    m_State = newState;
    m_State->m_StateMachine = this;
    m_State->m_CommandQueue = m_CommandQueue;
  }
  
  void OnUpdate(float dt)
  {
    m_State->OnStateUpdate(dt);

    // Check if need to switch state
    if (m_OldState)
    {
      m_OldState->OnStateExit();
      delete m_OldState;
      m_OldState = nullptr;
      m_State->OnStateEnter();
    }
  }

private:
  CarState* m_State;
  CarState* m_OldState;

  CarCommandQueue* m_CommandQueue;
};

#endif