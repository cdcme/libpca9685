#include "registers.h"

#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

// for waiting 500μs
static struct timespec req, rem;

static void beat() {
    // set up our timespec structs
    if(req.tv_nsec != BEAT) {
        req.tv_sec = 0;
        req.tv_nsec = BEAT;
    }

    nanosleep(&req, &rem);
}

/** Userland I2C bus read/write callback wrappers */

void pca9685_i2c_bus_read(pca9685_s *h, u8 r) {
    u8 result = h->bus_reader(h, r);
    h->command = "i2c_read";
    h->status = "ok";
    h->address = r;
    h->data = result;
}

void pca9685_i2c_bus_write(pca9685_s *h, u8 r, u8 d) {
    u8 result = h->bus_writer(h, r, d);
    h->command = "i2c_write";
    h->status = result == 0 ? "ok" : "error";
    h->address = r;
    h->data = d;
}

/** Calculations */

u8 channel_to_register_base(u8 c){
    int const in = (int)c;

    int const channel = (in < MIN_CHANNEL) ? MIN_CHANNEL
        : (in > MAX_CHANNEL) ? MAX_CHANNEL
        : in;

    return (u8)(LED_OFFSET + (MULTIPLIER * channel));
}

static u8 led_low(int v) {
    return (u8)(v & 0xFF);
}

static u8 led_high(int v) {
    return (u8)(v >> 0x08);
}

// Calculates prescale value from a PWM output frequency, in Hz
int calculate_prescale_from_frequency(int frequency){
    // Frequency range is 24Hz–1526Hz, or 3–255 (p. 25)
    frequency = (frequency < FREQ_MIN) ? FREQ_MIN
            : (frequency > FREQ_MAX) ? FREQ_MAX
            : frequency;

    double prescale_value = round((OSCILLATOR / (LED_MAX_STEPS * frequency)) - 1);

    return (int)prescale_value;
}

// Calculate delay from percentage
int calculate_delay_from_percentage(int d) {
    // Datasheet pp. 17 & 25
    int percent_delay = (d < 1) ? 1
        : (d > 100) ? 100
        : d;

    return (int)round(LED_MAX_STEPS * ((double)percent_delay / 100));
}

// Calculate on time from percentage
int calculate_on_time_from_percentage(int p) {
    // Datasheet pp. 17 & 25
    int percent_on = (p < 0) ? 0
        : (p > 100) ? 100
        : p;

    return (int)round((percent_on * (double)LED_MAX_STEPS) / 100);
}

// calculate off steps from delay and on time
int calculate_off_steps_from_delay_and_on_time(int d, int t) {
   int off_steps = (d + t) - 1;
   if(off_steps > LED_MAX_STEPS) off_steps = (off_steps - LED_MAX_STEPS);

   return off_steps;
}

/** Register operations */

void set_led_bytes(pca9685_s *h, int c, int on, int off) {
    u8 channel = (c == ALL) ? (u8)ALL_LED_ON_L : channel_to_register_base((u8)c);
    pca9685_i2c_bus_write(h, channel, led_low(on));
    pca9685_i2c_bus_write(h, channel + (u8)1, led_high(on));
    pca9685_i2c_bus_write(h, channel + (u8)2, led_low(off));
    pca9685_i2c_bus_write(h, channel + (u8)3, led_high(off));
}

// Sets the value of the PRE_SCALE register with the provided output frequency
void set_pwm_frequency(pca9685_s *h, int frequency) {
    // Datasheet p. 25
    frequency = (frequency < FREQ_MIN) ? FREQ_MIN
        : frequency > FREQ_MAX ? FREQ_MAX
        : frequency;

    const u8 prescale = (u8)calculate_prescale_from_frequency(frequency);

    pca9685_i2c_bus_write(h, MODE1, SLEEP);
    pca9685_i2c_bus_write(h, PRE_SCALE, prescale);
    pca9685_i2c_bus_write(h, MODE1, MODE1_NO_SLEEP);

    beat();
}

void set_pwm_duty_cycle(pca9685_s *h, int c, int d, int p) {
    int delay = calculate_delay_from_percentage(d);
    int on_time = calculate_on_time_from_percentage(p);

    int on_steps = delay - 1;
    int off_steps = calculate_off_steps_from_delay_and_on_time(delay, on_time);

    set_led_bytes(h, c, on_steps, off_steps);
}

// Reset without having to power cycle (p. 15)
void reset_driver_soft(pca9685_s *h){
    // Set oscillator sleep bit
    pca9685_i2c_bus_write(h, MODE1, SLEEP);

    // default duty cycle and frequency
    set_pwm_duty_cycle(h, ALL, 0, 0);
    set_pwm_frequency(h, 200);

    // check restart bit and reset; saves PWM register contents
    pca9685_i2c_bus_read(h, MODE1);
    u8 value = h->data;
    if(value & RESTART) pca9685_i2c_bus_write(h, MODE1, MODE1_NO_SLEEP);
    beat();
    pca9685_i2c_bus_write(h, MODE1, RESTART);
}

void reset_driver_hard(pca9685_s *h) {
    pca9685_i2c_bus_write(h, MODE1, SLEEP);

    /** Turn off all channels and reset frequency to default */
    set_pwm_duty_cycle(h, ALL, 0, 0);
    set_pwm_frequency(h, 200);

    /** Reset register default values */
    pca9685_i2c_bus_write(h, SUBADR1, SUBADR1_DEFAULTS);
    pca9685_i2c_bus_write(h, SUBADR2, SUBADR2_DEFAULTS);
    pca9685_i2c_bus_write(h, SUBADR3, SUBADR3_DEFAULTS);
    pca9685_i2c_bus_write(h, ALLCALLADR, ALLCALLADR_DEFAULTS);
    pca9685_i2c_bus_write(h, MODE2, MODE2_DEFAULTS);
    pca9685_i2c_bus_write(h, MODE1, MODE1_DEFAULTS);

    beat();
}

