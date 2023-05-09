/**
 * @file phyduino_map.h
 * @date 2023-05-08
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Phyduino IO map for user.
 *
 * @version 1.0.0.0
 * @copyright Copyright (c) 2023 
 */
#ifndef PHYSICS_ESS_PHYDUINO_MAP_H_
#define PHYSICS_ESS_PHYDUINO_MAP_H_

#include <cstdint>
#include <climits>
#include <algorithm>

namespace phyduino {
  using irq_num_t = std::uint32_t;
  using pin_t = std::uint32_t;
  // GPIO Interface Pins.
  struct gpio final {
      // Pins Available to user
      constexpr static pin_t D0      { 1  };
      constexpr static pin_t D1      { 0  };
      constexpr static pin_t D2      { 6  };
      constexpr static pin_t D3      { 7  };
      constexpr static pin_t D4      { 8  };
      constexpr static pin_t D5      { 9  };
      constexpr static pin_t D6      { 10 };
      constexpr static pin_t D7      { 15 };
      constexpr static pin_t D8      { 16 };
      constexpr static pin_t D9      { 17 };
      constexpr static pin_t D10     { 13 };
      constexpr static pin_t D11     { 11 };
      constexpr static pin_t D12     { 12 };
      constexpr static pin_t D13     { 14 };
      constexpr static pin_t D18     { 24 };
      constexpr static pin_t D19     { 25 };
      constexpr static pin_t A0      { 26 };
      constexpr static pin_t A1      { 27 };
      constexpr static pin_t A2      { 28 };
      constexpr static pin_t A3      { 128 };
      constexpr static pin_t A4      { 129 };
      constexpr static pin_t A5      { 130 };
      constexpr static pin_t AVBUS   { 131 };
      constexpr static pin_t LED     { 5  };
  };
  // RP2040 IRQ numbers
  struct IRQ_NUM final {
      constexpr static irq_num_t USB_PD       { I2C1_IRQ };
      constexpr static irq_num_t EXTERN_ADC   { SPI0_IRQ };
  };
}

#endif /* PHYSICS_ESS_PHYDUINO_MAP_H_ */
