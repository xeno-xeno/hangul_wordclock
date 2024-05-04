#include <avr/sleep.h>            // sleep 모드 라이브러리
#include <Adafruit_NeoPixel.h>    // LED SK6812 제어 라이브러리
#include <Wire.h>           // I2C 라이브러리 포함
#include <RTClib.h>         // RTC 라이브러리 포함

// SK6812 LED 설정
#define LED_PIN 7
#define NUM_LEDS 25    // LED 스트립에 있는 LED 개수 조정
#define BRIGHTNESS 15  // LED 밝기 설정 (0-255)

#define INTERRUPT_PIN 2  // sleep 모드 탈출을 위한 인터럽트 핀을 #2로 정의

// RGBW 스트립을 위한 Adafruit_NeoPixel 객체 생성
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);

// DS3231 RTC 객체 생성
RTC_DS3231 rtc; 

// hour_positions 배열의 선언 및 초기화를 수정합니다.
int hour_positions[12][3] = {
  { 1, 14, -1 },   // 1시
  { 9, 14, -1 },   // 2시
  { 3, 14, -1 },   // 3시
  { 4, 14, -1 },   // 4시
  { 2, 7, 14 },    // 5시
  { 8, 7, 14 },    // 6시
  { 6, 5, 14 },    // 7시
  { 10, 11, 14 },  // 8시
  { 12, 13, 14 },  // 9시
  { 0, 14, -1 },   // 10시
  { 0, 1, 14 },    // 11시
  { 0, 9, 14 },    // 12시 (자정과 정오 모두 이 위치 사용)
};


// 분 표시에 사용할 LED 위치 배열
int minute_positions[13][4] = {
  { -1, -1, -1, -1 },  // 0분
  { 23, 24, -1, -1 },  // 5분
  { 15, 24, -1, -1 },  // 10분
  { 15, 23, 24, -1 },  // 15분
  { 17, 22, 24, -1 },  // 20분
  { 17, 22, 23, 24 },  // 25분
  { 16, 15, 24, -1 },  // 30분
  { 16, 15, 23, 24 },  // 35분
  { 20, 15, 24, -1 },  // 40분
  { 20, 22, 23, 24 },  // 45분
  { 21, 22, 24, -1 },  // 50분
  { 21, 22, 23, 24 }   // 55분
};

volatile bool rtcAlarmFired = false;  // RTC 알람 플래그

void setup() {
  strip.begin();                    // NeoPixel 시작
  strip.show();                     // 모든 픽셀을 꺼짐 상태로 초기화

  Serial.begin(9600);  // 시리얼 통신 시작

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // 컴파일 시간으로 시간 설정
    // rtc.adjust(DateTime(2024, 5, 1, 12, 15, 0)); // 특정 시간으로 설정 (예: 2024년 5월 1일 12시 0분 0초)
  }

    pinMode(INTERRUPT_PIN, INPUT_PULLUP);  // D2 핀을 입력 풀업으로 설정
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), wakeUp, FALLING); // 인터럽트 설정
}

void show_time(int h, int m) {

    // 밝기 설정
    if (h >= 23 || h < 6) {
        strip.setBrightness(5);  // 저녁 11시부터 아침 6시까지 밝기 5
    } else {
        strip.setBrightness(15); // 그 외 시간에는 밝기 15
    }  

    // 12시간 형식으로 변환
    int hour12 = h % 12;
    hour12 = hour12 == 0 ? 12 : hour12;
    int hourIndex = hour12 - 1;

    // 5분 단위로 시간 업데이트를 확인
    int minuteIndex = m / 5;

    strip.clear();  // LED 스트립을 초기화

    // 시간에 따른 LED 활성화
    for (int i = 0; i < 3 && hour_positions[hourIndex][i] != -1; i++) {
        strip.setPixelColor(hour_positions[hourIndex][i], strip.Color(0, 0, 0, 50));
    }
  // 분 표시
  if (minuteIndex >= 0) {
    for (int i = 0; i < 4 && minute_positions[minuteIndex][i] != -1; i++) {
      // 분을 표시하는 LED를 흰색으로 설정
      strip.setPixelColor(minute_positions[minuteIndex][i], strip.Color(0, 0, 0, 50));
    }
  }
  // 자정과 정오에 추가로 표시할 LED 설정
  if (m <= 15) { // 분(m)이 15분 이내 일때만 실행
    if (h == 0) { // 자정
      strip.setPixelColor(18, strip.Color(255, 255, 80, 0));
      strip.setPixelColor(19, strip.Color(255, 255, 80, 0));
    } else if (h == 12) { // 정오
      strip.setPixelColor(18, strip.Color(50, 255, 255, 0));
      strip.setPixelColor(21, strip.Color(50, 255, 255, 0));
    }
  }

  // LED 스트립 업데이트
  strip.show();
}


void loop() {
    DateTime now = rtc.now();    // 현재 시간 정보를 얻음    
    show_time(now.hour(), now.minute()); // 시간 표시 (루프 시작 시 항상 실행)


    // 현재 시간이 5분 단위가 아니라면 다음 5분 단위까지 기다림
    if (now.minute() % 5 != 0) {
        unsigned long waitTime = (5 - now.minute() % 5) * 60000 - now.second() * 1000;

        delay(waitTime);  // 다음 5분 단위까지 대기
    }

    // 다시 현재 시간을 업데이트 하고 시간 표시 (슬립 모드 진입 전에 다시 실행)
    now = rtc.now();
    show_time(now.hour(), now.minute());


    // 알람 설정
    rtc.disableAlarm(1);  // 알람 비활성화
    rtc.clearAlarm(1);    // 알람 상태 초기화
    DateTime future = rtc.now() + TimeSpan(0, 0, 5, 0);  // 5분 후 알람
    rtc.setAlarm1(future, DS3231_A1_Minute);  // 5분 후 정확한 시간에 알람 설정
    rtc.writeSqwPinMode(DS3231_OFF);  // 모든 출력 비활성화
  
    enterSleepMode();      // 슬립 모드로 진입
      
}

// 대기 모드로 진입하는 함수
void enterSleepMode() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // 최대 전원 절약 모드 선택
  sleep_enable(); // 대기 모드 활성화
  sleep_cpu(); // 대기 모드 진입
  // 대기 모드에서 깨어나면 이후 코드가 실행됨
  sleep_disable(); // 대기 모드 비활성화
}

void wakeUp() {
    rtcAlarmFired = true; // 인터럽트에 의해 슬립 모드에서 깨어남
    detachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN)); // 인터럽트 비활성화
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), wakeUp, FALLING); // 인터럽트 재설정
}