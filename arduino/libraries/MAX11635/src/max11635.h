/**
 * @file max11635.h
 * @date 2023-03-30
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Analog Devices (formerly Maxium Integrated) MAX11635 class definitions.
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

#ifndef PHYDUINO_PICO_MAX11635_H
#define PHYDUINO_PICO_MAX11635_H

class ADC_MAX11635 final {
  public:
    using data_type = std::uint16_t;
    using pin_t = std::uint8_t;
    using SPIModule = SPIClassRP2040;

    ADC_MAX11635() noexcept = delete;
    /**
     * @brief Construct a new adc max11635 object
     * @param bus [in,out] Reference to Board SPI Module.
     * @note SPI SCK frequency set to 4MHz.
     */
    explicit ADC_MAX11635(SPIModule& bus) noexcept : 
      _bus(bus),
      _settings({ 4'000'000, MSBFIRST, SPIMode::SPI_MODE0 }){ }
    /**
     * @brief Construct a new adc max11635 object
     * @param bus [in,out] Reference to Board SPI Module.
     * @param freq [in] SPI bus SCK frequency.
     */
    ADC_MAX11635(SPIModule& bus, const unsigned long freq) noexcept : 
      _bus(bus), 
      _settings({ freq, MSBFIRST, SPIMode::SPI_MODE0 }) {}

    void configure_io(pin_t, pin_t, pin_t, pin_t, pin_t, pin_t) noexcept;

    void begin() noexcept;
    void begin(const arduino::SPISettings&) noexcept;
    void end() noexcept;

    void reset() noexcept;

    data_type analogRead(pin_t) noexcept;
    float get_voltage(pin_t) noexcept;

    static float to_voltage(data_type) noexcept;
    static constexpr float V_REF { 2.048F };
    static constexpr float V_RES { V_REF / 4096.0F };
    static constexpr std::uint8_t CHANNELS { 4 };
  private:
    static constexpr std::uint8_t DUMMY_BYTE { 0x00 };
    arduino::SPISettings _settings; // = SPISettings(phyduino::extern_adc::fclock, MSBFIRST, SPIMode::SPI_MODE0);
    SPIModule& _bus;
    max1163x_registers_t _regs { };
    pin_t _mosi { };
    pin_t _miso { };
    pin_t _sck { };
    pin_t _cs { };
    pin_t _n_cnvst { };
    pin_t _n_eoc { };

    void write_nbytes(std::uint8_t*, std::size_t = 1) noexcept;
    void read_nbytes(std::uint8_t*, std::size_t = 1) noexcept;
    data_type get_analog_value() noexcept;
    void config_regs() noexcept;
};


#endif /* PHYDUINO_PICO_MAX11635_H */ 
