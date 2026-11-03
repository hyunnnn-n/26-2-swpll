#include <Servo.h>

#define PIN_SERVO 10

#define _DUTY_MIN 553     
#define _DUTY_MAX 2399   
#define _SERVO_SPEED 3   
#define INTERVAL 20      

unsigned long last_sampling_time;
Servo myservo;
float duty_change_per_interval;   
float duty_curr;                

void setup() {
  myservo.attach(PIN_SERVO);
  duty_curr = _DUTY_MIN;
  myservo.writeMicroseconds(duty_curr);


  duty_change_per_interval =
    (float)(_DUTY_MAX - _DUTY_MIN) * (_SERVO_SPEED / 180.0) * (INTERVAL / 1000.0);

  last_sampling_time = millis();
}

void loop() {
  if (millis() < last_sampling_time + INTERVAL)
    return;

  if (duty_curr < _DUTY_MAX) {
    duty_curr += duty_change_per_interval;
    if (duty_curr > _DUTY_MAX)
      duty_curr = _DUTY_MAX;
  }

  myservo.writeMicroseconds((int)duty_curr);

  last_sampling_time += INTERVAL;
}
