/**
 * @file phyduino_analog_in.ino
 * @date 2023-05-08
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Phyduino Pico Template Code
 * 
 * @version 1.0.0
 * @copyright Copyright (c) 2023
 */
#include "phyduino.h"

// Internal ADC for Raspberry PI Pico : A0 - A2
// External ADC for MAX11635 ADC : A3 - A5, VBUS
#define EXTERNAL_ADC  // Comment out for internal ADC demo

void setup() {
  // Initialize Serial port
  Serial.begin(phyduino::serial_baud);
  // Initialize PCB.
  phyduino::initialize_board();
}

void loop() {
  phyduino::led_heart_beat();   // Blinks D8 Green LED every 200ms

  #ifdef EXTERNAL_ADC // Analog input pins A3 - A5
    std::uint16_t value = phyduino::analogRead(phyduino::gpio::A3);
    float voltage = max11635:driver::to_voltage(value);
    Serial.printf("External ");
  #else // Analog input pins A0 - A2
    std::uint16_t value = phyduino::analogRead(phyduino::gpio::A0);
    float voltage = phyduino::to_voltage(value);
    Serial.printf("Internal ");
  #endif

  Serial.printf("Analog Input :\r\n");
  Serial.printf("\tvalue = %d\r\n", value);
  Serial.printf("\tvoltage = %.3f\r\n", voltage);

  delay(100);
}
