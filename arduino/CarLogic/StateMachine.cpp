#include "StateMachine.h"
#include "Arduino.h"
#include "Bluetooth.h"
#include "BasicControl.h"

static CarState* CreateCarState(CarCommand command)
{
  switch (command)
  {
  case CarCommand::Forward:
    return new ForwardState();
  case CarCommand::RotateLeft:
    return new RotateLeftState();
  case CarCommand::RotateRight:
    return new RotateRightState();
  case CarCommand::TestRFID:
    return new TestRFIDState();
  }

  Serial.println("Error: Invalid CarCommand");
}

CarStateMachine::CarStateMachine()
{
}

CarStateMachine::~CarStateMachine()
{
  m_State->OnStateExit();
  delete m_State;
}

void CarStateMachine::WaitForInitialCommand()
{
  // Wait for the first two command message to arrive
  while (Bluetooth::AvailableStateMessageCount() < 2)
    ;

  CarCommand initialCommand = Bluetooth::ReadStateMessage();
  CarCommand secondCommand = Bluetooth::ReadStateMessage();

  Serial.println("First two message received");

  m_State = CreateCarState(initialCommand);
  m_State->m_StateMachine = this;
  m_State->OnStateEnter();

  m_BufferedCommand = secondCommand;
}

void CarStateMachine::NextState()
{
  m_StateEnded = true;
}

void CarStateMachine::OnUpdate(float dt)
{
  if (m_State)
    m_State->OnStateUpdate(dt);
  else
  {
    CarMotor::SetSpeed(0, 0); // Idle if no command has arrived
  }

  // Check if need to switch state
  if (m_StateEnded)
  {
    m_State->OnStateExit();
    Serial.println(m_DebugTimer.Tick());
    delete m_State;
    m_State = nullptr;
    m_StateEnded = false;
    Bluetooth::SendMessage(1, nullptr, 0); // Request a new command
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
    m_State->m_StateMachine = this;

    m_BufferedCommand = CarCommand::None;
  }
}

void CarStateMachine::DiscardNextCommand()
{
  while (m_BufferedCommand == CarCommand::None)
  {
    if (Bluetooth::AvailableStateMessageCount())
      m_BufferedCommand = Bluetooth::ReadStateMessage();
  }

  m_BufferedCommand = CarCommand::None;
}