//***********************************************
//
//            AUTOMATIC CHESSBOARD
//
//***********************************************

//******************************  INCLUDING FILES
#include <Wire.h>


//********************  DEFINING GLOBAL VARIABLES
//  Chessboard
int chess_board [8][8];
const float TROLLEY_START_POSITION_X = 0.73; // used for getting to e7 x coordinate
const float TROLLEY_START_POSITION_Y = 3.98; // used for getting to e7 y coordinate
byte trolley_coordinate_X = 5; // x coordinate for e7
byte trolley_coordinate_Y = 7; // y coordinate for e7
char mov [4] = {0, 0, 0, 0};
boolean no_valid_move = false;

//  Game setting
enum {start_up, calibration, player_white, player_black};
byte sequence = start_up;
enum {T_B, B_T, L_R, R_L, LR_BT, RL_TB, LR_TB, RL_BT, calibrate_speed};
// T=Top  -  B=Bottom  -  L=Left  -  R=Right
byte game_mode = 0;
enum {HvsH, HvsC};

// Magnet
const byte MAGNET (13);

//  Motor
const byte MOTOR_WHITE_DIR (2);
const byte MOTOR_WHITE_STEP (3);
const byte MOTOR_BLACK_DIR (4);
const byte MOTOR_BLACK_STEP (5);
const byte SQUARE_SIZE = 190; // configured for our chess board and seems to give good movements of a true square for each 1 unit
const int SPEED_SLOW (2000);
const int SPEED_FAST (1000);

//  Button - Switch (arcade buttons on the board)
const byte BUTTON_WHITE_SWITCH_MOTOR_WHITE (11);
const byte BUTTON_BLACK_SWITCH_MOTOR_BLACK (10);
enum {WHITE, BLACK};

//  Store values from Raspberry Pi for AI input
char lastM[5];

// Store values from Raspberry Pi for Human input
char lastH[5];


//****************************************  SETUP
void setup() {
  Serial.begin(9600);

  //  Electromagnet
  pinMode (MAGNET, OUTPUT);

  //  Motor
  pinMode (MOTOR_WHITE_STEP, OUTPUT);
  pinMode (MOTOR_WHITE_DIR, OUTPUT);
  pinMode (MOTOR_BLACK_STEP, OUTPUT);
  pinMode (MOTOR_BLACK_DIR, OUTPUT);

  //  Set the chess board status to beginning board
  for (byte i = 2; i < 6; i++) {
    for (byte j = 0; j < 8; j++) {
      chess_board[i][j] = 1;
    }
  }

  //  Arcade button - Limit Switch
  pinMode (BUTTON_WHITE_SWITCH_MOTOR_WHITE, INPUT_PULLUP);
  pinMode (BUTTON_BLACK_SWITCH_MOTOR_BLACK, INPUT_PULLUP);

}


//***************************************  Push button to end human move
boolean button(byte type) {
  if (type == WHITE && digitalRead(BUTTON_WHITE_SWITCH_MOTOR_WHITE) != HIGH) {
    delay(250);
    return true;
  }
  if (type == BLACK && digitalRead(BUTTON_BLACK_SWITCH_MOTOR_BLACK) != HIGH) {
    delay(250);
    return true;
  }
  return false;
}


//************************************  CALIBRATE
void calibrate() {
  //  Slow displacements up to touch the limit switches
  while (digitalRead(BUTTON_WHITE_SWITCH_MOTOR_WHITE) == HIGH) motor(L_R, SPEED_SLOW, calibrate_speed);
  while (digitalRead(BUTTON_BLACK_SWITCH_MOTOR_BLACK) == HIGH) motor(T_B, SPEED_SLOW, calibrate_speed);
  delay(500);

  //  Rapid displacements up to the Black start position (e7)
  motor(B_T, SPEED_FAST, TROLLEY_START_POSITION_X);
  motor(R_L, SPEED_FAST, TROLLEY_START_POSITION_Y);
  delay(500);
}

//****************************************  MOTOR
void motor(byte direction, int speed, float distance) {

  float step_number = 0;

  //  Calculate the distance
  if (distance == calibrate_speed) step_number = 4;
  else if (direction == LR_BT || direction == RL_TB || direction == LR_TB || direction == RL_BT) step_number = distance * SQUARE_SIZE * 1.86; //  Add an extra length for the diagonal (configured)
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


// ***********************  BLACK PLAYER MOVEMENT
void black_player_movement() {

  //  Convert the AI characters into variables
  // replacing with the values from Raspberry Pi
  int departure_coord_X = lastM[0] - 'a' + 1;
  int departure_coord_Y = lastM[1] - '0';
  int arrival_coord_X = lastM[2] - 'a' + 1;
  int arrival_coord_Y = lastM[3] - '0';
  byte displacement_X = 0;
  byte displacement_Y = 0;

  //  Trolley displacement to the starting position
  int convert_table [] = {0, 7, 6, 5, 4, 3, 2, 1, 0};
  byte white_capturing = 1;
  if (chess_board[convert_table[arrival_coord_Y]][arrival_coord_X - 1] == 0) white_capturing = 0; // checking if a peice already exists on this grid coordinate (hence a need to capture)

  //
  for (byte i = white_capturing; i < 2; i++) {
    if (i == 0) {
      displacement_X = abs(arrival_coord_X - trolley_coordinate_X);
      displacement_Y = abs(arrival_coord_Y - trolley_coordinate_Y);
    }
    else if (i == 1) {
      displacement_X = abs(departure_coord_X - trolley_coordinate_X);
      displacement_Y = abs(departure_coord_Y - trolley_coordinate_Y);
    }
    if (departure_coord_X > trolley_coordinate_X) motor(T_B, SPEED_FAST, displacement_X);
    else if (departure_coord_X < trolley_coordinate_X) motor(B_T, SPEED_FAST, displacement_X);
    if (departure_coord_Y > trolley_coordinate_Y) motor(L_R, SPEED_FAST, displacement_Y);
    else if (departure_coord_Y < trolley_coordinate_Y) motor(R_L, SPEED_FAST, displacement_Y);
    if (i == 0) {
      electromagnet(true);
      motor(R_L, SPEED_SLOW, 0.5);
      motor(B_T, SPEED_SLOW, arrival_coord_X - 0.5);
      electromagnet(false);
      motor(L_R, SPEED_FAST, 0.5);
      motor(T_B, SPEED_FAST, arrival_coord_X - 0.5);
      trolley_coordinate_X = arrival_coord_X;
      trolley_coordinate_Y = arrival_coord_Y;
    }
  }
  trolley_coordinate_X = arrival_coord_X;
  trolley_coordinate_Y = arrival_coord_Y;

  //  Move the Black chess piece to the arrival position
  displacement_X = abs(arrival_coord_X - departure_coord_X);
  displacement_Y = abs(arrival_coord_Y - departure_coord_Y);

  electromagnet(true);
  //  Bishop displacement
  if (displacement_X == 1 && displacement_Y == 2 || displacement_X == 2 && displacement_Y == 1) {
    if (displacement_Y == 2) {
      if (departure_coord_X < arrival_coord_X) {
        motor(T_B, SPEED_SLOW, displacement_X * 0.5);
        if (departure_coord_Y < arrival_coord_Y) motor(L_R, SPEED_SLOW, displacement_Y);
        else motor(R_L, SPEED_SLOW, displacement_Y);
        motor(T_B, SPEED_SLOW, displacement_X * 0.5);
      }
      else if (departure_coord_X > arrival_coord_X) {
        motor(B_T, SPEED_SLOW, displacement_X * 0.5);
        if (departure_coord_Y < arrival_coord_Y) motor(L_R, SPEED_SLOW, displacement_Y);
        else motor(R_L, SPEED_SLOW, displacement_Y);
        motor(B_T, SPEED_SLOW, displacement_X * 0.5);
      }
    }
    else if (displacement_X == 2) {
      if (departure_coord_Y < arrival_coord_Y) {
        motor(L_R, SPEED_SLOW, displacement_Y * 0.5);
        if (departure_coord_X < arrival_coord_X) motor(T_B, SPEED_SLOW, displacement_X);
        else motor(B_T, SPEED_SLOW, displacement_X);
        motor(L_R, SPEED_SLOW, displacement_Y * 0.5);
      }
      else if (departure_coord_Y > arrival_coord_Y) {
        motor(R_L, SPEED_SLOW, displacement_Y * 0.5);
        if (departure_coord_X < arrival_coord_X) motor(T_B, SPEED_SLOW, displacement_X);
        else motor(B_T, SPEED_SLOW, displacement_X);
        motor(R_L, SPEED_SLOW, displacement_Y * 0.5);
      }
    }
  }
  //  Diagonal displacement
  else if (displacement_X == displacement_Y) {
    if (departure_coord_X > arrival_coord_X && departure_coord_Y > arrival_coord_Y) motor(RL_BT, SPEED_SLOW, displacement_X);
    else if (departure_coord_X > arrival_coord_X && departure_coord_Y < arrival_coord_Y) motor(LR_BT, SPEED_SLOW, displacement_X);
    else if (departure_coord_X < arrival_coord_X && departure_coord_Y > arrival_coord_Y) motor(RL_TB, SPEED_SLOW, displacement_X);
    else if (departure_coord_X < arrival_coord_X && departure_coord_Y < arrival_coord_Y) motor(LR_TB, SPEED_SLOW, displacement_X);
  }
  //  Kingside castling
  else if (departure_coord_X == 5 && departure_coord_Y == 8 && arrival_coord_X == 7 && arrival_coord_Y == 8) {  //  Kingside castling
    motor(R_L, SPEED_SLOW, 0.5);
    motor(T_B, SPEED_SLOW, 2);
    electromagnet(false);
    motor(T_B, SPEED_FAST, 1);
    motor(L_R, SPEED_FAST, 0.5);
    electromagnet(true);
    motor(B_T, SPEED_SLOW, 2);
    electromagnet(false);
    motor(T_B, SPEED_FAST, 1);
    motor(R_L, SPEED_FAST, 0.5);
    electromagnet(true);
    motor(L_R, SPEED_SLOW, 0.5);
  }
  else if (departure_coord_X == 5 && departure_coord_Y == 8 && arrival_coord_X == 3 && arrival_coord_Y == 8) {  //  Queenside castling
    motor(R_L, SPEED_SLOW, 0.5);
    motor(B_T, SPEED_SLOW, 2);
    electromagnet(false);
    motor(B_T, SPEED_FAST, 2);
    motor(L_R, SPEED_FAST, 0.5);
    electromagnet(true);
    motor(T_B, SPEED_SLOW, 3);
    electromagnet(false);
    motor(B_T, SPEED_FAST, 1);
    motor(R_L, SPEED_FAST, 0.5);
    electromagnet(true);
    motor(L_R, SPEED_SLOW, 0.5);
  }
  //  Horizontal displacement
  else if (displacement_Y == 0) {
    if (departure_coord_X > arrival_coord_X) motor(B_T, SPEED_SLOW, displacement_X);
    else if (departure_coord_X < arrival_coord_X) motor(T_B, SPEED_SLOW, displacement_X);
  }
  //  Vertical displacement
  else if (displacement_X == 0) {
    if (departure_coord_Y > arrival_coord_Y) motor(R_L, SPEED_SLOW, displacement_Y);
    else if (departure_coord_Y < arrival_coord_Y) motor(L_R, SPEED_SLOW, displacement_Y);
  }
  electromagnet(false);

  //  Update the board states with the Black move
  chess_board[convert_table[departure_coord_Y]][departure_coord_X - 1] = 1;
  chess_board[convert_table[arrival_coord_Y]][arrival_coord_X - 1] = 0;

}

// ***********************  BLACK PLAYER MOVEMENT
void white_player_movement() {

  //  Convert the AI characters into variables
  // replacing with the values from Raspberry Pi
  int departure_coord_X = lastH[0] - 'a' + 1;
  int departure_coord_Y = lastH[1] - '0';
  int arrival_coord_X = lastH[2] - 'a' + 1;
  int arrival_coord_Y = lastH[3] - '0';
  byte displacement_X = 0;
  byte displacement_Y = 0;

  //  Trolley displacement to the starting position
  int convert_table [] = {0, 7, 6, 5, 4, 3, 2, 1, 0};
  byte white_capturing = 1;
  if (chess_board[convert_table[arrival_coord_Y]][arrival_coord_X - 1] == 0) white_capturing = 0; // checking if a peice already exists on this grid coordinate

  // move the white peice
  for (byte i = white_capturing; i < 2; i++) {
    if (i == 0) {
      displacement_X = abs(arrival_coord_X - trolley_coordinate_X);
      displacement_Y = abs(arrival_coord_Y - trolley_coordinate_Y);
    }
    else if (i == 1) {
      displacement_X = abs(departure_coord_X - trolley_coordinate_X);
      displacement_Y = abs(departure_coord_Y - trolley_coordinate_Y);
    }
    if (departure_coord_X > trolley_coordinate_X) motor(T_B, SPEED_FAST, displacement_X);
    else if (departure_coord_X < trolley_coordinate_X) motor(B_T, SPEED_FAST, displacement_X);
    if (departure_coord_Y > trolley_coordinate_Y) motor(L_R, SPEED_FAST, displacement_Y);
    else if (departure_coord_Y < trolley_coordinate_Y) motor(R_L, SPEED_FAST, displacement_Y);
    if (i == 0) {
      electromagnet(true);
      motor(R_L, SPEED_SLOW, 0.5);
      motor(B_T, SPEED_SLOW, arrival_coord_X - 0.5);
      electromagnet(false);
      motor(L_R, SPEED_FAST, 0.5);
      motor(T_B, SPEED_FAST, arrival_coord_X - 0.5);
      trolley_coordinate_X = arrival_coord_X;
      trolley_coordinate_Y = arrival_coord_Y;
    }
  }
  trolley_coordinate_X = arrival_coord_X;
  trolley_coordinate_Y = arrival_coord_Y;

  //  Move the Black chess piece to the arrival position
  displacement_X = abs(arrival_coord_X - departure_coord_X);
  displacement_Y = abs(arrival_coord_Y - departure_coord_Y);

  electromagnet(true);
  //  Bishop displacement
  if (displacement_X == 1 && displacement_Y == 2 || displacement_X == 2 && displacement_Y == 1) {
    if (displacement_Y == 2) {
      if (departure_coord_X < arrival_coord_X) {
        motor(T_B, SPEED_SLOW, displacement_X * 0.5);
        if (departure_coord_Y < arrival_coord_Y) motor(L_R, SPEED_SLOW, displacement_Y);
        else motor(R_L, SPEED_SLOW, displacement_Y);
        motor(T_B, SPEED_SLOW, displacement_X * 0.5);
      }
      else if (departure_coord_X > arrival_coord_X) {
        motor(B_T, SPEED_SLOW, displacement_X * 0.5);
        if (departure_coord_Y < arrival_coord_Y) motor(L_R, SPEED_SLOW, displacement_Y);
        else motor(R_L, SPEED_SLOW, displacement_Y);
        motor(B_T, SPEED_SLOW, displacement_X * 0.5);
      }
    }
    else if (displacement_X == 2) {
      if (departure_coord_Y < arrival_coord_Y) {
        motor(L_R, SPEED_SLOW, displacement_Y * 0.5);
        if (departure_coord_X < arrival_coord_X) motor(T_B, SPEED_SLOW, displacement_X);
        else motor(B_T, SPEED_SLOW, displacement_X);
        motor(L_R, SPEED_SLOW, displacement_Y * 0.5);
      }
      else if (departure_coord_Y > arrival_coord_Y) {
        motor(R_L, SPEED_SLOW, displacement_Y * 0.5);
        if (departure_coord_X < arrival_coord_X) motor(T_B, SPEED_SLOW, displacement_X);
        else motor(B_T, SPEED_SLOW, displacement_X);
        motor(R_L, SPEED_SLOW, displacement_Y * 0.5);
      }
    }
  }
  //  Diagonal displacement
  else if (displacement_X == displacement_Y) {
    if (departure_coord_X > arrival_coord_X && departure_coord_Y > arrival_coord_Y) motor(RL_BT, SPEED_SLOW, displacement_X);
    else if (departure_coord_X > arrival_coord_X && departure_coord_Y < arrival_coord_Y) motor(LR_BT, SPEED_SLOW, displacement_X);
    else if (departure_coord_X < arrival_coord_X && departure_coord_Y > arrival_coord_Y) motor(RL_TB, SPEED_SLOW, displacement_X);
    else if (departure_coord_X < arrival_coord_X && departure_coord_Y < arrival_coord_Y) motor(LR_TB, SPEED_SLOW, displacement_X);
  }
  //  Kingside castling
  else if (departure_coord_X == 5 && departure_coord_Y == 8 && arrival_coord_X == 7 && arrival_coord_Y == 8) {  //  Kingside castling
    motor(R_L, SPEED_SLOW, 0.5);
    motor(T_B, SPEED_SLOW, 2);
    electromagnet(false);
    motor(T_B, SPEED_FAST, 1);
    motor(L_R, SPEED_FAST, 0.5);
    electromagnet(true);
    motor(B_T, SPEED_SLOW, 2);
    electromagnet(false);
    motor(T_B, SPEED_FAST, 1);
    motor(R_L, SPEED_FAST, 0.5);
    electromagnet(true);
    motor(L_R, SPEED_SLOW, 0.5);
  }
  else if (departure_coord_X == 5 && departure_coord_Y == 8 && arrival_coord_X == 3 && arrival_coord_Y == 8) {  //  Queenside castling
    motor(R_L, SPEED_SLOW, 0.5);
    motor(B_T, SPEED_SLOW, 2);
    electromagnet(false);
    motor(B_T, SPEED_FAST, 2);
    motor(L_R, SPEED_FAST, 0.5);
    electromagnet(true);
    motor(T_B, SPEED_SLOW, 3);
    electromagnet(false);
    motor(B_T, SPEED_FAST, 1);
    motor(R_L, SPEED_FAST, 0.5);
    electromagnet(true);
    motor(L_R, SPEED_SLOW, 0.5);
  }
  //  Horizontal displacement
  else if (displacement_Y == 0) {
    if (departure_coord_X > arrival_coord_X) motor(B_T, SPEED_SLOW, displacement_X);
    else if (departure_coord_X < arrival_coord_X) motor(T_B, SPEED_SLOW, displacement_X);
  }
  //  Vertical displacement
  else if (displacement_X == 0) {
    if (departure_coord_Y > arrival_coord_Y) motor(R_L, SPEED_SLOW, displacement_Y);
    else if (departure_coord_Y < arrival_coord_Y) motor(L_R, SPEED_SLOW, displacement_Y);
  }
  electromagnet(false);

  //  Update the board states with the Black move
  chess_board[convert_table[departure_coord_Y]][departure_coord_X - 1] = 1;
  chess_board[convert_table[arrival_coord_Y]][arrival_coord_X - 1] = 0;
  
}



//************************** SERIAL COMMUNICATION
void recieveHumanMove(){

  while (Serial.available() <= 0){
    // spin while we wait for the human move to come through
  }

  // recieve human move
  String human_move = Serial.readStringUntil('\n');

  // At this point we have the move from the Raspberry Pi and need to set it to variables in our code
  // Need to parse the data into global variable lastH so it can be used in white_player_movement()

  int str_len = human_move.length() + 1;

  human_move.toCharArray(lastH, str_len);
}



//************************** SERIAL COMMUNICATION
void recieveAIMove(){
  
  while (Serial.available() <= 0){
    // spin while we wait for the AI move to come back
  }
  String data = Serial.readStringUntil('\n');

  // At this point we have the move from the Raspberry Pi and need to set it to variables in our code
  // Need to parse the data into global variable lastM so it can be used in black_player_movement()

  int str_len = data.length() + 1;

  data.toCharArray(lastM, str_len);
}

//*****************************************  LOOP
void loop() {

  switch (sequence) {
    case start_up:
      //Serial.println("Started");
      electromagnet(false);
      game_mode = HvsC;
      sequence = calibration;
      //Serial.println("Finished Startup");
      break;

    case calibration:
      //Serial.println("Calibrating");
      calibrate();
      sequence = player_white;
      break;

    case player_white:
      Serial.println("Human player move. Waiting for move from RPI...");  // sending acknowledge to RPI that we want a move
      recieveHumanMove(); // receive human move from raspberry pi and update array for last human move
      white_player_movement(); // make the human move, checking if we need to perform a capture or not 
      sequence = player_black;
      break;

    case player_black:
      Serial.println("AI move. Waiting for move from RPI...");  // sending acknowledge to RPI that we want a move
      recieveAIMove(); // receive human move from raspberry pi and update array for last human move
      black_player_movement();  // make the human move, checking if we need to perform a capture or not
      sequence = player_white;
      break;
  }
}
