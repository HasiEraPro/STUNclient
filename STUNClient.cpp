#include "STUNClient.h"

STUNClient::STUNClient() {
  stunServer = "";
  stunPort = 0;
}

bool STUNClient::begin(const char* server, uint16_t port) {
  stunServer = server;
  stunPort = port;

  // Initialize STUN request (Basic Binding Request)
  memset(stunRequest, 0, sizeof(stunRequest));
  stunRequest[0] = 0x00;  // Message Type (Binding Request)
  stunRequest[1] = 0x01;  // Message Type (Binding Request)

  // Start the UDP communication
  udp.begin(0);  // Use random local port for UDP

  return true;
}

void STUNClient::sendSTUNRequest() {
  udp.beginPacket(stunServer, stunPort);
  udp.write(stunRequest, sizeof(stunRequest));
  udp.endPacket();

  Serial.println("STUN request sent");
}

IPAddress STUNClient::parseSTUNResponse() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet size: ");
    Serial.println(packetSize);
    
    int len = udp.read(stunResponse, sizeof(stunResponse));
    if (len > 0) {
      Serial.print("Received STUN Response: ");
      for (int i = 0; i < len; i++) {
        Serial.print(stunResponse[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
      
      // Print full response in decimal for detailed analysis
      Serial.println("Full Response in Decimal:");
      for (int i = 0; i < len; i++) {
        Serial.print(stunResponse[i], DEC);
        Serial.print(" ");
      }
      Serial.println();
      
      // Check the family byte
      uint8_t family = stunResponse[25];  // Family byte is at index 24
      Serial.print("Family Byte: ");
      Serial.println(family, HEX);  // Print family byte in HEX
      
      if (family == 0x01) { // IPv4
        // Extract port (2 bytes, at byte 26 and 27)
        uint16_t mappedPort = (stunResponse[26] << 8) | stunResponse[27]; // Big-endian to little-endian
        // Extract IP address (4 bytes, at byte 28 to 31)
        uint8_t mappedAddress[4] = { stunResponse[28], stunResponse[29], stunResponse[30], stunResponse[31] };
        IPAddress publicIP(mappedAddress[0], mappedAddress[1], mappedAddress[2], mappedAddress[3]);
        
        // Print the public IP and port
        Serial.print("Public IP: ");
        Serial.println(publicIP);
        Serial.print("Mapped Port: ");
        Serial.println(mappedPort);
        return publicIP;
      } else {
        Serial.println("Unsupported address family (not IPv4).");
      }
    } else {
      Serial.println("Failed to read STUN response.");
    }
  } else {
    Serial.println("No STUN response received.");
  }

  return IPAddress(0,0,0,0);
}


IPAddress STUNClient::getPublicIP() {
  sendSTUNRequest();
  return parseSTUNResponse();
}
