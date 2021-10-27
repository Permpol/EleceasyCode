int _sensorValue[3];
int avr_sensorValue;

void sendSoilValue() {

  for (int i = 0; i >= 3; i++) {
    _sensorValue[i] = analogRead(A0);
  }

  if (i == 3) {
    avr_sensorValue = ( _sensorValue[0]+ _sensorValue[1]+ _sensorValue[2])/3;
    percentageHumididy = map(avr_sensorValue, 0, 1024, 0, 100);
    Blynk.virtualWrite(V5, percentageHumididy);
    Blynk.virtualWrite(V0, avr_sensorValue);
    Serial.print(percentageHumididy);
    Serial.println("%");
    Serial.print("A0 = ");
    Serial.println(avr_sensorValue);
    delay(100);
  }

}
