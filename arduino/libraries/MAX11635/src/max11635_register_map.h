/**
 * @file max11635_register_map.h
 * @date 2023-03-30
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Analog Devices (formerly Maxium Integrated) MAX11635 register definitions
 * 
 * @version 0.1
 * @copyright Copyright (c) 2023
 */
#include <cstdint>

#ifndef PHYDUINO_PICO_MAX11635_REGS_H
#define PHYDUINO_PICO_MAX11635_REGS_H

struct max1163x_conversion_bf final {
  std::uint8_t       : 1;
  std::uint8_t SCAN  : 2;
  std::uint8_t CHSEL : 3;
  std::uint8_t       : 2;
};

union max1163x_conversion_t final {
  std::uint8_t w;
  max1163x_conversion_bf b;
};

struct max1163x_setup_bf final {
  std::uint8_t DIFFSEL : 2;
  std::uint8_t REFSEL  : 2;
  std::uint8_t CKSEL   : 2;
  std::uint8_t         : 2;
};

union max1163x_setup_t final {
  std::uint8_t w;
  max1163x_setup_bf b;
};

struct max1163x_averaging_bf final {
  std::uint8_t NSCAN : 2;
  std::uint8_t NAVG  : 2;
  std::uint8_t AVGON : 1;
  std::uint8_t       : 3;
};

union max1163x_averaging_t final {
  std::uint8_t w;
  max1163x_averaging_bf b;
};

struct max1163x_reset_bf final {
  std::uint8_t       : 3;
  std::uint8_t RESET : 1;
  std::uint8_t       : 4;
};

union max1163x_reset_t final {
  std::uint8_t w;
  max1163x_reset_bf b;
};

struct max1163x_unipolar_bf final {
  std::uint8_t        : 4;
  std::uint8_t UCH6_7 : 1;
  std::uint8_t UCH4_5 : 1;
  std::uint8_t UCH2_3 : 1;
  std::uint8_t UCH0_1 : 1;
};

union max1163x_unipolar_t final {
  std::uint8_t w;
  max1163x_unipolar_bf b;
};

struct max1163x_bipolar_bf final {
  std::uint8_t        : 4;
  std::uint8_t BCH6_7 : 1;
  std::uint8_t BCH4_5 : 1;
  std::uint8_t BCH2_3 : 1;
  std::uint8_t BCH0_1 : 1;
};

union max1163x_bipolar_t final {
  std::uint8_t w;
  max1163x_bipolar_bf b;
};

struct max1163x_registers_t final {
  max1163x_registers_t() noexcept {
    conversion.w  = 0x80;
    setup.w       = 0x40;
    averaging.w   = 0x20;
    reset.w       = 0x10;
    unipolar.w    = 0x00;
    bipolar.w     = 0x00;
  }

  max1163x_conversion_t conversion;
  max1163x_setup_t      setup;
  max1163x_averaging_t  averaging;
  max1163x_reset_t      reset;
  max1163x_unipolar_t   unipolar;
  max1163x_bipolar_t    bipolar;
};

#endif /* PHYDUINO_PICO_MAX11635_REGS_H */
