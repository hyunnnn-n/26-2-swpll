
#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

#define SND_VEL 346.0
#define INTERVAL 25
#define PULSE_DURATION 10

#define _DIST_MIN 100
#define _DIST_MAX 300

#define TIMEOUT ((INTERVAL / 2) * 1000.0)
#define SCALE (0.001 * 0.5 * SND_VEL)

#define N 30

float samples[N];
int sample_count = 0;
int next_index = 0;

unsigned long last_sampling_time = 0;

float median_filter(float new_sample)
{
  samples[next_index] = new_sample;

  next_index = (next_index + 1) % N;

  if (sample_count < N) {
    sample_count++;
  }

  float sorted[N];

  for (int i = 0; i < sample_count; i++) {
    sorted[i] = samples[i];
  }

  for (int i = 1; i < sample_count; i++) {
    float key = sorted[i];
    int j = i - 1;

    while (j >= 0 && sorted[j] > key) {
      sorted[j + 1] = sorted[j];
      j--;
    }

    sorted[j + 1] = key;
  }

  if (sample_count % 2 == 1) {
    return sorted[sample_count / 2];
  } else {
    return (sorted[sample_count / 2 - 1]
          + sorted[sample_count / 2]) / 2.0;
  }
}

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  digitalWrite(PIN_TRIG, LOW);

  Serial.begin(57600);
}

void loop()
{
  if (millis() - last_sampling_time < INTERVAL) {
    return;
  }

  last_sampling_time = millis();

  float dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);

  float dist_median = median_filter(dist_raw);

  Serial.print("raw:");
  Serial.print(dist_raw);

  Serial.print("\tmedian:");
  Serial.println(dist_median);

  if (dist_raw < _DIST_MIN || dist_raw > _DIST_MAX) {
    digitalWrite(PIN_LED, HIGH);
  } else {
    digitalWrite(PIN_LED, LOW);
  }
}

float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}
