#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_ADS1X15.h>
#include <PCF8574.h>

// 1. Instellen van de adressen op basis van jouw scan
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Het scherm op 0x27
PCF8574 pcf(0x26);                   // De LED-expander op 0x26 (met de jumper op A0)

// 2. Overige componenten
Adafruit_BMP280 bmp;                 // BMP280 (moet 0x76 zijn)
Adafruit_ADS1115 ads;                // ADS1115 (moet 0x48 zijn)

const int pinKetel = 2;              // Rode LED op Arduino
const int pinAirco = 3;              // Ventilator op Arduino

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Start LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Systeem Start...");

  // Start BMP280
  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 niet gevonden!");
    lcd.setCursor(0, 1);
    lcd.print("Sensor Fout!");
    while (1);
  }

  // Start ADC en Expander
  ads.begin();
  pcf.begin();

  pinMode(pinKetel, OUTPUT);
  pinMode(pinAirco, OUTPUT);
  
  delay(1000);
  lcd.clear();
}

void loop() {
  // --- METINGEN ---
  float tempDigi = bmp.readTemperature();
  
  // Analoge meting via ADS1115 (LM335 op pin A0)
  int16_t adc0 = ads.readADC_SingleEnded(0);
  float voltage = ads.computeVolts(adc0);
  float tempAna = (voltage * 100.0) - 273.15; 

  float verschil = abs(tempDigi - tempAna);

  // --- THERMOSTAAT LOGICA ---
  String mode = "OK";
  if (tempDigi < 20.0) {
    digitalWrite(pinKetel, HIGH); digitalWrite(pinAirco, LOW);
    mode = "KETEL";
  } else if (tempDigi > 23.0) {
    digitalWrite(pinKetel, LOW); digitalWrite(pinAirco, HIGH);
    mode = "AIRCO";
  } else {
    digitalWrite(pinKetel, LOW); digitalWrite(pinAirco, LOW);
  }

  // --- LED THERMOMETER (PCF8574 op 0x26) ---
  // We mappen 15-30 graden naar 0-8 LED's
  int ledAantal = map(constrain(tempDigi, 15, 30), 15, 30, 0, 8);
  uint8_t ledPatroon = 0;
  for (int i = 0; i < ledAantal; i++) {
    ledPatroon |= (1 << i); 
  }
  // Schrijf naar de expander. Let op: ~ (tilde) keert de bits om als je LEDs aan GND zitten.
  pcf.write8(~ledPatroon); 

  // --- LCD UPDATE ---
  lcd.setCursor(0, 0);
  lcd.print("D:"); lcd.print(tempDigi, 1);
  lcd.print(" A:"); lcd.print(tempAna, 1);
  lcd.print("  "); lcd.print(mode);

  lcd.setCursor(0, 1);
  if (verschil > 2.0) {
    // Knipperen bij alarm
    if ((millis() / 500) % 2 == 0) {
      lcd.print("!! AFWIJKING !! ");
    } else {
      lcd.print("                ");
    }
  } else {
    lcd.print("Verschil: "); lcd.print(verschil, 1); lcd.print("C ");
  }

  delay(200);
}