//----------------------------------------- 
BLYNK_WRITE(V31)//RELAY1
{
  pinV_1 = param.asInt();
}

//------------------------------------------ 
BLYNK_WRITE(V32)//RELAY2
{
  pinV_2 = param.asInt();
}
//------------------------------------------  
BLYNK_WRITE(V33)//RELAY3
{
//  pinV_3 = param.asInt();
}

//------------------------------------------ 
BLYNK_WRITE(V34)//RELAY4
{
 // pinV_4 = param.asInt();
}
//-----------------------------------------  

BLYNK_WRITE(V7)  //Mode
{
  pinV_7 = param.asInt();
}
//----------------------------------------- 
 BLYNK_WRITE(V8)  //time start
{
  pinV_8 = param.asInt();
}
//----------------------------------------- 

BLYNK_WRITE(V9)  //time stop
{
  pinV_9 = param.asInt();
}

//----------------------------------------- 
BLYNK_WRITE(V10)  //temp max

{
  t_max1 = param.asString();//รับค่าข้อความจาก Blynk เข้ามา เก็บในรูปแบบ String

  t_max2 = t_max1.toFloat();//แปลงค่าจาก String เป็น Float

  EEPROM.put(0, t_max2); //เขียนค่า
  EEPROM.commit(); //จบการเขียน

  tmax = EEPROM.get(0, t_max2);//อ่าน ค่า
  Serial.print("Set Tmax : ");
  Serial.println(tmax);
}
//------------------------------------------
BLYNK_WRITE(V11)//temp min

{
  t_min1 = param.asString();//รับค่าข้อความจาก Blynk เข้ามา เก็บในรูปแบบ String

  t_min2 = t_min1.toFloat();//แปลงค่าจาก String เป็น Float

  EEPROM.put(10, t_min2); //เขียนค่า
  EEPROM.commit(); //จบการเขียน

  tmin = EEPROM.get(10, t_min2);//อ่าน ค่า
  Serial.print("Set Tmin : ");
  Serial.println(tmin);
}
//-----------------------------------------
BLYNK_WRITE(V46)//reset_kWh AC
{
   pinV_12 = param.asInt();
  
  if (pinV_12 == 1) 
  {
    Blynkreset_kWh = 1;
  }
  if (pinV_12 == 0) 
  {
    Blynkreset_kWh = 0;
  }

}
//-----------------------------------------
BLYNK_WRITE(V54) //reset_kW DC                                // Virtual push button to reset energy for Meter 1 
{
         if(param.asInt()==1)
          { 
            uint16_t u16CRC = 0xFFFF;                         /* declare CRC check 16 bits*/
            static uint8_t resetCommand = 0x42;               /* reset command code*/
            uint8_t slaveAddr = pzemSlaveAddr;                 // if you set different address, make sure this slaveAddr must change also
            u16CRC = crc16_update(u16CRC, slaveAddr);
            u16CRC = crc16_update(u16CRC, resetCommand);
            preTransmission();                                /* trigger transmission mode*/
            PZEMSerial.write(slaveAddr);                      /* send device address in 8 bit*/
            PZEMSerial.write(resetCommand);                   /* send reset command */
            PZEMSerial.write(lowByte(u16CRC));                /* send CRC check code low byte  (1st part) */
            PZEMSerial.write(highByte(u16CRC));               /* send CRC check code high byte (2nd part) */
            delay(10);
            postTransmission();                               /* trigger reception mode*/
            delay(100);
          }
}
//------------------------------------------ 
// This function will run every time Blynk connection is established
BLYNK_CONNECTED() 
{
  // Request Blynk server to re-send latest values for all pins
  Blynk.syncAll();
  rtc.begin();
}
//------------------------------------------
