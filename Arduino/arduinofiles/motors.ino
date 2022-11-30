#include <Wire.h>

//  Motor
const byte MOTOR_WHITE_DIR (2);
const byte MOTOR_WHITE_STEP (3);
const byte MOTOR_BLACK_DIR (4);
const byte MOTOR_BLACK_STEP (5);
const int stepsPerRevolution = 200;

void setup() {
  Serial.begin(9600);

  pinMode (MOTOR_WHITE_STEP, OUTPUT);
  pinMode (MOTOR_WHITE_DIR, OUTPUT);
  pinMode (MOTOR_BLACK_STEP, OUTPUT);
  pinMode (MOTOR_BLACK_DIR, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(MOTOR_WHITE_DIR, HIGH);
  digitalWrite(MOTOR_BLACK_DIR, HIGH);
  Serial.print("forward");
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(MOTOR_WHITE_STEP, HIGH);
    digitalWrite(MOTOR_BLACK_STEP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(MOTOR_WHITE_STEP, LOW);
    digitalWrite(MOTOR_BLACK_STEP, LOW);
    delayMicroseconds(1000);
  }
  delay(1000);
  
  Serial.print("backward");
  digitalWrite(MOTOR_WHITE_DIR, LOW);
  digitalWrite(MOTOR_BLACK_DIR, LOW);
  // Spin motor quickly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(MOTOR_WHITE_STEP, HIGH);
    digitalWrite(MOTOR_BLACK_STEP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(MOTOR_WHITE_STEP, LOW);
    digitalWrite(MOTOR_BLACK_STEP, LOW);
    delayMicroseconds(1000);
  }
  delay(1000); // Wait a second

  Serial.print("left");
  digitalWrite(MOTOR_WHITE_DIR, HIGH);
  digitalWrite(MOTOR_BLACK_DIR, LOW);
  // Spin motor quickly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(MOTOR_WHITE_STEP, HIGH);
    digitalWrite(MOTOR_BLACK_STEP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(MOTOR_WHITE_STEP, LOW);
    digitalWrite(MOTOR_BLACK_STEP, LOW);
    delayMicroseconds(1000);
  }
  delay(1000); // Wait a second

  Serial.print("right");
  digitalWrite(MOTOR_WHITE_DIR, LOW);
  digitalWrite(MOTOR_BLACK_DIR, HIGH);
  // Spin motor quickly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(MOTOR_WHITE_STEP, HIGH);
    digitalWrite(MOTOR_BLACK_STEP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(MOTOR_WHITE_STEP, LOW);
    digitalWrite(MOTOR_BLACK_STEP, LOW);
    delayMicroseconds(1000);
  }
  delay(1000); // Wait a second

  Serial.print("diagonal-1");
  digitalWrite(MOTOR_WHITE_DIR, LOW);
  // Spin motor quickly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(MOTOR_WHITE_STEP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(MOTOR_WHITE_STEP, LOW);
    delayMicroseconds(1000);
  }
  delay(1000); // Wait a second

    Serial.print("diagonal-2");
  digitalWrite(MOTOR_WHITE_DIR, HIGH);
  // Spin motor quickly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(MOTOR_WHITE_STEP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(MOTOR_WHITE_STEP, LOW);
    delayMicroseconds(1000);
  }
  delay(1000); // Wait a second

    Serial.print("diagonal-3");
  digitalWrite(MOTOR_BLACK_DIR, LOW);
  // Spin motor quickly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(MOTOR_BLACK_STEP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(MOTOR_BLACK_STEP, LOW);
    delayMicroseconds(1000);
  }
  delay(1000); // Wait a second

    Serial.print("diagonal-4");
  digitalWrite(MOTOR_BLACK_DIR, HIGH);
  // Spin motor quickly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(MOTOR_BLACK_STEP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(MOTOR_BLACK_STEP, LOW);
    delayMicroseconds(1000);
  }
  delay(1000); // Wait a second
  
}



/*
 For the wiring of the stepper motors we used the external power supply of 12V
 We have ground connected to the ground of the power supply for both inputs on the driver
 VMOT is 12V
 VDD was 5V from Arduino
*/
  
