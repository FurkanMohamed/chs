#include <Wire.h>

//  Electromagnet
#define MAGNET 13

void setup() {
  Serial.begin(9600);
  pinMode (MAGNET, OUTPUT);
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
  Serial.println("ON");
  electromagnet(true);
  delay(5000);
  Serial.println("OFF");
  electromagnet(false);
  delay(5000);
}
