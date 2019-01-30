/**
 * \file libpca9685.h
 *
 * \brief Driver for the NXP Semiconductors PCA9685 16-channel 12-bit PWM I2C-bus LED Controller
 *
 * \see https://www.nxp.com/docs/en/data-sheet/PCA9685.pdf
 * \see https://github.com/torvalds/linux/blob/master/include/linux/i2c.h#L152
 *
 * Usage:
 *
 * \code
 * // Set up your driver, passing your I2C read and write callbacks
 * my_driver = pca9685(.bus_reader=my_bus_reader, .bus_writer=my_bus_writer);
 *
 * // Set frequency to 250Hz
 * my_driver.set_frequency(&my_driver, 250);
 *
 * // Set PWM duty cycle on LED channel 5 to 25%
 * my_driver.set_duty_cycle(&my_driver, 5, 1, 25);
 *
 * // Turn LED channel on
 * my_driver.channel_on(&my_driver, 5);
 *
 * // Turn LED channel off
 * my_driver.channel_off(&my_driver, 5);
 *
 * // Turn all LEDs on
 * my_driver.channel_on(&my_driver, -1);
 *
 * // Turn all LEDs off
 * my_driver.channel_off(&my_driver, -1);
 *
 * // Set PWM duty cycle on all LED channels to 50%
 * my_driver.set_duty_cycle(&my_driver, -1, 1, 50);
 *
 * // Contains the data last read or written
 * uint8_t my_data = my_driver.data(&my_driver);
 *
 * // Contains the last register address read to or written from
 * uint8_t my_address = my_driver.address(&my_driver);
 *
 * // Contains the last command attempted
 * char *my_command = my_driver.command(&my_driver);
 *
 * // Contains the last command's status or an error message
 * char *my_status = my_driver.status(&my_driver);
 * \endcode
 */

#ifndef PCA9685_LIBRARY_H
#define PCA9685_LIBRARY_H

#include <inttypes.h>

/** Initializes a driver_handle struct with values provided by the user. */
#define pca9685(...) (pca9685_configure_handle((pca9685_s){__VA_ARGS__}))

/** Primitive data type aliases for readability & conciseness */
typedef char *string;
typedef uint8_t u8;

/** Aggregate types */
struct pca9685_driver;

/**
 * Types for the I2C bus read and write callbacks.
 * Modeled on the I2C interface for Linux (\c i2c_smbus_read_byte_data, etc.)
 *
 * \see https://github.com/torvalds/linux/blob/master/include/linux/i2c.h#L152
 */
typedef u8 (*pca9685_i2c_bus_read_cb)(struct pca9685_driver *driver, u8 address);
typedef u8 (*pca9685_i2c_bus_write_cb)(struct pca9685_driver *driver, u8 address, u8 data);

/** Public API function signatures */
typedef void (*pca9685_fn)(struct pca9685_driver *driver);
typedef void (*pca9685_chan_freq_fn)(struct pca9685_driver *driver, int chan_or_freq);
typedef void (*pca9685_duty_cycle_fn)(struct pca9685_driver *, int led_channel, int delay_percent, int percent_on);

/** Type definition for the driver handle */
typedef struct pca9685_driver {
    u8 data;                               // Data last read/written
    u8 address;                            // Last register read to/written from
    string command;                        // Last command attempted
    string status;                         // Status (ok) or error message
    pca9685_i2c_bus_read_cb bus_reader;    // I2C Bus reader callback
    pca9685_i2c_bus_write_cb bus_writer;   // I2C Bus writer callback
    pca9685_fn soft_reset;                 // Attempts an orderly shutdown, saving register values
    pca9685_fn hard_reset;                 // Resets to manufacturer defaults
    pca9685_chan_freq_fn set_frequency;    // Set output frequency; default is 200Hz
    pca9685_chan_freq_fn channel_on;       // Set LED channel on; ALL (-1) acts on all channels
    pca9685_chan_freq_fn channel_off;      // Set LED channel off; ALL (-1) acts on all channels
    pca9685_duty_cycle_fn set_duty_cycle;  // Set duty cycle % and delay % for more control; ALL (-1) acts on all channels
} pca9685_s;

/** Private, used by the macro defined above. */
pca9685_s pca9685_configure_handle(pca9685_s);

#endif
