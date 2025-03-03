#include <HTTPClient.h>
#include <WiFi.h>

const char *ssid = "SUB1";
const char *password = "newtown123";
const char *serverUrl = "http://192.168.1.110:5000/receiveMessage";

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Start the HTTP POST request
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    // Create a JSON payload (example)
    String payload = "{\"message\":\"Hello from ESP32-CAM\"}";

    // Send the POST request
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.println("Error on sending POST");
    }

    http.end();
  }

  delay(5000);  // Send a message every 5 seconds
}