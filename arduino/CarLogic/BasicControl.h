#ifndef BASIC_CONTROL_H
#define BASIC_CONTROL_H


class InferredSensorArray
{
public:
  static void Initialize();
  static void CollectState();
  static int GetState(int index); // 0 is the left-most sensor, 4 for the right-most sensor
  static int GetDetectionCount();
  static float GetNormalizedErrorValue(float e0, float e1, float e2, float e3, float e4);
  static int GetDetectionBlobCount();

  // Debug
  static void DebugOutput();

private:
  static int s_SensorStates[5];
};

class PIDController
{
public:
  PIDController();

  void Reset();
  void OnUpdate(float dt);
  void GetSpeed(int maxSpeed, float& leftWheelSpeed, float& rightWheelSpeed);

private:
  float m_PreviousError[2];
  float m_Timer;
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