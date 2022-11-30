#include <Wire.h>

//  Motor
// White Motor is the Left motor
// Black Motor is the Right motor
const byte MOTOR_WHITE_DIR (2);
const byte MOTOR_WHITE_STEP (3);
const byte MOTOR_BLACK_DIR (4);
const byte MOTOR_BLACK_STEP (5);
const byte SQUARE_SIZE = 200;
const int SPEED_SLOW (2000);
const int SPEED_FAST (1000);

enum {T_B, B_T, L_R, R_L, LR_BT, RL_TB, LR_TB, RL_BT, calibrate_speed};
// T=Top  -  B=Bottom  -  L=Left  -  R=Right
//T_B = RIGHT
//B_T = LEFT
//L_R = Towards the front
//R_L = Towards the back
//LR_BT = Diagonal towards front left
//RL_TB = Diagonal towards back right
//LR_TB = Diagonal towards front right
//RL_BT = Diagonal towards back left


const int stepsPerRevolution = 200;

//****************************************  MOTOR
void motor(byte direction, int speed, float distance) {

  float step_number = 0;

  //  Calcul the distance
  if (distance == calibrate_speed) step_number = 4;
  else if (direction == LR_BT || direction == RL_TB || direction == LR_TB || direction == RL_BT) step_number = distance * SQUARE_SIZE * 1.86; //  Add an extra length for the diagonal
  else step_number = distance * SQUARE_SIZE;

  //  Direction of the motor rotation
  if (direction == R_L || direction == T_B || direction == RL_TB) digitalWrite(MOTOR_WHITE_DIR, HIGH);
  else digitalWrite(MOTOR_WHITE_DIR, LOW);
  if (direction == B_T || direction == R_L || direction == RL_BT) digitalWrite(MOTOR_BLACK_DIR, HIGH);
  else digitalWrite(MOTOR_BLACK_DIR, LOW);

  //  Active the motors
  for (int x = 0; x < step_number; x++) {
    if (direction == LR_TB || direction == RL_BT) digitalWrite(MOTOR_WHITE_STEP, LOW);
    else digitalWrite(MOTOR_WHITE_STEP, HIGH);
    if (direction == LR_BT || direction == RL_TB) digitalWrite(MOTOR_BLACK_STEP, LOW);
    else digitalWrite(MOTOR_BLACK_STEP, HIGH);
    delayMicroseconds(speed);
    digitalWrite(MOTOR_WHITE_STEP, LOW);
    digitalWrite(MOTOR_BLACK_STEP, LOW);
    delayMicroseconds(speed);
  }
}

void setup() {
  Serial.begin(9600);

  //  Motor
  pinMode (MOTOR_WHITE_STEP, OUTPUT);
  pinMode (MOTOR_WHITE_DIR, OUTPUT);
  pinMode (MOTOR_BLACK_STEP, OUTPUT);
  pinMode (MOTOR_BLACK_DIR, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  // DIRECTION: RIGHT
  Serial.println("T_B");
  motor(T_B, SPEED_SLOW, 1.0);
  delay(2000);

  // DIRECTION: LEFT
  Serial.println("B_T");
  motor(B_T, SPEED_SLOW, 1.0);
  delay(2000);

  // DIRECTION: Towards the front
  Serial.println("L_R");
  motor(L_R, SPEED_SLOW, 1.0);
  delay(2000);

  // DIRECTION: Towards the back
  Serial.println("R_L");
  motor(R_L, SPEED_SLOW, 1.0);
  delay(2000);

  // DIRECTION: Diagonal towards front left
  Serial.println("LR_BT");
  motor(LR_BT, SPEED_SLOW, 1.0);
  delay(2000);

  // DIRECTION: Diagonal towards back right
  Serial.println("RL_TB");
  motor(RL_TB, SPEED_SLOW, 1.0);
  delay(2000);

  // DIRECTION: Diagonal towards front right
  Serial.println("LR_TB");
  motor(LR_TB, SPEED_SLOW, 1.0);
  delay(2000);

  // DIRECTION: Diagonal towards back left
  Serial.println("RL_BT");
  motor(RL_BT, SPEED_SLOW, 1.0);
  delay(2000);
}

/*
 For the wiring of the stepper motors we used the external power supply of 12V
 We have ground connected to the ground of the power supply for GND next to VMOT.
 VMOT is 12V
 VDD was 5V from Arduino
*/
