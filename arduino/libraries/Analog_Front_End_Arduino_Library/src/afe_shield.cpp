/**
 * @file afe_shield.cpp
 * @date 2023-05-10
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief 
 * 
 * @version 1.0.0
 * @copyright Copyright (c) 2023
 */
#include "afe_shield.h"
 
// /**
//  * @brief Calculate Temperature VOUT voltage from AIN reading.
//  * @param voltage AIN voltage value.
//  * @return constexpr float Temperature sensor voltage.
//  * @note This conversion is calculated directly from the schematic values
//  */
// constexpr float afe_shield::calc_temp_vout(float voltage) noexcept {
//     return (0.559F * voltage) + 0.153F + afe_shield::SIGNAL_CALIB_OFST;   
// }
/**
 * @brief Get the temperature from LMT84LPM
 * 
 * @param AIN Analog input from LMT84LPM
 * @param raw Read was from sensor directly no signal conditioning.
 * @param F Convert Fahrenheit (default Celius)
 * @return float Current temperature reading.
 */
float afe_shield::get_temperature(phyduino::pin_t AIN, bool raw, bool fahrenheit) noexcept{
    auto voltage = phyduino::analog_voltage(AIN);

    // determine voltage of sensor vout when not measuring LMT84 output directly.
    if(!raw){
        voltage = calc_temp_vout(voltage);
    #ifdef AFE_TEMP_DEBUG
        Serial.printf("Calculated Temperature Sensor VOUT %.3f\r\n", voltage);
    }else {
        Serial.printf("Temperature Sensor VOUT = %.3f\r\n", voltage);
    #endif
    }
    
    // Calculate Temperature using formula in datasheet. 
    float temperature = convert2temp(voltage);

    if(fahrenheit){
        temperature = (temperature * 1.8F) + 32.0F;
    }

    return temperature;
}
/**
 * @brief Get the J2 sensor output voltage.
 * @return float Voltage measured of the sensor
 */
float afe_shield::get_sensor_voltage() noexcept {
    auto value = phyduino::analog_read(afe_shield::io::SENSOR_VOUT);
    return max11635::driver::to_voltage(value);
}
/**
 * @brief Get the Potentiometer voltage
 * @return float Voltge of the potentiometer
 */
float afe_shield::get_potentiometer() noexcept {
    auto value = phyduino::analog_read(afe_shield::io::POTENTIOMETER);
    return max11635::driver::to_voltage(value);
}
/**
 * @brief Periodic millisecond execution of user defined function.
 * 
 * @param t_sample [in] Execution period in milliseconds
 * @param t_last [in,out] Pointer to the last system tick user function executed.
 * @param proc [in] User defined periodic executable function. 
 * @return true User executable function ran.
 * @return false User executable function did not run.
 */
bool afe_shield::periodic_ms(unsigned long t_sample, unsigned long* t_last, const std::function<bool()>& proc) noexcept {
    bool executed       { false };
    unsigned long delta { 0 };
    unsigned long time_ms = millis();        // Get system tick in millisecond

    if(time_ms < *t_last){                 // Check for system tick rollover and calculate difference.
        delta = (std::numeric_limits<unsigned long>::max() - *t_last) + time_ms;
    } else {
        delta = time_ms - *t_last;
    }

    if(delta > t_sample) {  // If sample period has passed
        *t_last = time_ms;                    // Save current system tick
        executed = proc();                  // Run user defined periodic processing function.
    }

    return executed;
}
/**
 * @brief Periodic microsecond execution of user defined function.
 * 
 * @param t_sample [in] execution period.
 * @param t_last [in,out] Pointer to the last execution time.
 * @param proc [in] Reference to function to be executed periodicaly
 * @return true Function executed.
 * @return false Function did not execute.
 */
bool afe_shield::periodic_us(unsigned long t_sample, unsigned long* t_last, const std::function<bool()>& proc) noexcept {
    bool executed { false };
    unsigned long delta;
    unsigned long time_us = micros();

    if(time_us < *t_last){                 // Check for system tick rollover and calculate difference.
        delta = (std::numeric_limits<unsigned long>::max() - *t_last) + time_us; 
    } else {
        delta = time_us - *t_last;
    }

    if(delta > t_sample) {                  // If sample period has passed
        *t_last = time_us;                    // Save current system tick
        executed = proc();                  // Run user defined periodic processing function.
    }

    return executed;    
}
