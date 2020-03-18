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
char auth[] = " qchomkKyG9aMHI8n4iziH9P-BrniHoSk";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Eleceasy";
char pass[] = "Bb20072536";

BLYNK_CONNECTED() {
    Blynk.syncAll();
}

void setup()
{
  // Debug console
  Serial.begin(9600);

 
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "oasiskit.com", 8080);
  timer.setInterval(4000L, sendTemp);
}

void sendTemp()
{
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  //if ((err = dht22.read2(pinDHT22, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    //Serial.print("Read DHT22 failed, err="); Serial.println(err);delay(1000);
    //return;
 // }  
  Blynk.virtualWrite(10, temperature); // virtual pin 
  Blynk.virtualWrite(11, humidity); // virtual pin 
}

void loop()
{
  Blynk.run();
  timer.run();
}