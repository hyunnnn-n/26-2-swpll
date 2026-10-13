//Cosine Function

#include <Servo.h>
#include <math.h>

#define PIN_SERVO 10
#define PIN_TRIG  12
#define PIN_ECHO  13

Servo myServo;


const float CLOSE_DISTANCE = 150.0; 
const unsigned long MOVING_TIME = 3000; 

const int DOWN_ANGLE = 30; 
const int UP_ANGLE = 90;  

unsigned long moveStartTime = 0;

int startAngle = DOWN_ANGLE;
int stopAngle = DOWN_ANGLE;

bool isMoving = false;
bool barrierUp = false;

void setup() {
  myServo.attach(PIN_SERVO);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  myServo.write(DOWN_ANGLE);
  delay(500);
}

void loop() {
  
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  unsigned long duration = pulseIn(PIN_ECHO, HIGH, 30000);
  float distance = duration * 0.346 / 2.0;

 
  bool shouldBeUp =
      (duration != 0 && distance <= CLOSE_DISTANCE);

  
  if (shouldBeUp != barrierUp && !isMoving) {
    startAngle = barrierUp ? UP_ANGLE : DOWN_ANGLE;
    stopAngle = shouldBeUp ? UP_ANGLE : DOWN_ANGLE;

    moveStartTime = millis();
    isMoving = true;
    barrierUp = shouldBeUp;
  }


  if (isMoving) {
    unsigned long progress = millis() - moveStartTime;

    if (progress >= MOVING_TIME) {
      myServo.write(stopAngle);
      isMoving = false;
    } else {
      
      float t = (float)progress / MOVING_TIME;

      
      float eased = (1.0 - cos(PI * t)) / 2.0;

     
      float angle = startAngle
                  + (stopAngle - startAngle) * eased;

      myServo.write((int)(angle + 0.5));
    }
  }

  delay(20);
}



//Sigmoid Function

#include <Servo.h>
#include <math.h>


#define PIN_SERVO 10
#define PIN_TRIG  12
#define PIN_ECHO  13

Servo myServo;


const float CLOSE_DISTANCE = 150.0; 
const unsigned long MOVING_TIME = 3000; 

const int DOWN_ANGLE = 30; 
const int UP_ANGLE = 90;  

unsigned long moveStartTime = 0;

int startAngle = DOWN_ANGLE;
int stopAngle = DOWN_ANGLE;

bool isMoving = false;
bool barrierUp = false;

void setup() {
  myServo.attach(PIN_SERVO);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  myServo.write(DOWN_ANGLE);
  delay(500);
}

void loop() {
  
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  unsigned long duration = pulseIn(PIN_ECHO, HIGH, 30000);
  float distance = duration * 0.346 / 2.0;

  
  bool shouldBeUp =
      (duration != 0 && distance <= CLOSE_DISTANCE);

  
  if (shouldBeUp != barrierUp && !isMoving) {
    startAngle = barrierUp ? UP_ANGLE : DOWN_ANGLE;
    stopAngle = shouldBeUp ? UP_ANGLE : DOWN_ANGLE;

    moveStartTime = millis();
    isMoving = true;
    barrierUp = shouldBeUp;
  }

  
  if (isMoving) {
    unsigned long progress = millis() - moveStartTime;

    if (progress >= MOVING_TIME) {
      myServo.write(stopAngle);
      isMoving = false;
    } else {
     
      float t = (float)progress / MOVING_TIME;

    
      float s = 1.0 / (1.0 + exp(-12.0 * (t - 0.5)));

    
      float s0 = 1.0 / (1.0 + exp(6.0));
      float s1 = 1.0 / (1.0 + exp(-6.0));
      float eased = (s - s0) / (s1 - s0);

     
      float angle = startAngle
                  + (stopAngle - startAngle) * eased;

      myServo.write((int)(angle + 0.5));
    }
  }

  delay(20);
}
