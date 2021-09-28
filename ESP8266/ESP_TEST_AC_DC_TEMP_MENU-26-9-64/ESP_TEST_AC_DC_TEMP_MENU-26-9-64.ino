/*-----------------------------------
 *      NTPLC TOTLBG
 *   ESP8266 NodeMCU V2
 *          TEST-10
 *      DATE : 28/9/64 
 * ----------------------------------
 *           PIN Connect
 * --------DHT11--------------------------          
 * NodeMCU     DHT11
 * D9           GND
 * 3.3V          V+
 * GND          GND
 * --------PZEM17 DC---------------------
 * NodeMCU    Serial to RS485      PZEM17
 * D0          MAX485_DI           GND
 * D5          MAX485_DE           A
 * D6          MAX485_RE           B
 * D7          MAX485_RO           5V
 * --------PZEM004T-V3 AC----------------
 * * NodeMCU   PZEM004T-V3
 * Vin           5V
 * D3            RX
 * D4            TX
 * GND           GND
 *  -----------Relay 2-4 Ch----------------
 *  * * NodeMCU   PZEM004T-V3
 * Vin           5V
 * D1            IN1
 * D2            IN2
 * GND           GND
  */

//----------------ESP8266WiFi---------------------//
#include <FS.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h> 
#include <EEPROM.h> 

//------------LINE_TOKEN-----------------------//
#include <TridentTD_LineNotify.h>
#define LINE_TOKEN  "OJe7jRLx6FUcjq51GyO4dQuJG968SmdAHY40ETJbm0k"

//------------------PZEM17 DC----------------------//
#include <SoftwareSerial.h>
#include <ModbusMaster.h>

SoftwareSerial PZEMSerial;

// ตั้งค่า pin สำหรับต่อกับ MAX485
#define MAX485_RO  D7
#define MAX485_RE  D6
#define MAX485_DE  D5
#define MAX485_DI  D0

// Address ของ PZEM-017 : 0x01-0xF7
static uint8_t pzemSlaveAddr = 0x01;

// ตั้งค่า shunt -->> 0x0000-100A, 0x0001-50A, 0x0002-200A, 0x0003-300A
static uint16_t NewshuntAddr = 0x0001;

ModbusMaster node;

float PZEMVoltage, PZEMCurrent, PZEMPower, PZEMEnergy;

unsigned long startMillisPZEM;                        /* start counting time for LCD Display */
unsigned long currentMillisPZEM;                      /* current counting time for LCD Display */
const unsigned long periodPZEM = 1000;                 // refresh every X seconds (in seconds) in LED Display. Default 1000 = 1 second

unsigned long startMillisReadData;                    /* start counting time for data collection */
unsigned long startMillis1;                           // to count time during initial start up (PZEM Software got some error so need to have initial pending time)

//----------------PZEM004T AC----------------------//
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>
#define PZEM_AC_RX_PIN D3
#define PZEM_AC_TX_PIN D4
SoftwareSerial pzemSWSerial_AC(PZEM_AC_RX_PIN, PZEM_AC_TX_PIN);
PZEM004Tv30 pzem(pzemSWSerial_AC);
float voltage ;
float current;
float power;
float energy ;
float frequency ;
float pf ;
float voltage_blynk=0;
float current_blynk=0;
float power_blynk=0;
float energy_blynk=0;
float frequency_blynk=0;
float pf_blynk=0;
int Blynkreset_kWh;

//----------------BLYNK--------------------------//
  //BLYNK
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
char auth[] = "VTjd-d615FpoSu8cFp4LUhbRixYbnZ5t";
///char server[] = "blynk.iot-cm.com";
///int port = 8080;
BlynkTimer timer;
WidgetRTC rtc;
//----------------DHT-Temp----------------------//
#include <DHT.h>
#define DHTPIN D9 
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301
DHT dht(DHTPIN, DHTTYPE);
float h;
float t;
//---------------Relay--------------------------//
#define Switch_1  D1  // PIN_RELAY1
#define Switch_2  D2  // PIN_RELAY2
//#define Switch_3  D5  // PIN_RELAY3
//#define Switch_4  D6  // PIN_RELAY4

//-----------------LINE API---------------------//
int Line_Switch_1=0 ; 
int Line_Switch_2=0 ; 
//int Line_Switch_3=0 ; 
//int Line_Switch_4=0 ; 
int Line_voltage_1=0 ;
int Line_voltage_DC_1=0 ;
//-------------------BLYNK----------------------//
//BLYNK
int pinV_1; //Relay 1
int pinV_2; //Relay 2
//int pinV_3; //Relay 3
//int pinV_4; //Relay 4
int pinV_7; //Menu widget MODE
int pinV_8; //time start
int pinV_9; //time stop
int pinV_12; //RESER KWH AC
WidgetLED led_rst_kWh(V47); //จะ ON เมื่อกดปุ่มค้างไว้ 5 วินาที
WidgetLED LED1(V21);
WidgetLED LED2(V22);
WidgetLED LED3(V23);
WidgetLED LED4(V24);

//---------การควบคุมค่า Min Max Temp-----------//  
float tmax;
String t_max1;
float t_max2;

float tmin;
String t_min1;
float t_min2; 

//------------------------------------------------// 
void setup() 
{
  startMillis1 = millis();
  Serial.begin(115200);
  connect_WiFi();
//------------------PZEM004T AC--------------------// 
   pinMode(D3, INPUT);
   pinMode(D4, OUTPUT);
//------------------PZEM17 DC----------------------// 
 
 PZEMSerial.begin(9600, SWSERIAL_8N2, MAX485_RO, MAX485_DI);           // software serial สำหรับติดต่อกับ MAX485

  startMillisPZEM = millis();                           /* Start counting time for run code */
  pinMode(MAX485_RE, OUTPUT);                           /* Define RE Pin as Signal Output for RS485 converter. Output pin means Arduino command the pin signal to go high or low so that signal is received by the converter*/
  pinMode(MAX485_DE, OUTPUT);                           /* Define DE Pin as Signal Output for RS485 converter. Output pin means Arduino command the pin signal to go high or low so that signal is received by the converter*/
  digitalWrite(MAX485_RE, 0);                           /* Arduino create output signal for pin RE as LOW (no output)*/
  digitalWrite(MAX485_DE, 0);                           /* Arduino create output signal for pin DE as LOW (no output)*/

  node.preTransmission(preTransmission);                // Callbacks allow us to configure the RS485 transceiver correctly
  node.postTransmission(postTransmission);
  node.begin(pzemSlaveAddr, PZEMSerial);
  delay(1000);                                          /* after everything done, wait for 1 second */

  // รอครบ 5 วินาที แล้วตั้งค่า shunt และ address
  while (millis() - startMillis1 < 5000) {
    delay(500);
    Serial.print(".");
  }
  
  setShunt(pzemSlaveAddr);                            // ตั้งค่า shunt
  changeAddress(0xF8, pzemSlaveAddr);                 // ตั้งค่า address 0x01 ซื่งเป็นค่า default ของตัว PZEM-017
  

  
//------------Switch Relay 4 CH-------------------//  
  pinMode(Switch_1,OUTPUT);
  pinMode(Switch_2,OUTPUT);
 // pinMode(Switch_3,OUTPUT);
  //pinMode(Switch_4,OUTPUT);
//------------------------------------------------//  

//If Active HIGH Set OFF  
  digitalWrite(Switch_1,HIGH);
  digitalWrite(Switch_2,HIGH);
  //digitalWrite(Switch_3,HIGH);
 // digitalWrite(Switch_4,HIGH);
//---------------------------------------------//  
    
  Serial.println(LINE.getVersion());
  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);
  Blynk.begin(auth, "", "", "blynk.iot-cm.com", 8080);
  ////เริ่มการเชื่อมต่อ Blynk Server*********สำหรับ Server local ที่แจกให้ เพิ่ม, server, port  

 setSyncInterval(10 * 60); // Sync interval in seconds (10 minutes)

  // Display digital clock every 1 seconds
   timer.setInterval(2000L,    sendSensor_DHT); // Sensor DHT 
   timer.setInterval(1000L,    clockDisplay);   // Digital clock display
   timer.setInterval(3000L,    PZEM004T_AC);    // Sensor AC 
   timer.setInterval(120000L,  Send_AC_Alam);   // Sensor AC Alam
   timer.setInterval(2000L,    SWITCH_MODE);    // MODE
   timer.setInterval(120000L,  Send_DC_Alam);   // Sensor DC Alam
   timer.setInterval(3000L,    PZEM17_DC);      // Sensor DC 
   
//--------------การควบคุมค่า Min Max Temp--------------------
  EEPROM.begin(512);

  tmax = EEPROM.get(0, t_max2);
  Serial.print("Tmax in Void setup : ");
  Serial.println(tmax);



  tmin = EEPROM.get(10, t_min2);
  Serial.print("Tmin in Void setup : ");
  Serial.println(tmin);
  
}//END void setup


//----------------void loop-----------------------  
void loop() 
{
  
  Blynk.run();
  timer.run();
  Reset_KWH_AC();
  //PZEM004T_AC();
}//END void loop
  

//--------Digital clock display of the time---
void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  String currentTime = String(hour()) + " : " + minute() + " : " + second();
  String currentDate = String(day()) + " / " + month() + " / " + year();
  Serial.print("Current time: ");
  Serial.print(currentTime);
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();

  // Send time to the App
  Blynk.virtualWrite(V5, currentTime);
  // Send date to the App
  Blynk.virtualWrite(V6, currentDate);
}

  //--------------SET_UP-PZEM17-DC-----------------------
void preTransmission()                                                                                    /* transmission program when triggered*/
{
  /* 1- PZEM-017 DC Energy Meter */
  if (millis() - startMillis1 > 5000)                                                               // Wait for 5 seconds as ESP Serial cause start up code crash
  {
    digitalWrite(MAX485_RE, 1);                                                                     /* put RE Pin to high*/
    digitalWrite(MAX485_DE, 1);                                                                     /* put DE Pin to high*/
    delay(1);                                                                                       // When both RE and DE Pin are high, converter is allow to transmit communication
  }
}

void postTransmission()                                                                                   /* Reception program when triggered*/
{

  /* 1- PZEM-017 DC Energy Meter */
  if (millis() - startMillis1 > 5000)                                                               // Wait for 5 seconds as ESP Serial cause start up code crash
  {
    delay(3);                                                                                       // When both RE and DE Pin are low, converter is allow to receive communication
    digitalWrite(MAX485_RE, 0);                                                                     /* put RE Pin to low*/
    digitalWrite(MAX485_DE, 0);                                                                     /* put DE Pin to low*/
  }
}

void setShunt(uint8_t slaveAddr)                                                                          //Change the slave address of a node
{

  /* 1- PZEM-017 DC Energy Meter */

  static uint8_t SlaveParameter = 0x06;                                                             /* Write command code to PZEM */
  static uint16_t registerAddress = 0x0003;                                                         /* change shunt register address command code */

  uint16_t u16CRC = 0xFFFF;                                                                         /* declare CRC check 16 bits*/
  u16CRC = crc16_update(u16CRC, slaveAddr);                                                         // Calculate the crc16 over the 6bytes to be send
  u16CRC = crc16_update(u16CRC, SlaveParameter);
  u16CRC = crc16_update(u16CRC, highByte(registerAddress));
  u16CRC = crc16_update(u16CRC, lowByte(registerAddress));
  u16CRC = crc16_update(u16CRC, highByte(NewshuntAddr));
  u16CRC = crc16_update(u16CRC, lowByte(NewshuntAddr));

  preTransmission();                                                                                /* trigger transmission mode*/

  PZEMSerial.write(slaveAddr);                                                                      /* these whole process code sequence refer to manual*/
  PZEMSerial.write(SlaveParameter);
  PZEMSerial.write(highByte(registerAddress));
  PZEMSerial.write(lowByte(registerAddress));
  PZEMSerial.write(highByte(NewshuntAddr));
  PZEMSerial.write(lowByte(NewshuntAddr));
  PZEMSerial.write(lowByte(u16CRC));
  PZEMSerial.write(highByte(u16CRC));
  delay(10);
  postTransmission();                                                                               /* trigger reception mode*/
  delay(100);
}


void changeAddress(uint8_t OldslaveAddr, uint8_t NewslaveAddr)                                            //Change the slave address of a node
{

  /* 1- PZEM-017 DC Energy Meter */

  static uint8_t SlaveParameter = 0x06;                                                             /* Write command code to PZEM */
  static uint16_t registerAddress = 0x0002;                                                         /* Modbus RTU device address command code */
  uint16_t u16CRC = 0xFFFF;                                                                         /* declare CRC check 16 bits*/
  u16CRC = crc16_update(u16CRC, OldslaveAddr);                                                      // Calculate the crc16 over the 6bytes to be send
  u16CRC = crc16_update(u16CRC, SlaveParameter);
  u16CRC = crc16_update(u16CRC, highByte(registerAddress));
  u16CRC = crc16_update(u16CRC, lowByte(registerAddress));
  u16CRC = crc16_update(u16CRC, highByte(NewslaveAddr));
  u16CRC = crc16_update(u16CRC, lowByte(NewslaveAddr));
  preTransmission();                                                                                 /* trigger transmission mode*/
  PZEMSerial.write(OldslaveAddr);                                                                       /* these whole process code sequence refer to manual*/
  PZEMSerial.write(SlaveParameter);
  PZEMSerial.write(highByte(registerAddress));
  PZEMSerial.write(lowByte(registerAddress));
  PZEMSerial.write(highByte(NewslaveAddr));
  PZEMSerial.write(lowByte(NewslaveAddr));
  PZEMSerial.write(lowByte(u16CRC));
  PZEMSerial.write(highByte(u16CRC));
  delay(10);
  postTransmission();                                                                                /* trigger reception mode*/
  delay(100);
}
