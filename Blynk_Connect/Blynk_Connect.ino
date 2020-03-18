#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleDHT.h>
#include <SimpleTimer.h>

// DHT Config
int pinDHT22 = D5;
SimpleDHT22 dht22;

// SET Timer
SimpleTimer timer;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = " 2giJUJkaaVIk4Z9_0r8p7UAn3w2bN-bP";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "PCR008-5G";
char pass[] = "0874597999";

BLYNK_CONNECTED() {
    Blynk.syncAll();
}

void setup()
{
  // Debug console
  Serial.begin(9600);

 
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "blynk.iot-cm.com", 8080);
  timer.setInterval(4000L, sendTemp);
}

void sendTemp()
{
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(pinDHT22, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT22 failed, err="); Serial.println(err);delay(1000);
    return;
  }  
  Blynk.virtualWrite(10, temperature); // virtual pin 
  Blynk.virtualWrite(11, humidity); // virtual pin 
}

void loop()
{
  Blynk.run();
  timer.run();
}