#include <math.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

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

/* RPM constants */
#define RPM_INPUT_PIN 8 // MAX 4294967295
#define RPM_SAMPLES 125000
#define INTERUPTS_PER_REVOLUTION 1.0
#define MILLI_SECONDS_IN_ONE_MINUTE 60000000

/* Frequency constants */
#define FREQUENCY_INPUT_PIN 9 // MAX 4294967295
#define FREQUENCY_SAMPLES 100000

/* Keypad constants */
#define PAD_ROWS 4
#define PAD_COLS 4
#define R1 (byte) 23
#define R2 (byte) 25
#define R3 (byte) 27
#define R4 (byte) 29
#define C1 (byte) 31
#define C2 (byte) 33 
#define C3 (byte) 35
#define C4 (byte) 37

const char keyboard[PAD_ROWS][PAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte row_pins[PAD_ROWS] = {R1, R2, R3, R4}; 
byte col_pins[PAD_COLS] = {C1, C2, C3, C4};


/* Intances */
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
Keypad keypad = Keypad(makeKeymap(keyboard), row_pins, col_pins, PAD_ROWS, PAD_COLS); 

typedef unsigned long long ull;

typedef struct LCDDisplay {
  String content;
  unsigned int cursor;
} LCDDisplay;

/* RPM variables */
ull time_start = 0L;
ull time_end = 0L;
bool high_state;
char event = NO_KEY;
char event_before = NO_KEY;

float getVoltage(int);
float getCurrent(int);
float getRPM();
float getTorque();
void printMenu();

void setup() {
  pinMode(D7, INPUT);
  pinMode(RPM_INPUT_PIN, INPUT);

  Serial.begin(9600);

  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.clear();

  printMenu();
}

void loop() {

  ull start = millis();
  
  if (event_before != NO_KEY) {
    lcd.print("[E]");
    lcd.setCursor(13,1);
  }
  while ((millis() - start) < 1000) {
    event = keypad.getKey();
    if (event != NO_KEY)
      break;
  }

  if (event == NO_KEY)
    event = event_before;
  
  switch (event){
    case '1':
      lcd.clear();
      lcd.print("Leyendo pin " + String(FREQUENCY_INPUT_PIN));
      lcd.setCursor(0,1);
      lcd.print(String(getFrequency(FREQUENCY_INPUT_PIN), 2) + " Hz");
      break;
    case '2':
      lcd.clear();
      lcd.print("Leyendo pin " + String(RPM_INPUT_PIN));
      lcd.setCursor(0,1);
      lcd.print(String(getRPM(), 2) + " rpm");
      break;
    default:
      printMenu();
      break;
  }

  switch (event){
    case '1':
    case '2':
      event_before = event;
      delay(1000);
      break;
    default:
      event_before = NO_KEY;
      break;
  }
}

void printMenu() {
  lcd.clear();
  lcd.print("Selecciona:");
  lcd.setCursor(0,1);
  lcd.print("1: freq; 2 rpm;");
}
