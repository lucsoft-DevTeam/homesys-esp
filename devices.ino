#include <DHT11.h>
#include <Ticker.h>

#define DHTPIN 4

DHT11 dht(DHTPIN);

Ticker readSensor;

int dhtHumidity = -1;
int dhtTemperature = -1;
bool dhtLastSensor = false;

void setupDeviceReader() {
  readSensor.attach(3, [] {
    dhtLastSensor = !dhtLastSensor;
    if (dhtLastSensor) {
      dhtHumidity = dht.readHumidity();
    } else {
      dhtTemperature = dht.readTemperature();
    }
  });
}