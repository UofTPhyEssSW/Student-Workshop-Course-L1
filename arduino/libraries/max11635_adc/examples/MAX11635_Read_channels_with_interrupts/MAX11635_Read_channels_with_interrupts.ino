/**
 * @file MAX11635_Read_channels_with_interrupts.ino
 * @date 2023-10-04
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Example for how to interface with MAX11635 ADC using interrupts.
 * 
 * @note Example is for the Phyduino PICO Rev C board, can be modified for other boards.
 * 
 * @version 1.0.0
 * @copyright Copyright (c) 2023
 */
#include "max11635.h"

#define ARDUINO_SERIAL_PLOTTER  // Comment out for text output instead of Serial plotter format.

// Phyduino Pico Rev C GPIO assignments (Change accoding to board being used.)
static constexpr std::uint32_t LED_IO       { 5 };
static constexpr std::uint32_t ADC_MOSI_IO  { 23 };
static constexpr std::uint32_t ADC_MISO_IO  { 20 };
static constexpr std::uint32_t ADC_SCK_IO   { 22 };
static constexpr std::uint32_t ADC_CS_IO    { 21 };
static constexpr std::uint32_t ADC_nCNST_IO { 18 };
static constexpr std::uint32_t ADC_nEOC_IO  { 19 };

static constexpr unsigned long COMM_BAUD_RATE { 115'200 };
static constexpr unsigned long COMM_DELAY { 2 };

// LED blink variables.
static unsigned long last_time { 0UL };
static constexpr unsigned long blink_delay { 200UL };

// Sample Buffer
max11635::driver::data_type Sample [max11635::driver::max_channels] { };    // Sample buffer, 1 sample per channel.
std::uint8_t i;                                                             // Sample current index variable.

static bool Conversion_Flag { false };                                      // When true indicates a sample is ready to be read.
static bool ADC_Busy { false };                                             // Whne true indicates the ADC is busy reading a channel.

void ISR_AnalogConversion() noexcept;                                       // nEOC interrupt handler function.

/**
 * @brief Arduino Setup function.
 * @details
 * This function initializes the GPIO pins, MAX11635 driver, and the end
 * of conversion (nEOC) pin interrupt. Global variables i, conversion flag, and 
 * adc busy are set to there default values.
 */
void setup() {
    // Initalize USB Serial port. (default BAUD: 115,200B)
    Serial.begin(COMM_BAUD_RATE);
    // Set board LED GPIO initial mode and state
    pinMode(LED_IO, OUTPUT);
    digitalWrite(LED_IO, LOW);

    // Configure MAX11635 ADC GPIO's, must be called before begin()!
    MAX11635_ADC.configure_io(ADC_MOSI_IO, ADC_MISO_IO, ADC_SCK_IO, ADC_CS_IO, ADC_nCNST_IO, ADC_nEOC_IO);
    
    // Initialize MAX11635 ADC (Default SPI clock frequency 4.8MHz)
    MAX11635_ADC.begin();

    // Initialize Global variables
    i = 0;
    Conversion_Flag = false;
    ADC_Busy = false;
  
    // Enabling GPIO interrupt for the nEOC pin and attaching a ISR Handler function (ISR_AnalogConversion)
    attachInterrupt(digitalPinToInterrupt(ADC_nEOC_IO), ISR_AnalogConversion, LOW);
}

/**
 * @brief Arduino Main loop.
 * @details
 * This is a simple program to demostrate how to use the MAX11635 driver to read 
 * channel voltages using the interrupt function of the Arduino IDE compatible 
 * board. The function starts by blinking the LED ON and OFF, the is a blocking 
 * delay of 200ms total. Next the function starts an ADC conversion on a selected
 * channel only if the ADC is not busy with a conversion already. When the ADC 
 * conversion starts the ADC_Busy flag is set to true to indicate a conversion is
 * on going. When the ADC indicates the conversion is complete the nEOC is set to
 * LOW and the interrupt is triggered. Once the interrupt is complete, the 
 * conversion flag is set to true and the new data is processed. After all 4 
 * channels are sampled the loop outputs the values to the USB Virtual COM port
 * and the process begins again. 
 */
void loop() {
  // Blink Built-in LED ON and OFF
  auto current_time = millis();
  if((current_time-last_time) >= blink_delay){
    digitalWrite(LED_IO, (digitalRead(LED_IO) == LOW? HIGH : LOW));
    last_time = current_time;
  }

  if(!ADC_Busy) {
    MAX11635_ADC.set_channel(i);                          // Set ADC channel to sample.
    MAX11635_ADC.start_conversion();                      // Start Conversion.
    ADC_Busy = true;                                      // Set Busy flag to indicate ADC is in the middle of conversion.
  }
  
  if(ADC_Busy && Conversion_Flag){
    ADC_Busy = false;                                     // ADC Busy Flag false to indicate the conversion and read out are complete.
    Conversion_Flag = false;                              // Set conversion flag to false. This indicates that the sample read has been processed.

    if(i == max11635::driver::max_channels - 1) {         // Output Data once all 4 channels are read.
        #ifdef ARDUINO_SERIAL_PLOTTER
            Serial.printf("CH%d:%d,CH%d:%d,CH%d:%d,CH%d:%d\r\n", 0, Sample[0], 1, Sample[1], 2, Sample[2], i, Sample[i]);
        #else
            Serial.printf("CH%d:%d\r\n", 0, Sample[0]);
            Serial.printf("CH%d:%d\r\n", 1, Sample[1]);
            Serial.printf("CH%d:%d\r\n", 2, Sample[2]);
            Serial.printf("CH%d:%d\r\n", i, Sample[i]);
        #endif
        delay(COMM_DELAY);                                // Delay to prevent buffer overrun on the Serial class.
        i = 0;                                            // Set channel counter back to 0.
    } else {
        i++;                                              // Increment channel counter for next sample.
    }
  }
}

/**
 * @brief Interrupt Handler for nEOC pin being LOW.
 * @details
 * When the nEOC pin of the MAX11635 is logic LOW this function is called by
 * interrupt service routine. It clears the interrupt by reading the ADC data
 * through the SPI bus. Once read the ADC data is stored in the Sample buffer
 * and the conversion flag is set to true to indicate new data is available in
 * the buffer.
 */
void ISR_AnalogConversion() noexcept {
  Sample[i] = MAX11635_ADC.read_conversion();   // Read ADC data through SPI
  Conversion_Flag = true;                       // Set conversion flag to true.
}
