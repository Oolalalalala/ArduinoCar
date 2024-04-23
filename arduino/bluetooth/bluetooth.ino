#include <SoftwareSerial.h>
#include <stdint.h>

void setup() {
  Serial.begin(9600);   // 與電腦序列埠連線
  Serial.println("BT is ready!");

  Serial1.begin(9600);
  
  while (Serial.available() == 0);

  while (Serial.available())
    Serial.read();
}

char val;  // 儲存接收資料的變數
uint8_t message[32];

void loop() 
{
  //while (Serial.available())
  //{
  //  val = Serial.read();
  //  Serial1.write("<14abcd>", 8);
  //}
  

  while (Serial1.available())
  {
    val = Serial1.read();
    Serial.print(val);
  }

  message[0] = '<';
  message[1] = 1;
  message[2] = 0;
  message[3] = '>';
  
  Serial.println("aaa");
  Serial1.write(message, 4);
  
  delay(1000);

  
  message[0] = '<';
  message[1] = 2;
  message[2] = 4;
  uint32_t uid = 0xABCDEF01;
  memcpy(message + 3, &uid, 4);
  message[7] = '>';

  Serial.println("bbb");
  Serial.print(uid);
  Serial1.write(message, 8);

  delay(5000);

}