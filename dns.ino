#include <DNSServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(8,8,4,4); // The default android DNS
DNSServer dnsServer;

void setupDNS() {
  dnsServer.start(DNS_PORT, "*", apIP);
}

void stepDNS() {
  dnsServer.processNextRequest();
}