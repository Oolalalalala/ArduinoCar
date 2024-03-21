#ifndef ROTATE_STATE_H
#define ROTATE_STATE_H

#include "BaseState.h"

class TurnLeft1State : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

private:
  bool leftRoute;
};


class TurnRight1State : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

private:
  bool leftRoute;
};


class TurnLeft2State : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

private:
  bool leftRoute;
  bool routeCount;
};

#endif