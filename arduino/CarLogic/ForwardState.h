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
  float offset, leftWheelSpeed, rightWheelSpeed;
  bool m_OnNode;
};

class TestRFIDState : public CarState
{
public:
  virtual void OnStateEnter() override;

  virtual void OnStateUpdate(float dt) override;
  
  virtual void OnStateExit() override;

private:
  float offset, leftWheelSpeed, rightWheelSpeed;

  bool m_ReturnedNode;
  bool m_RFIDDetected;
};

#endif