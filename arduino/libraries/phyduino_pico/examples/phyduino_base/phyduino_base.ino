/**
 * @file phyduino_base.ino
 * @date 2023-05-08
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Phyduino Pico Template Code
 * 
 * @version 1.0.0
 * @copyright Copyright (c) 2023
 */
//#define FORMAT_STUSB4500    // Uncomment this line if this the first time running a program on the Phyduino Board
#include "phyduino.h"

void setup() {
  // Initialize Serial port [ optional ]
  Serial.begin(phyduino::serial_baud);
  // Initialize PCB [ required ]
  phyduino::initialize_board();

  // TODO : User Initialization code here.
}

void loop() {
  phyduino::led_heart_beat();   // Blinks D8 Green LED

  // TODO : Add source code here

}
