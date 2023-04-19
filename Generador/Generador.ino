#include <math.h>
#include <LiquidCrystal.h>

/* LCD constants */
#define RS 7
#define EN 6
#define D4 5
#define D5 4
#define D6 3
#define D7 2
#define LCD_COLUMNS 16
#define LCD_ROWS 2
#define ARDUINO_BITS_MAXIMUM 1023
#define ARDUINO_VOLTAGE 5
#define RPM_INPUT_PIN 8

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

typedef enum current_type {
  AC,
  DC  
} CurrentType;

typedef enum phases {
  SINGLE_PHASE = 1,
  BIPHASIC,
  THREE_PHASE
} Phases;

typedef unsigned long long ull;

typedef struct LCDDisplay {
  String content;
  unsigned int cursor;
} LCDDisplay;

typedef struct RunMethods {
  bool voltage = false;
  bool current = false;
  bool power = false;
  bool frequency = false;
  bool rpm = false;
  bool torque = false;
} RunMethods;

typedef struct InputPins {
  int voltage_pin;
  int current_pin;
  int frequency_pin;
  struct InputPins* next = NULL;
  struct InputPins* previous = NULL;
} InputPins;

InputPins* head = NULL;
InputPins* cursor = NULL;

/* RPM variables */
ull time_start = 0L;
ull time_end = 0L;
bool high_state;

float getVoltage(int);
float getCurrent(int);
float getRPM();
float getTorque();
//String formatFloatingPointNumber(float);

void setup() {
  pinMode(D7, INPUT);
  pinMode(7, INPUT);

  Serial.begin(9600);

  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.clear();

  Phases phases = SINGLE_PHASE;
  CurrentType currentType = DC;

  RunMethods runMethods;

  runMethods.frequency = runMethods.frequency && currentType != DC;
  runMethods.power = runMethods.power && (runMethods.current && runMethods.voltage);
  runMethods.torque = runMethods.power && runMethods.rpm;

  int current_analog_pin = A0;
  int current_digital_pin = 8;
  
  head = (InputPins* ) calloc(1, sizeof(InputPins));
  cursor = head;

  for (unsigned int i = 0; i < phases; ++i) {
    cursor->voltage_pin = current_analog_pin++;
    cursor->current_pin = current_analog_pin++;
    cursor->frequency_pin = current_digital_pin++;
  }
  
}

void loop() {

  for (unsigned int i = 0; i < phases; ++i) {
    lcd.print("Midiendo fase: " + String(i));
  }
}



