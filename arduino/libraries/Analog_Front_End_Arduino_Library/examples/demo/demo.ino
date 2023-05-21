/**
 * @file demo.ino
 * @date 2023-05-10
 * @author Robert Morley (robert.morley@utoronto.ca)
 * 
 * @brief 
 * 
 * @version 1.0.0
 * @copyright Copyright (c) 2023
 */
#include "afe_shield.h"
#include <RotaryEncoder.h>  // https://github.com/mathertel/RotaryEncoder

// #define USE_INTERNAL_ADC            // Uses the Microcontroller ADC, COMMENT OUT for external ADC measurement.
#define ARUDUINO_SERIAL_PLOT        // Serial output is for Arduino Serial Plotter. COMMENT OUT for serial text output.

/*
 * Global Variable Declaration.
 */
static constexpr unsigned long sample_rate  { 100 };
static unsigned long t_sample { };
static constexpr unsigned long blink_rate   { 400 };
static unsigned long last_blink_green { };
static unsigned long last_blink_red { };
static uint8_t temp_ch_sel { 0 };

RotaryEncoder *encoder { nullptr };

/*
 * Interrupt Handler function prototypes.
 */
void irq_handler_enc_a();
void irq_handler_enc_b();
void irq_handler_enc_sw();

void setup(){
    Serial.begin();
    // Initializes Phyduino Pico Board.
    phyduino::initialize_board();

    // Set up LED's
    pinMode(afe_shield::io::GREEN_LED, OUTPUT);
    pinMode(afe_shield::io::RED_LED, OUTPUT);
    digitalWrite(afe_shield::io::RED_LED, (temp_ch_sel == 0? LOW : HIGH));
    digitalWrite(afe_shield::io::GREEN_LED, (temp_ch_sel == 1? LOW : HIGH));
    // Setup Rotary Encodes
    pinMode(afe_shield::io::ENC_A , INPUT);
    pinMode(afe_shield::io::ENC_B , INPUT);
    pinMode(afe_shield::io::ENC_SW, INPUT);

    encoder = new RotaryEncoder(afe_shield::io::ENC_A, afe_shield::io::ENC_B, RotaryEncoder::LatchMode::TWO03);

    // Register Interrupts
    attachInterrupt(digitalPinToInterrupt(afe_shield::io::ENC_A), irq_handler_enc_a, CHANGE);
    attachInterrupt(digitalPinToInterrupt(afe_shield::io::ENC_B), irq_handler_enc_b, CHANGE);
    attachInterrupt(digitalPinToInterrupt(afe_shield::io::ENC_SW), irq_handler_enc_sw, LOW);
}


void loop() {
    phyduino::led_heart_beat();

    /*
     * Used to execute a function at a periodic basis. 
     */
    afe_shield::periodic_ms(sample_rate, &t_sample, []()->bool { 
        // Read Analog Signals
        auto pot_value = phyduino::analog_read(afe_shield::io::POTENTIOMETER);
        auto pot_voltage = max11635::driver::to_voltage(pot_value);

        auto sensor_value = phyduino::analog_read(afe_shield::io::SENSOR_VOUT);
        auto sensor_vout  = max11635::driver::to_voltage(sensor_value);

        auto temp_sensor = afe_shield::get_temperature(afe_shield::io::TEMP_SENSOR, true);

        auto vbus = phyduino::get_vbus();

        float temp;

        if(temp_ch_sel == 0) {
            temp = afe_shield::get_temperature(afe_shield::io::TEMPERATURE0);
        }
        else {
            temp = afe_shield::get_temperature(afe_shield::io::TEMPERATURE1);
        }

        #ifdef ARUDUINO_SERIAL_PLOT
            Serial.printf("TEMP_SENSOR(°C):%.1f,TEMPERATURE(°C):%.1f,POTENTIOMETER(V):%.3f,SENSOR_INPUT(V):%.3f,VBUS(V):%.2f, ENC_POS:%d\r\n", temp_sensor, temp, pot_voltage, sensor_vout, vbus, encoder->getPosition());
        #else
            Serial.printf("Analog Voltages\r\n");
            Serial.printf("\tTemperature Sensor output: %.1f°C\r\n", temp_sensor);
            Serial.printf("\tTemperature: %.1f°C\r\n", temp);
            Serial.printf("\tPotentiometer: %.3fV\r\n", pot_voltage);
            Serial.printf("\tSensor Input: %.3fV\r\n", sensor_vout);
            Serial.printf("\tVBus: %.2fV\r\n", vbus);
        #endif

        return true;
    });

    // Indicate which Temperature Channel is selected.
    digitalWrite(afe_shield::io::RED_LED, (temp_ch_sel == 0? LOW : HIGH));
    digitalWrite(afe_shield::io::GREEN_LED, (temp_ch_sel == 1? LOW : HIGH));
}

void irq_handler_enc_a() {
    encoder->tick();
}

void irq_handler_enc_b() {
    encoder->tick();
}

void irq_handler_enc_sw() {
    temp_ch_sel = temp_ch_sel == 0? 1 : 0;
}