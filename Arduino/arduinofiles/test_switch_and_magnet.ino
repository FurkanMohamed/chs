#include <Wire.h>

int reed_sensor_status [8][8];
int reed_sensor_record [8][8];
int reed_sensor_status_memory [8][8];
int reed_sensor;

byte reed_colone[2];
byte reed_line[2];

//  Electromagnet
#define MAGNET 6
#define REED 7

//  Motor
const byte MOTOR_WHITE_DIR (2);
const byte MOTOR_WHITE_STEP (3);
const byte MOTOR_BLACK_DIR (4);
const byte MOTOR_BLACK_STEP (5);

void setup() {
  Serial.begin(9600);

  pinMode (MOTOR_WHITE_STEP, OUTPUT);
  pinMode (MOTOR_WHITE_DIR, OUTPUT);
  pinMode (MOTOR_BLACK_STEP, OUTPUT);
  pinMode (MOTOR_BLACK_DIR, OUTPUT);

}

// *******************************  ELECTROMAGNET
void electromagnet(boolean state) {

  if (state == true)  {
    digitalWrite(MAGNET, HIGH);
    delay(600);
  }
  else  {
    delay(600);
    digitalWrite(MAGNET, LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(MOTOR_WHITE_STEP, HIGH);
  digitalWrite(MOTOR_WHITE_DIR, HIGH);
  digitalWrite(MOTOR_BLACK_STEP, HIGH);
  digitalWrite(MOTOR_BLACK_DIR, HIGH);
}
