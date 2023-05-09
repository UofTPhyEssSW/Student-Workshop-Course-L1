/**
 * @file phyduino_blinky.ino
 * @date 2023-05-08
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Phyduino Pico Template Code
 * 
 * @version 0.1
 * @copyright Copyright (c) 2023
 */
#include "phyduino.h"

void setup() {
  // Initialize PCB. ( required )
  phyduino::initialize_board();

  digitalWrite(phyduino::gpio::LED, HIGH);
}

void loop() {
  digitalWrite(phyduino::gpio::LED, LOW);
  delay(200);
  digitalWrite(phyduino::gpio::LED, HIGH);
  delay(200);
}
