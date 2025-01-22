#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>

// Replace with your WiFi credentials
const char* ssid = "SUB1";
const char* password = "newtown123";

// Pin definitions for ESP32-CAM
#define MOTOR_IN1 14  // L298N IN1
#define MOTOR_IN2 15  // L298N IN2
#define MOTOR_ENA 12  // L298N ena

#define STEERING_SERVO 13

// Servo settings
Servo steeringServo;
int steeringAngle = 90; // Default servo position (center)

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize motor pins
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);

  // Initialize servo
  steeringServo.attach(STEERING_SERVO, 500, 2400);
  steeringServo.write(steeringAngle);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Web server routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", R"rawliteral(
      <html>
        <head><title>ESP32 Motor Control</title></head>
        <body>
          <h1>Motor and Steering Control</h1>
          <button onclick="sendCommand('forward')">Forward</button>
          <button onclick="sendCommand('backward')">Backward</button>
          <button onclick="sendCommand('stop')">Stop</button><br>
          <button onclick="sendCommand('left')">Left</button>
          <button onclick="sendCommand('right')">Right</button>
          <button onclick="sendCommand('center')">Center</button>
          <script>
            function sendCommand(cmd) {
              fetch('/action?command=' + cmd);
            }
          </script>
        </body>
      </html>
    )rawliteral");
  });

  server.on("/action", HTTP_GET, [](AsyncWebServerRequest *request){
    String command;
    if (request->hasParam("command")) {
      command = request->getParam("command")->value();
      if (command == "forward") {
        digitalWrite(MOTOR_IN1, HIGH);
        digitalWrite(MOTOR_IN2, LOW);
        for (int i = 0; i < 256; i++){
          analogWrite(MOTOR_ENA, i);
          delay(50);
        }
      } else if (command == "backward") {
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, HIGH);
        for (int i = 0; i < 256; i++){
          analogWrite(MOTOR_ENA, i);
          delay(50);
        }
      } else if (command == "stop") {
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, LOW);
      } else if (command == "left") {
        steeringServo.write(45); // Turn left
      } else if (command == "right") {
        steeringServo.write(135); // Turn right
      } else if (command == "center") {
        steeringServo.write(90); // Center steering
      }
    }
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
}

void loop() {
  // No code needed here for async web server
}
