#include "DHT.h"
#include <TridentTD_LineNotify.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define DHTPIN D4     // what digital pin we're connected to
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21 // DHT 21 (AM2301)
#define BLYNK_PRINT Serial // Comment this out to disable prints and save space
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
#define LINE_TOKEN "x" // แก้ Line Token
char auth[] = "Tx";
int measurePin = A0;
int ledPower = D2;
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
float d = dustDensity;
float t; // Declare the variables
float h;
bool lock = false;
WidgetLCD lcd(V0);
void setup()
{
    Serial.begin(9600);
    pinMode(ledPower, OUTPUT);
    Serial.println("DHTxx test!");
    Blynk.begin(auth, "WTF", "xxxxxxxxx0!", "blynk.honey.co.th", 8080);
    Serial.println(LINE.getVersion());
    // Serial.printf("WiFi connecting to %s\n");
    // while (WiFi.status() != WL_CONNECTED) {
    // Serial.print(".");
    // delay(400);
    // }
    Serial.println(WiFi.localIP());
    LINE.setToken(LINE_TOKEN);
    LINE.notify("PM 2.5: ");
    dht.begin();
    lcd.clear();                        //Use it to clear the LCD Widget
    lcd.print(1, 0, "Air measurement"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
    lcd.print(5, 1, "Monitor");
}
BLYNK_WRITE(V5)
{

     if (param.asInt()==1) {
         lock = true;

 }else{
         lock = false;

 }

}
//BLYNK_WRITE(V5)
//{
// LINE.notify("PM 2.5: "+String(dustDensity)+" µg/m³");
// // You'll get HIGH/1 at startTime and LOW/0 at stopTime.
// // this method will be triggered every day
// // until you remove widget or stop project or
// // clean stop/start fields of widget
// Serial.print("Got a value: ");
// Serial.println(param.asStr());
//}
void loop()
{
    Blynk.run();
    timer.run();
    digitalWrite(ledPower, LOW); // power on the LED
    delayMicroseconds(samplingTime);
    voMeasured = analogRead(measurePin); // read the dust value
    delayMicroseconds(deltaTime);
    digitalWrite(ledPower, HIGH); // turn the LED off
    delayMicroseconds(sleepTime);
    // 0 - 3.3V mapped to 0 - 1023 integer values
    // recover voltage
    calcVoltage = voMeasured * (3.3 / 1024);
    dustDensity = 0.17 * calcVoltage - 0.1;
    dustDensity = dustDensity * 1000;
    Serial.print(" - Dust Density: ");
    Serial.print(dustDensity);
    Serial.println(" µg/m³");
    delay(1000);
    Blynk.virtualWrite(V3, dustDensity);
    Blynk.virtualWrite(V4, voMeasured);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    // เช็คถ้าอ่านค่าไม่สำเร็จให้เริ่มอ่านใหม่
    if (isnan(h) || isnan(t) || isnan(f))
    {
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
    Blynk.virtualWrite(V1, t);
    Blynk.virtualWrite(V2, h);
    // if (pm1>=30){
    // LINE.notify("PM 1.0: "+String(pm1)+" ug/m3");
    // }
     if (lock == true)
    {
        LINE.notify("PM 2.5: " + String(dustDensity) + " µg/m³");
        Blynk.virtualWrite(V5, LOW);
        lock = false;
    }else
    {
        lcd.clear();
        lcd.print(3, 0, "Off The Light");
        lcd.print(5, 1, "<<NOW>>");
    }
}