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

#ifndef MAX11635_REGS_H
#define MAX11635_REGS_H

namespace max11635 {

  using adc_reg_t = std::uint8_t;

  struct max1163x_conversion_bf final {
    adc_reg_t       : 1;
    adc_reg_t SCAN  : 2;
    adc_reg_t CHSEL : 3;
    adc_reg_t       : 2;
  };

  union max1163x_conversion_t final {
    adc_reg_t w;
    max1163x_conversion_bf b;
  };

  struct max1163x_setup_bf final {
    adc_reg_t DIFFSEL : 2;
    adc_reg_t REFSEL  : 2;
    adc_reg_t CKSEL   : 2;
    adc_reg_t         : 2;
  };

  union max1163x_setup_t final {
    adc_reg_t w;
    max1163x_setup_bf b;
  };

  struct max1163x_averaging_bf final {
    adc_reg_t NSCAN : 2;
    adc_reg_t NAVG  : 2;
    adc_reg_t AVGON : 1;
    adc_reg_t       : 3;
  };

  union max1163x_averaging_t final {
    adc_reg_t w;
    max1163x_averaging_bf b;
  };

  struct max1163x_reset_bf final {
    adc_reg_t       : 3;
    adc_reg_t RESET : 1;
    adc_reg_t       : 4;
  };

  union max1163x_reset_t final {
    adc_reg_t w;
    max1163x_reset_bf b;
  };

  struct max1163x_unipolar_bf final {
    adc_reg_t        : 4;
    adc_reg_t UCH6_7 : 1;
    adc_reg_t UCH4_5 : 1;
    adc_reg_t UCH2_3 : 1;
    adc_reg_t UCH0_1 : 1;
  };

  union max1163x_unipolar_t final {
    adc_reg_t w;
    max1163x_unipolar_bf b;
  };

  struct max1163x_bipolar_bf final {
    adc_reg_t        : 4;
    adc_reg_t BCH6_7 : 1;
    adc_reg_t BCH4_5 : 1;
    adc_reg_t BCH2_3 : 1;
    adc_reg_t BCH0_1 : 1;
  };

  union max1163x_bipolar_t final {
    adc_reg_t w;
    max1163x_bipolar_bf b;
  };

  struct registers_t final {
    registers_t() noexcept {
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

}

#endif /* MAX11635_REGS_H */
