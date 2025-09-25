// Arduino pin assignment
#define PIN_LED   9
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25       // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300.0   // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // coefficent to convert duration to distance

unsigned long last_sampling_time;   // unit: msec
float distance = _DIST_MAX;

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 

  // initialize serial port
  Serial.begin(115200);
  last_sampling_time = millis();   // 초기화
}

void loop() { 
  // wait until next sampling time. (polling 방식)
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  // 거리 측정
  distance = USS_measure(PIN_TRIG, PIN_ECHO);

  // 범위를 벗어나면 보정
  float d = distance;
  if (d < _DIST_MIN) d = _DIST_MIN;
  if (d > _DIST_MAX) d = _DIST_MAX;

  // 밝기 계산 (삼각형 패턴)
  // 200mm에서 pwm=0 (최대 밝기), 100/300mm에서 pwm=255 (꺼짐), 150/250mm에서 pwm≈127 (50%)
  int pwm = (int)round(255.0 * fabs(d - 200.0) / 100.0);
  pwm = constrain(pwm, 0, 255);

  analogWrite(PIN_LED, pwm);  // active-low PWM 제어

  // 시리얼 출력 (디버깅용)
  Serial.print("Min:");        Serial.print(_DIST_MIN);
  Serial.print(",distance:");  Serial.print(distance);
  Serial.print(",Max:");       Serial.print(_DIST_MAX);
  Serial.print(",PWM:");       Serial.print(pwm);
  Serial.println("");

  // 샘플링 시간 갱신
  last_sampling_time += INTERVAL;
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
