#include "libpca9685.h"
#include "registers.h"

#include <stdio.h>

pca9685_s pca9685_configure_handle(pca9685_s driver){
    if(!(driver.bus_reader && driver.bus_writer)){
        driver.command = "cb_check";
        driver.status = "Invalid input: bus reader and bus writer callbacks are both required.";
    } else {
        driver.command = "rw_check";

        // Quick & dirty read/write checks; only verifies the interface, should break on the caller's side.
        i2c_bus_read(&driver, LED0_ON_L);
        i2c_bus_write(&driver, LED0_ON_L, driver.data);
    }

    driver.command = driver.command == NULL ? "init" : driver.command;
    driver.status = driver.status == NULL ? "ok" : driver.status;

    return driver;
}

void set_frequency(pca9685_s *h, int frequency) {
    set_pwm_frequency(h, frequency);
}

void set_duty_cycle(pca9685_s *h, int channel, int delay, int percent) {
   set_pwm_duty_cycle(h, channel, delay, percent);
}

void channel_on(pca9685_s *h, int channel) {
    set_duty_cycle(h, channel, 1, 100);
}

void channel_off(pca9685_s *h, int channel) {
    set_duty_cycle(h, channel, 1, 0);
}

void soft_reset(pca9685_s *h) {
    reset_driver_soft(h);
}

void hard_reset(pca9685_s *h) {
    reset_driver_hard(h);
}

