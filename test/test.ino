/*
  https://www.arduino.cc/en/tutorial/button
  https://www.arduino.cc/en/reference/servo
  https://www.arduino.cc/en/Tutorial/Knob
  https://www.pololu.com/product/948
*/
#include <Servo.h>

const int BUTTON_PIN = 2;
const int SERVO_PIN = 9;
const int PAUSE_DURATION = 200;
const int RUN_DURATION = 50;

long triggeredAt = 0;  // timestamp of last mode switch
long debounce = 200;   // the debounce time

bool isOnRandomMode = false;

Servo servo;

void setup() {
  servo.attach(SERVO_PIN);
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);
}

void randomMode() {
  // Rotate forward at full speed
  // for RUN_DURATION amount of time
  servo.write(180);
  delay(RUN_DURATION);
  servo.write(90);

  // Wait for PAUSE_DURATION amount of time
  delay(PAUSE_DURATION);
  
  // Rotate forward at full speed
  // for RUN_DURATION amount of time
  servo.write(180);
  delay(RUN_DURATION);
  servo.write(90);

  // Wait for PAUSE_DURATION amount of time
  delay(PAUSE_DURATION);
}

void controlledMode() {
  // Stop the rotation (shouldn't be necessary)
  servo.write(90);

  // Wait for PAUSE_DURATION amount of time
  delay(PAUSE_DURATION * 3);
}

void loop() {
  // Read the state of the pushbutton value:
  if (digitalRead(BUTTON_PIN) == HIGH && (!triggeredAt || millis() - triggeredAt > debounce)) {
    triggeredAt = millis();
    
    // Switch mode
    isOnRandomMode = !isOnRandomMode;
    Serial.print("Mode switched, isOnRandomMode: ");
    Serial.println(isOnRandomMode);
  }

  if (isOnRandomMode) {
    Serial.println("Starting randomMode.");
    randomMode();
    Serial.println("Finished randomMode.");
  } else {
    Serial.println("Starting controlledMode.");
    controlledMode();
    Serial.println("Finished controlledMode.");
  }
}
