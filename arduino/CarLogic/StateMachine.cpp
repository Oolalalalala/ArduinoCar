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
  case CarCommand::Sprint:
    return new SprintState();
  case CarCommand::TurnLeft:
    return new TurnLeftState();
  case CarCommand::TurnRight:
    return new TurnRightState();
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
  
  m_PreviousCommand = CarCommand::None;
  m_CurrentCommand = initialCommand;
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
    m_PreviousCommand = m_CurrentCommand;
    Bluetooth::SendMessage(1, nullptr, 0); // Request a new command
  }

  if (m_BufferedCommand == CarCommand::None)
  {
    if (Bluetooth::AvailableStateMessageCount())
      m_BufferedCommand = Bluetooth::ReadStateMessage();
  }

  if (m_BufferedCommand != CarCommand::None && m_ShouldDiscardCommand)
  {
    m_BufferedCommand = CarCommand::None;
    m_ShouldDiscardCommand = false;
  }

  if (!m_State && m_BufferedCommand != CarCommand::None)
  {
    // New state from command
    m_State = CreateCarState(m_BufferedCommand);
    m_State->OnStateEnter();
    m_State->m_StateMachine = this;

    m_CurrentCommand = m_BufferedCommand;
    m_BufferedCommand = CarCommand::None;
  }
}

void CarStateMachine::DiscardNextCommand()
{
  m_ShouldDiscardCommand = true;
}

CarCommand CarStateMachine::GetPreviousCommand()
{
  return m_PreviousCommand;
}