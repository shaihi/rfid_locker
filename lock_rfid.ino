#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address and the number of columns and rows
LiquidCrystal_I2C lcd(0x3F, 20, 4); // Change the address if necessary

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
Servo myservo;

//The card ID to which we will grant access
byte knownCard[] = {0x9e, 0x23, 0x68, 0x1d};

void setup() {
  // Initialize the LCD
  lcd.init();

  // Turn on the backlight
  lcd.backlight();

  // Print a message to the LCD
  lcd.setCursor(0, 0);
  lcd.print("Hello, LCD!");
  // put your setup code here, to run once:
  Serial.begin(9600);    // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  // setup servo pin
  myservo.attach(3);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  if(compareUID(mfrc522.uid.uidByte, knownCard, mfrc522.uid.size)) {
    Serial.println("Access granted!");
    myservo.write(0);
    delay(1000);
    myservo.write(90);
    delay(1000);
    myservo.write(180);
    delay(1000);
  } else {
    Serial.println("Access Denied!!");
  }
}

//a helper function to compare cards' id
bool compareUID(byte* uid1, byte* uid2, byte size) {
  for (byte i=0; i < size; i++){
    if (uid1[i] != uid2[i]) {
      return false; // card does not match our card
    }
  }
  return true; //it is our card
}
