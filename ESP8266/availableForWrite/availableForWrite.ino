#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

#define trigger  D1 
#define echo     D2 
#define trigger1 D3 
#define echo1    D4 

long level; 
Servo myservo;

 char auth[] = "DvPECKDzS-ZsKDum0LUvNT0e3Iyq3G_F  "; //หมายเลขที่ใช้เชื่อมต่อกับBlynk
 char ssid[] = "Fon_2.4GHz";                  //ชื่อwifi Kanthika_2.4GHz/Kai_2.4GHz
 char pass[] = "supattra";
 
  BLYNK_WRITE(V0) //บอกว่าในคำสั่งนี้คือของปุ่มBotton(V0)
  {
    myservo.write(70);   //ให้servoอ่านค่าที่0องศา
    delay(500);        //ตั้งการหน่วงในการทำงาน 0.5วินาที
    myservo.write(30); //ให้servoอ่านค่าที่100องศา
    delay(500);           //ตั้งการหน่วงในการทำงาน 
     myservo.write(0); //ให้servoอ่านค่าที่100องศา
    delay(500);           //ตั้งการหน่วงในการทำงาน
     myservo.write(100); //ให้servoอ่านค่าที่100องศา
    delay(500);           //ตั้งการหน่วงในการทำงาน
  }

  BLYNK_WRITE(V1)   //บอกว่าในคำสั่งนี้คือของปุ่มBotton(V1)
  {
    myservo.write(param.asInt()); //เป็นคำสั่งการตรวจสอบว่าค่าในปุ่มใน blynk มีการสั่งการอะไร   
  }

void setup()
{
  // Debug console
  {
  myservo.attach(D6);
  } 
  Serial.begin(9600);
  pinMode( trigger, OUTPUT );
  pinMode( echo   , INPUT );
  pinMode( trigger1, OUTPUT );
  pinMode( echo1   , INPUT );
  Blynk.begin(auth, ssid, pass);
 
}

void loop() {
 {
  digitalWrite( trigger , LOW );   
  digitalWrite( trigger , HIGH );    
  digitalWrite( trigger , LOW );
  long duration = pulseIn( echo, HIGH , 10000 ); 
  level = 0.034 * duration / 2;
  Blynk.virtualWrite(V7, level);
 }  
 { 
  digitalWrite( trigger1 , LOW );   
  digitalWrite( trigger1 , HIGH );    
  digitalWrite( trigger1 , LOW );
  long duration = pulseIn( echo1, HIGH , 10000 ); 
  level = 0.034 * duration / 2;
  Blynk.virtualWrite(V8, level);
  Blynk.run();
 }
}
