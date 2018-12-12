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

/** Type definition for the driver handle */
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

/** Sets the output frequency; default is 200Hz. */
void set_frequency(pca9685_s *, int frequency);

/** Sets the duty cycle. Use this if channel on/off are not enough for your application.
 *  If channel is NONE, this is set for all channels. Delay and percent are both % values. */
void set_duty_cycle(pca9685_s *, int channel, int delay, int percent);

/** Turn on/off LEDs or motors by channel. If channel is NONE, acts on all channels. */
void channel_on(pca9685_s *, int channel);
void channel_off(pca9685_s *, int channel);

/** Resets. Soft attempts an orderly reset, which saves register values. Hard resets to manufacturer defaults. */
void soft_reset(pca9685_s *);
void hard_reset(pca9685_s *);

/** more to come... */

#endif
