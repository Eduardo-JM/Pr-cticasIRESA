#include <LiquidCrystal.h>
#include <math.h>

typedef unsigned long long ull;

/* LCD constants */
#define RS 12
#define EN 11
#define D4 5
#define D5 4
#define D6 3
#define D7 2
#define LCD_COLUMNS 16
#define LCD_ROWS 2

/* System */

#define RESISTANCE 1030000.0
#define VOLTAGE_INPUT_PIN A0
#define CURRENT_INPUT_PIN_1 A0
#define CURRENT_INPUT_PIN_2 A2
#define TEMPERATURE_INPUT_PIN A3
#define ARDUINO_BITS_MAXIMUM 1023
#define VOLTAGE_SAMPLES 50.0
#define ARDUINO_VOLTAGE 5


LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

typedef struct LCDDisplay {
  String content;
  unsigned int cursor;
} LCDDisplay;

float getVoltage(int);
float getCurrent(int, int);
float getTemperature(int);

void setup() {
  pinMode(D7, INPUT);
  pinMode(7, INPUT);

  Serial.begin(9600);

  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.clear();
}

void loop() {
  /*lcd.clear();
  lcd.print(String(getVoltage(VOLTAGE_INPUT_PIN), 2) + " V ");
  lcd.print(String(getTemperature(TEMPERATURE_INPUT_PIN), 1) + " C");
  lcd.setCursor(0, 1);
  lcd.print(String(getCurrent(CURRENT_INPUT_PIN_1, CURRENT_INPUT_PIN1_2), 3) + " mA");*/
  Serial.println(String(getVoltage(VOLTAGE_INPUT_PIN), 2) + " V ");
  Serial.println(String(getTemperature(TEMPERATURE_INPUT_PIN), 1) + " C");
  Serial.println(String(getCurrent(CURRENT_INPUT_PIN_1, CURRENT_INPUT_PIN_2) * 1000, 3) + " mA");
  delay(500);
}

float getVoltage(int input_pin) {
  float total = 0.f;
  for (int i = 0; i < VOLTAGE_SAMPLES; ++i ) {
    total += analogRead(input_pin);
    delay(5);
  }
  return (total * ARDUINO_VOLTAGE) / (ARDUINO_BITS_MAXIMUM * VOLTAGE_SAMPLES);
}

float getCurrent(int input_pin_1, int input_pin_2) {
  float voltage_1 = getVoltage(input_pin_1);
  float voltage_2 = getVoltage(input_pin_2);
  return (voltage_1 - voltage_2) / RESISTANCE;
}

float getTemperature(int input_pin) {
  float voltage = (getVoltage(VOLTAGE_INPUT_PIN) * 100.0) * (ARDUINO_VOLTAGE/ARDUINO_BITS_MAXIMUM);
}