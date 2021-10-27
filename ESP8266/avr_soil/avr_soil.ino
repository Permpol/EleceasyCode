void sendSoilValue() {
  sensorValue = analogRead(A0);
  percentageHumididy = map(sensorValue, 0, 1024, 0, 100);

  Blynk.virtualWrite(V5, percentageHumididy);
  Blynk.virtualWrite(V0, sensorValue);

  Serial.print(percentageHumididy);
  Serial.println("%");
  Serial.print("A0 = ");
  Serial.println(sensorValue);
  delay(100);
}
