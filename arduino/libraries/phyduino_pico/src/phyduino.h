/**
 * @file phyduino.h
 * @date 2023-05-08
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Physics Phyduino Pico Board Rev C header file.
 * 
 * @version 1.0.0.0
 * @copyright Copyright (c) 2023
 */
#ifndef PHYSICS_ESS_PHYDUINO_H
#define PHYSICS_ESS_PHYDUINO_H

#include <cstdint>
#include <Arduino.h>              // Arduino common library
#include <Wire.h>                 // Arduino I2C library
#include "phyduino_map.h"         // Phyduino IO map
#include "pico/stdlib.h"          // Raspberry PI Pico standard library
#include "hardware/adc.h"           // Raspberry PI Pico ADC library
#include "SparkFun_STUSB4500.h"   // Depandant library
#include "max11635.h"             // Dependant library

//#define STUSB4500_DEBUG_LEVEL1    // General Debug Messages [ Comment line to stop messages ]
//#define STUSB4500_FORMAT          // Format STUSB4500 [ Comment line to stop formatting STUSB4500 ]

namespace phyduino {
  using ms_time_t = unsigned long;
  using baud_rate_t = unsigned long;

  static constexpr float R15_VALUE                { 100e3F  };
  static constexpr float R14_VALUE                { 10.2e3F };
  static constexpr float VBUS_DIV_RATIO           { R14_VALUE / (R15_VALUE + R14_VALUE) };
  static constexpr float VBUS_DIV_RATIO_INV       { 1 / VBUS_DIV_RATIO };

  namespace adc {
    namespace internal {
      static constexpr std::uint32_t bits   { 12 };
      static constexpr float vref         { 3.30F };
      static constexpr float calibration  { 4.046F };
      static constexpr float resolution   { (vref / static_cast<float>(1 << bits)) * calibration };
    } 
    namespace external {
      static constexpr std::uint32_t bits   { 12 };
      static constexpr float vref         { 2.048F };
      static constexpr float resolution   { vref / static_cast<float>(1 << bits) };
    }
  }
  // static constexpr float internal_adc_calibration { 4.046F };
  // static constexpr float internal_adc_resolution  { (INTERNAL_ADC_VREF / std::pwr(2.0F, INTERNAL_ADC_BITS) 4096.0F) * internal_adc_calibration };
  static constexpr float vbus_multiplier          { 10.8F };

  static constexpr ms_time_t led_delay            { 400 };        // 400ms LED blink time
  static constexpr uint8_t usbpd_address          { 0x28 };       // USB-PD I2C Address.
  static constexpr baud_rate_t serial_baud        { 115200 };     // Default Serial Port Baud

  void led_heart_beat() noexcept;
  void pin_heart_beat(pin_t, ms_time_t, ms_time_t*) noexcept;
  
  void initialize_board() noexcept;

  float to_voltage(std::uint16_t, bool = false) noexcept;
  
  std::uint16_t analog_read(pin_t) noexcept;
  float analog_voltage(pin_t) noexcept;

  float get_vbus() noexcept;
}

#endif /* PHYSICS_ESS_PHYDUINO_H */
