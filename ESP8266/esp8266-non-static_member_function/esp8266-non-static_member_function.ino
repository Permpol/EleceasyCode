//--------------------------------------------------------------- LIBRARY SENSOR -----------------------------------------------------------------------------------------//
#define BLYNK_PRINT Serial
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <Scheduler.h> 
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"
#define DHTPIN D3
#define DHTTYPE DHT22   
#define prelay D4

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
LiquidCrystal_I2C lcd(0x27, 16, 2);  
WidgetLED led1(V9);
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------- VARIABLES --------------------------------------------------------------------------------------------//
int measurePin = A0;
int ledPower = D5;    
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
int statureset = 0;
float humi=0,tem=0,fa=0,hif=0,hic=0;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------- FUNCTION BLYNK --------------------------------------------------------------------------------------//
BLYNK_CONNECTED() {
  Blynk.syncAll();
}
class BlynkWriteTask : public Task { 
protected:
  BLYNK_WRITE(V11)
  {
   int statuspupm = 0;
   statuspupm = param.asInt();
   if(statuspupm==true){
    led1.on();
    pinMode(prelay,HIGH);
    //Serial.println("StatusPupm = ON");
    lcd.home();
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("StatusPupm = ON");
  }
  else{
    led1.off();
    pinMode(prelay,LOW);
    //Serial.println("StatusPupm = OFF");
    lcd.home();
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("StatusPupm = OFF");
  }
}
BLYNK_WRITE(V8)
{
  statureset = param.asInt();
  if(statureset==true){
    //Serial.println("Statureset = ON");
    //Serial.print("Count Restart MCU....");
    for(int i = 3; i >= 0; i--){
      //Serial.println(i);
      Blynk.virtualWrite(V7,i);
      delay(500);
    }
    Blynk.virtualWrite(V8,LOW);
    lcd.home();
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Restart MCU .....");
    ESP.restart(); 
  }
}
void setup() {  
      lcd.begin();
      lcd.backlight();
    }
    void loop() {
    } 
private:
    uint8_t state;
} blynkWrite_task;

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------- FUNCTION --------------------------------------------------------------------------------------------//
class Wifi_connect_Task : public Task { 
protected:
void connect_internet(){
  char auth[34] = "yz-122hagdcFwGKRmfL8jEWFdy5lWgew";
  char sever[15] = "103.233.194.42";
  int port = 8080;
  const char* ssid = "aisfibre_2.4G_Netpuk";
  const char* pass = "kaew170217";
   WiFi.begin(ssid, pass); 
   Blynk.begin(auth, ssid, pass, "103.233.194.42", 8080);
   while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting...  "); 
    Serial.printf("Connection Status: %d\n", WiFi.status()); 
    delay(1000);
  }
  Serial.print("Wi-Fi connected."); 
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
  Serial.print("WifiPercentSignal(%) : ");
  Serial.println(WifiPercentSignal());
}
  int WifiPercentSignal() {
  int CurrentWiFiSignal,WifiSignal;
  CurrentWiFiSignal = WiFi.RSSI();
  if(CurrentWiFiSignal > -40) CurrentWiFiSignal = -40;
  if(CurrentWiFiSignal < -90) CurrentWiFiSignal = -90;
  WifiSignal = map(CurrentWiFiSignal, -40, -90, 0, 100);
  return WifiSignal;
  }
void sendsensor(){
  Blynk.virtualWrite(V10,WifiPercentSignal());
} 
    void setup() {  
      connect_internet();
      timer.setInterval(1000L, sendsensor);
    }
    void loop() {
    } 
private:
    uint8_t state;
} wifi_task;

class AirDustTask : public Task { 
protected:
int Airdust(){
  digitalWrite(ledPower, LOW); 
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin); 
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH); 
  delayMicroseconds(sleepTime);
  calcVoltage = voMeasured * (3.3 / 1024);
  dustDensity = 0.17 * calcVoltage - 0.1;
  Serial.print("Raw Signal Value (0-1023): ");
  Serial.print(voMeasured);
  Serial.print(" - Voltage: ");
  Serial.print(calcVoltage);
  if (dustDensity <= 0.00) {
    dustDensity = 0.00;
  }
  dustDensity = dustDensity * 1000;
  Serial.print(" - Dust Density: ");
  Serial.print(dustDensity);
  Serial.println(" µg/m³");
  delay(1000);
  lcd.home();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Dust Density");
  lcd.setCursor(2, 1);
  lcd.print(dustDensity); 
  lcd.print(" ug/m3  ");
  return dustDensity; 
}
void sendsensor(){
  Blynk.virtualWrite(V1,Airdust());
}
    void setup() { 
      lcd.begin();
      lcd.backlight(); 
      timer.setInterval(1000L, sendsensor);
    }
    void loop() {
      Airdust();
    } 
private:
    uint8_t state;
} airdust_task;

class DhtTask: public Task { 
protected:
void dht2302() {
  humi = dht.readHumidity();
  tem = dht.readTemperature();
  fa = dht.readTemperature(true);
  if (isnan(humi) || isnan(tem) || isnan(fa)) {
   // Serial.println("Failed to read from DHT sensor!");
  lcd.home();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Failed to read from");
  lcd.setCursor(2, 1);
  lcd.print("from DHT sensor!");
  }
  hif = dht.computeHeatIndex(fa, humi);
  hic = dht.computeHeatIndex(tem, humi, false);
  humi=5;
  tem=6;
  fa=4;
  hic=7;
  hif=8;
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Humidity");
  lcd.setCursor(2, 1);
  lcd.print(humi); 
  lcd.print(" % ");
  delay(1000);
  lcd.home();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Temperature");
  lcd.setCursor(2, 1);
  lcd.print(tem); 
  lcd.print(" C ");
  delay(1000);
  lcd.home();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Fahrenheit");
  lcd.setCursor(2, 1);
  lcd.print(fa); 
  lcd.print(" F ");
  delay(1000);
  lcd.home();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Index in Fahrenheit");
  lcd.setCursor(2, 1);
  lcd.print(hif); 
  lcd.print(" F ");
  delay(1000);
  lcd.home();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Index in Celsius");
  lcd.setCursor(2, 1);
  lcd.print(hic); 
  lcd.print(" C ");
}
void sendsensor(){
  Blynk.virtualWrite(V3,humi);
  Blynk.virtualWrite(V2,tem);
  Blynk.virtualWrite(V4,fa);
  Blynk.virtualWrite(V5,hic);
  Blynk.virtualWrite(V6,hif);
}
    void setup() {  
      lcd.begin();
      lcd.backlight();
      dht.begin();
      timer.setInterval(1000L, sendsensor);
    }
    void loop() {
      dht2302(); 
    } 
private:
    uint8_t state;
} dht_task;

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------- SET DEFAULT--------------------------------------------------------------------------------------------//
void setup() {
   Serial.begin(9600);
   Scheduler.begin();
   Scheduler.start(&dht_task);
   Scheduler.start(&airdust_task);
   Scheduler.start(&wifi_task);
   Scheduler.start(&blynkWrite_task);
}
void loop() {
   Blynk.run();
   timer.run();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
