#ifndef PCA9685_LIBRARY_H
#define PCA9685_LIBRARY_H

#include <inttypes.h>

/**
 * Main header file for the PCA9685 driver.
 * See https://www.nxp.com/docs/en/data-sheet/PCA9685.pdf
 * and ../tests/suites/test_start_state.c for additional information.
 */

// Conveniences for testing
#define ZERO 0x00
#define ONE  0x01

// Bus logic bits: 0 for write, 1 for read
#define BUS_ENABLED  ZERO
#define BUS_DISABLED ONE

// LED bits: 0 for completely off, 1 for completely on
#define LED_OFF ZERO
#define LED_ON  ONE

// Register definitions - all defaults are in base16,
// and all flags are expressed as bit shifts

// Mode registers
#define MODE1 ZERO
#define MODE2 ONE

// I2C-bus subaddress registers
#define SUBADR1 0x02
#define SUBADR2 0x03
#define SUBADR3 0x04

// I2C-bus subaddress register defaults
#define SUBADR1_DEFAULTS 0xe2
#define SUBADR2_DEFAULTS 0xe4
#define SUBADR3_DEFAULTS 0xe8

// LED All Call I2C-bus address register
#define ALLCALLADR 0x05

// LED All Call I2C-bus address register defaults
#define ALLCALLADR_DEFAULTS 0xe0

// LED 0 output and brightness control registers
#define LED0_ON_L  0x06
#define LED0_ON_H  0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09

// To calculate the remaining LED register addresses,
// we only need to LED0_ON_L + (channel * 4)
#define MAX_CHANNEL 15
#define MULTIPLIER   4

// Registers for loading all LED registers by byte
#define ALL_LED_ON_L  0xFA
#define ALL_LED_ON_H  0xFB
#define ALL_LED_OFF_L 0xFC
#define ALL_LED_OFF_H 0xFD

// Prescaler register for PWM output frequency
#define PRE_SCALE 0xFE

// Bits for Mode register 1: 8 bits total
#define ALLCALL 1<<0
#define SUB3    1<<1
#define SUB2    1<<2
#define SUB1    1<<3
#define SLEEP   1<<4
#define AI      1<<5
#define EXTCLK  1<<6
#define RESTART 1<<7

// Mode 1 register defaults
#define MODE1_DEFAULTS 0x88

// Bits for mode register 2: 8 bits total
// (bits 5–7 are unused)
#define OUTNE0 1<<0
#define OUTNE1 1<<1
#define OUTDRV 1<<2
#define OCH    1<<3
#define INVRT  1<<4

// Mode 2 register defaults
#define MODE2_DEFAULTS 0x20

// Function prototypes
void create_led_driver(uint8_t *address);
void create_all_call_bus(uint8_t *address);
void create_subcall_bus(uint8_t *bus_address, uint8_t register_address);
void create_mode_register(uint8_t *defaults, uint8_t register_address);

void turn_on_led(uint8_t led_number);
void turn_off_led(uint8_t led_number);

#endif
