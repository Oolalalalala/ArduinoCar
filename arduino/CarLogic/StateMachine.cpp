#include "StateMachine.h"

#include "ForwardState.h"
#include "RotateState.h"
#include "Bluetooth.h"
#include "Arduino.h"

static CarState* CreateCarState(CarCommand command)
{
    switch (command)
    {
      case CarCommand::Forward:   return new ForwardState();
      case CarCommand::TurnLeft:  return new TurnLeft1State();
      case CarCommand::TurnRight: return new TurnRight1State();
      case CarCommand::TurnLeft2: return new TurnLeft2State();
      case CarCommand::TestRFID:  return new TestRFIDState();
    }

    Serial.println("Error: Invalid CarCommand");
}

CarStateMachine::CarStateMachine()
{
  while (Bluetooth::AvailableStateMessageCount() < 2); // Wait for first two command message

  CarCommand initialCommand = Bluetooth::ReadStateMessage();
  CarCommand secondCommand = Bluetooth::ReadStateMessage();

  m_State = CreateCarState(initialCommand);
  m_State->m_StateMachine = this;
  m_State->OnStateEnter();
  
  m_BufferedCommand = secondCommand;
}

CarStateMachine::~CarStateMachine()
{
  m_State->OnStateExit();
  delete m_State;
}
// Mark the current state as finished, a new state will be grabbed from 
void CarStateMachine::NextState()
{
  m_StateEnded = true;
}

void CarStateMachine::OnUpdate(float dt)
{
    if (m_State)
      m_State->OnStateUpdate(dt);

    // Check if need to switch state
    if (m_StateEnded)
    {
      m_State->OnStateExit();
      delete m_State;
      m_State = nullptr;
      Bluetooth::SendMessage(1, nullptr, 0);
    }

    if (m_BufferedCommand == CarCommand::None)
    {
        if (Bluetooth::AvailableStateMessageCount())
            m_BufferedCommand = Bluetooth::ReadStateMessage();
    }

    if (!m_State && m_BufferedCommand != CarCommand::None)
    {
      // New state from command
      m_State = CreateCarState(m_BufferedCommand);
      m_State->OnStateEnter();

      m_BufferedCommand = CarCommand::None;
      m_StateEnded = false;
    }
}