void connect_WiFi()
{
   WiFiManager wifiManager;
   //wifiManager.setBreakAfterConfig(true);
   
 //reset settings - for testing
  //wifiManager.resetSettings(); //เอาไว้รีเซ็ตไวไฟ
       if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.restart();
    delay(5000);
       }
   Serial.println("connected...yeey :)");
   Serial.println("local ip");
   Serial.println(WiFi.localIP());
}
