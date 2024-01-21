#include <WiFi.h>

#define VERSION "0.0.1"
#define LED_BUILTIN 2

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println();
  Serial.println("Booting into HomeSYS-ESP");
  Serial.println(String("HomeSYS NodeID: ") + nodeId());
  setupDeviceReader();
  setupStorage();
  setupFiles();
  WiFi.setHostname(nodeId().c_str());
  if (hasSSIDStored()) {
    setupWifiClient();
  } else {
    setupWifiAP();
    setupDNS();
  }
  setupWebServer();
  checkRequiredFilesExists();
  Serial.println("Starting Loop");
}

void loop() {
  delay(10);
}
