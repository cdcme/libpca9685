#include "pca9685.h"
#include "registers.h"

#include <stdio.h>

static void set_frequency(pca9685_s *h, int frequency) {
    set_pwm_frequency(h, frequency);
}

static void set_duty_cycle(pca9685_s *h, int channel, int delay, int percent) {
   set_pwm_duty_cycle(h, channel, delay, percent);
}

static void channel_on(pca9685_s *h, int channel) {
    set_pwm_duty_cycle(h, channel, 0, 100);
}

static void channel_off(pca9685_s *h, int channel) {
    set_pwm_duty_cycle(h, channel, 0, 0);
}

static void soft_reset(pca9685_s *h) {
    reset_driver_soft(h);
}

static void hard_reset(pca9685_s *h) {
    reset_driver_hard(h);
}

pca9685_s pca9685_configure_handle(pca9685_s handle){
    if(!(handle.bus_reader && handle.bus_writer)){
        handle.command = "cb_check";
        handle.status = "Invalid input: bus reader and bus writer callbacks are both required.";
    } else {
        handle.command = "rw_check";

        // Quick & dirty read/write checks; only verifies the interface, should break on the caller's side.
        pca9685_i2c_bus_read(&handle, LED0_ON_L);
        pca9685_i2c_bus_write(&handle, LED0_ON_L, handle.data);
    }

    handle.soft_reset = soft_reset;
    handle.hard_reset = hard_reset;
    handle.set_frequency = set_frequency;
    handle.channel_on = channel_on;
    handle.channel_off = channel_off;
    handle.set_duty_cycle = set_duty_cycle;

    handle.command = handle.command == NULL ? "init" : handle.command;
    handle.status = handle.status == NULL ? "ok" : handle.status;

    return handle;
}

