// Station WiFi Connection for temperature control 001
// use with temperature_control_system_2-001_Mega-2560
// en Hanna P 2026

// Replace with your network credentials (STATION)
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Set web server port number
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Assign input variables to GPIO pins

// Variable to temperatures etc.
String heating_burner;
String heating_tanktop;
String heating_inline;
String heating_hotwater;
String outdoor;
String uptime_m;

// Load Wi-Fi library
#include <WiFi.h>

// Define TX and RX pins for UART (change if needed)
#define TXD1 19
#define RXD1 21

// Use Serial1 for UART communication
HardwareSerial mySerial(2);

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
}

void loop() {
  // put your main code here, to run repeatedly:

}
