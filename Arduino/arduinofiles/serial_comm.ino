#define NUM_MOVES 3

const int buttonPin = 2;
int buttonState = 0;
int i = 0;

const char *moves[NUM_MOVES];
//edit these for the moves that we are sending to Raspberry Pi
moves[0] = "e4\n";
moves[1] = "Nf3\n";
moves[2] = "Bc4\n";

void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}
void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  
  if(buttonState == HIGH){
    Serial.print(moves[i]);
    i++;
  
    while (Serial.available() <= 0) {
      
    }
    String data = Serial.readStringUntil('\n');
    // data now has the move that we have received from the AI
    // from here we have to send the data to the rest of the program to make the move on the board
    
    //Serial.print("You sent me: ");
    //Serial.println(data);

    
  }


}
