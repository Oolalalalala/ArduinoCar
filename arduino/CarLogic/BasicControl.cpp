#include "BasicControl.h"
#include "Arduino.h"
#include "Config.h"


#define PWMA 11
#define PWMB 12
#define AIN1 2
#define AIN2 3
#define BIN1 5
#define BIN2 6


int InferredSensorArray::s_SensorStates[5];

void InferredSensorArray::Initialize()
{
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}

void InferredSensorArray::CollectState()
{
  s_SensorStates[0] = digitalRead(32);
  s_SensorStates[1] = digitalRead(34);
  s_SensorStates[2] = digitalRead(36);
  s_SensorStates[3] = digitalRead(38);
  s_SensorStates[4] = digitalRead(40);
}

int InferredSensorArray::GetState(int index)
{
  return s_SensorStates[index];
}

int InferredSensorArray::GetDetectionCount()
{
  return s_SensorStates[0] + s_SensorStates[1] + s_SensorStates[2] + s_SensorStates[3] + s_SensorStates[4];
}

float InferredSensorArray::GetNormalizedErrorValue(float e0, float e1, float e2, float e3, float e4)
{
  int count = GetDetectionCount();
  if (count == 0)
    return 0;
  float error = e0 * s_SensorStates[0] + e1 * s_SensorStates[1] + e2 * s_SensorStates[2] + e3 * s_SensorStates[3] + e4 * s_SensorStates[4];
  return error / count;
}

void InferredSensorArray::DebugOutput()
{
  Serial.print(s_SensorStates[0]);
  Serial.print(s_SensorStates[1]);
  Serial.print(s_SensorStates[2]);
  Serial.print(s_SensorStates[3]);
  Serial.print(s_SensorStates[4]);
  Serial.print('\n');

  
}


int InferredSensorArray::GetDetectionBlobCount()
{
  int blobs = s_SensorStates[0];
  for (int i = 1; i < 5; i++)
  {
    if (!s_SensorStates[i - 1] && s_SensorStates[i])
      blobs++;
  }
  return blobs;
}

void CarMotor::SetSpeed(int leftWheelSpeed, int rightWheelSpeed)
{
  if (leftWheelSpeed > 0)
  {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, leftWheelSpeed);
  }
  else
  {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, -leftWheelSpeed);
  }

  
  if (rightWheelSpeed > 0)
  {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, rightWheelSpeed);
  }
  else
  {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, -rightWheelSpeed);
  }
}


PIDController::PIDController()
{
  Reset();
}

void PIDController::Reset()
{
  m_Timer = 0.0f;
  m_PreviousError[0] = 0.0f;
  m_PreviousError[1] = 0.0f;
}

void PIDController::OnUpdate(float dt)
{
  m_Timer += dt;
  while (m_Timer >= 0.01f)
  {
    m_PreviousError[1] = m_PreviousError[0];
    m_PreviousError[0] = InferredSensorArray::GetNormalizedErrorValue(CAR_PATH_TRACE_PID_WEIGHT);
    m_Timer -= 0.01f;
  }
}

void PIDController::GetSpeed(int maxSpeed, float& leftWheelSpeed, float& rightWheelSpeed)
{
  float currentError = InferredSensorArray::GetNormalizedErrorValue(CAR_PATH_TRACE_PID_WEIGHT);
  float derivativeSum = 0.0f;

  float derivative = (currentError - m_PreviousError[0]) / 0.01f + (currentError - m_PreviousError[1]) / 0.01f * 2.1f;
  float offset = maxSpeed * (currentError * CAR_PATH_TRACE_PID_P + derivative * CAR_PATH_TRACE_PID_D);

  
  if (offset > 0)
  {
    leftWheelSpeed = maxSpeed;
    rightWheelSpeed = maxSpeed - offset;
  }
  else
  {
    rightWheelSpeed = maxSpeed;
    leftWheelSpeed = maxSpeed + offset;
  }
}

Timer::Timer()
{
  m_Time = micros();
}

float Timer::Tick()
{
  unsigned long currentTime = micros();
  unsigned long delta = currentTime - m_Time;
  m_Time = currentTime;
  return delta * 0.000001f;
}