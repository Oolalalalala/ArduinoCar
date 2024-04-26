#include "BasicControl.h"

#define IFA InferredSensorArray


//   +--(FL)--C--(FR)--+
//   |                 |
//   |                 |
//   |                 |
//   |                 |
//   |                 |
//   +-(BL)-AB-DE-(BR)-+

enum class RelPos
{
  None = 0, A_C, B_C, D_C, E_C, Perfect, C_BL, C_BR, FL_BL, FR_BR, FL_A, FL_B, FL_D, FL_E, FR_A, FR_B, FR_D, FR_E
};


RelPos relPos = RelPos::None;

float Function


void setup() 
{
  IFA::Initialize();

  int a = IFA::GetState(0);
  int b = IFA::GetState(1);
  int c = IFA::GetState(2); // The back one
  int d = IFA::GetState(3);
  int e = IFA::GetState(4);

  if (c)
  {
    if      (a) relPos = RelPos::A_C;
    else if (b) relPos = RelPos::B_C;
    else if (d) relPos = RelPos::D_C;
    else if (e) relPos = RelPos::E_C;
  }
}

void loop()
{
  int a = IFA::GetState(0);
  int b = IFA::GetState(1);
  int c = IFA::GetState(2); // The back one
  int d = IFA::GetState(3);
  int e = IFA::GetState(4);

  float leftWheelSpeed, rightWheelSpeed;

  if (c)
  {
    if      (a) relPos = RelPos::A_C;
    else if (b) relPos = RelPos::B_C;
    else if (d) relPos = RelPos::D_C;
    else if (e) relPos = RelPos::E_C;
  }
  else
  {
    switch (relPos)
    {
      case RelPos::A_C:
      {
        if (a == 0)
        {

        }
        break;
      }

      case RelPos::B_C:
      {

        break;
      }

      case RelPos::D_C:
      {

        break;
      }

      case RelPos::E_C:
      {

        break;
      }
    }
  }

  switch (relPos)
  {
    case RelPos::A_C:
    {
      leftWheelSpeed = 50;
      rightWheelSpeed = 40;
      break;
    }

    case RelPos::B_C:
    {
      leftWheelSpeed = 50;
      rightWheelSpeed = 45;
      break;
    }

    case RelPos::D_C:
    {
      leftWheelSpeed = 45;
      rightWheelSpeed = 50;
      break;
    }

    case RelPos::E_C:
    {
      leftWheelSpeed = 40;
      rightWheelSpeed = 50;
      break;
    }

    case RelPos::Perfect:
    {
      leftWheelSpeed = 50;
      rightWheelSpeed = 50;
      break;
    }

    case RelPos::C_BL:
    {
      leftWheelSpeed = 55;
      rightWheelSpeed = 35;
      break;
    }

    case RelPos::C_BR:
    {
      leftWheelSpeed = 35;
      rightWheelSpeed = 55;
      break;
    }

    case RelPos::FL_BL:
    {
      leftWheelSpeed = 0;
      rightWheelSpeed = 0;
      break;
    }

    case RelPos::FR_BR:
    {
      leftWheelSpeed = 0;
      rightWheelSpeed = 0;
      break;
    }

    case RelPos::FL_A:
    {
      leftWheelSpeed = 0;
      rightWheelSpeed = 0;
      break;
    }

    case RelPos::FL_B:
    {
      leftWheelSpeed = 0;
      rightWheelSpeed = 0;
      break;
    }

    case RelPos::FL_D:
    {
      leftWheelSpeed = 0;
      rightWheelSpeed = 0;
      break;
    }

    case RelPos::FL_E:
    {
      leftWheelSpeed = 0;
      rightWheelSpeed = 0;
      break;
    }

    case RelPos::FR_A:
    {
      leftWheelSpeed = 0;
      rightWheelSpeed = 0;
      break;
    }

    case RelPos::FR_B:
    {
      leftWheelSpeed = 0;
      rightWheelSpeed = 0;
      break;
    }

    case RelPos::FR_D:
    {
      leftWheelSpeed = 0;
      rightWheelSpeed = 0;
      break;
    }

    case RelPos::FR_E:
    {
      leftWheelSpeed = 0;
      rightWheelSpeed = 0;
      break;
    }
  }

  CarMotor::SetSpeed(-rightWheelSpeed, -leftWheelSpeed);
}