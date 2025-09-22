// 실습 3: GPIO 7번 LED 제어
const uint8_t LED_PIN = 7;

const unsigned long INITIAL_ON_MS = 1000;   // 처음 1초 켜기
const uint8_t BLINK_COUNT = 5;              // 다음 1초 동안 5회 깜빡
const unsigned long BLINK_TOTAL_MS = 1000;  // 깜빡이는 총 시간 1초

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // 시작은 꺼진 상태
}

void loop() {
  // 1) 처음 1초 동안 켜기
  digitalWrite(LED_PIN, HIGH);
  delay(INITIAL_ON_MS);

  // 2) 다음 1초 동안 5회 깜빡이기 (ON/OFF 각 100ms)
  const unsigned long halfPeriod = BLINK_TOTAL_MS / (BLINK_COUNT * 2); // 100ms
  for (uint8_t i = 0; i < BLINK_COUNT+1; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(halfPeriod);
    digitalWrite(LED_PIN, LOW);
    delay(halfPeriod);
  }

  // 3) LED 끄고 무한루프(종료)
  digitalWrite(LED_PIN, LOW);
  while (1) { /* infinite loop */ }
}
