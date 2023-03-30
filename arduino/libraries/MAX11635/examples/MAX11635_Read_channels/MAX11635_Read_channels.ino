/**
 * @file MAX11635_Read_channels.ino
 * @date 2023-03-30
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Example for how to interface with MAX11635 ADC
 * 
 * @note Example is for the Phyduino PICO Rev C board.
 * 
 * @version 1.0.0
 * @copyright Copyright (c) 2023
 */
#include "max11635.h"

// SPI clock frequency
constexpr unsigned long SPI_freq { 4'800'000 }; // 4.8MHz SCK frequency
// Phyduino Pico Rev C GPIO assignments
constexpr pin_size_t LED_IO       { 4 };
constexpr pin_size_t ADC_MOSI_IO  { 23 };
constexpr pin_size_t ADC_MISO_IO  { 20 };
constexpr pin_size_t ADC_SCK_IO   { 22 };
constexpr pin_size_t ADC_CS_IO    { 21 };
constexpr pin_size_t ADC_nCNST_IO { 18 };
constexpr pin_size_t ADC_nEOC_IO  { 19 };

// MAX11635 ADC interface
ADC_MAX11635 ExtADC { SPI, SPI_freq };
// Sample Buffer
ADC_MAX11635::data_type Sample [ADC_MAX11635::CHANNELS] { };

void setup() {
  Serial.begin();
  // Set board LED initialize state
  pinMode(LED_IO, OUTPUT);
  digitalWrite(LED_IO, LOW);
  // Configure MAX11635 ADC GPIO's, must be called before begin()!
  ExtADC.configure_io(ADC_MOSI_IO, ADC_MISO_IO, ADC_SCK_IO, ADC_CS_IO, ADC_nCNST_IO, ADC_nEOC_IO);
  // Initialize MAX11635 ADC's
  ExtADC.begin();
}

void loop() {
  delay(200);
  digitalWrite(LED_IO, HIGH);

  // Read channels
  for(std::size_t i = 0; i < ADC_MAX11635::CHANNELS; i++){
    // Read Analog value
    Sample[i] = ExtADC.analogRead(i);
    Serial.printf("Channel[%d] : %.2fV [0x%04x]", i, Sample[i], ADC_MAX11635::to_voltage(Sample[i]));
    Serial.println();
  }

  delay(200);
  digitalWrite(LED_IO, LOW);
}