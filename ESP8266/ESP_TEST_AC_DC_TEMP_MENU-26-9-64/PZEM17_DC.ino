 void PZEM17_DC()
{
   currentMillisPZEM = millis();
  // อ่านค่าจาก PZEM-017
  if (currentMillisPZEM - startMillisPZEM >= periodPZEM)                                            /* for every x seconds, run the codes below*/
  {
    uint8_t result;                                                                                 /* Declare variable "result" as 8 bits */
    result = node.readInputRegisters(0x0000, 6);                                                    /* read the 9 registers (information) of the PZEM-014 / 016 starting 0x0000 (voltage information) kindly refer to manual)*/
    if (result == node.ku8MBSuccess)                                                                /* If there is a response */
    {
      uint32_t tempdouble = 0x00000000;                                                           /* Declare variable "tempdouble" as 32 bits with initial value is 0 */
      PZEMVoltage = node.getResponseBuffer(0x0000) / 100.0;                                       /* get the 16bit value for the voltage value, divide it by 100 (as per manual) */
      // 0x0000 to 0x0008 are the register address of the measurement value
      PZEMCurrent = node.getResponseBuffer(0x0001) / 100.0;                                       /* get the 16bit value for the current value, divide it by 100 (as per manual) */

      tempdouble =  (node.getResponseBuffer(0x0003) << 16) + node.getResponseBuffer(0x0002);      /* get the power value. Power value is consists of 2 parts (2 digits of 16 bits in front and 2 digits of 16 bits at the back) and combine them to an unsigned 32bit */
      PZEMPower = tempdouble / 10.0;                                                              /* Divide the value by 10 to get actual power value (as per manual) */

      tempdouble =  (node.getResponseBuffer(0x0005) << 16) + node.getResponseBuffer(0x0004);      /* get the energy value. Energy value is consists of 2 parts (2 digits of 16 bits in front and 2 digits of 16 bits at the back) and combine them to an unsigned 32bit */
      PZEMEnergy = tempdouble;
    }
    else // ถ้าติดต่อ PZEM-017 ไม่ได้ ให้ใส่ค่า NAN(Not a Number)
    {
      PZEMVoltage = NAN;
      PZEMCurrent = NAN;
      PZEMPower = NAN;
      PZEMEnergy = NAN;
    }

    // แสดงค่าที่ได้จากบน Serial monitor
    Serial.print("Vdc : "); Serial.print(PZEMVoltage); Serial.println(" V ");
    Serial.print("Idc : "); Serial.print(PZEMCurrent); Serial.println(" A ");
    Serial.print("Power : "); Serial.print(PZEMPower); Serial.println(" W ");
    Serial.print("Energy : "); Serial.print(PZEMEnergy); Serial.println(" Wh ");
    Blynk.virtualWrite(V50,PZEMVoltage);                                                           // Send data to Blynk Server. Voltage value as virtual pin V0
    Blynk.virtualWrite(V51,PZEMCurrent);
    Blynk.virtualWrite(V52,PZEMPower);
    Blynk.virtualWrite(V53,PZEMEnergy);
    startMillisPZEM = currentMillisPZEM ;                                                       /* Set the starting point again for next counting time */
  }
                                                                              
}

void Send_DC_Alam()

 {
//-------------status DC- online----------------------- 
    if (PZEMVoltage >= 8)
    { 
      Blynk.virtualWrite(V15,"DC : Online");
      if (Line_voltage_DC_1==1)
          {
          //----------------แจ้งส่ง LINE ----------------------
            String LINESEND="ชุมสายแมกไม้ 2 \n";
                   LINESEND+="Status DC : Online \n";
                   LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                   LINESEND+=String(day()) + "-" + month() + "-" + year();
            Line_voltage_DC_1=0;
            LINE.notify(LINESEND);
           }
       
    } 
    
//-------------status DC- FAIL-----------------------      
    else  
    {  
      Blynk.virtualWrite(V15,"DC : FAIL"); 
      if (Line_voltage_DC_1==0)
           {
           //----------------แจ้งส่ง LINE ----------------------
           String LINESEND="ชุมสายแมกไม้ 2 \n";
                  LINESEND+="Status DC : Fail \n";
                  LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                  LINESEND+=String(day()) + "-" + month() + "-" + year();
            Line_voltage_DC_1=1;
            LINE.notify(LINESEND);
           }
        
      }  
//-----------------------------------------------         
 }
