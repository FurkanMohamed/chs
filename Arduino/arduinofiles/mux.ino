#include <Wire.h>

int reed_sensor_status [8][8];
int reed_sensor_record [8][8];
int reed_sensor_status_memory [8][8];
int reed_sensor;

byte reed_colone[2];
byte reed_line[2];

char mov [4] = {0, 0, 0, 0};

//  Multiplexer
const byte MUX_ADDR [4] = {A3, A2, A1, A0};
const byte MUX_SELECT [4] = {9, 8, 7, 6}; //controls which mux we are getting input from
const byte MUX_OUTPUT (12);
const byte MUX_CHANNEL[16][4] = {
  {0, 0, 0, 0},
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {1, 1, 0, 0},
  {0, 0, 1, 0},
  {1, 0, 1, 0},
  {0, 1, 1, 0},
  {1, 1, 1, 0},
  {0, 0, 0, 1},
  {1, 0, 0, 1},
  {0, 1, 0, 1},
  {1, 1, 0, 1},
  {0, 0, 1, 1},
  {1, 0, 1, 1},
  {0, 1, 1, 1},
  {1, 1, 1, 1}
};

void setup() {
  Serial.begin(9600);

  //  Multiplexer
  for (byte i = 0; i < 4; i++) {
    pinMode (MUX_ADDR [i], OUTPUT);
    digitalWrite(MUX_ADDR [i], LOW);
    pinMode (MUX_SELECT [i], OUTPUT);
    digitalWrite(MUX_SELECT [i], HIGH);
  }
  pinMode (MUX_OUTPUT, INPUT_PULLUP);

  //  Set the reed switches status
  //this line was edited from initial program
  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      reed_sensor_status[i][j] = 1;
      reed_sensor_status_memory[i][j] = 1;
    }
  }

}

//************************  DETECT HUMAN MOVEMENT
void detect_human_movement() {

  //  Record the reed switches status
  byte column = 6;
  byte row = 0;

  for (byte i = 0; i < 4; i++) {
    digitalWrite(MUX_SELECT[i], LOW);
    for (byte j = 0; j < 16; j++) {
      for (byte k = 0; k < 4; k++) {
        digitalWrite(MUX_ADDR [k], MUX_CHANNEL [j][k]);
      }
      reed_sensor_record[column][row] = digitalRead(MUX_OUTPUT);
      // added in the below line
      /*if (reed_sensor_record[column][row] == 0){
        Serial.print("Detected in row ");
        Serial.print(row);
        Serial.print("column ");
        Serial.println(column);
      }*/
      
      row++;
      if (j == 7) {
        column++;
        row = 0;
      }
    }
    for (byte l = 0; l < 4; l++) {
      digitalWrite(MUX_SELECT[l], HIGH);
    }
    if (i == 0) column = 4;
    if (i == 1) column = 2;
    if (i == 2) column = 0;
    row = 0;
  }
  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      reed_sensor_status_memory[7 - i][j] = reed_sensor_record[i][j];
    }
  }
  
  //  Compare the old and new status of the reed switches

  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      if (reed_sensor_status[i][j] != reed_sensor_status_memory[i][j]) {
        if (reed_sensor_status_memory[i][j] == 1) { // if no piece present
          reed_colone[0] = i; // column the piece came from
          reed_line[0] = j; // row the peice came from
        }
        if (reed_sensor_status_memory[i][j] == 0) { // if piece now present
          reed_colone[1] = i; // column that piece was moved to
          reed_line[1] = j; // row that piece was moved to
        }
      }
    }
  }
  //  Set the new status of the reed sensors
  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      reed_sensor_status[i][j] = reed_sensor_status_memory[i][j];
    }
  }
}

//**************************  PLAYER DISPLACEMENT
void player_displacement() {

  //  Convert the reed sensors switches coordinates in characters
  char table1[] = {'8', '7', '6', '5', '4', '3', '2', '1'};
  char table2[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

  mov[0] = table2[reed_line[0]]; // row the peice came from
  mov[1] = table1[reed_colone[0]]; // column the piece came from
  mov[2] = table2[reed_line[1]]; // row that piece was moved to
  mov[3] = table1[reed_colone[1]]; // column that piece was moved to
}

void loop() {
  detect_human_movement();
  player_displacement();
  Serial.println(mov);
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      Serial.print(reed_sensor_status_memory[j][i]);
    }
    Serial.println("");
  }
  Serial.println("--------");
  //Serial.println("Read reed switches. Printing values that have a valid piece");
  /*
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if (reed_sensor_status_memory[i][j] == 0){
        Serial.print("i:");
        Serial.print(i);
        Serial.print(" j:");
        Serial.print(j);
        Serial.print("\n");
      }
    }
  }
  */

  //test for one reed sensor
  //output 1 when nothing there, 0 when something there
  //reed_sensor = digitalRead(13);
  //Serial.println(reed_sensor);

  //test pins one by one
  /*
  digitalWrite(9, HIGH); //mux select to low (read this select pin) (green enable)
  digitalWrite(8, HIGH); //mux select to high (no read) (Brown enable)
  digitalWrite(7, HIGH); //mux select to high (no read)  (purple enable)
  digitalWrite(6, LOW); //mux select to high (no read)  (yellow enable)

  digitalWrite(A3, LOW); //LSB (add 1)
  digitalWrite(A2, LOW); // (add 2)
  digitalWrite(A1, LOW); // (add 4)
  digitalWrite(A0, LOW); //MSB (add 8)
  

  reed_sensor = digitalRead(MUX_OUTPUT);
  Serial.println(reed_sensor);
  */
  delay(2000); //wait for every 5 seconds
}
