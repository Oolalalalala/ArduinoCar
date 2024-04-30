#include "BasicControl.h"

Timer timer;
MusicController m_controller;

void setup(){
    Serial.begin(9600);
}

void loop()
{
  float leftWheelSpeed, rightWheelSpeed;
  float m_DelayTimer;

  float dt = timer.Tick();
  m_controller.OnUpdate(dt);
  m_controller.GetMusicSpeed(leftWheelSpeed, rightWheelSpeed);
  }