/**
 * @file simple_demo.ino
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "phyduino.h"
#include "afe_shield.h"

static constexpr unsigned long COMM_BAUD { 115'200UL };             // BAUD Rate for the USB Virtual COM port
static constexpr unsigned long LED_DELAY { 250UL };                 // Delay time in milliseconds between LED states.
static unsigned long led_last_time { 0UL };                         // Last time the LED change of state

static constexpr unsigned long TEMP_SAMPLE_DELAY { 100UL };         // Sample delay for reading the current sensor.
static unsigned long temp_last_time { 0UL };                        // Last recorded the Temperature was sampled.

bool TemperatureSensorRead();                                       // Periodic execution function for reading the temperature.

void setup() {
    phyduino::initialize_board();                                   // Initialize Phyduino Pico board.
    Serial.begin(COMM_BAUD);                                        // Start USB Virtual COM port connection.
    
    // IO PIN setup.
    pinMode(afe_shield::io::GREEN_LED, OUTPUT);                     // Set Pin mode for the GREEN LED AFE Shield to OUTPUT.
    digitalWrite(afe_shield::io::GREEN_LED, LOW);                   // Set default value for the GREEN LED, OFF.
    pinMode(afe_shield::io::RED_LED, OUTPUT);                       // Set Pin mode for the RED LED AFE Sheild to OUTPUT.
    digitalWrite(afe_shield::io::RED_LED, HIGH);                    // Set default value for the RED LED, ON.

    led_last_time = 0UL;                                            // Reset LED last time.
    temp_last_time = 0UL;                                           // Reset Temperature Sample last time.
}

void loop() {
    phyduino::led_heart_beat();                                                             // Blinks Built-in LED on the Phyduino Pico board.
    phyduino::pin_heart_beat(afe_shield::io::GREEN_LED,LED_DELAY,&led_last_time);            // Blinks the GREEN LED on the AFE Shield board.
    afe_shield::periodic_ms(TEMP_SAMPLE_DELAY, &temp_last_time, TemperatureSensorRead);     // Sample the Temperature Sensor at set intervals.
}
/**
 * @brief Reads Temperature sensor both the raw output and analog front end signals.
 * @return true Always true.
 */
bool TemperatureSensorRead() {
    std::uint16_t RawTempVal = phyduino::analog_read(afe_shield::io::TEMP_SENSOR);      // Read Temperature sensor signal directly
    std::uint16_t AfeTempVal = phyduino::analog_read(afe_shield::io::TEMPERATURE1);     // Read Amplified/Filtered sensor signal

    float RawTempVoltage = phyduino::to_voltage(RawTempVal);                            // Convert ADC Value to voltage.
    float AfeTempVoltage = phyduino::to_voltage(AfeTempVal, true);

    float RawTemp = afe_shield::convert2temp(RawTempVoltage);                           // 

    float SensorVoltage = afe_shield::calc_temp_vout(AfeTempVoltage);                   //
    float AfeTemp = afe_shield::convert2temp(SensorVoltage);                            //

    // Transmit Sensor data
    Serial.printf("RawVal:%d,AFEVal:%d,RawVoltage:%.3f,AFEVoltage:%.3f,RawTemp:%.1f,CalcSensor:%.3f,AfeTemp:%.1f\r\n", 
                   RawTempVal, 
                   AfeTempVal, 
                   RawTempVoltage, 
                   AfeTempVoltage, 
                   RawTemp,
                   SensorVoltage,
                   AfeTemp);

    return true;
}