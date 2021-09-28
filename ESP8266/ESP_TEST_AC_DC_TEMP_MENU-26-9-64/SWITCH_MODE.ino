void SWITCH_MODE()
{
  switch (pinV_7)
  {
    case 1: // Munual
//---------------------RELAY 1-----------------------------     
           if (pinV_1==1)
        {
           //Active ON
            digitalWrite(Switch_1,LOW);
            Blynk.virtualWrite(V31,HIGH);
            LED1.on(); 
     //----------------แจ้งส่ง LINE ----------------------
            if (Line_Switch_1==0)
               {
                  String LINESEND="ชุมสายแมกไม้ 2 \n";
                         LINESEND+="Switch-1 ON \n";
                         LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                         LINESEND+=String(day()) + "-" + month() + "-" + year();
                  Line_Switch_1=1;
                  LINE.notify(LINESEND);
               }
            
         }
        else if (pinV_1==0)
         {
            //Active OFF
            digitalWrite(Switch_1,HIGH);
            Blynk.virtualWrite(V31,LOW);
            LED1.off();
        //----------------แจ้งส่ง LINE ----------------------    
             if (Line_Switch_1==1)
               {
                  String LINESEND="ชุมสายแมกไม้ 2 \n" ;
                         LINESEND+="Switch-1 OFF \n";
                         LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                         LINESEND+=String(day()) + "-" + month() + "-" + year();
                  Line_Switch_1=0;
                  LINE.notify(LINESEND);
                }
 
        }
 //---------------------RELAY 2-----------------------------       
        if (pinV_2==1)
        {
           //Active ON
            digitalWrite(Switch_2,LOW);
            Blynk.virtualWrite(V32,HIGH);
            LED2.on(); 
      //----------------แจ้งส่ง LINE ----------------------
            if (Line_Switch_2==0)
             {
                String   LINESEND="ชุมสายแมกไม้ 2 \n";
                         LINESEND+="Switch-2 ON \n";
                         LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                         LINESEND+=String(day()) + "-" + month() + "-" + year();
                Line_Switch_2=1;
                LINE.notify(LINESEND);
             }
                 
        }
      else if (pinV_2==0)
        {
           //Active OFF
            digitalWrite(Switch_2,HIGH);
            Blynk.virtualWrite(V32,LOW);
            LED2.off();
       //----------------แจ้งส่ง LINE ----------------------    
           if (Line_Switch_2==1)
             {
                 String  LINESEND="ชุมสายแมกไม้ 2 \n";
                         LINESEND+="Switch-2 OFF \n";
                         LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                         LINESEND+=String(day()) + "-" + month() + "-" + year();
                Line_Switch_2=0;
                LINE.notify(LINESEND);
                
             }
                 
          }
  /*        
 //---------------------RELAY 3-----------------------------     
           if (pinV_3==1)
        {
           //Active ON
            digitalWrite(Switch_3,LOW);
            Blynk.virtualWrite(V33,HIGH);
            LED3.on(); 
     //----------------แจ้งส่ง LINE ----------------------
            if (Line_Switch_3==0)
               {
                  String  LINESEND="ชุมสายแมกไม้ 2 \n";
                          LINESEND+="Switch-3 ON \n";
                          LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                          LINESEND+=String(day()) + "-" + month() + "-" + year();
                  Line_Switch_3=1;
                  LINE.notify(LINESEND);
                  
               }
            
         }
        else if (pinV_3==0)
         {
            //Active OFF
            digitalWrite(Switch_3,HIGH);
            Blynk.virtualWrite(V33,LOW);
            LED3.off();
        //----------------แจ้งส่ง LINE ----------------------    
             if (Line_Switch_3==1)
               {
                String LINESEND="ชุมสายแมกไม้ 2 \n";
                       LINESEND+="Switch-3 OFF \n";
                       LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                       LINESEND+=String(day()) + "-" + month() + "-" + year();
                Line_Switch_3=0;
                LINE.notify(LINESEND);
      
                }
 
        }         
//---------------------RELAY 4-----------------------------     
           if (pinV_4==1)
        {
           //Active ON
            digitalWrite(Switch_4,LOW);
            Blynk.virtualWrite(V34,HIGH);
            LED4.on(); 
     //----------------แจ้งส่ง LINE ----------------------
            if (Line_Switch_4==0)
               {
                  String  LINESEND="ชุมสายแมกไม้ 2 \n";
                          LINESEND+="Switch-4 ON \n";
                          LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                          LINESEND+=String(day()) + "-" + month() + "-" + year();
                  Line_Switch_4=1;
                  LINE.notify(LINESEND);
                  
               }
            
         }
        else if (pinV_4==0)
         {
            //Active OFF
            digitalWrite(Switch_4,HIGH);
            Blynk.virtualWrite(V34,LOW);
            LED4.off();
        //----------------แจ้งส่ง LINE ----------------------    
             if (Line_Switch_4==1)
               {
                  String LINESEND="ชุมสายแมกไม้ 2 \n";
                         LINESEND+="Switch-4 OFF \n";
                         LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                         LINESEND+=String(day()) + "-" + month() + "-" + year();
                  Line_Switch_4=0;
                  LINE.notify(LINESEND);
      
                }
        }
     */   
      Serial.println("Munual selected");
      break;
//-----------------------------------------------     
    case 2: // Temp
     if ( t > tmin && t <= tmax ) 
      {
                 digitalWrite(Switch_1,LOW);
                 Blynk.virtualWrite(V31,HIGH);
                 LED1.on(); 
                
    //----------------แจ้งส่ง LINE ----------------------
             if (Line_Switch_1==0)
           {  
                 String LINESEND="ชุมสายแมกไม้ 2 \n";
                        LINESEND+="อุณหภูมิเกินกำหนด  " + String(t) + " °C \n";
                        LINESEND+="ชุดระบบควบคุมอุณหภูมิ ON \n";
                        LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                        LINESEND+=String(day()) + "-" + month() + "-" + year();
                 Line_Switch_1=1;
                 LINE.notify(LINESEND);    
            }
       }  
//-----------------------------------------------      
      else   
       {
              digitalWrite(Switch_1,HIGH);
              Blynk.virtualWrite(V31,LOW);
              LED1.off();
              
     //----------------แจ้งส่ง LINE ----------------------
            if (Line_Switch_1==1)
           {
               String LINESEND="ชุมสายแมกไม้ 2 \n";
                      LINESEND+="อุณหภูมิปกติ  " + String(t) + " °C \n";
                      LINESEND+="ชุดระบบควบคุมอุณหภูมิ OFF \n";
                      LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                      LINESEND+=String(day()) + "-" + month() + "-" + year();
               Line_Switch_1=0;
               LINE.notify(LINESEND);
      
            }
        }
      Serial.println("Temp selected");
      break;
  //-----------------------------------------------     
    case 3: // Time
    if(hour() >= pinV_8 && hour() < pinV_9)
       {
                 digitalWrite(Switch_1,LOW);
                 Blynk.virtualWrite(V31,HIGH);
                 LED1.on(); 
                
         //----------------แจ้งส่ง LINE ----------------------
            if (Line_Switch_1==0)
           {
                 String LINESEND="ชุมสายแมกไม้ 2 \n";
                        LINESEND+="อุปกรณ์ทำงานตามเวลา ON \n";
                        LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                        LINESEND+=String(day()) + "-" + month() + "-" + year();
                 Line_Switch_1=1;
                 LINE.notify(LINESEND);     
            }
       } 
 //----------------------------------------------- 
       else
       {
              digitalWrite(Switch_1,HIGH);
              Blynk.virtualWrite(V31,LOW);
              LED1.off();
             
      //----------------แจ้งส่ง LINE ----------------------  
          if (Line_Switch_1==1)
           {
                 String LINESEND="ชุมสายแมกไม้ 2 \n";
                        LINESEND+="อุปกรณ์หยุดทำงานตามเวลา OFF \n";
                        LINESEND+="DATE : " + String(hour()) +":" + minute() + ":" + second() + "   "; 
                        LINESEND+=String(day()) + "-" + month() + "-" + year();
                 Line_Switch_1=0;
                 LINE.notify(LINESEND); 
            }
             
        }
      Serial.println("Time selected");
      break;
    default:
      Serial.println("Unknown item selected");
  }
}
