#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11   // DHT 11

//const int waterLevelSensor = 2;
const int moistureLevelSensor = 34;
//const int relay = 23;

//int waterLevel;
int moistureLevel;
int MOTOR_EN = 4;
int motor = 0;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Go to the Project Settings (nut icon).
char auth[] = "mRcpBmizT-8Uz6ie33nXe9mYQpzD_1nP";

char ssid[] = "Ngoi Nha Hoa Hong";
char pass[] = "243tamtrinh";


void sendSensor1()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
    
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);

}

void sendSensor2()
{
//  waterLevel = analogRead(waterLevelSensor);
//  waterLevel = map(waterLevel,0,2000,0,20);
//  moistureLevel = analogRead(moistureLevelSensor);
//  moistureLevel = map(moistureLevel,0,4000,0,20);
  moistureLevel = 100 - map(analogRead(moistureLevelSensor), 0, 5000, 0, 100);
  

//  Blynk.virtualWrite(V7, waterLevel);
  Blynk.virtualWrite(V7, moistureLevel);
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(MOTOR_EN, OUTPUT);
  digitalWrite(MOTOR_EN, LOW);
  dht.begin();

    // Setup a function to be called every second
  timer.setInterval(2000L, sendSensor1);
  timer.setInterval(1000L, sendSensor2);

}


void loop() {
  Blynk.run();
  timer.run();
  if(moistureLevel<30)
  motor = 1;
  else motor = 0;
  
  digitalWrite(MOTOR_EN, motor);
  delay(300);
}
