#include <Wire.h>
#include <Adafruit_BMP280.h>

// We gebruiken nu de BMP bibliotheek
Adafruit_BMP280 bmp; 

const int pinKetel = 2;      // Rode LED
const int pinVentilator = 3; // Ventilator/Airco

void setup() {
  Serial.begin(9600);
  pinMode(pinKetel, OUTPUT);
  pinMode(pinVentilator, OUTPUT);

  Serial.println("BMP280 Thermostaat Test");

  // Initialisatie op adres 0x76
  if (!bmp.begin(0x76)) {
    Serial.println("Fout: Kon geen BMP280 sensor vinden op 0x76!");
    Serial.println("Check of de draden goed vastzitten.");
    while (1);
  }

  /* Instellingen voor de sensor (optioneel, voor betere nauwkeurigheid) */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
  // Temperatuur uitlezen
  float temp = bmp.readTemperature();
  float druk = bmp.readPressure() / 100.0F; // Luchtdruk in hPa

  // Weergave in Seriële Monitor
  Serial.print("Temperatuur: ");
  Serial.print(temp);
  Serial.print(" *C | ");
  Serial.print("Druk: ");
  Serial.print(druk);
  Serial.println(" hPa");

  // De thermostaat logica
  if (temp < 27.0) {
    digitalWrite(pinKetel, HIGH);   // Verwarming AAN
    digitalWrite(pinVentilator, LOW);
    Serial.println("STATUS: Verwarming actief [T < 20]");
  } 
  else if (temp > 28.0) {
    digitalWrite(pinKetel, LOW);
    digitalWrite(pinVentilator, HIGH); // Koeling AAN
    Serial.println("STATUS: Koeling actief [T > 23]");
  } 
  else {
    digitalWrite(pinKetel, LOW);
    digitalWrite(pinVentilator, LOW);
    Serial.println("STATUS: Temperatuur OK");
  }

  Serial.println("-----------------------------------");
  delay(2000); // Wacht 2 seconden voor de volgende meting
}