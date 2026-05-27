#include <WiFi.h>
#include <WiFiServer.h>

// WiFi credentials - Change these!
const char* ssid = "YOUR_HOTSPOT_NAME";
const char* password = "YOUR_PASSWORD";

// Static IP - Change to match your network!
IPAddress local_IP(10, 94, 77, 100);
IPAddress gateway(10, 94, 77, 52);
IPAddress subnet(255, 255, 255, 0);

// Server on port 8888
WiFiServer server(8888);

// UART pins to Arduino
#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server started! Waiting for commands...");
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client connected!");
    while (client.connected()) {
      if (client.available()) {
        char command = client.read();
        Serial.print("Command received: ");
        Serial.println(command);
        Serial2.write(command);
      }
    }
    client.stop();
    Serial.println("Client disconnected!");
  }
}