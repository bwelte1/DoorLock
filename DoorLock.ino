#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN         5
#define SERVO_PIN       9
#define BOARD_LED_PIN   13
#define GREEN_LED_PIN   22
#define RED_LED_PIN     23
#define SS_PIN          53


byte blueRFID[4] = {0x63, 0xD3, 0x21, 0x2D};
bool access = false;
Servo lockServo;

MFRC522 mfrc522(RST_PIN, SS_PIN);

void setup()
{
  Serial.begin(9600);

  pinMode(SERVO_PIN, OUTPUT);
  pinMode(BOARD_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  lockServo.attach(SERVO_PIN);

  SPI.begin();
  mfrc522.PCD_Init(SS_PIN, RST_PIN);
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop()
{
  lockServo.write(0);
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
    Serial.println("Authorised Access");
    lockServo.write(90);
    authorisedRoutine();
  }
  else {
    Serial.println("Unauthorised Access");
    UNauthorisedRoutine();
  }
  access = false;
}

void authorisedRoutine()
{
  for (int i=0;i<2;i++)
  {
    digitalWrite(GREEN_LED_PIN, HIGH);
    delay(750);
    digitalWrite(GREEN_LED_PIN, LOW);
    delay(750);
  }
}

void UNauthorisedRoutine()
{
  for (int i=0;i<8;i++)
  {
    digitalWrite(RED_LED_PIN, HIGH);
    delay(750/4);
    digitalWrite(RED_LED_PIN, LOW);
    delay(750/4);
  }
}
