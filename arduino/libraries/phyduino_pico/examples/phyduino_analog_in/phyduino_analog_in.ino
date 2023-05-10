/**
 * @file phyduino_analog_in.ino
 * @date 2023-05-08
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Phyduino Pico Analog input demo code.
 * 
 * @version 1.0.0
 * @copyright Copyright (c) 2023
 */
#include "phyduino.h"

// Internal ADC for Raspberry PI Pico : A0 - A2
// External ADC for MAX11635 ADC : A3 - A5, VBUS
#define EXTERNAL_ADC                    // Comment out for internal ADC demo
#define ARDUINO_SERIAL_PLOT             // Comment out for text output. 

static constexpr unsigned long sample_rate { 100 };
static unsigned long last_sample { };

void setup() {
  // Initialize Serial port
  Serial.begin(phyduino::serial_baud);
  // Initialize PCB.
  phyduino::initialize_board();
}

void loop() {
  phyduino::led_heart_beat();   // Blinks D8 Green LED every 400ms

  auto ctime = millis();

  if((ctime - last_sample) > sample_rate){ 
    std::uint16_t value;
    float voltage;

    #ifdef EXTERNAL_ADC // Analog input pins A3 - A5
      value   = phyduino::analog_read(phyduino::gpio::A3);
      voltage = max11635:driver::to_voltage(value);
      Serial.printf("External");
    #else // Analog input pins A0 - A2
      value   = phyduino::analog_read(phyduino::gpio::A0);
      voltage = phyduino::to_voltage(value);
      Serial.printf("Internal");
    #endif

    #ifdef ARDUINO_SERIAL_PLOT
      Serial.printf(" Voltage:%.3f,Value:%d\r\n", voltage, value);
    #else
      Serial.printf(" Analog Input :\r\n");
      Serial.printf("\tvalue = %d\r\n", value);
      Serial.printf("\tvoltage = %.3f\r\n", voltage);
    #endif
  }
}
