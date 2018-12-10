#include "libpca9685.h"
#include "registers.h"

#include <stdio.h>
#include <stdlib.h>

pca9685_s configure_handle(pca9685_s driver){
    if(!(driver.bus_reader && driver.bus_writer)){
        driver.command = "cb_check";
        driver.status = "Invalid input: bus reader and bus writer callbacks are both required.";
    } else {
        driver.command = "rw_check";

        // Quick & dirty read/write check; only verifies the interface!
        uint8_t r = driver.bus_reader(&driver, LED0_ON_L);
        uint8_t w = driver.bus_writer(&driver, LED0_ON_L, r);

        // A read from register LED0_ON_L should only ever return 0x00 on init, since LEDs
        // are off by default, and a successful write should always return 0 to our caller.
        int rw = (r == LED_OFF && w == OK);

        if(!rw) driver.status = "Invalid configuration: cannot read and/or write via provided callbacks.";

        driver.address = LED0_ON_L;
        driver.data = r;
    }

    driver.command = driver.command == NULL ? "init" : driver.command;
    driver.status = driver.status == NULL ? "ok" : driver.status;

    return driver;
}
