// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

// configurable parameters
#define SND_VEL 346.0
#define INTERVAL 25
#define PULSE_DURATION 10

#define _DIST_MIN 100.0
#define _DIST_MAX 300.0

#define TIMEOUT ((INTERVAL / 2.0) * 1000.0)
#define SCALE (0.001 * 0.5 * SND_VEL)

unsigned long last_sampling_time;

void setup() {

  last_sampling_time = millis();

  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  digitalWrite(PIN_TRIG, LOW);

  // LED OFF
  analogWrite(PIN_LED, 255);

  // initialize serial port
  Serial.begin(115200);
}

void loop() {

  float distance;
  int led_value;

  // wait until next sampling time
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  // measure distance
  distance = USS_measure(PIN_TRIG, PIN_ECHO);

  // 100 ~ 300 mm에서 LED 밝기 조절
  if ((distance == 0.0) || (distance >= _DIST_MAX)) {

    // 300mm 이상 또는 측정 실패
    // LED OFF
    led_value = 255;

  }
  else if (distance <= _DIST_MIN) {

    // 100mm 이하
    // LED OFF
    led_value = 255;

  }
  else if (distance <= 200.0) {

    // 100 ~ 200mm
    // 100mm : OFF
    // 200mm : MAX

    led_value = (int)(255.0 * (200.0 - distance) / 100.0);

  }
  else {

    // 200 ~ 300mm
    // 200mm : MAX
    // 300mm : OFF

    led_value = (int)(255.0 * (distance - 200.0) / 100.0);
  }

  // LED 밝기 설정
  analogWrite(PIN_LED, led_value);

  // Serial 출력
  Serial.print("Min:");
  Serial.print(_DIST_MIN);

  Serial.print(",distance:");
  Serial.print(distance);

  Serial.print(",Max:");
  Serial.print(_DIST_MAX);

  Serial.print(",LED:");
  Serial.println(led_value);

  // update last sampling time
  last_sampling_time += INTERVAL;
}


// 초음파 센서 거리 측정
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}
