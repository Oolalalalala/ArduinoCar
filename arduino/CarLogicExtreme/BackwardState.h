#ifndef BACKWARD_STATE_H
#define BACKWARD_STATE_H

#include "../Core/BaseState.h"

class ForwardState : public CarState
{
public:
  virtual void OnStateEnter() override;

  virtual void OnStateUpdate(float dt) override;
  
  virtual void OnStateExit() override;

private:
  bool exitNode;
  float offset, leftWheelSpeed, rightWheelSpeed;
};


#endif
