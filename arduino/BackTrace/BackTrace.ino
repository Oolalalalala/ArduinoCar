#include "BasicControl.h"
#include "Config.h"

#define CAR_RFID_REVERSE_SPEED -150

float m_ReturnCorrection = 0.0f;
Timer timer;
void setup() 
{
  Serial.begin(9600);
  InferredSensorArray::Initialize();
  timer.Tick();
}

int c = 0;

void loop()
{
    InferredSensorArray::CollectState();
    float dt = timer.Tick();

    if (InferredSensorArray::GetState(0) || InferredSensorArray::GetState(1))
      m_ReturnCorrection = -CAR_RFID_REVERSE_SPEED * -0.3f;
    if (InferredSensorArray::GetState(3) || InferredSensorArray::GetState(4))
      m_ReturnCorrection = -CAR_RFID_REVERSE_SPEED * 0.3f;
    if (c == 0 && InferredSensorArray::GetState(2))
      m_ReturnCorrection = 0;

    m_ReturnCorrection *= pow(0.2, dt);

    float leftWheelSpeed, rightWheelSpeed;
    if (m_ReturnCorrection > 0)
    {
      leftWheelSpeed = CAR_RFID_REVERSE_SPEED;
      rightWheelSpeed = CAR_RFID_REVERSE_SPEED + m_ReturnCorrection;
    }
    else
    {
      leftWheelSpeed = CAR_RFID_REVERSE_SPEED - m_ReturnCorrection;
      rightWheelSpeed = CAR_RFID_REVERSE_SPEED;
    }
    Serial.println("-----");
    Serial.println(leftWheelSpeed);
    Serial.println(rightWheelSpeed);
    CarMotor::SetSpeed(leftWheelSpeed, rightWheelSpeed);
}