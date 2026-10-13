//Cosine Function

#include <Servo.h>
#include <math.h>

#define PIN_SERVO 10

Servo myServo;

unsigned long MOVING_TIME = 3000; 
unsigned long moveStartTime;

int startAngle = 30; 
int stopAngle  = 90; 

void setup() {
  myServo.attach(PIN_SERVO);

  myServo.write(startAngle); 
  delay(500);

  moveStartTime = millis(); 
}

void loop() {
  unsigned long progress = millis() - moveStartTime;

  if (progress <= MOVING_TIME) {
   
    float t = (float)progress / MOVING_TIME;

   
    float eased = (1.0 - cos(PI * t)) / 2.0;

   
    float angle = startAngle
                + (stopAngle - startAngle) * eased;

    myServo.write((int)(angle + 0.5));
  } else {
   
    int temp = startAngle;
    startAngle = stopAngle;
    stopAngle = temp;

    moveStartTime = millis(); 
  }
}

#include <Servo.h>
#include <math.h>

#define PIN_SERVO 10

Servo myServo;

unsigned long MOVING_TIME = 3000; 
unsigned long moveStartTime;

int startAngle = 30; 
int stopAngle  = 90;

void setup() {
  myServo.attach(PIN_SERVO);

  myServo.write(startAngle); 
  delay(500);

  moveStartTime = millis(); 
}

void loop() {
  unsigned long progress = millis() - moveStartTime;

  if (progress <= MOVING_TIME) {
    
    float t = (float)progress / MOVING_TIME;

    float eased = (1.0 - cos(PI * t)) / 2.0;

    float angle = startAngle
                + (stopAngle - startAngle) * eased;

    myServo.write((int)(angle + 0.5));
  } else {
    int temp = startAngle;
    startAngle = stopAngle;
    stopAngle = temp;

    moveStartTime = millis(); 
  }
}



//Sigmoid Function

#include <Servo.h>
#include <math.h>

#define PIN_SERVO 10

Servo myServo;

unsigned long MOVING_TIME = 3000; 
unsigned long moveStartTime;

int startAngle = 30;
int stopAngle  = 90; 

void setup() {
  myServo.attach(PIN_SERVO);

  myServo.write(startAngle); 
  delay(500);

  moveStartTime = millis(); 
}

void loop() {
  unsigned long progress = millis() - moveStartTime;

  if (progress <= MOVING_TIME) {
    
    float t = (float)progress / MOVING_TIME;

   
    float x = 12.0 * t - 6.0;
    float sigmoid = 1.0 / (1.0 + exp(-x));


    float s0 = 1.0 / (1.0 + exp(6.0));
    float s1 = 1.0 / (1.0 + exp(-6.0));
    float eased = (sigmoid - s0) / (s1 - s0);

   
    float angle = startAngle
                + (stopAngle - startAngle) * eased;

    myServo.write((int)(angle + 0.5));
  } else {
   
    int temp = startAngle;
    startAngle = stopAngle;
    stopAngle = temp;

    moveStartTime = millis();
  }
}
