int PWMA = 11;
int PWMB = 12;
int AIN1 = 2;
int AIN2 = 3;
int BIN1 = 5;
int BIN2 = 6;
int speed = 255;
float adjustConstant = speed / 5;

enum class CarState
{
  None = 0, Forward, RotateLeft1, RotateLeft2, RotateRight1, RotateRight2
};

CarState currentState = CarState::Forward;

void setup() {
    pinMode(PWMA, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    Serial.begin(9600);
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

void ReadPinValue()
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

bool ReachNode()
{
  return r1 & r2 & r3 & r4 & r5;
}

bool MeetRoute()
{
  return r1 | r2 | r3 | r4 | r5;
}

int oldTime = 0, currentTime = 0;

float GetDeltaTime()
{
   oldTime = currentTime;
   currentTime = millis();
   return 0.001 * (currentTime - oldTime);
}
float leftWheelSpeed = 100.0, rightWheelSpeed = 100.0;

bool x = false;
void loop() 
{
  if (x)
  MoveWheel(255, 100);
  else
  MoveWheel(255,255);
  x = !x;
  delay(1000);
  return;
  ReadPinValue();

  float offset = GetOffsetValue() * adjustConstant;// * GetDeltaTime();
  if (offset > 0)
  {
    leftWheelSpeed = speed;
    rightWheelSpeed = speed - offset;
  }
  else
  {
    rightWheelSpeed = speed;
    leftWheelSpeed = speed + offset;
  }
  leftWheelSpeed *= 0.85;
  MoveWheel(leftWheelSpeed, rightWheelSpeed);


  delay(1);
}