#ifndef BACIS_CONTROL_H
#define BASIC_CONTROL_H

#include "Arduino.h"

#define PWMA 11
#define PWMB 12
#define AIN1 2
#define AIN2 3
#define BIN1 5
#define BIN2 6
const int speed = 255;

void InitPins();
void MoveWheel(int leftSpeed, int rightSpeed);
void CollectPinValue();
int GetOffsetValue();
bool OnNode();
bool OnRoute();
int GetDetectorCount();
float GetDeltaTime();

#endif