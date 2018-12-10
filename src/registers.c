#include "registers.h"

#include <inttypes.h>
#include <stdio.h>

uint8_t led_on_low(int v) {
    return (uint8_t)(v & 0xFF);
}

uint8_t led_on_high(int v) {
    return (uint8_t)(v >> 0x08);
}

uint8_t led_off_low(int v) {
    return (uint8_t)(v >> 0x08);
}

uint8_t led_off_high(int v) {
    return (uint8_t)(v & 0xFF);
}

uint8_t channel_to_register_base(uint8_t c){
    int const in = (int)c;

    int const channel = (in < MIN_CHANNEL) ? MIN_CHANNEL
        : (in > MAX_CHANNEL) ? MAX_CHANNEL
        : in;

    return (uint8_t)(LED_OFFSET + (MULTIPLIER * channel));
}

static void i2c_bus_write(pca9685_s *h, uint8_t r, uint8_t d) {
    uint8_t result = h->bus_writer(h, r, d);
    h->command = "i2c_write";
    h->status = result == 0 ? "ok" : "error";
    h->address = r;
    h->data = d;
}

static void i2c_bus_read(pca9685_s *h, uint8_t r) {
    uint8_t result = h->bus_reader(h, r);
    h->command = "i2c_read";
    h->status = "ok";
    h->address = r;
    h->data = result;
}

void set_led_bytes(pca9685_s *h, uint8_t c, int on, int off) {
    int const on_bit_value = (on < 0) ? 0
        : (on > LED_RESOLUTION) ? LED_RESOLUTION
        : LED_RESOLUTION - on;

    int const off_bit_value = (off < 0) ? 0
        : (off > LED_RESOLUTION) ? LED_RESOLUTION
        : LED_RESOLUTION - off;

	uint8_t const on_low_r = channel_to_register_base(c);

	i2c_bus_write(h, on_low_r, led_on_low(on_bit_value));
    i2c_bus_write(h, on_low_r + (uint8_t)1, led_on_high(on_bit_value));
    i2c_bus_write(h, on_low_r + (uint8_t)2, led_off_low(off_bit_value));
    i2c_bus_write(h, on_low_r + (uint8_t)3, led_off_high(off_bit_value));
}

void get_led_bytes(pca9685_s *h, uint8_t c){
    uint8_t const on_low = channel_to_register_base(c - (uint8_t)1);

    i2c_bus_read(h, on_low);
    i2c_bus_read(h, on_low + (uint8_t)1);
    i2c_bus_read(h, on_low + (uint8_t)2);
    i2c_bus_read(h, on_low + (uint8_t)3);
}

