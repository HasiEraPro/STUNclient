#include <WiFi.h>
#include "STUNClient.h"

const char* ssid = "your ssid here";
const char* password = "your password here";

STUNClient stunClient;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi");
 Serial.println(WiFi.localIP());

  // Initialize STUN client with Google's STUN server
  stunClient.begin("stun.l.google.com", 19302);

  // Attempt to get public IP
  IPAddress publicIP = stunClient.getPublicIP();
  Serial.print("Public IP: ");
  Serial.println(publicIP);
}

void loop() {

   // Attempt to get public IP
  IPAddress publicIP = stunClient.getPublicIP();
  Serial.print("Public IP: ");
  Serial.println(publicIP);
  delay(2000);
}