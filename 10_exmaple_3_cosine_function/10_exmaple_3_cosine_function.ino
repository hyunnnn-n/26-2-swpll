
#include <Servo.h>
#include <math.h>

#define PIN_SERVO 10

Servo myServo;

unsigned long MOVING_TIME = 3000; // moving time is 3 seconds
unsigned long moveStartTime;

int startAngle = 30; // 30°
int stopAngle  = 90; // 90°

void setup() {
  myServo.attach(PIN_SERVO);

  myServo.write(startAngle); // Set position
  delay(500);

  moveStartTime = millis(); // Start moving
}

void loop() {
  unsigned long progress = millis() - moveStartTime;

  if (progress <= MOVING_TIME) {
    // Normalize progress to 0.0 ~ 1.0
    float t = (float)progress / MOVING_TIME;

    // Cosine function for smooth movement
    float eased = (1.0 - cos(PI * t)) / 2.0;

    // Calculate servo angle
    float angle = startAngle
                + (stopAngle - startAngle) * eased;

    myServo.write((int)(angle + 0.5));
  } else {
    // Reverse direction
    int temp = startAngle;
    startAngle = stopAngle;
    stopAngle = temp;

    moveStartTime = millis(); // Reset start time
  }
}

#include <Servo.h>
#include <math.h>

#define PIN_SERVO 10

Servo myServo;

unsigned long MOVING_TIME = 3000; // moving time is 3 seconds
unsigned long moveStartTime;

int startAngle = 30; // 30°
int stopAngle  = 90; // 90°

void setup() {
  myServo.attach(PIN_SERVO);

  myServo.write(startAngle); // Set position
  delay(500);

  moveStartTime = millis(); // Start moving
}

void loop() {
  unsigned long progress = millis() - moveStartTime;

  if (progress <= MOVING_TIME) {
    // Normalize progress to 0.0 ~ 1.0
    float t = (float)progress / MOVING_TIME;

    // Cosine function for smooth movement
    float eased = (1.0 - cos(PI * t)) / 2.0;

    // Calculate servo angle
    float angle = startAngle
                + (stopAngle - startAngle) * eased;

    myServo.write((int)(angle + 0.5));
  } else {
    // Reverse direction
    int temp = startAngle;
    startAngle = stopAngle;
    stopAngle = temp;

    moveStartTime = millis(); // Reset start time
  }
}
