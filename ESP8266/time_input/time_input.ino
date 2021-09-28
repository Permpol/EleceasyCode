#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <EEPROM.h>

#define EEPROM_STATE_ADDRESS_1 128
#define EEPROM_STATE_ADDRESS_2 144
#define relayLed D1

SimpleTimer timer;
WidgetRTC rtc;
WidgetTerminal terminal(V3);
WidgetLED led1(V4);
WidgetLED led2(V5);


char Date[16];
char Time[16];
char auth[] = "iwMucHa6hKw4M9LqJk8nE7SnKEGrjiE3";
char ssid[] = "KhunFonShop";
char pass[] = "23092529";
long startsecondswd;
long stopsecondswd;
long nowseconds;
bool isFirstConnect = true;

String displaycurrenttimepluswifi;
int wifisignal;
int manual = 0;
int oldstatus;
int automode;

char ledstate = 0;
char schestate = 0;
bool check = true;
bool check2 = true;
bool check3 = true;

void setup()
{
  pinMode(relayLed, OUTPUT);
  digitalWrite(relayLed, LOW); // set LED ON
  Serial.begin(115200);
  Serial.println("\Starting");
  Blynk.begin(auth, ssid, pass);
  int mytimeout = millis() / 1000;
  while (Blynk.connect() == false) { // try to connect to server for 10 seconds
  if ((millis() / 1000) > mytimeout) { // try local server if not connected within 9 seconds
     break;
    }
  }

  EEPROM.begin(512);
  ledstate = EEPROM.read(EEPROM_STATE_ADDRESS_1) == 1 ? 1 : 0;
  schestate = EEPROM.read(EEPROM_STATE_ADDRESS_2) == 1 ? 1 : 0;
  ledonstart();

  rtc.begin();
  timer.setInterval(10000L, activetoday);  // check every 10 SECONDS if schedule should run today
  //timer.setInterval(30000L, reconnectBlynk);  // check every 30s if still connected to server
  timer.setInterval(10000L, clockvalue);  // check value for time
  timer.setInterval(5000L, sendWifi);    // Wi-Fi singal
  timer.setInterval(1000L, check_status);
  timer.setInterval(1000L, check_sche);
}

void ledonstart() {
  if (ledstate == 1) {
    digitalWrite(relayLed, 1);
    Blynk.virtualWrite(V2, 1);
  }
  else {
    digitalWrite(relayLed, 0);
    Blynk.virtualWrite(V2, 0);
  }

  if (schestate == 1) {
    Blynk.virtualWrite(V11, 1);
    led1.on();
    led2.off();

    if ((schestate == 1) && (check3 == true)) {
      timer.setTimeout(50, resetTerminal);
      timer.setTimeout(50, checklastbuttonpressed);
      automode = 1;
      schestate = 1;
      check3 = false;
    }
  }
  else {
    Blynk.virtualWrite(V11, 0);
    led1.off();
    led2.on();
  }
}

void check_status() {
  if (digitalRead(relayLed) == LOW) {
    ledstate = 1;
    if (check == true) {
      EEPROM.write(EEPROM_STATE_ADDRESS_1, ledstate);
      EEPROM.commit();
      Serial.println("EEPROM Write ledstate ON 1");
    }
    check = false;
  }
  else {
    ledstate = 0;
    if (check == false) {
      EEPROM.write(EEPROM_STATE_ADDRESS_1, ledstate);
      EEPROM.commit();
      Serial.println("EEPROM Write ledstate OFF 1");
    }
    check = true;
  }
}

void check_sche() {
  if (automode == 1) {
    schestate = 1;
    if (check2 == true) {
      EEPROM.write(EEPROM_STATE_ADDRESS_2, schestate);
      EEPROM.commit();
      Serial.println("EEPROM Write schestate ON 1");

      led1.on();
      led2.off();
    }
    check2 = false;
  }
  else {
    schestate = 0;
    if (check2 == false) {
      EEPROM.write(EEPROM_STATE_ADDRESS_2, schestate);
      EEPROM.commit();
      Serial.println("EEPROM Write schestate OFF 1");

      led1.off();
      led2.on();
    }
    check2 = true;
  }
}

BLYNK_CONNECTED() {
  if (isFirstConnect) {
    Blynk.syncAll();
    Blynk.notify("TIMER STARTING!!!!");
    isFirstConnect = false;
  }
}

void sendWifi() {
  wifisignal = map(WiFi.RSSI(), -105, -40, 0, 100);
}

void clockvalue() // Digital clock display of the time
{

  int gmthour = hour();
  if (gmthour == 24) {
    gmthour = 0;
  }
  String displayhour =   String(gmthour, DEC);
  int hourdigits = displayhour.length();
  if (hourdigits == 1) {
    displayhour = "0" + displayhour;
  }
  String displayminute = String(minute(), DEC);
  int minutedigits = displayminute.length();
  if (minutedigits == 1) {
    displayminute = "0" + displayminute;
  }

  displaycurrenttimepluswifi = "                                          Clock:  " + displayhour + ":" + displayminute + "               Signal:  " + wifisignal + " %";
  Blynk.setProperty(V3, "label", displaycurrenttimepluswifi);

}

void activetoday() {       // check if schedule should run today
  if (year() != 1970) {
    if (automode == 1) {
      Blynk.syncVirtual(V10); // sync timeinput widget
    }
  }
}

void checklastbuttonpressed () {
  if (automode == 1) {
    oldstatus = 1;  //5
  } else {
    oldstatus = 2;  //6
  }
}

void restorelastbuttonpressed () {
  if (oldstatus == 1) {
    automode = 1;
    Blynk.virtualWrite(V11, 1);
  } else {
    automode = 0;
    Blynk.virtualWrite(V11, 0);
  }
}

void resetTerminal()
{
  terminal.println();
  terminal.println();
  terminal.println();
  terminal.println("AUTO Mode has been selected");
  terminal.println("Wait for update (10 seconds as maximum)");
  terminal.println();
  terminal.println();
  terminal.println();
  terminal.flush();
}

BLYNK_WRITE(V2)  // ON-OFF Manual
{
  if (param.asInt() == 1) { // boton encendido
    terminal.println();
    terminal.println();
    terminal.println();
    terminal.println("Manual MODE is ON");
    terminal.println("Press ON/OFF button if required");
    terminal.println("Device is ON");
    terminal.println();
    terminal.println();
    terminal.flush();

    if (manual == 0) { //est? en modo autom?tico
      checklastbuttonpressed ();
      manual = 1;
      automode = 0;
      Blynk.virtualWrite(V11, 0);
      digitalWrite(relayLed, LOW); // set LED ON
      Blynk.virtualWrite(V2, 1);   //Turn ON Button Widget

    } else {             //est? en modo manual
      automode = 0;
      Blynk.virtualWrite(V11, 0);
      digitalWrite(relayLed, LOW); // set LED ON
      Blynk.virtualWrite(V2, 1);   //Turn ON Button Widget
    }
  } else {

    terminal.println();
    terminal.println();
    terminal.println();
    terminal.println("Manual MODE is ON");
    terminal.println("Press ON/OFF button if required");
    terminal.println("Device is OFF");
    terminal.println();
    terminal.println();
    terminal.flush();

    if (manual == 0) {   //modo autom?tico
      checklastbuttonpressed ();
      manual = 1;
      automode = 0;
      Blynk.virtualWrite(V11, 0);
      digitalWrite(relayLed, HIGH); // set LED OFF
      Blynk.virtualWrite(V2, 0);   //Turn OFF Button Widget
    } else {
      automode = 0;
      Blynk.virtualWrite(V11, 0);
      digitalWrite(relayLed, HIGH); // set LED OFF
      Blynk.virtualWrite(V2, 0);   //Turn OFF Button Widget
    }
  }
}

BLYNK_WRITE(V11)  // Up to you selected
{
  if (param.asInt() == 1) {
    timer.setTimeout(50, resetTerminal);
    timer.setTimeout(50, checklastbuttonpressed);
    automode = 1;
    schestate = 1;
  } else {
    automode = 0;
    schestate = 0;
  }
}

BLYNK_WRITE(V10)//Up to you
{
  if (automode == 1) {
    sprintf(Date, "%02d/%02d/%04d",  day(), month(), year());
    sprintf(Time, "%02d:%02d:%02d", hour(), minute(), second());

    TimeInputParam t(param);

    terminal.print("Auto Mode Checked schedule at: ");
    terminal.println(Time);
    terminal.flush();
    int dayadjustment = -1;
    if (weekday() == 1) {
      dayadjustment =  6; // needed for Sunday, Time library is day 1 and Blynk is day 7
    }
    if (t.isWeekdaySelected(weekday() + dayadjustment)) { //Time library starts week on Sunday, Blynk on Monday
      terminal.println("Auto Mode ACTIVE today");
      terminal.flush();
      if (t.hasStartTime()) // Process start time
      {
        terminal.println(String("Start: ") + t.getStartHour() + ":" + t.getStartMinute() + ":" + t.getStartSecond());
        terminal.flush();
      }
      if (t.hasStopTime()) // Process stop time
      {
        terminal.println(String("Stop : ") + t.getStopHour() + ":" + t.getStopMinute() + ":" + t.getStopSecond());
        terminal.flush();
      }
      // Display timezone details, for information purposes only
      terminal.println(String("Time zone: ") + t.getTZ()); // Timezone is already added to start/stop time
      terminal.println("At least ONE day MUST be selected");
      //terminal.println(String("Time zone offset: ") + t.getTZ_Offset()); // Get timezone offset (in seconds)
      terminal.flush();

      for (int i = 1; i <= 7; i++) {  // Process weekdays (1. Mon, 2. Tue, 3. Wed, ...)
        if (t.isWeekdaySelected(i)) {
          terminal.println(String("Day ") + i + " is selected");
          terminal.flush();
        }
      }
      nowseconds = ((hour() * 3600) + (minute() * 60) + second());
      startsecondswd = (t.getStartHour() * 3600) + (t.getStartMinute() * 60) + (t.getStartSecond());
      //Serial.println(startsecondswd);  // used for debugging
      if (nowseconds >= startsecondswd) {
        terminal.print("Auto Mode STARTED at");
        terminal.println(String(" ") + t.getStartHour() + ":" + t.getStartMinute() + ":" + t.getStartSecond());
        terminal.flush();
        if (nowseconds <= startsecondswd + 90) {  // 90s on 60s timer ensures 1 trigger command is sent
          digitalWrite(relayLed, LOW); // set LED ON
          Blynk.virtualWrite(V2, 1);
          // code here to switch the relay ON
        }
      }
      else {
        digitalWrite(relayLed, HIGH); // set LED OFF
        Blynk.virtualWrite(V2, 0);
        terminal.println("Auto Mode Device NOT STARTED today");
        terminal.flush();

      }
      stopsecondswd = (t.getStopHour() * 3600) + (t.getStopMinute() * 60) + (t.getStopSecond());
      //Serial.println(stopsecondswd);  // used for debugging
      if (nowseconds >= stopsecondswd) {
        digitalWrite(relayLed, HIGH); // set LED OFF
        Blynk.virtualWrite(V2, 0);
        terminal.print("Auto Mode STOPPED at");
        terminal.println(String(" ") + t.getStopHour() + ":" + t.getStopMinute() + ":" + t.getStopSecond());
        terminal.flush();
        if (nowseconds <= stopsecondswd + 90) { // 90s on 60s timer ensures 1 trigger command is sent
          digitalWrite(relayLed, HIGH); // set LED OFF
          Blynk.virtualWrite(V2, 0);
          // code here to switch the relay OFF
        }
       }
      else {
        if (nowseconds >= startsecondswd) {
          digitalWrite(relayLed, LOW); // set LED ON
          Blynk.virtualWrite(V2, 1);
          terminal.println("Auto Mode is ON");
         terminal.flush();

        }
      }
    }
    else {
      terminal.println("Auto Mode INACTIVE today");
      terminal.flush();
      // nothing to do today, check again in 30 SECONDS time
    }
    terminal.println();
  }
}

//void reconnectBlynk() {
  //if (!Blynk.connected()) {
    //if (Blynk.connect()) {
      //BLYNK_LOG("Reconnected");
    //} else {
      //BLYNK_LOG("Not reconnected");
    //}
  //}
//}

void loop()
{
  if (Blynk.connected()) {
    Blynk.run();
  }
  timer.run();
}
