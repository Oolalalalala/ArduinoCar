#ifndef BASIC_CONTROL_H
#define BASIC_CONTROL_H
#include "MusicConfig.h"

class MusicController
{
public:
  MusicController();

  void Reset();
  void OnUpdate(float dt);
  void GetMusicSpeed(float& leftWheelSpeed, float& rightWheelSpeed);

private:
  float time_from_start;
  int left_note_sequence[4] = {
    ___, ___, ___, ___};
  int right_note_sequence = {};
  int left_node_to_output[25] = {};
  int right_node_to_output[25] = {};
};

class CarMotor
{
public:
  static void SetSpeed(int leftSpeed, int rightSpeed); // Range: [-255, 255]
};

class Timer
{
public:
  Timer();

  float Tick(); // In seconds
  
private:
  unsigned long m_Time;
};

#endif