#include "Arduino.h"

// Pin for the camera flash (LED)
#define FLASH_LED_PIN 4

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize the flash LED pin as an output
  pinMode(FLASH_LED_PIN, OUTPUT);

  // Turn off the flash LED initially
  digitalWrite(FLASH_LED_PIN, LOW);
  
  // Delay to see initial state
  delay(1000);
}

void loop() {
  // Turn the flash LED on
  Serial.println("Turning flash LED ON");
  digitalWrite(FLASH_LED_PIN, HIGH);  
  delay(2000); // Keep it on for 2 seconds
  
  // Turn the flash LED off
  Serial.println("Turning flash LED OFF");
  digitalWrite(FLASH_LED_PIN, LOW);
  delay(2000); // Keep it off for 2 seconds
}
