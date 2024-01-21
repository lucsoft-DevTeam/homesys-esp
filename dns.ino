#include <DNSServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(8, 8, 4, 4);  // The default android DNS
DNSServer dnsServer;
Ticker dnsTask;

void setupDNS() {
  dnsServer.start(DNS_PORT, "*", apIP);
  dnsTask.attach_ms(300, []() {
    dnsServer.processNextRequest();
  });
}