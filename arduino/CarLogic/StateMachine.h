#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "CarState.h"
#include "BasicControl.h" 

class CarStateMachine
{
public:
  CarStateMachine();
  ~CarStateMachine();
  
  void WaitForInitialCommand();
  void OnUpdate(float dt);

  // Mark the current state as finished, a new state will be grabbed from bluetooth
  void NextState();
  void DiscardNextCommand(); // May block is next command has not arrive

private:
  CarState* m_State;
  bool m_StateEnded = false;

  CarCommand m_BufferedCommand;

  Timer m_DebugTimer;
};

#endif