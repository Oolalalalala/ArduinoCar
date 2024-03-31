#ifndef FORWARD_STATE_H
#define FORWARD_STATE_H

#include "BaseState.h"

class ForwardState : public CarState
{
public:
  virtual void OnStateEnter() override;

  virtual void OnStateUpdate(float dt) override;
  
  virtual void OnStateExit() override;

private:
  bool inNode;
  float offset, leftWheelSpeed, rightWheelSpeed;
};


#endif
