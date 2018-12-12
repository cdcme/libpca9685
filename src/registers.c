#include "registers.h"

#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

// for waiting 500μs
static struct timespec req, rem;

static void beat() {
    // set up our timespec structs upon first MODE1 register write
    if(req.tv_nsec != BEAT) {
        req.tv_sec = 0;
        req.tv_nsec = BEAT;
    }

    nanosleep(&req, &rem);
}

/** Userland I2C bus read/write callback wrappers */

void i2c_bus_read(pca9685_s *h, uint8_t r) {
    uint8_t result = h->bus_reader(h, r);
    h->command = "i2c_read";
    h->status = "ok";
    h->address = r;
    h->data = result;
}

void i2c_bus_write(pca9685_s *h, uint8_t r, uint8_t d) {
    uint8_t result = h->bus_writer(h, r, d);
    h->command = "i2c_write";
    h->status = result == 0 ? "ok" : "error";
    h->address = r;
    h->data = d;
}

/** Calculations */

uint8_t channel_to_register_base(uint8_t c){
    int const in = (int)c;

    int const channel = (in < MIN_CHANNEL) ? MIN_CHANNEL
        : (in > MAX_CHANNEL) ? MAX_CHANNEL
        : in;

    return (uint8_t)(LED_OFFSET + (MULTIPLIER * channel));
}

static uint8_t led_low(int v) {
    return (uint8_t)(v & 0xFF);
}

static uint8_t led_high(int v) {
    return (uint8_t)(v >> 0x08);
}

// Calculates prescale value from a PWM output frequency, in Hz
int calculate_prescale_from_frequency(int frequency){
    double prescale_value = round((OSCILLATOR / (LED_MAX_STEPS * frequency)) - 1);

    int prescale = (int)prescale_value;

    // Frequency range is 24Hz–1526Hz, or 3–255 (p. 25)
    return (prescale < 3) ? 3
        : (prescale > 255) ? 255
        : prescale;
}

/** Register operations */

void set_led_bytes(pca9685_s *h, int c, int on, int off) {
    uint8_t channel = (c == NONE) ? (uint8_t)ALL_LED_ON_L : channel_to_register_base((uint8_t)c);
    i2c_bus_write(h, channel, led_low(on));
    i2c_bus_write(h, channel + (uint8_t)1, led_high(on));
    i2c_bus_write(h, channel + (uint8_t)2, led_low(off));
    i2c_bus_write(h, channel + (uint8_t)3, led_high(off));
}

// Sets the value of the PRE_SCALE register with the provided output frequency
void set_pwm_frequency(pca9685_s *h, int frequency) {
    // Datasheet p. 25
    frequency = (frequency < 24) ? 24
        : frequency > 1526 ? 1526
        : frequency;

    const uint8_t prescale = (uint8_t)calculate_prescale_from_frequency(frequency);

    i2c_bus_write(h, MODE1, SLEEP);
    i2c_bus_write(h, PRESCALE, prescale);
    i2c_bus_write(h, MODE1, MODE1_NO_SLEEP);

    beat();
}

void set_pwm_duty_cycle(pca9685_s *h, int c, int d, int p) {
    // Datasheet pp. 17 & 25
    int percent_delay = (d < 1) ? 1
        : (d > 100) ? 100
        : d;

    int percent_on = (p < 0) ? 0
        : (p > 100) ? 100
        : p;

    int delay = (int)round(LED_MAX_STEPS * ((double)percent_delay / 100));
    int on_time = (int)round((percent_on * (double)LED_MAX_STEPS) / 100);

    int on_steps = delay - 1;
    int off_steps = (delay + on_time) - 1;

    if(off_steps > LED_MAX_STEPS) off_steps = (off_steps - LED_MAX_STEPS);

    set_led_bytes(h, c, on_steps, off_steps);
}

// Reset without having to power cycle (p. 15)
void reset_driver_soft(pca9685_s *h){
    // Set oscillator sleep bit
    i2c_bus_write(h, MODE1, SLEEP);

    // default duty cycle and frequency
    set_pwm_duty_cycle(h, NONE, 0, 0);
    set_pwm_frequency(h, 200);

    // check restart bit and reset; saves PWM register contents
    i2c_bus_read(h, MODE1);
    uint8_t value = h->data;
    if(value & RESTART) i2c_bus_write(h, MODE1, MODE1_NO_SLEEP);
    beat();
    i2c_bus_write(h, MODE1, RESTART);
}

void reset_driver_hard(pca9685_s *h) {
    i2c_bus_write(h, MODE1, SLEEP);

    /** Turn off all channels and reset frequency to default */
    set_pwm_duty_cycle(h, NONE, 0, 0);
    set_pwm_frequency(h, 200);

    /** Reset register default values */
    i2c_bus_write(h, SUBADR1, SUBADR1_DEFAULTS);
    i2c_bus_write(h, SUBADR2, SUBADR2_DEFAULTS);
    i2c_bus_write(h, SUBADR3, SUBADR3_DEFAULTS);
    i2c_bus_write(h, ALLCALLADR, ALLCALLADR_DEFAULTS);
    i2c_bus_write(h, MODE2, MODE2_DEFAULTS);
    i2c_bus_write(h, MODE1, MODE1_DEFAULTS);

    beat();
}

