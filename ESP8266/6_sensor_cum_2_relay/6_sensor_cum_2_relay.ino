#include <ESP8266WiFi.h>
#include <MCP3008.h>
#include <SPI.h>
MCP3008 adc(D5, D7, D6, D8);

int moisture1 = 0;
int moisture2 = 0;
int moisture3 = 0;
int moisture4 = 0;
int moisture5 = 0;
int moisture6 = 0;
int Relay3 = 2;
int Relay2 = 0;
int Relay1 = 4;

void setup()
{
  Serial.begin(115200);
  pinMode(Relay3,OUTPUT);
  pinMode(Relay2,OUTPUT);
  pinMode(Relay1,OUTPUT);
}

void loop()
{
  //moisture 1-3
  int moisture1 = adc.readADC(0);
  int moisture2 = adc.readADC(1);
  int moisture3 = adc.readADC(2);
  //moisture 4-6
  int moisture4 = adc.readADC(3);
  int moisture5 = adc.readADC(4);
  int moisture6 = adc.readADC(5);
  
  //moitsture 1-3
  Serial.print("Moisture1 = ");
  Serial.println(moisture1);
  delay(500);
  Serial.print("Moisture2 = ");
  Serial.println(moisture2);
  delay(500);
  Serial.print("Moisture3 = ");
  Serial.println(moisture3);
  delay(500);
  Serial.println("-----------------------------------------");
  //moisture 4-6
  Serial.print("Moisture4 = ");
  Serial.println(moisture4);
  delay(500);
  Serial.print("Moisture5 = ");
  Serial.println(moisture5);
  delay(500);
  Serial.print("Moisture6 = ");
  Serial.println(moisture6);
  delay(500);
  Serial.println("-----------------------------------------");
  if (moisture1 < 46)
  if (moisture2 < 46)
  if (moisture3 < 46){
    digitalWrite(Relay3, LOW);
    digitalWrite(Relay2, LOW);
  }
  else{
    digitalWrite(Relay3, HIGH);
    digitalWrite(Relay2, HIGH);
}
  if (moisture4 < 46)
  if (moisture5 < 46)
  if (moisture6 < 46){
    digitalWrite(Relay3, LOW);
    digitalWrite(Relay1, LOW);
  }
  else{
    digitalWrite(Relay3, HIGH);
    digitalWrite(Relay1, HIGH);
  }
}