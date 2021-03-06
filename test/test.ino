/*
  https://www.arduino.cc/en/tutorial/button
  https://www.arduino.cc/en/reference/servo
  https://www.arduino.cc/en/Tutorial/Knob
  https://www.pololu.com/product/948
*/
#include <Servo.h>

const int MODE_BUTTON_PIN = 2;
const int PUSH_BUTTON_PIN = 3;
const long DEBOUNCE = 200;  // the debounce time for the mode-switch button

const int SERVO_PIN = 9;
const int CR_SERVO_PIN = 10;

const int CR_PERIOD = 1500;

const int DEFAULT_POSITION = 90;
const int PERIOD = 100;
const int MIN_POSITION = 60;
const int MAX_POSITION = 120;

long triggeredAt = 0;     // timestamp of the last mode switch
int angularPosition = 0;  // angular position of the shaft in degrees

bool isOnRandomMode = false;
Servo servo;
Servo CRservo;

/*
 * s0: idle
 * -- button push --> activate
 * --> s1: active
 * -- been running for longer than CR_PERIOD --> deactivate
 * --> s0
 */
int CRServoState = 0;     
long activatedAt = 0;   // timestamp of the last activation

void setup() {
  Serial.begin(9600);
  
  servo.attach(SERVO_PIN);
  pinMode(PUSH_BUTTON_PIN, INPUT);
  pinMode(MODE_BUTTON_PIN, INPUT);  
  servo.write(DEFAULT_POSITION);

  CRservo.attach(CR_SERVO_PIN); 
}

void randomMode() {
  angularPosition = random(MIN_POSITION, MAX_POSITION);
  servo.write(angularPosition);
  Serial.println(angularPosition);
  delay(PERIOD);
}

void controlledMode() {
  if (servo.read() != DEFAULT_POSITION) {
    servo.write(DEFAULT_POSITION);
    Serial.println(DEFAULT_POSITION);
  }
}

void activateCRServo() {
  Serial.println("activateCRServo()");
  CRServoState = 1;
  CRservo.write(0);
  activatedAt = millis();
}

void deactivateCRServo() {
  Serial.println("deactivateCRServo()");
  CRServoState = 0;
  CRservo.write(90);
}

void CRServoStateMachine() {
  switch(CRServoState) {
    case 0:
      // Read the state of the push button value:
      if (digitalRead(PUSH_BUTTON_PIN) == HIGH) {
        activateCRServo();
      }
      break;
    case 1:
      // Check if the CR servo has been running for longer than CR_PERIOD
      if (millis() > activatedAt) {
        deactivateCRServo();
      }
      break;
    default:
      Serial.println("Uh oh, unknown CRServo state: " + CRServoState);
  }
  
}

void loop() {
  CRServoStateMachine();
  
  // Read the state of the mode-switch button value:
  if (digitalRead(MODE_BUTTON_PIN) == HIGH && (!triggeredAt || millis() - triggeredAt > DEBOUNCE)) {
    triggeredAt = millis();
    
    // Switch mode
    isOnRandomMode = !isOnRandomMode;
    Serial.print("Mode switched, isOnRandomMode: ");
    Serial.println(isOnRandomMode);
  }

  if (isOnRandomMode) {
    randomMode();
  } else {
    controlledMode();
  }
}