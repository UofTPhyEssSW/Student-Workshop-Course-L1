/**
 * @file max11635.cpp
 * @date 2023-03-26
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief Analog Devices (formerly Maxium Integrated) MAX11635 class source code.
 * 
 * @version 1.0.0
 * @copyright Copyright (c) 2023
 */
#include "max11635.h"

/**
 * @brief Configures MAX11635 IO pins
 * 
 * @param mosi [in] SPI module MOSI pin GPIO number.
 * @param miso [in] SPI module MISO pin GPIO number.
 * @param sck [in] SPI module SCK pin GPIO number.
 * @param cs [in] ADC chipselect pin GPIO Number.
 * @param cnvst [in] ADC nCNVST pin GPIO Number.
 * @param eoc [in] ADC nEOC pin GPIO Number.
 */
void ADC_MAX11635::configure_io(pin_t mosi, pin_t miso, pin_t sck, pin_t cs, pin_t cnvst, pin_t eoc) noexcept {
  _mosi = mosi;
  _miso = miso;
  _sck = sck;
  _cs = cs;
  _n_cnvst = cnvst;
  _n_eoc = eoc;
}
/**
 * @brief Initializes MAX11635 ADC interface and device initialization.
 * @param settings [in] reference SPI Settings
 */
void ADC_MAX11635::begin(const arduino::SPISettings& settings) noexcept {
  _settings = settings;
  initialize();
}
/**
 * @brief Initializes MAX11635 ADC interface and device initialization.
 */
void ADC_MAX11635::begin() noexcept {
  // Configure and initialize Interface GPIO's
  pinMode(_n_eoc, INPUT_PULLUP);
  pinMode(_n_cnvst, OUTPUT);
  digitalWrite(_n_cnvst, HIGH);
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);
  pinMode(_mosi, OUTPUT);

  // Configure SPI Module IO's 
  _bus.setRX(static_cast<pin_size_t>(_mosi));
  _bus.setTX(static_cast<pin_size_t>(_miso));
  _bus.setSCK(static_cast<pin_size_t>(_sck));

  // Start SPI Module interface.
  _bus.begin();

  config_regs();
}

void ADC_MAX11635::reset() noexcept {
  _regs.reset.b.RESET = 0b1;
  write_nbytes(&_regs.reset.w);
}
/**
 * @brief Configures MAX11635 ADC internal Registers.
 */
void ADC_MAX11635::config_regs() noexcept {
  // Reset Register
  reset();
  // Conversion Register
  _regs.conversion.b.CHSEL = 0b000U;    // AIN1 selected.
  _regs.conversion.b.SCAN  = 0b11U;     // No scan selection
  write_nbytes(&_regs.conversion.w);
  // Setup Register
  _regs.setup.b.REFSEL  = 0b01U;        // Voltage reference External single-ended
  _regs.setup.b.DIFFSEL = 0b10U;        // 1 byte of data follows the setup byte and is written to the unipolar mode register.
  _regs.unipolar.w      = 0x00U;        // 1 to configure AIN0 - AIN3 for unipolar differential conversion
  std::uint8_t setup[2] { _regs.setup.w, _regs.unipolar.w };
  write_nbytes(setup, sizeof(setup));
  // Averaging Register
  _regs.averaging.b.AVGON = 0b0U;       // Averaging turned off.
  _regs.averaging.b.NAVG  = 0b00U;
  _regs.averaging.b.NSCAN = 0b00U;
  write_nbytes(&_regs.averaging.w);
}
/**
 * @brief Stops MAX11635 interface.
 */
void ADC_MAX11635::end() noexcept {
  reset();    // Reset ADC.
  _bus.end(); // Stop SPI interface.
}
/**
 * @brief Read Analog Channel on the MAX11635 ADC.
 * @param analog_pin [in] ADC Analog in channel (0 to 4)
 * @return ADC_MAX11635::data_type If channel selected is outside of channel range will return 0, else the ADC conversion value.
 */
ADC_MAX11635::data_type ADC_MAX11635::analogRead(const pin_t analog_pin) noexcept {
  data_type retval { 0 };

  if(analog_pin < 4){
    // Configrue Analog Channel Read
    _regs.conversion.b.CHSEL = analog_pin;    // Set channel to read.
    _regs.conversion.b.SCAN  = 0b11U;         // Set scan mode to none.
    write_nbytes(&_regs.conversion.w);        // Write to ADC.

    // Start Analog to Digital conversion.
    digitalWrite(_n_cnvst, LOW);
    digitalWrite(_n_cnvst, HIGH);
    // Wait for conversion to finish
    while(digitalRead(_n_eoc) == HIGH);
    // Read conversion value.
    retval = get_analog_value();
  }

  return retval;
}
/**
 * @brief Gets the voltage on the requested channel
 * 
 * @param analog_pin [in] ADC Analog channel (0 - 4)
 * @return float Voltage on the channel selected.
 */
float ADC_MAX11635::get_voltage(const pin_t analog_pin) noexcept {
  auto val = analogRead(analog_pin);
  return to_voltage(val);
}
/**
 * @brief Reads analog data from the MAX11635.
 * @return ADC_MAX11635::data_type 16bit data from ADC channel.
 */
ADC_MAX11635::data_type ADC_MAX11635::get_analog_value() noexcept {
  // Read 2 bytes from ADC
  std::uint8_t adc_rd[2] { };
  read_nbytes(adc_rd, sizeof(adc_rd));
  // Convert to bytes to 16bit data
  auto analog_val = static_cast<data_type>(adc_rd[0]) << 8U;
  analog_val |= static_cast<data_type>(adc_rd[1]);
  
  return analog_val;
}
/**
 * @brief Converts the ADC value returned to voltage.
 * @param val [in] Analog count value from ADC.
 * @return float 
 */
float ADC_MAX11635::to_voltage(const data_type val) noexcept {
  return static_cast<float>(val) * ADC_MAX11635::V_RES;
}
/**
 * @brief Writes N number of bytes to the MAX11635 ADC.
 * 
 * @param buf [in] Pointer to write buffer.
 * @param n [in] Number of bytes to be written.
 */
void ADC_MAX11635::write_nbytes(std::uint8_t* buf, const std::size_t n) noexcept {
  _bus.beginTransaction();
  digitalWrite(_cs, LOW);   // Select ADC. 

  _bus.transfer(buf, n);    // Write data to ADC.

  digitalWrite(_cs, HIGH);  // Deselect ADC.
  _bus.endTransaction();
}
/**
 * @brief Read N number of bytes from the ADC.
 * 
 * @param buf [out] Pointer to read buffer.
 * @param n [in] Number of bytes to be read.
 */
void ADC_MAX11635::read_nbytes(std::uint8_t* buf, const std::size_t n) noexcept {
  _bus.beginTransaction();
  digitalWrite(_cs, LOW);   // Select ADC. 

  for(std::size_t i = 0; i < n; i++){
    *buf = _bus.transfer(ADC_MAX11635::DUMMY_BYTE); // Read byte from MAX11635
    buf++;                                          // Next memory space.
  }

  digitalWrite(_cs, HIGH);  // Deselect ADC.
  _bus.endTransaction();  
}
