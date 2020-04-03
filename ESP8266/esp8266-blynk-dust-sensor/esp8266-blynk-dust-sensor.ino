
#include "DHT.h"

#define DHTPIN D3 // what digital pin we're connected to
#define DHTTYPE DHT22 // DHT 11
//#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21 // DHT 21 (AM2301)


#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
char auth[] = "xxxxxxxxxxxxxxxxxxxxxxxxx";


int measurePin = A0;
int ledPower = D5;
 
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
 
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

float t;                                   // Declare the variables 
float h;
 
void setup(){
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
  Serial.begin(9600);
Serial.println("DHTxx test!");
  Serial.begin(9600);
  Blynk.begin(auth, "xxxxxxxxx", "xxxxxxxxxxxx","blynk.honey.co.th",8080);
  timer.setInterval(2000, sendUptime);


dht.begin();
}

void sendUptime()
{
  
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  float d = dustDensity ;
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V3, dustDensity);
}

void loop(){
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
 
  voMeasured = analogRead(measurePin); // read the dust value
 
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
 
  // 0 - 3.3V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (3.3 / 1024);
 
  
  dustDensity = 0.17 * calcVoltage - 0.1;
 
if (dustDensity <= 0.00) {
    dustDensity = 0.00;

    Blynk.run();
    timer.run();
  }


  dustDensity = dustDensity * 1000;

  Serial.print(" - Dust Density: ");
  Serial.print(dustDensity);
  Serial.println(" µg/m³");
  delay(3000);

float d = dustDensity ;  
float h = dht.readHumidity();
float t = dht.readTemperature();
float f = dht.readTemperature(true);

// เช็คถ้าอ่านค่าไม่สำเร็จให้เริ่มอ่านใหม่
if (isnan(h) || isnan(t) || isnan(f)) {
Serial.println("Failed to read from DHT sensor!");
return;
}

Serial.print("Humidity: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(t);
Serial.print(" *C ");
Serial.print(f);
Serial.print(" *F\t\n");
}