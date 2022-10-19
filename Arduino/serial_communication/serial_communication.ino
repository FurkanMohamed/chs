const int buttonPin = 2;
int buttonState = 0;

// used only in this dummy program
int counter = 0;
const char *moves[3] = {"e4", "Nf3", "Bb5"};


void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // get move from array of predetermined moves used for testing
  String _move = moves[counter];

  if(buttonState == HIGH){
    // send the Raspberry Pi the move that was made on the board
    Serial.print(_move);

    while (Serial.available() <= 0){
      // spin while we wait for the AI moves to come back
    }
    String data = Serial.readStringUntil('\n');

    // only print when we are debugging
    // Serial.print("You sent me: ");
    // Serial.println(data);

    // can remove once we actually get a real move from the board
    counter++;
    
  }

}
