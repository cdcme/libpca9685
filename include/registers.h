#ifndef PCA9685_REGISTERS_H
#define PCA9685_REGISTERS_H

#include <inttypes.h>
#include "libpca9685.h"

/** Conveniences */
#define ZERO 0x00
#define LOW  ZERO
#define OK   ZERO

#define ONE  0x01
#define HIGH ONE
#define ERR  ONE

/** For flags */
#define ALL -1

/** Bus logic bits: 0 for write/enabled, 1 for read/disabled */
#define BUS_ENABLED  ZERO
#define BUS_DISABLED ONE

/** LED bits: 0 for completely off, 1 for completely on */
#define LED_OFF LOW
#define LED_ON  HIGH

/** The PCA9685 has 12-bit resolution, or 4096 (0–4095) steps from full off to full on */
#define LED_MAX_BITS  4095
#define LED_MAX_STEPS (LED_MAX_BITS + 1)

/** Oscillator clock frequency is 25MHz */
#define OSCILLATOR 25000000.0

/** Osclillator takes 500μs (500000ns) to switch states */
#define BEAT 500000

/* To calculate the remaining LED register addresses for a single channel,
 * we only need to calculate the base and add 1–3. To calculate the base, LED_OFFSET + (MULTIPLIER * channel).
 *
 * \see registers.c */
#define LED_OFFSET         6
#define MAX_CHANNEL       15
#define MIN_CHANNEL        0
#define MULTIPLIER         4
#define PRESCALE_MIN       3
#define PRESCALE_MAX     255
#define FREQ_MIN          24
#define FREQ_MAX        1526

/** Register definitions - all defaults are in base16, and all flags are expressed as bit shifts */

/** Mode registers */
#define MODE1 ZERO
#define MODE2 ONE

/** I2C-bus subaddress registers */
#define SUBADR1 0x02
#define SUBADR2 0x03
#define SUBADR3 0x04

/* I2C-bus subaddress register defaults */
#define SUBADR1_DEFAULTS 0xe2
#define SUBADR2_DEFAULTS 0xe4
#define SUBADR3_DEFAULTS 0xe8

/* LED All Call I2C-bus address register */
#define ALLCALLADR 0x05

/* LED All Call I2C-bus address register defaults */
#define ALLCALLADR_DEFAULTS 0xe0

/* LED 0 output and brightness control registers */
#define LED0_ON_L 0x06

/** Registers for loading all LED registers by byte */
#define ALL_LED_ON_L  0xFA

/** Prescaler register for PWM output frequency */
#define PRE_SCALE 0xFE

/** Bits for Mode register 1: 8 bits total */
#define ALLCALL 1<<0
#define SUB3    1<<1
#define SUB2    1<<2
#define SUB1    1<<3
#define SLEEP   1<<4
#define AI      1<<5
#define EXTCLK  1<<6
#define RESTART 1<<7

/** Mode 1 register default and sleep disabled settings */
#define MODE1_DEFAULTS 0x88
#define MODE1_NO_SLEEP 0x01

/** Bits for mode register 2: 8 bits total (bits 5–7 are unused) */
#define OUTNE0 1<<0
#define OUTNE1 1<<1
#define OUTDRV 1<<2
#define OCH    1<<3
#define INVRT  1<<4

/** Mode 2 register defaults */
#define MODE2_DEFAULTS 0x40

/** Channel to register base conversion.
 * Takes a zero-based channel (like 10) and returns the first register (0x32). */
uint8_t channel_to_register_base(uint8_t channel);

/* Calculations */
int calculate_prescale_from_frequency(int frequency);
int calculate_delay_from_percentage(int delay);
int calculate_on_time_from_percentage(int percent);
int calculate_off_steps_from_delay_and_on_time(int delay, int on_time);

/** Low-level access to user callback wrappers for initialization */
void pca9685_i2c_bus_read(pca9685_s *, uint8_t r);
void pca9685_i2c_bus_write(pca9685_s *, uint8_t r, uint8_t d);

/** Low-level access to register writes for testing */
void set_led_bytes(pca9685_s *, int c, int on, int off);

/** Reset driver state */
void reset_driver_soft(pca9685_s *h);
void reset_driver_hard(pca9685_s *h);

/** Sets the value of the PRE_SCALE register with the provided PWM output frequency */
void set_pwm_frequency(pca9685_s *h, int frequency);

/** Sets the PWM duty cycle with a % delay at a provided % on time */
void set_pwm_duty_cycle(pca9685_s *h, int channel, int delay, int percent);

#endif

