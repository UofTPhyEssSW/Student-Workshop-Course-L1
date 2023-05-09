/**
 * @file phyduino_io.h
 * @date 2023-05-08
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Phyduino Internal use IO map.
 * 
 * @version 1.0.0.0 
 * @copyright Copyright (c) 2023 
 */
#ifndef PHYSICS_ESS_PHYDUINO_IO_H_
#define PHYSICS_ESS_PHYDUINO_IO_H_

#include <cstdint>
#include <climits>
#include <algorithm>
#include "phyduino_map.h"

namespace phyduino {
  // MAX11635 ADC Interface Pins.
  struct extern_adc final {
      constexpr static pin_t MOSI     { 23 };
      constexpr static pin_t MISO     { 20 };
      constexpr static pin_t SCK      { 22 };
      constexpr static pin_t nCS      { 21 };
      constexpr static pin_t nEOC     { 19 };
      constexpr static pin_t nCNVST   { 18 };
      constexpr static std::uint32_t fclock  { 5'000'000 };
  };
  // STUSB4500 Interface IO's
  struct usbpd final {
      constexpr static pin_t SDA      { 2  };
      constexpr static pin_t SCL      { 3  };
      constexpr static pin_t nINT     { 4  };
      constexpr static pin_t RESET    { 29 };
  };

}

#endif /* PHYSICS_ESS_PHYDUINO_IO_H_ */
