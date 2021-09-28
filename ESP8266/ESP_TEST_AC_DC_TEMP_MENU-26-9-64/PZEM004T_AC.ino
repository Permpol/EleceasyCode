void PZEM004T_AC()
{
  voltage = pzem.voltage();
    if(voltage != NAN){
        Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V"); voltage_blynk =voltage;
    } else {
        Serial.println("Error reading voltage");
    }

     current = pzem.current();
    if(current != NAN){
        Serial.print("Current: "); Serial.print(current); Serial.println("A"); current_blynk=current;
    } else {
        Serial.println("Error reading current");
    }

     power = pzem.power();
    if(current != NAN){
        Serial.print("Power: "); Serial.print(power); Serial.println("W"); power_blynk=power;
    } else {
        Serial.println("Error reading power");
    }

    energy = pzem.energy();
    if(current != NAN){
        Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh"); energy_blynk =energy;
    } else {
        Serial.println("Error reading energy");
    }

     frequency = pzem.frequency();
    if(current != NAN){
        Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz"); frequency_blynk =frequency;
    } else {
        Serial.println("Error reading frequency");
    }

    pf = pzem.pf();
    if(current != NAN){
        Serial.print("PF: "); Serial.println(pf); pf_blynk =pf;
    } else {
        Serial.println("Error reading power factor");
    }

    Serial.println();
 

    
            
            Blynk.virtualWrite(V40, voltage_blynk);
            Blynk.virtualWrite(V41, current_blynk  );            
            Blynk.virtualWrite(V42, power_blynk);
            Blynk.virtualWrite(V43, energy_blynk  );
            Blynk.virtualWrite(V44, frequency_blynk  );
            Blynk.virtualWrite(V45, pf_blynk  );
                  

}

void Send_AC_Alam()

 {
//-------------status AC- online----------------------- 
    if (voltage >= 100)
    { 
      Blynk.virtualWrite(V14,"AC : Online");
      if (Line_voltage_1==1)
          {
          //----------------แจ้งส่ง LINE ----------------------
            String LINESEND="ชุมสายแมกไม้ 2 \n";
                   LINESEND+="Status AC : Online \n";
                   LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                   LINESEND+=String(day()) + "-" + month() + "-" + year();
            Line_voltage_1=0;
            LINE.notify(LINESEND);
           }
       
    } 
    
//-------------status AC- FAIL-----------------------      
    else  
    {  
      Blynk.virtualWrite(V14,"AC : FAIL"); 
      if (Line_voltage_1==0)
           {
           //----------------แจ้งส่ง LINE ----------------------
           String LINESEND="ชุมสายแมกไม้ 2 \n";
                  LINESEND+="Status AC : Fail \n";
                  LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                  LINESEND+=String(day()) + "-" + month() + "-" + year();
            Line_voltage_1=1;
            LINE.notify(LINESEND);
           }
        
      }  
//-----------------------------------------------         
 }
