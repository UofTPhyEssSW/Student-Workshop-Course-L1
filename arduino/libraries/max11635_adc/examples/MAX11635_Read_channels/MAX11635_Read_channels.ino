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

#define ARDUINO_SERIAL_PLOTTER  // Comment out for text output instead of Serial plotter format.
#define GET_ADC_COUNT           // Comment out to get the converted voltage value.

// Phyduino Pico Rev C GPIO assignments (Change accoding to board being used.)
static constexpr std::uint32_t LED_IO       { 5 };
static constexpr std::uint32_t ADC_MOSI_IO  { 23 };
static constexpr std::uint32_t ADC_MISO_IO  { 20 };
static constexpr std::uint32_t ADC_SCK_IO   { 22 };
static constexpr std::uint32_t ADC_CS_IO    { 21 };
static constexpr std::uint32_t ADC_nCNST_IO { 18 };
static constexpr std::uint32_t ADC_nEOC_IO  { 19 };

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
    Sample[i] = MAX11635_ADC.analogRead(i);

    #ifdef ARDUINO_SERIAL_PLOTTER
    if(i == max11635::driver::max_channels - 1){
      #ifdef GET_ADC_COUNT
        Serial.printf("CH[%d]:0x%04x\r\n", i, Sample[i]);
      #else
        Serial.printf("CH[%d]:%.3f\r\n", i, max11635::driver::to_voltage(Sample[i]));
      #endif
    } else{
      #ifdef GET_ADC_COUNT
        Serial.printf("CH[%d]:0x%04x,", i, Sample[i]);
      #else
        Serial.printf("CH[%d]:%.3f,", i, max11635::driver::to_voltage(Sample[i]));
      #endif
    }
    #else
    // Read Analog value
    Serial.printf("Channel[%d] : %.3fV [0x%04x]\r\n", i, max11635::driver::to_voltage(Sample[i]), Sample[i]);
    #endif
  }

  delay(200);
  digitalWrite(LED_IO, LOW);
}