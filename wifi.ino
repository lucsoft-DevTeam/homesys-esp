void setupWifiClient() {
  WiFi.begin(getSSID(), getPassword());
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
  Serial.println("");
}

void setupWifiAP() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("HomeSYS " + nodeId());
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
}