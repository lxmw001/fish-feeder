#include <Servo.h>

Servo servofeeder;

int minPosition = 30; //30
int maxPosition = 130; //130
// 28 only to tests
// 28 to test feed for one week
int times = 2; // replace to 1 by the moment
int servoSpeed = 50;

void setup() {
  // put your setup code here, to run once:
  servofeeder.attach(2);
  servofeeder.write(minPosition);
  Serial.begin(9600);
}

void loop() {
  feed();
  delay(hoursToMiliseconds(6));
}

void feed() {
  for (int i = 0; i < times; i++) {
    goUp();
    goDown();
    delay(1000);
  }
}

void goUp() {
  for (int i = minPosition; i <= maxPosition; i += 10) {
    Serial.print("up");
    Serial.println(i);
    servofeeder.write(i);
    delay(servoSpeed);
  }
}

void goDown() {
  for (int i = maxPosition; i >= minPosition; i -= 10) {
    Serial.print("down");
    Serial.println(i);
    servofeeder.write(i);
    delay(servoSpeed);
  }
}

int hoursToMiliseconds(int hours) {
  return hours * 1000 * 60 * 60;
}
