
#include <Servo.h>

#define PIN_LED   9   
#define PIN_TRIG  12  
#define PIN_ECHO  13 
#define PIN_SERVO 10  


#define LED_ACTIVE_LOW 1


#define SND_VEL 346.0       
#define INTERVAL 25      
#define PULSE_DURATION 10   
#define _DIST_MIN 180.0    
#define _DIST_MAX 360.0   
#define TIMEOUT ((INTERVAL / 2) * 1000UL) 
#define SCALE (0.001 * 0.5 * SND_VEL)    
#define _EMA_ALPHA 0.3     


#define _DIST_SERVO_LOW  180.0
#define _DIST_SERVO_HIGH 360.0

float dist_ema = _DIST_MAX;
float dist_prev = _DIST_MAX;
unsigned long last_sampling_time = 0;
Servo myservo;


void setLED(bool on) {
#if LED_ACTIVE_LOW
  digitalWrite(PIN_LED, on ? LOW : HIGH);
#else
  digitalWrite(PIN_LED, on ? HIGH : LOW);
#endif
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
  setLED(false); 

  myservo.attach(PIN_SERVO);
  myservo.write(180);

  Serial.begin(57600);
}

void loop() {
  if (millis() - last_sampling_time < INTERVAL) {
    return;
  }
  last_sampling_time = millis();

  float dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);
  float dist_filtered;
  bool in_range = (dist_raw >= _DIST_MIN) && (dist_raw <= _DIST_MAX);


  if (in_range) {
    dist_filtered = dist_raw;
    dist_prev = dist_raw;
  } else {
    dist_filtered = dist_prev; 
  }

  
  dist_ema = _EMA_ALPHA * dist_filtered + (1.0 - _EMA_ALPHA) * dist_ema;


  int servo_angle;
  if (dist_ema <= _DIST_SERVO_LOW) {
    servo_angle = 0;
  } else if (dist_ema >= _DIST_SERVO_HIGH) {
    servo_angle = 180;
  } else {
    servo_angle = 30 + (int)((dist_ema - _DIST_SERVO_LOW) * 120.0 /
                             (_DIST_SERVO_HIGH - _DIST_SERVO_LOW) + 0.5);
  }
  myservo.write(servo_angle);


  setLED(in_range);


  Serial.print("Min:");    Serial.print(_DIST_MIN);
  Serial.print(",dist:");  Serial.print(dist_raw);
  Serial.print(",ema:");   Serial.print(dist_ema);
  Serial.print(",Servo:"); Serial.print(myservo.read());
  Serial.print(",Max:");   Serial.println(_DIST_MAX);
}


float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}
