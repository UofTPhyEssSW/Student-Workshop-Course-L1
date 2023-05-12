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

/**
 * @brief Get the temperature from LMT84LPM
 * 
 * @param AIN Analog input from LMT84LPM
 * @param raw Read was from sensor no 
 * @param F Convert Fei
 * @return float 
 */
float afe_shield::get_temperature(phyduino::pin_t AIN, bool raw, bool fahrenheit) noexcept{
    float temperature { 0.0F };
    auto voltage = phyduino::analog_read(AIN);

    if(!raw){ // Find Sensor voltage
        // vin = (vout  - 0.262) / 1.77
        voltage = (voltage - 0.262F) / 1.77F;
    }
    // Calculate Temperature using formula in datasheet. 
    temperature = (5.506F * std::sqrt( 30.32F + (6.13F - (7.04 * voltage))) / -0.00352F) + 30.0F;

    if(fahrenheit){
        temperature = (temperature * 1.8F) + 32.0F;
    }

    return 0.0F;
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
bool afe_shield::periodic_ms(unsigned long t_sample, unsigned long* t_last, const function<bool()>& proc) noexcept {
    bool executed       { false };
    unsigned long delta { 0 };
    unsigned long ctime { millis() };        // Get system tick in millisecond

    if(ctime < *t_last){                 // Check for system tick rollover and calculate difference.
        delta = (std::numeric_limit<unsigned long>::max() - *t_last) + ctime; 
    } else {
        delta = ctime - *t_last;
    }

    if(delta > t_sample) {  // If sample period has passed
        *t_last = ctime;                    // Save current system tick
        executed = proc();                  // Run user defined periodic processing function.
    }

    return executed;
}
/**
 * @brief Periodic microsecond execution of user defined function.
 * 
 * @param t_sample [in]
 * @param t_last [in,out]
 * @param proc [in]
 * @return true 
 * @return false 
 */
bool afe_shield::periodic_us(unsigned long t_sample, unsigned long* t_last, const function<bool()>& proc) noexcept {
    bool executed { false };
    unsigned long delta;
    unsigned long utime { micros() };

    if(utime < *t_last){                 // Check for system tick rollover and calculate difference.
        delta = (std::numeric_limit<unsigned long>::max() - *t_last) + utime; 
    } else {
        delta = utime - *t_last;
    }

    if(delta > t_sample) {                  // If sample period has passed
        *t_last = ctime;                    // Save current system tick
        executed = proc();                  // Run user defined periodic processing function.
    }

    return executed;    
}
