#ifndef CAR_STATE_H
#define CAR_STATE_H

#include "BasicControl.h"

enum class CarCommand : unsigned char
{
    None = 0, Forward = 1, RotateLeft = 2, RotateRight = 3, TestRFID = 4, SprintState = 5, TurnLeftState = 6, TurnRightState = 7
};

// Forward declaration
class CarStateMachine;

class CarState
{
public:
  CarState() = default;
  virtual ~CarState() = default;

  virtual void OnStateEnter() = 0;
  virtual void OnStateUpdate(float dt) = 0; // dt in seconds
  virtual void OnStateExit() = 0;

protected:
  CarStateMachine* m_StateMachine = nullptr;

  friend class CarStateMachine;
};

class ForwardState : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

private:
  PIDController m_Controller;

  float m_DelayTimer;
  bool m_OnNode;
  bool m_ExitNode;
};

class TestRFIDState : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

  void ReturnSequence(float dt);
  void RetestSequence(float dt);

private:
  float m_SlowDownTimer;
  bool m_LeftNode;
  bool m_ReturnedNode;
  bool m_RFIDDetected;
  float m_ExitDelayTimer;
  bool m_ExitNode;
  int m_Correction;

  bool m_Retested;
  int m_RetestStage;
  float m_ReturnCorrection;

  // Variable for Retest
  bool m_RetestLeftNode;
  bool m_RetestOnNode;
  float m_RetestBrakeTimer;
  float m_RetestRotateBrakeTimer;
  bool m_RetestExitRotate;
  bool m_RetestExitNode;
  float m_RetestForwardTimer;
  float m_RetestRotateTimer;
};

class RotateLeftState : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

private:
  float m_BrakeTimer;
  bool m_ExitState;
  bool leftRoute;
  float m_Timer;
};


class RotateRightState : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

private:
  float m_BrakeTimer;
  bool m_ExitState;
  bool leftRoute;
  float m_Timer;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Extreme operations

class SprintState : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

private:
  PIDController m_Controller;

  bool m_LeftNode;
  float m_ImmunityTimer;
  bool m_OnNode;
  float m_BrakeTimer;
};

class TurnLeftState : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

private:
  float m_ImmunityTimer;
  float m_BrakeTimer;
  bool m_Exit;
  bool m_TwoBlobs;
};

class TurnRightState : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;
  
private:
  float m_ImmunityTimer;
  float m_BrakeTimer;
  bool m_Exit;
  bool m_TwoBlobs;
};

#endif