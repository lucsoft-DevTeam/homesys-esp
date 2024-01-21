#include <Preferences.h>

Preferences preferences;

void setupStorage() {
  preferences.begin("homesys", false);
  unsigned int counter = preferences.getUInt("counter", 0);
  Serial.print("Restart Count");
  Serial.println(counter);
  counter++;
  preferences.putUInt("counter", counter);
}

bool hasSSIDStored() {
  return preferences.getBool("setup");
}

String getSSID() {
  return preferences.getString("ssid", "");
}

String getPassword() {
  return preferences.getString("password", "");
}

void setWifiConnection(String ssid, String password) {
  preferences.putBool("setup", true);
  preferences.putString("ssid", ssid);
  preferences.putString("password", password);
}

bool unsetStorage() {
  preferences.putBool("setup", false);
}