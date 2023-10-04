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
  public:
    max1163x_conversion_t() noexcept: max1163x_conversion_t(RESET_VALUE)  { }
    explicit max1163x_conversion_t(const adc_reg_t val) noexcept : w(ADDRESS_PREFIX | (val & DATA_MASK)) { }

    max1163x_conversion_t(const max1163x_conversion_t&) noexcept = default;
    max1163x_conversion_t& operator=(const max1163x_conversion_t&) noexcept = default;
    max1163x_conversion_t(max1163x_conversion_t&&) noexcept = default;
    max1163x_conversion_t& operator=(max1163x_conversion_t&&) noexcept = default;

    max1163x_conversion_t& operator=(const adc_reg_t val) noexcept {
      w = ADDRESS_PREFIX | (val & DATA_MASK);
      return *this;
    }

    void reset() noexcept{ *this = RESET_VALUE; }

    adc_reg_t w;
    max1163x_conversion_bf b;
  private:
    static constexpr adc_reg_t ADDRESS_PREFIX  { 0x80 };
    static constexpr adc_reg_t RESET_VALUE { 0x00 };
    static constexpr adc_reg_t DATA_MASK { 0x3E };
  };

  struct max1163x_setup_bf final {
    adc_reg_t DIFFSEL : 2;
    adc_reg_t REFSEL  : 2;
    adc_reg_t CKSEL   : 2;
    adc_reg_t         : 2;
  };

  union max1163x_setup_t final {
  public:
    max1163x_setup_t() noexcept : max1163x_setup_t(RESET_VALUE){ }
    explicit max1163x_setup_t(const adc_reg_t val) noexcept : w(ADDRESS_PREFIX | (val & DATA_MASK)) { }

    max1163x_setup_t(const max1163x_setup_t&) noexcept = default;
    max1163x_setup_t& operator=(const max1163x_setup_t&) noexcept = default;
    max1163x_setup_t(max1163x_setup_t&&) noexcept = default;
    max1163x_setup_t& operator=(max1163x_setup_t&&) noexcept = default;

    max1163x_setup_t& operator=(const adc_reg_t val) noexcept {
      w = ADDRESS_PREFIX | (val & DATA_MASK);
      return *this;
    }

    void reset() noexcept{ *this = RESET_VALUE; }

    adc_reg_t w;
    max1163x_setup_bf b;
  private:
    static constexpr adc_reg_t ADDRESS_PREFIX { 0x40 };
    static constexpr adc_reg_t RESET_VALUE { 0x00 };
    static constexpr adc_reg_t DATA_MASK { 0x3F };
  };

  struct max1163x_averaging_bf final {
    adc_reg_t NSCAN : 2;
    adc_reg_t NAVG  : 2;
    adc_reg_t AVGON : 1;
    adc_reg_t       : 3;
  };

  union max1163x_averaging_t final {
  public:
    max1163x_averaging_t() noexcept : max1163x_averaging_t(RESET_VALUE) { }
    explicit max1163x_averaging_t(const adc_reg_t val) noexcept : w(ADDRESS_PREFIX | (val & DATA_MASK)){ }

    max1163x_averaging_t(const max1163x_averaging_t&) noexcept = default;
    max1163x_averaging_t& operator=(const max1163x_averaging_t&) noexcept = default;
    max1163x_averaging_t(max1163x_averaging_t&&) noexcept = default;
    max1163x_averaging_t& operator=(max1163x_averaging_t&&) noexcept = default;

    max1163x_averaging_t& operator=(const adc_reg_t val) noexcept {
      w = ADDRESS_PREFIX | (val & DATA_MASK);
      return *this;
    }

    void reset() noexcept{ *this = RESET_VALUE; }

    adc_reg_t w;
    max1163x_averaging_bf b;
  private:
    static constexpr adc_reg_t ADDRESS_PREFIX { 0x20 };
    static constexpr adc_reg_t RESET_VALUE { 0x00 };
    static constexpr adc_reg_t DATA_MASK { 0x1F};
  };

  struct max1163x_reset_bf final {
    adc_reg_t       : 3;
    adc_reg_t RESET : 1;
    adc_reg_t       : 4;
  };

  union max1163x_reset_t final {
  public:
    max1163x_reset_t() noexcept : max1163x_reset_t(RESET_VALUE) { }
    explicit max1163x_reset_t(const adc_reg_t val) noexcept : w(ADDRESS_PREFIX | (val & DATA_MASK)) { }

    max1163x_reset_t(const max1163x_reset_t&) noexcept = default;
    max1163x_reset_t& operator=(const max1163x_reset_t&) noexcept = default;
    max1163x_reset_t(max1163x_reset_t&&) noexcept = default;
    max1163x_reset_t& operator=(max1163x_reset_t&&) noexcept = default;

    max1163x_reset_t& operator=(const adc_reg_t val) noexcept {
      w = ADDRESS_PREFIX | (val & DATA_MASK);
      return *this;
    }

    void reset() noexcept{ *this = RESET_VALUE; }

    adc_reg_t w;
    max1163x_reset_bf b;
  private:
    static constexpr adc_reg_t ADDRESS_PREFIX { 0x10 };
    static constexpr adc_reg_t RESET_VALUE { 0x00 };
    static constexpr adc_reg_t DATA_MASK { 0x08 };
  };

  struct max1163x_unipolar_bf final {
    adc_reg_t        : 4;
    adc_reg_t UCH6_7 : 1;
    adc_reg_t UCH4_5 : 1;
    adc_reg_t UCH2_3 : 1;
    adc_reg_t UCH0_1 : 1;
  };

  union max1163x_unipolar_t final {
  public:
    max1163x_unipolar_t() noexcept : max1163x_unipolar_t(RESET_VALUE) { }
    explicit max1163x_unipolar_t(const adc_reg_t val) noexcept : w(val & DATA_MASK) { }

    max1163x_unipolar_t(const max1163x_unipolar_t&) noexcept = default;
    max1163x_unipolar_t& operator=(const max1163x_unipolar_t&) noexcept = default;
    max1163x_unipolar_t(max1163x_unipolar_t&&) noexcept = default;
    max1163x_unipolar_t& operator=(max1163x_unipolar_t&&) noexcept = default;

    max1163x_unipolar_t& operator=(const adc_reg_t val) noexcept {
      w = val & DATA_MASK;
      return *this;
    }

    void reset() noexcept{ *this = RESET_VALUE; }

    adc_reg_t w;
    max1163x_unipolar_bf b;
  private:
    static constexpr adc_reg_t RESET_VALUE { 0x00 };
    static constexpr adc_reg_t DATA_MASK { 0xF0 };
  };

  struct max1163x_bipolar_bf final {
    adc_reg_t        : 4;
    adc_reg_t BCH6_7 : 1;
    adc_reg_t BCH4_5 : 1;
    adc_reg_t BCH2_3 : 1;
    adc_reg_t BCH0_1 : 1;
  };

  union max1163x_bipolar_t final {
  public:
    max1163x_bipolar_t() noexcept : max1163x_bipolar_t(RESET_VALUE){ }
    explicit max1163x_bipolar_t(const adc_reg_t val) noexcept : w(val & DATA_MASK){ }

    max1163x_bipolar_t(const max1163x_bipolar_t&) noexcept = default;
    max1163x_bipolar_t& operator=(const max1163x_bipolar_t&) noexcept = default;
    max1163x_bipolar_t(max1163x_bipolar_t&&) noexcept = default;
    max1163x_bipolar_t& operator=(max1163x_bipolar_t&&) noexcept = default;

    max1163x_bipolar_t& operator=(const adc_reg_t val) noexcept {
      w = val & DATA_MASK;
      return *this;
    }

    void reset() noexcept{ *this = RESET_VALUE; }

    adc_reg_t w;
    max1163x_bipolar_bf b;
  private:
    static constexpr adc_reg_t RESET_VALUE { 0x00 };
    static constexpr adc_reg_t DATA_MASK { 0xF0 };
  };

  struct registers_t final {
    registers_t() noexcept = default;

    registers_t(const registers_t&) noexcept;
    registers_t& operator=(const registers_t&) noexcept;
    registers_t(registers_t&&) noexcept;
    registers_t& operator=(registers_t&&) noexcept;

    void reset_all() noexcept {
      conversion.reset();
      setup.reset();
      averaging.reset();
      unipolar.reset();
      bipolar.reset();
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

