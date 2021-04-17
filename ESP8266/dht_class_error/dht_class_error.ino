
#define BLYNK_PRINT Serial    
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h> 
#include <SimpleTimer.h> 
#include <DHT.h> 
#include <DHT_U.h>
#define DHTPIN 12 
#define DHTTYPE DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);

char auth[] = "xxxxxxxxxxxxxxxxxxxx";
 
SimpleTimer timer;
 
void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, "aisfibre_2.4G annne", "0872668855");
 
  dht.begin();
  
  // Setup a function to be called every second
  timer.setInterval(1000L, sendUptime);
}
 
void sendUptime()
{
  Blynk.virtualWrite(V5, millis() / 1000);
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) {
    Blynk.virtualWrite(V1, event.temperature);
    if(event.temperature > 30){
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  dht.humidity().getEvent(&event);
  if (!isnan(event.relative_humidity)) {
    Blynk.virtualWrite(V2, event.relative_humidity);
  }
}
void loop()
{
  Blynk.run();
  timer.run();
}
