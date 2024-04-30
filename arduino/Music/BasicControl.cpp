#include "BasicControl.h"
#include "Arduino.h"
#include "MusicConfig.h"


#define PWMA 11
#define PWMB 12
#define AIN1 2
#define AIN2 3
#define BIN1 5
#define BIN2 6

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


MusicController::MusicController()
{
  Reset();
}

void MusicController::Reset()
{
  time_from_start = 0;
}

void MusicController::OnUpdate(float dt)
{
  time_from_start += dt;
}

void MusicController::GetMusicSpeed(float& leftWheelSpeed, float& rightWheelSpeed)
{
  return;
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