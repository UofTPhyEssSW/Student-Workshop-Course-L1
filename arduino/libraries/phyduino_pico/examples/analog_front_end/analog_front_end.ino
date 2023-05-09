/**
 * @file analog_front_end.ino
 * @date 2023-05-08
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Analog Front End demo code
 * 
 * @version 1.0.0
 * @copyright Copyright (c) 2023
 */
#include "phyduino.h"

#define SERIAL_PLOT   // Comment out to output text instead of plot format.

// Sample delays
static constexpr unsigned long sample_delay { 100 };
static unsigned long last_delay { 0 };
static constexpr unsigned long blink_delay { 400 };
static unsigned long led_red_time { };
static unsigned long led_green_time { };

// Shield IO definitions
static constexpr phyduino::pin_t encoder_sw_io    { phyduino::gpio::D2 };
static constexpr phyduino::pin_t encoder_a_io     { phyduino::gpio::D3 };
static constexpr phyduino::pin_t encoder_b_io     { phyduino::gpio::D4 };
static constexpr phyduino::pin_t led_red_io       { phyduino::gpio::D5 };
static constexpr phyduino::pin_t led_green_io     { phyduino::gpio::D6 };
static constexpr phyduino::pin_t temp_sensor_int  { phyduino::gpio::A0 };
static constexpr phyduino::pin_t temp_sensor_raw  { phyduino::gpio::A1 };
static constexpr phyduino::pin_t temp_sensor_ext  { phyduino::gpio::A3 };
static constexpr phyduino::pin_t senor_output     { phyduino::gpio::A4 };
static constexpr phyduino::pin_t potentiometer    { phyduino::gpio::A5 };

void setup() {
  // Enable Serial port and set baud
  Serial.begin(phyduino::serial_baud);
  // Initialize PCB 
  phyduino::initialize_board();

  // Rotary Encoder inputs
  pinMode(encoder_a_io, INPUT);
  pinMode(encoder_b_io, INPUT);
  pinMode(encoder_sw_io, INPUT);
  // LED outputs
  pinMode(led_red_io, OUTPUT);
  pinMode(led_green_io, OUTPUT);

  digitalWrite(led_red_io, HIGH);
  digitalWrite(led_green_io, LOW);
}

void loop() {
  phyduino::led_heart_beat();   // Blinks D8 Green LED

  if((millis() - last_delay) > sample_delay){
    // Read Analog Pins [ A0 - A5 ]
    auto a0 = phyduino::analogRead(temp_sensor_int);
    auto a1 = phyduino::analogRead(temp_sensor_raw);
    auto a2 = phyduino::analogRead(phyduino::gpio::A2); // Not connected (noise).
    auto a3 = phyduino::analogRead(temp_sensor_ext);
    auto a4 = phyduino::analogRead(senor_output);
    auto a5 = phyduino::analogRead(potentiometer);
    auto vbus = phyduino::get_vbus();
    // Convert ADC readings to Voltage
    auto v_a0 = phyduino::to_voltage(a0);
    auto v_a1 = phyduino::to_voltage(a1);
    auto v_a2 = phyduino::to_voltage(a2);
    auto v_a3 = max11635::driver::to_voltage(a3);
    auto v_a4 = max11635::driver::to_voltage(a4);
    auto v_a5 = max11635::driver::to_voltage(a5);

    #ifdef SERIAL_PLOT
      Serial.printf("A0:%.3f,A1:%.3f,A2:%.3f,A3:%.3f,A4:%.3f,A5:%.3f,VBUS:%.3f\r\n", 
                    v_a0, v_a1, v_a2, v_a3, v_a4, v_a5, vbus );
    #else
      Serial.printf("Analog Pin A0 : %.3fV [%d]\r\n", a0, v_a0); // Temperature Reading External
      Serial.printf("Analog Pin A1 : %.3fV [%d]\r\n", a1, v_a1); // Raw Sensor voltage
      Serial.printf("Analog Pin A2 : %.3fV [%d]\r\n", a2, v_a2); // Not Connected not shield
      Serial.printf("Analog Pin A3 : %.3fV [%d]\r\n", a3, v_a3); // Temperature Reading External
      Serial.printf("Analog Pin A4 : %.3fV [%d]\r\n", a4, v_a4); // External Sensor connector
      Serial.printf("Analog Pin A5 : %.3fV [%d]\r\n", a5, v_a5); // Potentiometer wiper
      Serial.printf("USB VBus voltage : %.3fV\r\n", vbus);       // Current USB VBus voltage.
    #endif
  }

  phyduino::pin_heart_beat(led_green_io, blink_delay, &led_green_time);
  phyduino::pin_heart_beat(led_red_io, blink_delay, &led_red_time);
}