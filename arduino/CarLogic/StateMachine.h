#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "CarCommandQueue.h"
#include "BaseState.h"

class CarStateMachine
{
public:
  CarStateMachine();
  ~CarStateMachine();
  
  void OnUpdate(float dt);

  // Mark the current state as finished, a new state will be grabbed from 
  void NextState();

private:
  CarState* m_State;
  bool m_StateEnded = false;

  CarCommand m_BufferedCommand;
};

#endif