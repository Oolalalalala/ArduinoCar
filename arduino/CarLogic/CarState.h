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
};

class TestRFIDState : public CarState
{
public:
  virtual void OnStateEnter() override;
  virtual void OnStateUpdate(float dt) override;
  virtual void OnStateExit() override;

private:
  bool m_ReturnedNode;
  bool m_RFIDDetected;
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