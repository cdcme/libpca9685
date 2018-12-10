/**
 * \file libpca9685.h
 *
 * \brief Driver for the NXP Semiconductors PCA9685 16-channel 12-bit PWM I2C-bus LED Controller
 *
 * \see https://www.nxp.com/docs/en/data-sheet/PCA9685.pdf
 * \see https://github.com/torvalds/linux/blob/master/include/linux/i2c.h#L152
 */

#ifndef PCA9685_LIBRARY_H
#define PCA9685_LIBRARY_H

#include <inttypes.h>

/**
 * Usage:
 *
 * \code
 * #include "libpca9685.h"
 *
 * static uint8_t my_bus_byte_reader(pca9685_s *driver, register_address r) {
 *     // Your I2C read logic here
 *
 *    printf("\n%s", __func__);
 *    printf("\n===============\n");
 *    printf("Driver status: %s\n", driver->status);
 *    printf("Register address: %" PRIu8 "\n", r);
 *
 *    return 0;
 * }
 *
 * static uint8_t my_bus_byte_writer(pca9685_s *driver, register_address r, data_in d) {
 *     // Your I2C write logic here
 *
 *    printf("\n%s", __func__);
 *    printf("\n===============\n");
 *    printf("Driver status: %s\n", driver->status);
 *    printf("Register address: %" PRIu8 "\n", r);
 *
 *    return 0;
 * }
 *
 * static pca9685_s my_driver = pca9685(.bus_reader=my_bus_byte_reader, .bus_writer=my_bus_byte_writer);
 *
 * turn_on_led(&my_driver, 3);
 * printf("\nResult returned!\ncommand: %s\nstatus: %s\n", my_driver.command, my_driver.status);
 *
 * set_pwm_frequency(&my_driver, 25000);
 * printf("\nResult returned!\ncommand: %s\nstatus: %s\n", my_driver.command, my_driver.status);
 *
 * \endcode
 *
 * Output:
 *
 * \code
 *
 * turn_on_led
 * ===============
 * Driver status: ok
 * Register address: 0x13
 *
 * Result returned!
 * command: set_pwm_frequency
 * status: ok
 *
 * set_pwm_frequency
 * ===============
 * Driver status: ok
 * Register address: 0xFE
 *
 * Result returned!
 * command: set_pwm_frequency
 * status: ok
 *
 * \endcode
 */

/** Initializes a driver_handle struct with values provided by the user. */
#define pca9685(...) (configure_handle((pca9685_s){__VA_ARGS__}))

/** Primitive data type aliases for readability. */
typedef const char *command, *status;

/** Aggregate types */
struct driver_handle;

/**
 * Types for the I2C bus read and write callbacks.
 * Modeled on the I2C interface for Linux (\c i2c_smbus_read_byte_data, etc.)
 *
 * \see https://github.com/torvalds/linux/blob/master/include/linux/i2c.h#L152
 */
typedef uint8_t (*i2c_bus_read_cb)(struct driver_handle *, uint8_t);
typedef uint8_t (*i2c_bus_write_cb)(struct driver_handle *, uint8_t , uint8_t);

/** Type definition for the driver handle. */
typedef struct driver_handle {
    uint8_t data;                  // Data last read/written
    uint8_t address;               // Last register read to/written from
    command command;               // Last command attempted
    status status;                 // Status (ok) or error message
    i2c_bus_read_cb bus_reader;    // I2C Bus reader callback
    i2c_bus_write_cb bus_writer;   // I2C Bus writer callback
} pca9685_s;

/** Private, used by the macro defined above. */
pca9685_s configure_handle(pca9685_s);

/** Public API */

#endif
