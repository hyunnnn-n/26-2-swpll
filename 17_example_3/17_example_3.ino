#include <Servo.h>


#define PIN_IR     A0
#define PIN_LED    9
#define PIN_SERVO  10

#define _DUTY_MIN  861     
#define _DUTY_NEU  1476    
#define _DUTY_MAX  2194   


#define _DIST_MIN  100.0f
#define _DIST_MAX  250.0f


#define EMA_ALPHA  0.2f


#define LOOP_INTERVAL  20UL

Servo myservo;

unsigned long last_loop_time = 0;
float dist_ema = 0.0f;
bool ema_init = false;

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  myservo.attach(PIN_SERVO);
  myservo.writeMicroseconds(_DUTY_NEU);

  Serial.begin(115200);   

  last_loop_time = millis();
}

void loop()
{
  unsigned long time_curr = millis();

  if (time_curr - last_loop_time < LOOP_INTERVAL)
    return;
  last_loop_time = time_curr;

  
  float a_value = analogRead(PIN_IR);
  float dist_raw;

  if (a_value > 9.5f)
    dist_raw = ((6762.0f / (a_value - 9.0f)) - 4.0f) * 10.0f;
  else
    dist_raw = _DIST_MAX + 1.0f;  

  
  bool in_range = (dist_raw >= _DIST_MIN && dist_raw <= _DIST_MAX);
  digitalWrite(PIN_LED, in_range ? HIGH : LOW);

  float dist_clamped = dist_raw;
  if (dist_clamped < _DIST_MIN) dist_clamped = _DIST_MIN;
  if (dist_clamped > _DIST_MAX) dist_clamped = _DIST_MAX;

  
  if (!ema_init) {
    dist_ema = dist_clamped;
    ema_init = true;
  } else {
    dist_ema = EMA_ALPHA * dist_clamped + (1.0f - EMA_ALPHA) * dist_ema;
  }

  
  float duty_f = _DUTY_MIN
               + (dist_ema - _DIST_MIN) * (float)(_DUTY_MAX - _DUTY_MIN)
               / (_DIST_MAX - _DIST_MIN);

  int duty = (int)(duty_f + 0.5f);
  if (duty < _DUTY_MIN) duty = _DUTY_MIN;
  if (duty > _DUTY_MAX) duty = _DUTY_MAX;

  myservo.writeMicroseconds(duty);

  
  float angle = (duty - _DUTY_MIN) * 180.0f / (float)(_DUTY_MAX - _DUTY_MIN);

  
  Serial.print("DIST_MIN:");  Serial.print(_DIST_MIN);
  Serial.print(",IR:");       Serial.print(a_value);
  Serial.print(",dist_raw:"); Serial.print(dist_raw);
  Serial.print(",ema:");      Serial.print(dist_ema);
  Serial.print(",servo_deg:");Serial.print(angle);
  Serial.print(",DIST_MAX:"); Serial.print(_DIST_MAX);
  Serial.println();
}
