#ifndef ROTATE_STATE_H
#define ROTATE_STATE_H

#include "../Core/BaseState.h"

// Fast turn (Non-stopping)
class TurnLeftState : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

private:
  bool exitRoute;
};


// Fast turn (Non-stopping)
class TurnRightState : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

private:
  bool exitRoute;
};


// Slow turn (Stationary)
class RotateLeftState : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

private:
  bool exitRoute;
};


// Slow turn (Stationary)
class RotateRightState : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

private:
  bool exitRoute;
};

#endif