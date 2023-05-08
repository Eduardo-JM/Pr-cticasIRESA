#define VOLTAGE_SAMPLES 2000.0
#define VOLTAGE_INPUT_PIN A0
#define CURRENT_INPUT_PIN A1

#define ACS_MAX_CURRENT 20
#define ACS_VOLTAGE_AT_NO_CHARGE 2.49965f
#define ACS_SENSIVITY 0.100f

/**
 * This method read {@code VOLTAGE_SAMPLES} samples
 * of the specified pin
 * @param input_pin analog pin from which voltage will be read
 * @return voltage samples mean
*/
float getVoltage(int input_pin) {
  ull total = 0L;
  for (int i = 0; i < VOLTAGE_SAMPLES; ++i ) {
    total += analogRead(input_pin);
    delay(2);
  }
  return (total * ARDUINO_VOLTAGE) / (ARDUINO_BITS_MAXIMUM * VOLTAGE_SAMPLES);
}

/**
 * This method uses {@link #getVoltage() getVoltage}
 * method to read and then parse the current of the specified
 * pin using ACS712 20A formula
 * @param input_pin analog pin from which voltage will be parsed
 * to current using
 * @return current samples mean
*/
float getCurrent(int input_pin) {
  float voltage_ACS = getVoltage(input_pin);
  return (voltage_ACS - ACS_VOLTAGE_AT_NO_CHARGE) / (ACS_SENSIVITY);
}

/**
 * This method read {@code VOLTAGE_SAMPLES} samples
 * of the specified pin
 * @param input_pin analog pin from which voltage will be read
 * @return voltage samples mean
*/
float getFrequency(int input_pin){
  ull counter = 0L;
  time_start = micros();
  high_state = digitalRead(input_pin) == HIGH;
  for (ull i = 0; i < FREQUENCY_SAMPLES; ++i) {
     if ( (digitalRead(FREQUENCY_INPUT_PIN) == HIGH) != high_state ){
       high_state = !high_state;
       ++counter;
     }
  }
  time_end = micros();
  counter>>1;
  return ((double) (counter * 1000000.0)) / (time_end - time_start);
}