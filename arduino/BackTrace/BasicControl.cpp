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
  for (int i = 0; i < D_DATA_AMOUNT; i++){
    m_ErrorDataPoints[i] = InferredSensorArray::GetNormalizedErrorValue(CAR_PATH_TRACE_INFERRED_WEIGHT);
    m_BackErrorDataPoints[i] = InferredSensorArray::GetNormalizedErrorValue(CAR_PATH_TRACE_INFERRED_WEIGHT);
  }
}

void PIDController::OnUpdate(float dt)
{
  m_Timer += dt;
  while (m_Timer >= D_UPDATE_RATE)
  {
    for (int i = 0; i < D_DATA_AMOUNT; i++)
    //   m_ErrorDataPoints[i + 1] = m_ErrorDataPoints[i];
      m_ErrorDataPoints[D_DATA_AMOUNT - i] = m_ErrorDataPoints[D_DATA_AMOUNT - i - 1];

    m_ErrorDataPoints[0] = InferredSensorArray::GetNormalizedErrorValue(CAR_PATH_TRACE_INFERRED_WEIGHT);
    m_Timer -= D_UPDATE_RATE;
  }
}

void PIDController::OnBackUpdate(float dt)
{
  m_Timer += dt;
  while (m_Timer >= D_UPDATE_RATE)
  {
    for (int i = 0; i < D_DATA_AMOUNT; i++)
      m_BackErrorDataPoints[D_DATA_AMOUNT - i] = m_BackErrorDataPoints[D_DATA_AMOUNT - i - 1];

    m_BackErrorDataPoints[0] = InferredSensorArray::GetNormalizedErrorValue(CAR_PATH_TRACE_INFERRED_WEIGHT);
    m_Timer -= D_UPDATE_RATE;
  }
  //Debug
  // Serial.print(m_ErrorDataPoints[0]); Serial.print(" ");
  // Serial.print(m_ErrorDataPoints[1]); Serial.print(" ");
  // Serial.print(m_ErrorDataPoints[2]); Serial.print(" ");
  // Serial.print(m_ErrorDataPoints[3]); Serial.print(" ");
  // Serial.print(m_ErrorDataPoints[4]); Serial.print(" ");
  // Serial.print(m_ErrorDataPoints[5]); Serial.print(" ");
  // Serial.print(m_ErrorDataPoints[6]); Serial.print(" ");


}

void PIDController::GetSpeed(float& leftWheelSpeed, float& rightWheelSpeed)
{
  float currentError = InferredSensorArray::GetNormalizedErrorValue(CAR_PATH_TRACE_INFERRED_WEIGHT);
  float derivativeSum = 0.0f;

  for (int i = 0; i < D_DATA_AMOUNT; i++)
  {
    derivativeSum += (currentError - m_ErrorDataPoints[i]);
  }

  derivativeSum /= (D_DATA_AMOUNT * D_DATA_AMOUNT * D_UPDATE_RATE / 2);
  //Serial.println(derivativeSum); // Debug

  float offset = CAR_SPEED * (InferredSensorArray::GetNormalizedErrorValue(CAR_PATH_TRACE_INFERRED_WEIGHT) * CAR_PATH_TRACE_ADJUST_P + derivativeSum * CAR_PATH_TRACE_ADJUST_D);
  
  if (offset > 0)
  {
    leftWheelSpeed = CAR_SPEED;
    rightWheelSpeed = CAR_SPEED - offset;
  }
  else
  {
    rightWheelSpeed = CAR_SPEED;
    leftWheelSpeed = CAR_SPEED + offset;
  }
}

void PIDController::GetBackSpeed(float& leftWheelSpeed, float& rightWheelSpeed)
{
  float currentError = InferredSensorArray::GetNormalizedErrorValue(CAR_PATH_TRACE_INFERRED_WEIGHT);
  float derivativeSum = 0.0f;

  for (int i = 0; i < D_DATA_AMOUNT; i++)
  {
    derivativeSum += (currentError - m_BackErrorDataPoints[i]) * ((i + 2) / D_DATA_AMOUNT);
  }
  derivativeSum /= (D_DATA_AMOUNT * D_DATA_AMOUNT * D_UPDATE_RATE / 2);

  // Serial.print(currentError); // Debug
  // Serial.print(" ");
  Serial.println(derivativeSum * D_UPDATE_RATE); // Debug

  float offset = CAR_SPEED * derivativeSum * CAR_BACK_TRACE_ADJUST_D;


  if (offset > 0)
  {
    leftWheelSpeed = - CAR_SPEED;
    rightWheelSpeed = - CAR_SPEED - offset;
  }
  else
  {
    rightWheelSpeed = - CAR_SPEED;
    leftWheelSpeed = - CAR_SPEED + offset;
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