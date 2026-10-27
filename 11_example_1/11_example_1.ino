
#include <Servo.h>

// Arduino pin assignment
#define PIN_LED   9   // LED active-low
#define PIN_TRIG  12  // sonar sensor TRIGGER
#define PIN_ECHO  13  // sonar sensor ECHO
#define PIN_SERVO 10  // servo motor

// LED 극성 설정: active-low이면 1, active-high이면 0
#define LED_ACTIVE_LOW 1

// Sonar settings
#define SND_VEL 346.0       // sound velocity (m/sec)
#define INTERVAL 25         // sampling interval (msec)
#define PULSE_DURATION 10   // trigger pulse duration (usec)
#define _DIST_MIN 180.0     // minimum valid distance: 18 cm (mm)
#define _DIST_MAX 360.0     // maximum valid distance: 36 cm (mm)
#define TIMEOUT ((INTERVAL / 2) * 1000UL) // echo timeout (usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // duration to distance (mm)
#define _EMA_ALPHA 0.3      // EMA weight for new sample

// Servo mapping thresholds (mm)
#define _DIST_SERVO_LOW  180.0
#define _DIST_SERVO_HIGH 360.0

float dist_ema = _DIST_MAX;
float dist_prev = _DIST_MAX;
unsigned long last_sampling_time = 0;
Servo myservo;

// LED 켜기/끄기 (극성 자동 처리)
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
  setLED(false); // 시작 시 LED 꺼짐

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

  // Range filter: accept only measurements from 18 cm through 36 cm.
  if (in_range) {
    dist_filtered = dist_raw;
    dist_prev = dist_raw;
  } else {
    dist_filtered = dist_prev; // reject invalid reading and hold last valid value
  }

  // Exponential moving average
  dist_ema = _EMA_ALPHA * dist_filtered + (1.0 - _EMA_ALPHA) * dist_ema;

  // 18 cm or less: 0 degrees; 18-36 cm: 30-150 degrees; 36 cm or more: 180.
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

  // 유효 범위(18~36cm) 안이면 LED 켜고, 벗어나면 끔
  setLED(in_range);

  // Serial Plotter format
  Serial.print("Min:");    Serial.print(_DIST_MIN);
  Serial.print(",dist:");  Serial.print(dist_raw);
  Serial.print(",ema:");   Serial.print(dist_ema);
  Serial.print(",Servo:"); Serial.print(myservo.read());
  Serial.print(",Max:");   Serial.println(_DIST_MAX);
}

// Get a distance reading from USS; returns millimeters.
float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}
