#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

const int irSensorPin = 14;  // Pino do sensor IR

#define SDA_PIN 5
#define SCK_PIN 18
#define MOSI_PIN 23
#define MISO_PIN 19
#define RST_PIN 4

#define RELAY_PIN 32


LiquidCrystal_I2C lcd(0x27, 16, 2);

MFRC522 rfid(SDA_PIN, RST_PIN);

const int pinBuzzer = 12;

void setup() {

  pinMode(irSensorPin, INPUT);

  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SDA_PIN);
  rfid.PCD_Init();
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);

  pinMode(pinBuzzer, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(1, 0);
  lcd.print("CPS: ");
}

void loop() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String conteudo = "";
    for (byte i = 0; i < 4; i++) {
      conteudo.concat(String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""));
      conteudo.concat(String(rfid.uid.uidByte[i], HEX));
    }

    int irSensorValue = digitalRead(irSensorPin);
    
    if (conteudo == "165ca489") {  //Substitua XXYYZZAA pelo código do cartão autorizado
      Serial.println("Cartão autorizado!");

      lcd.clear();
      lcd.print("AUTORIZADO!");

      digitalWrite(pinBuzzer, HIGH);
      digitalWrite(RELAY_PIN, HIGH);  // Ativa o relé e destrava a mini solenoide
      delay(1000);
      digitalWrite(pinBuzzer, LOW);
    } else {
      Serial.println("Cartão não autorizado!");

      lcd.clear();
      lcd.print("NAO AUTORIZADO!");

      digitalWrite(pinBuzzer, HIGH);
      delay(1000);
      digitalWrite(pinBuzzer, LOW);
    }
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
