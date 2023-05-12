/**
 * @file max11635.h
 * @date 2023-03-30
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Analog Devices (formerly Maxium Integrated) MAX11635 driver class definitions.
 * 
 * @note This class will on work with RP2040 projects.
 *
 * @version 1.0.0
 * @copyright Copyright (c) 2023
 */
#include <cstdint>
#include <Arduino.h>
#include <SPI.h>
#include "max11635_register_map.h"

#ifndef _MAX11635_ADC_H
#define _MAX11635_ADC_H

// #define MAX11635_DEBUG_L1   // Used only to debug analog_read()
// #define MAX11635_DEBUG_L2   // For internal debugging of the class.
// #define MAX11635_DEBUG_L3   // used for read and write functions
// #define MAX11635_SPI_CTRL_CS, // only works for single byte transfer.

namespace max11635 {
  static constexpr float conv_vbus = 10.8F;
  /**
   * @brief MAX11635 Driver Class
   */
  class driver final {
    public:
      using data_type = std::uint16_t;
      using pin_t = std::uint32_t;
      using SPIModule = SPIClassRP2040;
      
      static constexpr float v_reference         { 2.048F };                 // ADC voltage reference.
      static constexpr float v_resolution        { v_reference / 4096.0F };  // ADC Voltage steps.
      static constexpr std::size_t max_channels  { 4 };                      // Maximum number of analog channels.
          
      driver() noexcept = delete;
      
      driver(SPIModule* bus) noexcept : _bus(bus) { }
      
      void configure_io(pin_t, pin_t, pin_t, pin_t, pin_t, pin_t) noexcept;
      
      void begin() noexcept;
      void begin(arduino::SPISettings*) noexcept;
      void end() noexcept;

      void reset() noexcept;
      
      data_type analogRead(std::uint8_t) noexcept;
      float get_voltage(std::uint8_t) noexcept;

      bool is_initialized() const noexcept { return initialized; }
      /**
       * @brief Set the drivers pointer
       * @param spi Pointer to SPI module.
       * @return driver& Reference to this driver.
       * @note begin function must be called after this operator is called.
       */
      driver& operator=(SPIModule* spi) noexcept {
        _bus = spi;
        initialized = false;
        return *this;
      }
      /**
       * @brief Sets the SPI settings for MAX11635 SPI interface
       * @param settings Pointer to SPI Settings
       * @return driver& Referecne to this driver.
       */
      driver& operator=(arduino::SPISettings* settings) noexcept {
        _settings = settings;
        return *this;
      }

      static float to_voltage(data_type) noexcept;
    private:
      static constexpr std::uint8_t dummy_byte { 0x00 };
      static constexpr std::uint32_t fclock_default { 4'800'000 };
      static arduino::SPISettings default_setting;
      /*
      * SPI MODE 0 : CPOL = 0; CPHA = 0; <= THIS OR
      * SPI MODE 1 : CPOL = 0; CPHA = 1;
      * SPI MODE 2 : CPOL = 1; CPHA = 0;
      * SPI MODE 3 : CPOL = 1; CPHA = 1; <= THIS
      */
      arduino::SPISettings* _settings { nullptr };
      SPIModule* _bus;
      max11635::registers_t _regs { };
      pin_t _mosi       { 23 };
      pin_t _miso       { 20 };
      pin_t _sck        { 22 };
      pin_t _cs         { 21 };
      pin_t _n_eoc      { 19 };
      pin_t _n_cnvst    { 18 };
      bool initialized  { false };

      void write_nbytes(std::uint8_t*, std::size_t = 1) noexcept;
      void read_nbytes(std::uint8_t*, std::size_t = 1) noexcept;
      void initialize() noexcept;
      data_type get_conversion() noexcept;
      void config_regs() noexcept;
  };
}

extern max11635::driver MAX11635_ADC;

#endif
