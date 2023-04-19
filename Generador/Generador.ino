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
  bool voltage = true;
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

/* RPM variables */
ull time_start = 0L;
ull time_end = 0L;
bool high_state;

float getVoltage(int);
float getCurrent(int);
float getRPM();
float getTorque();
//String formatFloatingPointNumber(float);

Phases phases;
RunMethods runMethods;
InputPins* inputPins = NULL;

void setup() {
  pinMode(D7, INPUT);
  pinMode(7, INPUT);

  Serial.begin(9600);

  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.clear();

  phases = BIPHASIC;
  CurrentType currentType = DC;

  runMethods.frequency = runMethods.frequency && currentType != DC;
  runMethods.power = runMethods.power && (runMethods.current && runMethods.voltage);
  runMethods.torque = runMethods.power && runMethods.rpm;

  int current_analog_pin = A0;
  int current_digital_pin = 8;

  inputPins = (InputPins* ) calloc(1, sizeof(InputPins));

  for (unsigned int i = 0; i < phases;) {
    inputPins->voltage_pin = current_analog_pin++;
    inputPins->current_pin = current_analog_pin++;
    inputPins->frequency_pin = current_digital_pin++;
    if (++i < phases) {
      inputPins->next = (InputPins* ) calloc(1, sizeof(InputPins));
      inputPins->next->previous = inputPins;
      inputPins = inputPins->next;
    }
  }
  
}

void loop() {
  while (inputPins->previous != NULL) {
    inputPins = inputPins->previous;
  }

  for (unsigned int i = 0; i < phases; ++i) {
    lcd.print("Midiendo fase: " + String(i));
    String text = "";
    if (runMethods.voltage){
      text += String(getVoltage(inputPins->voltage_pin), 2) + " V; ";
    }
    lcd.setCursor(0,1);
    lcd.print(text);
    if (inputPins->next == NULL) {
      break;
    }
    inputPins = inputPins->next;
    delay(2000);
    lcd.clear();
  }
}



