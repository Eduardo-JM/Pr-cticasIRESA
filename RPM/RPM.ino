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

/* RPM constanst */
// MAX 4294967295
#define RPM_SAMPLES 250000
#define RPM_INPUT_PIN 7
#define INTERUPTS_PER_REVOLUTION 4.0

/* Torque constants */
#define VOLTAGE_INPUT_PIN A0
#define CURRENT_INPUT_PIN A1
#define ARDUINO_BITS_MAXIMUM 1023
#define VOLTAGE_SAMPLES 250.0
#define ARDUINO_VOLTAGE 5

#define ACS_MAX_CURRENT 20
#define ACS_VOLTAGE_AT_NO_CHARGE 2.5f
#define ACS_SENSIVITY 0.100f


/* RPM variables */
ull time_start = 0L;
ull time_end = 0L;
bool high_state;
float rpm = 0.f;

/* Torque variables */
float torque = 0.f;
float voltage = 0.f;
float current = 0.f;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

typedef struct LCDDisplay {
  String content;
  unsigned int cursor;
} LCDDisplay;

float getVoltage(int);
float getCurrent(int);
float getRPM();
float getTorque();

void setup() {
  pinMode(D7, INPUT);
  pinMode(7, INPUT);

  Serial.begin(9600);

  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.clear();
  rpm = getRPM();
}

void loop() {
  lcd.clear();
  lcd.print(String(rpm, 2) + " rpm");
  lcd.setCursor(0, 1);
  lcd.print(String(getTorque(), 2) + " Nm");
  rpm = getRPM();
}

float getRPM(){
  ull counter = 0L;
  time_start = micros();
  high_state = digitalRead(RPM_INPUT_PIN) == HIGH;
  for (ull i = 0; i < RPM_SAMPLES; ++i) {
     if ( (digitalRead(RPM_INPUT_PIN) == HIGH) != high_state ){
       high_state = !high_state;
       ++counter;
     }
  }
  time_end = micros();
  //Serial.println(String(((double) (counter * 60000000.0)) / (INTERUPTS_PER_REVOLUTION * (time_end - time_start))) + "rpm. Time: " +  String((time_end - time_start) * 1.0) + " us" + " steps: " + String((counter * 1.0)));
  return ((double) (counter * 60000000.0)) / (INTERUPTS_PER_REVOLUTION * (time_end - time_start));
}

float getTorque() {
  voltage = getVoltage(VOLTAGE_INPUT_PIN);
  current = getCurrent(CURRENT_INPUT_PIN);
  return voltage * current * M_PI * rpm / 30.0;
}

float getVoltage(int input_pin) {
  float total = 0.f;
  for (int i = 0; i < VOLTAGE_SAMPLES; ++i ) {
    total += analogRead(input_pin);
    delay(5);
  }
  return (total * ARDUINO_VOLTAGE) / (ARDUINO_BITS_MAXIMUM * VOLTAGE_SAMPLES);
}

float getCurrent(int input_pin) {
  float voltage_ACS = getVoltage(input_pin);
  return (voltage_ACS - ACS_VOLTAGE_AT_NO_CHARGE) / (ACS_SENSIVITY);
}