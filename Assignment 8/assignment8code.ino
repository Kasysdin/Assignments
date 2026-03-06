#define BLYNK_TEMPLATE_ID "TMPL5-u1emwDB"
#define BLYNK_TEMPLATE_NAME "les"
#define BLYNK_AUTH_TOKEN "RfsZGXvbXndfn778aGixsIdD8vvSJscN"

#include <WiFi.h> // Gebruik <ESP8266WiFi.h> voor D1 Mini
#include <BlynkSimpleEsp32.h> // Gebruik <BlynkSimpleEsp8266.h> voor D1 Mini

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Daudtm";
char pass[] = "Daud2006";

const int ledPin = 2;    // GPIO2
const int potPin = 3;   // GPIO34 (Voor D1 Mini gebruik A0)

BlynkTimer timer;

// Functie om de ADC waarde te lezen en te versturen
void sendSensorData() {
  int sensorValue = analogRead(potPin); 
  Blynk.virtualWrite(V2, sensorValue); // Stuur naar smartphone
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  
  // Stuur elke seconde de potmeter waarde naar de app
  timer.setInterval(1000L, sendSensorData);
}

// Wordt aangeroepen als je de knop in de app indrukt
BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
  digitalWrite(ledPin, pinValue);
}

void loop() {
  Blynk.run();
  timer.run();
}

