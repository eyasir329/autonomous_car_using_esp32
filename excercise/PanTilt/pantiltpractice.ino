//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 01 ESP32 Cam Test Servo
/*
 * Reference :
 * - ESP32Servo (Author: Kevin Harrington,John K. Bennett) : - https://www.arduino.cc/reference/en/libraries/esp32servo/
 *                                                           - https://github.com/madhephaestus/ESP32Servo
 * - Timer on ESP32 : https://openlabpro.com/guide/timer-on-esp32/
 * 
 * and from various other sources.
 */

/* ======================================== Including the libraries. */
#include <ESP32Servo.h>
/* ======================================== */

/* ======================================== SERVOs GPIO */
#define servo_LeftRight_Pin   14
#define servo_UpDown_Pin      15
/* ======================================== */

/* ======================================== Variables declaration */
// Variable for servo position
int servo_LeftRight_Pos = 75;
int servo_UpDown_Pos = 75;
/* ======================================== */

/* ======================================== Initialize servos */
/*
 * Maybe in this section you are a little confused, why did I initialize 4 servos, even though in this project only 2 servos are used. Below is a brief explanation.
 * 
 * The ESP32 has 4 hardware timers (please correct me if I'm wrong). If only initialize 2 servos, then automatically based on servo library, 
 * then servo_1 will use timer_0 (channel_0) and servo_2 will use timer_1 (channel_1). 
 * That would be a problem because timer_0 (channel_0) and timer_1 (channel_1) are already in use by the camera.
 * 
 * Therefore, initialization of 4 servos is used, so that the 2 servos that are actually used will shift to timer_2 (channel_2) and timer_3 (channel_3). Below is the simple flow:
 * 
 * servo_Dummy_1    --> timer_0 or channel_0
 * servo_Dummy_2    --> timer_1 or channel_1
 * servo_LeftRight  --> timer_2 or channel_2
 * servo_UpDown     --> timer_3 or channel_3
 * 
 * Based on the flow above, the initialization of servo_Dummy_1 and servo_Dummy_2 is only used to shift the timer, 
 * so the 2 servos used in this project will use timer_2 (channel_2) and timer_3 (channel_3). 
 * servo_Dummy_1 and servo_Dummy_2 are not actually used, because timer_0 (channel_0) and timer_1 (channel_1) are already used by the camera.
 */
Servo servo_Dummy_1;
Servo servo_Dummy_2;
Servo servo_LeftRight;
Servo servo_UpDown;
/* ======================================== */

/* ________________________________________________________________________________ VOID SETUP() */
void setup() {
  // put your setup code here, to run once:

  /* ---------------------------------------- Init serial communication speed (baud rate). */
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.println();
  /* ---------------------------------------- */

  /* ---------------------------------------- Setting Servos */
  Serial.println();
  Serial.println("------------");
  Serial.println("Start setting Servos...");
  servo_LeftRight.setPeriodHertz(50); //--> // Standard 50hz servo
  servo_UpDown.setPeriodHertz(50);    //--> // Standard 50hz servo
  servo_Dummy_1.attach(12, 1000, 2000);
  servo_Dummy_2.attach(13, 1000, 2000);
  servo_LeftRight.attach(servo_LeftRight_Pin, 1000, 2000);
  servo_UpDown.attach(servo_UpDown_Pin, 1000, 2000);
  
  servo_LeftRight.write(servo_LeftRight_Pos);
  servo_UpDown.write(servo_UpDown_Pos);
  Serial.println("Setting up the servos was successful.");
  Serial.println("------------");
  Serial.println();
  /* ---------------------------------------- */
  
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  for (int i = 75; i <= 175; i+=5) {
    servo_LeftRight.write(i);
    Serial.print("SLR : ");
    Serial.println(i);
    delay(100);
  }

  for (int i = 180; i >= 5; i-=5) {
    servo_LeftRight.write(i);
    Serial.print("SLR : ");
    Serial.println(i);
    delay(100);
  }

  for (int i = 0; i <= 75; i+=5) {
    servo_LeftRight.write(i);
    Serial.print("SLR : ");
    Serial.println(i);
    delay(100);
  }

  delay(2000);

  for (int i = 75; i <= 175; i+=5) {
    servo_UpDown.write(i);
    Serial.print("SUD : ");
    Serial.println(i);
    delay(100);
  }

  for (int i = 180; i >= 5; i-=5) {
    servo_UpDown.write(i);
    Serial.print("SUD : ");
    Serial.println(i);
    delay(100);
  }

  for (int i = 0; i <= 75; i+=5) {
    servo_UpDown.write(i);
    Serial.print("SUD : ");
    Serial.println(i);
    delay(100);
  }

  delay(2000);
}
