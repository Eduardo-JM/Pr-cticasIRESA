/* Torque variables */
float voltage = 0.f;
float current = 0.f;
float rpm = 0.f;

/**
 * This method reads and counts how many times
 * the 
*/
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
  counter = counter >> 1;
  rpm = ((double) (counter * MILLI_SECONDS_IN_ONE_MINUTE)) / (INTERUPTS_PER_REVOLUTION * (time_end - time_start));
  return rpm;
}

float getTorque() {
  voltage = getVoltage(VOLTAGE_INPUT_PIN);
  current = getCurrent(CURRENT_INPUT_PIN);
  return rpm * voltage * current * M_PI / 30.0;
}