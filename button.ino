const int buttonPin = 2;
const int ledPin =  13;

long triggeredAt = 0;  // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

bool isOnModeOne = true;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // read the state of the pushbutton value:
  int buttonState= digitalRead(buttonPin);
  if (buttonState == HIGH && (!triggeredAt || millis() - triggeredAt > debounce)) {
    triggeredAt = millis();
    digitalWrite(ledPin, HIGH);
    
    // Switch mode
    isOnModeOne = !isOnModeOne;
    Serial.print("isOnModeOne :");
    Serial.println(isOnModeOne);
  }
}
