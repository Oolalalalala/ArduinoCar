#include <SoftwareSerial.h>


void setup() {
  Serial.begin(9600);   // 與電腦序列埠連線
  Serial.println("BT is ready!");

  Serial1.begin(9600);
}

char val;  // 儲存接收資料的變數
void loop() 
{
  while (Serial.available())
  {
    val = Serial.read();
    Serial1.print(val);
  }

  while (Serial1.available())
  {
    val = Serial1.read();
    Serial.print(val);
  }
}