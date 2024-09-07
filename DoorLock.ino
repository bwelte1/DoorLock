#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   5
#define SS_PIN    53

byte blueRFID[4] = {0x63, 0xD3, 0x21, 0x2D};
bool access = false;

MFRC522 mfrc522(RST_PIN, SS_PIN);

void setup()
{
    pinMode(13, OUTPUT);
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init(SS_PIN, RST_PIN);
    delay(400);
    mfrc522.PCD_DumpVersionToSerial();
    Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop()
{
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  access = true;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    if (mfrc522.uid.uidByte[i] != blueRFID[i]) 
    {
      access = false;
      break;
    }
  }

  if (access == true){
    Serial.println("RFID Authorised");
    delay(2000);
  }
  else {
    Serial.println("Unauthorised Access");
    delay(2000);
  }
  access = false;
}

