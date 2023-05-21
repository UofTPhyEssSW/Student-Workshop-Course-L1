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

namespace afe_shield {

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

    float get_temperature(phyduino::pin_t, bool=false, bool=false) noexcept;
    float get_sensor_voltage() noexcept;
    float get_potentiometer() noexcept;
    bool periodic_ms(unsigned long, unsigned long*, const std::function<bool()>&) noexcept;
    bool periodic_us(unsigned long, unsigned long*, const std::function<bool()>&) noexcept;
}

#endif
