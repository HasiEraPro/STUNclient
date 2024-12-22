#ifndef STUNCLIENT_H
#define STUNCLIENT_H

#include <WiFi.h>
#include <WiFiUdp.h>

class STUNClient {
  public:
    STUNClient();
    bool begin(const char* server, uint16_t port);
    IPAddress getPublicIP();
    
  private:
    const char* stunServer;
    uint16_t stunPort;
    WiFiUDP udp;
    uint8_t stunRequest[20];  // STUN Request Packet
    uint8_t stunResponse[512];  // STUN Response Packet
    
    void sendSTUNRequest();
    IPAddress parseSTUNResponse();
};

#endif
