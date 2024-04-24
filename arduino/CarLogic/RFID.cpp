#include "RFID.h"
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 53
static MFRC522 *mfrc522;

void RFID::Initialize()
{
    SPI.begin();
    mfrc522 = new MFRC522(SS_PIN, RST_PIN);
    mfrc522->PCD_Init();
    Serial.println("Initialize RFID");
}

bool RFID::HasValue()
{
    return mfrc522->PICC_IsNewCardPresent() && mfrc522->PICC_ReadCardSerial();
}

uint32_t RFID::ReadValue()
{
    // Logging
    mfrc522->PICC_DumpDetailsToSerial(&(mfrc522->uid));

    uint32_t value;
    value =  mfrc522->uid.uidByte[0] << 24;
    value += mfrc522->uid.uidByte[1] << 16;
    value += mfrc522->uid.uidByte[2] << 8;
    value += mfrc522->uid.uidByte[3];

    mfrc522->PICC_HaltA();
    mfrc522->PCD_StopCrypto1();

    return value;
}