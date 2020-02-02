/*
  https://www.arduino.cc/en/Tutorial/Knob
  https://www.pololu.com/product/948
*/
#include <Servo.h>

const int SERVO_PIN = 9;
const int PERIOD = 500;
const int MAX_RUNTIME = 20000;

Servo servo;
int currentSpeed = 0;
long lastUpdated = 0;
bool stopped = false;

void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
}

void loop() {
  if (stopped) return;
  
  if (millis() > MAX_RUNTIME) {
    servo.write(90);
    stopped = true;
    return;
  }
  
  if (millis() - lastUpdated > PERIOD){
    lastUpdated = millis();
    Serial.println(currentSpeed);
    servo.write(currentSpeed);
    
    if (currentSpeed == 180){
      currentSpeed = 0;
    } else{
      currentSpeed = 180;
    }
  }
}
