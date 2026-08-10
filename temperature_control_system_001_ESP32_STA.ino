// Station WiFi Connection for temperature control 001
// use with temperature_control_system_2-001_Mega-2560
// en Hanna P 2026

// Replace with your network credentials (STATION)
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

unsigned long previousMillis = 0;
unsigned long interval = 30000;

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
  Serial.println("Setup start.. ");
  mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1);  // UART setup
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
  
  server.begin();
  
  Serial.println("Setup done");
}

void loop() {
  unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
  }
  
  //testReadData(); // for Uart testing
  readData(); // for Uart read all temperatures
  // Listen for incoming clients
  WiFiClient client = server.available();
  
  // If a new client connects
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    // loop while the client's connected
    while (client.connected()) {
      // if there's bytes to read from the client,
      if (client.available()) {
        // read a byte, then
        char c = client.read();
        Serial.write(c);
        header += c;
        // if the byte is a newline character
        if (c == '\n') {
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // palce for functional code

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: left;}");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println("</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Access Point for temperature control 001</h1>");
            // JavaScript reload timer   
            client.println("<script> setTimeout(function(){");
            client.println("location.reload(); }, 10000); </script>");
            
            // Web Page content
            client.print("<p>Heating burner: ");
            client.print(heating_burner);
            client.println("</p>");
            client.print("<p>Heating tanktop: ");
            client.print(heating_tanktop);
          }
        }
      }
    }
  }
}

void testReadData() {
  // Check if data is available to read
  if (mySerial.available()) {
    // Read data and display it for debug
    String txt, val;
    String message = mySerial.readStringUntil('\n');
    for (int i = 0; i < message.length(); i++) {
      if (message.substring(i, i+1) == "#") {
        txt = message.substring(0, i);
        val = message.substring(i+1);
          break;
      }
    }
    Serial.println("Received: " + message);
    Serial.println("txt: " + txt);
    Serial.println("val: " + val);
    if (txt == "Test") {
      heating_burner = val;
    }
  }
}

void readData() {
  // Check if data is available to read
  if (mySerial.available()) {
    // Read data for HTML web page
    String dItem, dValue;
    String tData = mySerial.readStringUntil('\n');
    Serial.println("Received: " + tData);
    for (int i = 0; i < tData.length(); i++) {
      if (tData.substring(i, i+1) == "#") {
        dItem = tData.substring(0, i);
        dValue = tData.substring(i+1);
        break;
      }
    }
    if (dItem == "Burner") {
      heating_burner = dValue;
    } else if (dItem == "Tanktop") {
      heating_tanktop = dValue;
    } else if (dItem == "Inline") {
      heating_inline = dValue;
    } else if (dItem == "Hotwater") {
      heating_hotwater = dValue;
    } else if (dItem == "Outdoor") {
      outdoor = dValue;
    } else if (dItem == "UpTime_m") {
      uptime_m = dValue;
    ]
  }
}
