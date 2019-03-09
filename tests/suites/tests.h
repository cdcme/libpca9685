#ifndef TESTS_H
#define TESTS_H

#include "greatest.h"
#include "theft.h"

#include "pca9685.h"
#include "registers.h"

typedef struct mock_register {
    u8 address;
    u8 value;
} mock_register_s;

void set_up_mock_driver(pca9685_s *);
void set_up_theft_run_config_u8(struct theft_run_config *config);
void set_up_theft_run_config_int(struct theft_run_config *config);
void set_up_theft_run_config_int_int(struct theft_run_config *config);
void set_up_theft_run_config_u8_int_int(struct theft_run_config *config);
u8 mock_bus_reader(pca9685_s *, u8 address);
u8 mock_bus_writer(pca9685_s *, u8 address, u8 data_in);

#endif
