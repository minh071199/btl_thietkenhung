#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define BLYNK_PRINT Serial
#define DHTPIN 2
#define DHTTYPE DHT11
#define waterLevelSensor 32
#define moistureLevelSensor 34
#define lightSensor 35
#define MOTOR_EN 4
#define LIGHT_EN 5

float waterLevel = 0;
float moistureLevel = 0;
float light_level = 0;
float t = 0;
float h = 0;
bool motor = 0;
bool light = 0;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

char auth[] = "5YStVOxN_2ANlCBwm7TOwI7iwuBosYuI";
char ssid[] = "Ngoi Nha Hoa Hong";
char pass[] = "243tamtrinh";

void sendSensor1() //DHT11
{
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void sendSensor2() //Do am dat
{
  moistureLevel = 100 - map(analogRead(moistureLevelSensor), 0, 4095, 0, 100);
  Blynk.virtualWrite(V7, moistureLevel);
}

void sendSensor3() //Light sensor
{
  light_level = map(analogRead(lightSensor), 0, 4095, 0, 100);
  Blynk.virtualWrite(V8, light_level);
}

void sendSensor4() //Water Level sensor
{
  waterLevel = map(analogRead(waterLevelSensor), 0, 4095, 0, 100);
  Blynk.virtualWrite(V9, waterLevel);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(MOTOR_EN, OUTPUT);
  digitalWrite(MOTOR_EN, HIGH);
  pinMode(LIGHT_EN, OUTPUT);
  digitalWrite(LIGHT_EN, HIGH);
  dht.begin();
  timer.setInterval(2000L, sendSensor1); //DHT11 send
  timer.setInterval(5000L, sendSensor2); //Do Am Dat
  timer.setInterval(1000L, sendSensor3); //LightSensor
  timer.setInterval(1000L, sendSensor4); //WaterLevel
}

void loop() {
  Blynk.run();
  timer.run();
  
  if(moistureLevel<=70)
  motor = 1;
  else if (moistureLevel>=90)
  motor = 0;
  digitalWrite(MOTOR_EN, motor);
  
  if(waterLevel>=33)
  motor = 0;
  digitalWrite(MOTOR_EN, motor);
  
  if(light_level<=30)
  light = 1;
  else light = 0;
  digitalWrite(LIGHT_EN, light);
}
