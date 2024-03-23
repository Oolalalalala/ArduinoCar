#include "BasicControl.h"
#include "Arduino.h"


void InitPins()
{
    pinMode(PWMA, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
}


void MoveWheel(int leftSpeed, int rightSpeed)
{
  if (leftSpeed > 0)
  {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, leftSpeed);
  }
  else
  {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, -leftSpeed);
  }

  
  if (rightSpeed > 0)
  {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, rightSpeed);
  }
  else
  {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, -rightSpeed);
  }
}


int r1, r2, r3, r4, r5;

void CollectPinValue()
{
  r1 = digitalRead(32);
  r2 = digitalRead(34);
  r3 = digitalRead(36);
  r4 = digitalRead(38);
  r5 = digitalRead(40);
}

int GetOffsetValue()
{
  return (r1 * -4 + r2 * -1 + r4 * 1 + r5 * 4) / (r1 + r2 + r3 + r4 + r5);
}

bool OnNode()
{
  return r1 & r2 & r3 & r4 & r5;
}

bool OnRoute()
{
  if (OnNode())
    return false;
  return r1 | r2 | r3 | r4 | r5;
}


int oldTime = 0, currentTime = 0;

float GetDeltaTime()
{
   oldTime = currentTime;
   currentTime = micros();
   return 0.000001 * (currentTime - oldTime);
}