const byte BUTTON_WHITE_SWITCH_MOTOR_WHITE (11);
const byte BUTTON_BLACK_SWITCH_MOTOR_BLACK (10);
enum {WHITE, BLACK};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode (BUTTON_WHITE_SWITCH_MOTOR_WHITE, INPUT_PULLUP);
  pinMode (BUTTON_BLACK_SWITCH_MOTOR_BLACK, INPUT_PULLUP);
}

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


void loop() {
  // put your main code here, to run repeatedly:
  byte buttonState = digitalRead(BUTTON_BLACK_SWITCH_MOTOR_BLACK);
  
  if (buttonState == LOW) {
      Serial.println("Button is pressed");
  }
  else {
      Serial.println("Button is not pressed");
  }
  delay(100);
  
}
