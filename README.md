# ESP32 Surveillance Car
It's 3rd Year, 5th Semester, Microcontroller Lab Project

## Overview
The ESP32 Surveillance Car project combines the capabilities of the ESP32-CAM module and L298N motor driver to create a Wi-Fi-controlled car equipped with a camera for surveillance. The project includes pan-tilt camera control, car movement, adjustable light intensity, and a live video feed accessible via a web interface.

## Features
1. **Camera Stream**
   - Live video stream available over Wi-Fi.
   - Adjustable pan and tilt angles for the camera.
2. **Car Movement Control**
   - Forward, backward, left, and right movements.
   - Adjustable motor speed using PWM.
   - Center steering control.
3. **Light Control**
   - Adjustable light brightness using PWM.
4. **User Interface**
   - Accessible web interface for control and video streaming.
   - Cross-platform compatibility.
5. **Additional Controls**
   - Pan-tilt camera movement.
   - Car steering and speed adjustments.

## Hardware Components
1. **ESP32-CAM Module**
   - Wi-Fi-enabled microcontroller with camera functionality.
2. **L298N Motor Driver**
   - Dual H-bridge driver for motor control.
3. **Servo Motors**
   - Used for pan and tilt camera movement and car steering.
4. **Power Supply**
   - External power source for ESP32 and motors.
5. **Miscellaneous**
   - Jumper wires, breadboard, etc.

## Pin Configuration
| Component        | ESP32 Pin  | Description           |
|------------------|------------|-----------------------|
| Camera PWDN      | GPIO 32    | Power-down control    |
| Camera RESET     | GPIO -1    | Reset (not used)      |
| Camera XCLK      | GPIO 0     | External clock input  |
| Motor IN1        | GPIO 13    | Motor direction 1     |
| Motor IN2        | GPIO 15    | Motor direction 2     |
| Motor ENA        | GPIO 12    | Motor speed control   |
| Steering Servo   | GPIO 14    | Servo control         |
| Pan Servo        | GPIO 2     | Pan servo control     |
| Tilt Servo       | GPIO 4     | Tilt servo control    |
| Light PWM        | GPIO 16    | Adjustable light PWM  |

## Software Implementation

### Libraries Used
- `esp_camera.h`: For ESP32-CAM camera control.
- `WiFi.h`: For Wi-Fi connectivity.
- `esp_http_server.h`: HTTP server for hosting the web interface.
- `ESP32Servo.h`: Servo motor control.

### Key Functions
1. **Web Server Initialization**
   - Hosts a web page for controlling the car and viewing the video stream.
2. **Camera Stream Handling**
   - Captures frames from the camera and sends them to the client.
3. **Car Control**
   - Implements motor control for forward, backward, left, and right movements.
   - Uses PWM for speed adjustments.
4. **Pan-Tilt Control**
   - Adjusts the pan and tilt angles of the camera using servo motors.
5. **Light Brightness Adjustment**
   - Controls light intensity using PWM.

### Web Interface
The web interface includes:
- Live video stream.
- Buttons for car and camera control.
- Sliders for speed and brightness adjustments.

### Code Structure
1. **Main File (`setup` and `loop`):**
   - Configures the ESP32-CAM, initializes the web server, and sets up GPIO pins.
2. **HTTP Handlers:**
   - `index_handler`: Serves the HTML page.
   - `stream_handler`: Handles the video stream.
   - `cmd_handler`: Processes commands for car movement, camera angles, and light adjustments.

## Setup and Configuration
1. **Hardware Setup**
   - Connect the ESP32-CAM module, L298N motor driver, servo motors, and power supply as per the pin configuration table.
2. **Software Setup**
   - Install Arduino IDE and configure ESP32 board support.
   - Upload the provided code to the ESP32-CAM module.
3. **Network Configuration**
   - Update the Wi-Fi credentials (`ssid` and `password`) in the code.

## Usage Instructions
1. Power on the system.
2. Connect to the ESP32’s Wi-Fi network.
3. Open the web interface using the ESP32’s IP address in a browser.
4. Use the web interface to control the car and view the live video stream.

## Troubleshooting
1. **No Video Stream**
   - Check the camera connections.
   - Ensure the ESP32 is connected to Wi-Fi.
2. **Car Not Moving**
   - Verify motor connections.
   - Check the power supply.
3. **Servo Motors Not Working**
   - Ensure correct GPIO pin connections.
   - Test with a known working servo.

## Future Improvements
1. **Obstacle Detection**
   - Integrate ultrasonic sensors for autonomous navigation.
2. **Battery Monitoring**
   - Add battery voltage monitoring and alerts.
3. **Mobile App Control**
   - Develop a mobile application for enhanced user experience.
4. **Additional Sensors**
   - Include environmental sensors for data collection.

## References
- ESP32-CAM Documentation
- Arduino Servo Library
- L298N Motor Driver Datasheet
 
- https://lastminuteengineers.com/esp32-cam-pinout-reference/

- https://randomnerdtutorials.com/esp32-dc-motor-l298n-motor-driver-control-speed-direction/