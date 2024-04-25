#ifndef CAR_STATE_H
#define CAR_STATE_H


enum class CarCommand : unsigned char
{
    None = 0, Forward = 1, RotateLeft = 2, RotateRight = 3, TestRFID = 4
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
  void AbortSequence(float dt);

private:
  bool m_ReturnedNode;
  bool m_RFIDDetected;
  float m_ExitDelayTimer;
  bool m_ExitNode;

  bool m_Aborted;
  int m_AbortStage;

  // Variable for abort
  bool m_AbortLeftNode;
  bool m_AbortOnNode;
  float m_AbortEnterNodeImmunityTimer;
  bool m_AbortExitNode;
  float m_AbortExitDelayTimer;
};

class RotateLeftState : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

private:
  bool leftRoute;
};


class RotateRightState : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

private:
  bool leftRoute;
};

#endif