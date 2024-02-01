/**
 * @file demo.ino
 * @date 2023-05-10
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief 
 * 
 * @version 1.0.0
 * @copyright Copyright (c) 2023
 */
#include "afe_shield.h"
#include <RotaryEncoder.h>  // https://github.com/mathertel/RotaryEncoder

// #define USE_INTERNAL_ADC            // Uses the Microcontroller ADC, COMMENT OUT for external ADC measurement.
#define ARUDUINO_SERIAL_PLOT        // Serial output is for Arduino Serial Plotter. COMMENT OUT for serial text output.
// Selects which channel the temperature sensor is read from.
static constexpr uint8_t READ_TEMP_FROM_INT   { 0 };
static constexpr uint8_t READ_TEMP_FROM_EXT   { 1 };
// Time period constant values in milliseconds.
static constexpr unsigned long sample_rate  { 100 };    // Samples Analog Channels every 100ms
static constexpr unsigned long blink_rate   { 400 };    // Blinks LED's every 400ms.
// Last action time value variables
static unsigned long t_sample { };                      // Sample time variable
static unsigned long last_blink_green { };              // LED Green time variable
static unsigned long last_blink_red { };                // LED Red time variable
// Analog channel selection variable be Temperature. 
//  - If JP2 is set to INT set to READ_TEMP_FROM_INT (default)
//  - If JP2 is set to EXT set to READ_TEMP_FROM_EXT 
static uint8_t temp_ch_sel { READ_TEMP_FROM_INT };

RotaryEncoder *encoder { nullptr };

/*
 * Interrupt Handler function prototypes.
 */
void irq_handler_enc_a();       // GPIO Interrupt Handler for Rotary Encode Pin A.
void irq_handler_enc_b();       // GPIO Interrupt Handler for Rotary Encoder Pin B.
void irq_handler_enc_sw();      // GPIO Interrupt Handler for Rotary Encoder Switch Pin.
// Periodic Function
bool GetAnalogValues();         // Periodic function for reading temperatures.

void setup(){
    Serial.begin();
    // Initializes Phyduino Pico Board. 
    phyduino::initialize_board();

    // Set up LED's
    pinMode(afe_shield::io::GREEN_LED, OUTPUT);
    pinMode(afe_shield::io::RED_LED, OUTPUT);
    digitalWrite(afe_shield::io::RED_LED, (temp_ch_sel == 0? LOW : HIGH));      // Indicates if the INT ADC channels is being used to read the temperatre.
    digitalWrite(afe_shield::io::GREEN_LED, (temp_ch_sel == 1? LOW : HIGH));    // Indicates if the EXT ADC channel is being used to read the temperature.
    // Setup Rotary Encodes
    pinMode(afe_shield::io::ENC_A , INPUT);
    pinMode(afe_shield::io::ENC_B , INPUT);
    pinMode(afe_shield::io::ENC_SW, INPUT);

    encoder = new RotaryEncoder(afe_shield::io::ENC_A, afe_shield::io::ENC_B, RotaryEncoder::LatchMode::TWO03);

    // Register Interrupts
    attachInterrupt(digitalPinToInterrupt(afe_shield::io::ENC_A), irq_handler_enc_a, CHANGE);
    attachInterrupt(digitalPinToInterrupt(afe_shield::io::ENC_B), irq_handler_enc_b, CHANGE);
    attachInterrupt(digitalPinToInterrupt(afe_shield::io::ENC_SW), irq_handler_enc_sw, LOW);
}


void loop() {
    phyduino::led_heart_beat();
    /*
     * Used to execute a function at a periodic basis. 
     */
    afe_shield::periodic_ms(sample_rate, &t_sample, GetAnalogValues);

    // Indicate which Temperature Channel is selected.
    digitalWrite(afe_shield::io::RED_LED, (temp_ch_sel == 0? LOW : HIGH));
    digitalWrite(afe_shield::io::GREEN_LED, (temp_ch_sel == 1? LOW : HIGH));
}

void irq_handler_enc_a() {
    encoder->tick();
}

void irq_handler_enc_b() {
    encoder->tick();
}

void irq_handler_enc_sw() {
    temp_ch_sel = temp_ch_sel == 0? 1 : 0;
}

// [0] AFE voltage value
// [1] Calculated Sensor voltage from [0]
// [2] Temperature Sensor output in °C
// [3] Temperature Sensor
static float MeasuredValues[3] { };

bool GetAnalogValues() {
    // Get Temperature reading
    if(temp_ch_sel == 0) {
        MeasuredValues[0] = phyduino::analog_voltage(afe_shield::io::TEMPERATURE0);         // Get INT ADC temperature measurement
    }
    else {
        MeasuredValues[0] = phyduino::analog_voltage(afe_shield::io::TEMPERATURE1);         // Get EXT ADC temperature measurement
    }
    MeasuredValues[1] = afe_shield::calc_temp_vout(MeasuredValues[0]);                      // Calculate Temperature sensor voltage using input voltage.

    // Read Analog Signals
    MeasuredValues[2] = afe_shield::get_temperature(afe_shield::io::TEMP_SENSOR, true);     // A1 - LMT84 voltage output.
    MeasuredValues[3] = afe_shield::convert2temp(MeasuredValues[1]);                        // Calcuate temperture from sensor voltage

    #ifdef ARUDUINO_SERIAL_PLOT
        // Output Arduino Serial Plotter format.
        Serial.printf("TEMP_SENSOR(°C):%.1f,"
                      "TEMPERATURE(°C):%.1f,"
                      "ENC_POS:%d\r\n", 
                      MeasuredValues[2], 
                      MeasuredValues[3], 
                      encoder->getPosition());
    #else
        // Output Text.
        Serial.printf("Analog Voltages\r\n");
        Serial.printf("\tTemperature Sensor output: %.1f°C\r\n", MeasuredValues[2]);
        Serial.printf("\tTemperature: %.1f°C\r\n", MeasuredValues[3]);
        Serial.printf("\tEncoder Position : %d\r\n", encoder->getPosition());
        Serial.printf("\tToggle Switch: %d\r\n", temp_ch_sel);
    #endif

    return true;
}