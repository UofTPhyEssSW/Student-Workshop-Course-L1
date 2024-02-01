/**
 * @file afe_shield.h
 * @date 2023-05-10
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief 
 * 
 * @version 1.0.0
 * @copyright Copyright (c) 2023
 */
#ifndef PHYSIC_ESS_AFE_SHIELD_H_
#define PHYSIC_ESS_AFE_SHIELD_H_

#include <functional>
#include <climits>
#include <cmath>
#include <phyduino.h>

// #define AFE_TEMP_DEBUG

namespace afe_shield {

    static constexpr float SIGNAL_GAIN      { 4.31F };
    static constexpr float SIGNAL_GAIN_INV  { 1 / SIGNAL_GAIN };
    static constexpr float SIGNAL_OFFSET    { 0.638F };

    static constexpr float R14_VALUE        { 10.0e3F };
    static constexpr float R15_VALUE        { 6.98e3F };
    static constexpr float AIN_DIVIDER      { R15_VALUE / (R14_VALUE + R15_VALUE) };
    static constexpr float AIN_DIVIDER_INV  { 1 / AIN_DIVIDER };

    // static constexpr float SIGNAL_CALIB_OFST    { 0.13F };
    static constexpr float SIGNAL_CALIB_OFST    { 0.0F };
    /**
     * @brief Calculates the LMT84 temperature sensor voltage based on signal conditioning circuit.
     * @param ain Analog Input voltage
     * @return float Calculated Temperature sensor voltage.
     */
    constexpr float temp_sensor_voltage(float ain) noexcept {
        return ((ain * AIN_DIVIDER_INV) + SIGNAL_OFFSET) * SIGNAL_GAIN_INV;
    }

    struct io final {
        static constexpr phyduino::pin_t RED_LED        { phyduino::gpio::D5 };
        static constexpr phyduino::pin_t GREEN_LED      { phyduino::gpio::D6 };
        static constexpr phyduino::pin_t ENC_A          { phyduino::gpio::D4 };
        static constexpr phyduino::pin_t ENC_B          { phyduino::gpio::D3 };
        static constexpr phyduino::pin_t ENC_SW         { phyduino::gpio::D2 };
        static constexpr phyduino::pin_t TEMPERATURE0   { phyduino::gpio::A0 };
        static constexpr phyduino::pin_t TEMP_SENSOR    { phyduino::gpio::A1 };
        static constexpr phyduino::pin_t TEMPERATURE1   { phyduino::gpio::A3 };
        static constexpr phyduino::pin_t SENSOR_VOUT    { phyduino::gpio::A4 };
        static constexpr phyduino::pin_t POTENTIOMETER  { phyduino::gpio::A5 };
    };

    /**
     * @brief Converts voltage output to temperature
     * @param voltage Measured voltage at VOUT of the LMT84 temp sensor.
     * @return constexpr float Temperature reading.
     */
    constexpr float convert2temp(const float voltage) noexcept {
        return ((5.506F - std::sqrt(36.449F - (0.00704F * voltage * 1000.0F)))/-0.00352F) + 30.0F;
    }

    float get_temperature(phyduino::pin_t, bool=false, bool=false) noexcept;
    float get_sensor_voltage() noexcept;
    float get_potentiometer() noexcept;
    bool periodic_ms(unsigned long, unsigned long*, const std::function<bool()>&) noexcept;
    bool periodic_us(unsigned long, unsigned long*, const std::function<bool()>&) noexcept;
    
    constexpr float calc_temp_vout(float voltage, bool cal = true) noexcept {
        auto value =  (0.559F * voltage) + 0.153F;   
        return cal? value + afe_shield::SIGNAL_CALIB_OFST : value;
    }
}

#endif
