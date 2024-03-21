#ifndef BASE_STATE_H
#define BASE_STATE_H

// Forward delcaration
class CarStateMachine;

class CarState
{
public:
  CarState() {};
  virtual ~CarState() {};

  virtual void OnStateEnter() = 0;
  virtual void OnStateUpdate(float dt) = 0; // dt in seconds
  virtual void OnStateExit() = 0;

protected:
  CarStateMachine* m_StateMachine = nullptr;

  friend class CarStateMachine;
};

class CarStateMachine
{
public:
  CarStateMachine(CarState* beginState)
  {
    m_State = beginState;
    m_State->m_StateMachine = this;
    m_State->OnStateEnter();
  }

  ~CarStateMachine()
  {
    m_State->OnStateExit();
    delete m_State;
  }

  void SwitchState(CarState* newState)
  {
    m_State->OnStateExit();
    delete m_State;
    m_State = newState;
    m_State->OnStateEnter();
    m_State->m_StateMachine = this;
  }
  
  void OnUpdate(float dt)
  {
    m_State->OnStateUpdate(dt);
  }

private:
  CarState* m_State;
};

#endif