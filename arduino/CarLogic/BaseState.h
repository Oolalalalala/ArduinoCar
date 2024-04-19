#ifndef BASE_STATE_H
#define BASE_STATE_H

#include "CarCommandQueue.h"

// Forward declaration
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

  friend class CarStateMachine;
};



#endif