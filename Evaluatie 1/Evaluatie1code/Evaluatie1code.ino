#include <DHT11.h>          
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Pins definiëren
DHT11 dht11(2);             // DHT11 aangesloten op Digitale Pin 2
const int LM335PIN = A0;    // LM335 aangesloten op Analoge Pin A0

// Display initialiseren (Adres 0x27 is standaard voor de meeste I2C modules)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  lcd.init();
  lcd.backlight();
  
  // Geen dht.begin() nodig bij deze specifieke bibliotheek
  lcd.setCursor(0, 0);
  lcd.print("Systeem start...");
  delay(1000);
  lcd.clear();
}

void loop() {
  // --- DHT11 (Binnen) uitlezen met de nieuwe methode ---
  int temperatuurBinnen = 0;
  int luchtvochtigheid = 0;
  
  // Gebruik de methode uit jouw uitleg: readTemperatureHumidity(int &t, int &h)
  int resultaat = dht11.readTemperatureHumidity(temperatuurBinnen, luchtvochtigheid);

  // --- LM335 (Buiten) uitlezen ---
  int rawValue = analogRead(LM335PIN);
  // Omzetten naar spanning (ervan uitgaande dat Arduino precies 5V geeft)
  float voltage = rawValue * (5.0 / 1023.0);
  // Omzetten van Kelvin naar Celsius: (Spanning * 100) - 273.15
  float temperatuurBuiten = (voltage * 100) - 273.15;
  float offset = 2.0; // Het verschil dat je nu ziet
  temperatuurBuiten = temperatuurBuiten + offset;

  // --- LCD Display Update ---
  
  // Regel 1: Binnen (DHT11)
  lcd.setCursor(0, 0);
  if (resultaat == 0) { // 0 betekent meestal succes bij deze libraries
    lcd.print("Binnen: ");
    lcd.print(temperatuurBinnen);
    lcd.print((char)223); // Graden symbool
    lcd.print("C ");
  } else {
    // Foutafhandeling met getErrorString uit jouw uitleg
    lcd.print("DHT Fout!     "); 
    // Optioneel: Serial.println(DHT11::getErrorString(resultaat));
  }

  // Regel 2: Buiten (LM335)
  lcd.setCursor(0, 1);
  lcd.print("Buiten: ");
  lcd.print(temperatuurBuiten, 1); // 1 decimaal voor LM335
  lcd.print((char)223);
  lcd.print("C ");

  // De DHT11 heeft minimaal 1 tot 2 seconden nodig tussen metingen
  delay(2000); 
}