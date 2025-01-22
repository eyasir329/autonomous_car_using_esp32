#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Wi-Fi Credentials
const char* ssid = "SUB1";
const char* password = "newtown123";

// Web server instance
AsyncWebServer server(80);

// Function to log Wi-Fi status
void logWiFiStatus() {
  int status = WiFi.status();
  Serial.print("Wi-Fi Status Code: ");
  Serial.println(status); // Print raw status code

  switch (status) {
    case WL_IDLE_STATUS:
      Serial.println("WL_IDLE_STATUS");
      break;
    case WL_NO_SSID_AVAIL:
      Serial.println("WL_NO_SSID_AVAIL");
      break;
    case WL_CONNECT_FAILED:
      Serial.println("WL_CONNECT_FAILED");
      break;
    case WL_CONNECTED:
      Serial.println("WL_CONNECTED");
      break;
    case WL_DISCONNECTED:
      Serial.println("WL_DISCONNECTED");
      break;
    default:
      Serial.println("Unknown Status");
      break;
  }
}

// Connect to Wi-Fi
void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi...");
  
  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    logWiFiStatus(); // Print Wi-Fi status every second
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// Web Server Root handler
void handleRoot(AsyncWebServerRequest *request) {
  request->send(200, "text/html", "<h1>Hello, World!</h1><p>This is a basic web server running on ESP32.</p>");
}

void setup() {
  // Start Serial Monitor for debugging
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  connectToWiFi();

  // Define routes
  server.on("/", HTTP_GET, handleRoot);
  
  // Start the server
  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  // Nothing to do here for now, the server will run in the background
}
