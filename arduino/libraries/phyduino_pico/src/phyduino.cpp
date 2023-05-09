/**
 * @file phyduino.cpp
 * @date 2023-05-08
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Phyduino Pico source code.
 * 
 * @version 1.0.0.0
 * @copyright Copyright (c) 2023
 */
#include "phyduino.h"
#include "phyduino_io.h"

STUSB4500 usb_pd { };
/**
 * @brief Initializes Board GPIO's
 */
void initialize_gpio() noexcept {
  // Initialize on board LED
  pinMode(phyduino::gpio::LED, OUTPUT);
  gpio_set_drive_strength(phyduino::gpio::LED, GPIO_DRIVE_STRENGTH_2MA);
  digitalWrite(phyduino::gpio::LED, HIGH);
  // Disable ADC GPIO pins digital input (Errata RP2040-E6, pg627 of datasheet)
  gpio_set_input_enabled(phyduino::gpio::A0, false);
  adc_gpio_init(phyduino::gpio::A0 );
  gpio_set_input_enabled(phyduino::gpio::A1, false);
  adc_gpio_init(phyduino::gpio::A1);
  gpio_set_input_enabled(phyduino::gpio::A2, false);
  adc_gpio_init(phyduino::gpio::A2);
}
/**
 * @brief Read USB Power Delivery (STUSB4500) Power Data Object.
 * @param n [in] Power Data Object (PDO) number.
 * @param usb [in,out] Reference to USB Power Delivery Driver.
 */
void read_usbpd_pdo(std::uint8_t n, STUSB4500& usb) noexcept {
    Serial.print("\nPDO : ");
    Serial.println(n);
    Serial.print("\tVoltage (V): ");
    Serial.println(usb.getVoltage(n));
    Serial.print("\tCurrent (A): ");
    Serial.println(usb.getCurrent(n));
    Serial.print("\tLower Voltage Tolerance (%): ");
    Serial.println(usb.getLowerVoltageLimit(n));
    Serial.print("\tUpper Voltage Tolerance (%): ");
    Serial.println(usb.getUpperVoltageLimit(n));
    Serial.println();
}
/**
 * @brief Sets PDO parameters
 * @param usb [in,out] Refrence to STUSB4500 class.
 * @param pdo [in] PDO number (1 - 3)
 * @param voltage [in] Requested voltage in volts
 * @param current [in] Requested current in amps
 * @param tolerence [in] Upper/Lower tolerence of the voltage (default 5%)
 */
void set_usbpd_pdo(STUSB4500& usb, std::uint8_t pdo, const float voltage, const float current, const std::uint8_t tolerence = 5) noexcept {
  // if the PDO number is great than available PDO
  if(pdo > usb.getPdoNumber()) {
    return;
  }

  // if pdo = 1 is always a fixed 5V cannot set voltage.
  if(pdo > 1) { 
    usb.setVoltage(pdo, voltage); 
    usb.setLowerVoltageLimit(pdo, tolerence);
  }

  usb.setCurrent(pdo, current);
  usb.setUpperVoltageLimit(pdo, tolerence);
}
/**
 * @brief Initializes USB-PD controller STUSB4500
 */
void initialize_usbpd() noexcept {
  pinMode(phyduino::usbpd::RESET, OUTPUT);
  digitalWrite(phyduino::usbpd::RESET, LOW)
  // Reset STUSB4500 USB-PD Controller
  digitalWrite(phyduino::usbpd::RESET, HIGH);
  delay(10);
  digitalWrite(phyduino::usbpd::RESET, LOW);

  // Set I2C pins and start I2C Driver.
  Wire1.setSCL(phyduino::usbpd::SCL);
  Wire1.setSDA(phyduino::usbpd::SDA);
  Wire1.begin();                              // Connected STUSB4500 to Pico.
  
  delay(500);

  if(!usb_pd.begin(phyduino::usbpd_address, Wire1)){
    #ifdef STUSB4500_DEBUG_LEVEL1
      Serial.println("Cannot connect to STUSB4500.");
      Serial.println("Are the I2C GPIO pins configured ? Is the device ID correct?");
      while(true){
        Serial.println("STUSB4500 initialization failed.");
        digitalWrite(phyduino::gpio::LED, HIGH);
        delay(2000);
        digitalWrite(phyduino::gpio::LED, HIGH);
        delay(2000);
      }
    #else
      while(1) {
        digitalWrite(phyduino::gpio::LED, HIGH);
        delay(2000);
        digitalWrite(phyduino::gpio::LED, HIGH);
        delay(2000);
      }
    #endif
  }
  // Configure Power requests
  #ifdef FORMAT_STUSB4500
    usb_pd.setPdoNumber(3);
    set_usbpd_pdo(usb_pd, 1, 5.0F, 0.2F);
    set_usbpd_pdo(usb_pd, 2, 9.0F, 1.1F);     // 10W power
    set_usbpd_pdo(usb_pd, 3, 20.0F, 0.5F);    // 10W power
    usb_pd.write();                           // Writes PDO settings to STUSB4500 NVM
  #endif

  usb_pd.read();                            // Reads PDO settings from STUSB4500 NVM

  #ifdef STUSB4500_DEBUG_LEVEL1
    Serial.println("Connected to STUSB4500!");
    delay(100);

    auto pdo_n = usb_pd.getPdoNumber();
    
    Serial.print("PDO Number: ");
    Serial.println(pdo_n);

    for(int i = 1; i <= pdo_n; i++){
      read_usbpd_pdo(i, usb_pd);
    }

  #endif
}
/**
 * @brief Initialize Board
 */
void phyduino::initialize_board() noexcept {
  initialize_gpio();          // Initializes ALL communication GPIO's
  initialize_usbpd();         // Initializes USB-PD controller.
  MAX11635_ADC.begin();       // Initializes External ADC.
}
/**
 * @brief Blinks D8 LED at fixed rate.
 */
void phyduino::led_heart_beat() noexcept {
  static auto last_blink = millis();
  pin_heart_beat(phyduino::gpio::LED, phyduino::led_delay, &last_blink);
}
/**
 * @brief Toggles Pin at fixed rate in milliseconds
 * @param idx [in] Pin index 
 * @param delay [in] Toggle Delay.
 * @param last [in] Pointer to last time index.
 */
void phyduino::pin_heart_beat(pin_t idx, ms_time_t delay, ms_time_t* last) noexcept {
  // Check if time delay has passed.
  if(auto time = millis(); time - *last > delay){
    // Toggle LED 
    auto IO_State = digitalRead(idx);

    digitalWrite(idx, IO_State == HIGH ? LOW : HIGH);

    // Save time to compare for next blink delay.
    *last = time;
  }
}
/**
 * @brief Converst Internal ADC value to voltage.
 * @param value [in] ADC output value.
 * @return float ADC measured voltage.
 */
float phyduino::to_voltage(std::uint16_t value) noexcept {
  return static_cast<float>(value) * phyduino::internal_adc_resolution;
}
/**
 * @brief Analog Read for the Phyduino Pico 
 * @param idx [in] Analog Pin index.
 * @return std::uint16_t ADC read value.
 */
std::uint16_t phyduino::analogRead(pin_t idx) noexcept {
  std::uint16_t value { 0 };

  if(idx >= phyduino::gpio::A0 && idx <= phyduino::gpio::A2){
    value = analogRead(idx);
  }else if(idx >= phyduino::gpio::A3 && idx <= phyduino::gpio::VBUS){
    std::uint8_t channel = static_cast<std::uint8_t>(idx) - phyduino::gpio::A3;
    value = MAX11635_ADC.analogRead(channel);
  }

  return value;
}
/**
 * @brief Get Voltage value of USB VBUS.
 * @return float USB VBUS voltage value.
 */
float phyduino::get_vbus() noexcept{
  auto value = phyduino::analogRead(phyduino::gpio::VBUS);                          // Get VBus ADC value 
  float voltage = max11635::driver::to_voltage(value) * phyduino::vbus_multiplier;  // Convert voltage.
  return voltage;
}
