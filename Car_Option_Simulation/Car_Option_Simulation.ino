/* 이 코드는 자동차 옵션 시뮬레이션으로 다음의 기능을 구현 합니다.
  1. 조도에 따른 LED, RGB 자동 밝기 조절
  2. 방향지시등 및 비상 깜빡이 
  3. Water Sensor 감지로 와이퍼 동작 및 감지값에 따른 자동 속도 조절 */

  // ----------LIGHT SENSOR-----------
const uint8_t LIGHT_SENSOR {A0};   //라이트 센서 ANALOG PIN A0
const uint8_t LEDS[] {2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U};  //LEDS PWM PIN
enum RGB { RED = 13, GREEN = 12, BLUE = 11 };  //RGB PWM PIN

  // ----------Vehicle Blink-----------
const uint8_t LEFT_LED {24U};  //LEFT_LED DIGITAL PIN
const uint8_t RIGHT_LED {26U};  //RIGHT_LED DIGITAL PIN
const uint8_t RELAY_SWITCH {32U};  //RELAY_SWITCH DIGITAL PIN
const uint8_t BUTTON_SWITCH {28U}; //BUTTON_SWITCH DIGITAL PIN
  // ----------Wiper WaterSensor-----------
const uint8_t WATER_SENSOR {A8};  //WATER_SENSOR ANALOG PIN A8
const uint8_t SERVO_MOTOR {10U};  //SERVO_MOTOR PWM PIN

void setup() {
  // ----------LIGHT SENSOR-----------
  Serial.begin(115200UL);
  pinMode(LIGHT_SENSOR, INPUT); 
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  for(auto&& i : LEDS) {
    pinMode(i, OUTPUT);
  }                            
  // ----------Vehicle Blink-----------
  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
  pinMode(RELAY_SWITCH, OUTPUT);
  pinMode(BUTTON_SWITCH, INPUT);   

  // ----------Wiper WaterSensor-----------
  pinMode(WATER_SENSOR, INPUT);
  pinMode(SERVO_MOTOR, OUTPUT);
}
void loop() {
  // ----------LIGHT SENSOR-----------
  uint16_t light_value = analogRead(LIGHT_SENSOR);  //라이트센서 값 읽어오기
  light_value = constrain(light_value, 200, 850);  //조도 최소값 200, 최대값 850 지정
  Serial.print(F("Light value : "));
  Serial.println(light_value);  //출력값 화면 표시
  for(int index = 0; index < 8; ++index) {
    uint16_t brightness = map(light_value, 200, 850, 0, 255);  // 지정한 조도값에 따른 LED,RGB 밝기 조절값 0 ~ 255 mapping
    digitalWrite(LEDS[index], brightness);
    analogWrite(RED, brightness);
    analogWrite(GREEN, brightness);
    analogWrite(BLUE, brightness);
  }
  delay(100UL);  // 딜레이 0.1초 
  // ----------Vehicle Blink-----------
  static uint8_t button_count {0u};
  static bool button_switch_state {false};
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RIGHT_LED, LOW);
  digitalWrite(RELAY_SWITCH, LOW);  //초기화

  if(!digitalRead(BUTTON_SWITCH)) {
    Serial.println(F("BUTTON SWITCH ON"));
    ++button_count;
    if(button_count == 3) button_count = 0; // 3번 카운드되면 0으로
    if(button_count % 3 == 1) {
      digitalWrite(LEFT_LED, HIGH);
      Serial.println(F("LEFT LED ON"));
      for(uint8_t i {0u}; i < 5; ++i) {
        digitalWrite(RELAY_SWITCH, HIGH);
        delay(500UL);
        digitalWrite(RELAY_SWITCH, LOW);
        delay(500UL);
      }
    } else if(button_count % 3 == 2) {
      digitalWrite(RIGHT_LED, HIGH);
      Serial.println(F("RIGHT LED ON"));
      for(uint8_t i {0u}; i < 5; ++i) {
        digitalWrite(RELAY_SWITCH, HIGH);
        delay(500UL);
        digitalWrite(RELAY_SWITCH, LOW);
        delay(500UL);
     }
    } else {
      digitalWrite(LEFT_LED, HIGH);
      digitalWrite(RIGHT_LED, HIGH);
      Serial.println(F("EMERGENCY LED ON"));
      for(uint8_t i {0u}; i < 5; ++i) {
        digitalWrite(RELAY_SWITCH, HIGH);
        delay(500UL);
        digitalWrite(RELAY_SWITCH, LOW);
        delay(500UL);
      }
    }
  }
  // ----------Wiper WaterSensor-----------
  uint16_t water_value = analogRead(WATER_SENSOR);
  water_value = constrain(water_value, 0, 60);
  uint16_t mapped_value = map(water_value, 0, 60, 100, 10); // 0일때는 천천히 650가까울때는 빨리
    if(water_value > 5) { //비를 감지
      for(int i = 0; i < 256; i += 10) {
        analogWrite(SERVO_MOTOR, i);
        delay(mapped_value);
      }
    } 
  Serial.print(F("Water value : "));
  Serial.println(water_value);
  delay(100UL); // 0.1초 딜레이
}
