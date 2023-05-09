/**
 * @file MAX11635_Read_channels.ino
 * @date 2023-03-30
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Example for how to interface with MAX11635 ADC
 * 
 * @note Example is for the Phyduino PICO Rev C board, can be modified for other boards.
 * 
 * @version 1.0.0
 * @copyright Copyright (c) 2023
 */
#include "max11635.h"

// Phyduino Pico Rev C GPIO assignments (Change accoding to board being used.)
static constexpr pin_t LED_IO       { 4 };
static constexpr pin_t ADC_MOSI_IO  { 23 };
static constexpr pin_t ADC_MISO_IO  { 20 };
static constexpr pin_t ADC_SCK_IO   { 22 };
static constexpr pin_t ADC_CS_IO    { 21 };
static constexpr pin_t ADC_nCNST_IO { 18 };
static constexpr pin_t ADC_nEOC_IO  { 19 };

// Sample Buffer
max11635::driver::data_type Sample [max11635::driver::max_channels] { };

void setup() {
  Serial.begin();
  // Set board LED initialize state
  pinMode(LED_IO, OUTPUT);
  digitalWrite(LED_IO, LOW);
  // Configure MAX11635 ADC GPIO's, must be called before begin()!
  MAX11635_ADC.configure_io(ADC_MOSI_IO, ADC_MISO_IO, ADC_SCK_IO, ADC_CS_IO, ADC_nCNST_IO, ADC_nEOC_IO);
  // Initialize MAX11635 ADC (Default SPI clock frequency 4.8MHz)
  MAX11635_ADC.begin();
}

void loop() {
  delay(200);
  digitalWrite(LED_IO, HIGH);

  // Read channels
  for(std::size_t i = 0; i < max11635::driver::max_channels; i++){
    // Read Analog value
    Sample[i] = MAX11635_ADC.analogRead(i);
    Serial.printf("Channel[%d] : %.3fV [0x%04x]\r\n", i, max11635::driver::to_voltage(Sample[i]), Sample[i]);
  }

  delay(200);
  digitalWrite(LED_IO, LOW);
}