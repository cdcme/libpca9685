#include "tests.h"

#include <stdint.h>

mock_register_s mock_driver;

void set_up_mock_driver(pca9685_s *driver) {
    *driver = pca9685(.bus_reader=mock_bus_reader, .bus_writer=mock_bus_writer);
}

void set_up_theft_run_config_u8(struct theft_run_config *config) {
    theft_seed seed = theft_seed_of_time();

    *config = (struct theft_run_config){
        .type_info = {
            theft_get_builtin_type_info(THEFT_BUILTIN_uint8_t)
        },
        .seed = seed,
        .trials = 100
    };
}

void set_up_theft_run_config_int(struct theft_run_config *config) {
    theft_seed seed = theft_seed_of_time();

    *config = (struct theft_run_config){
        .type_info = {
            theft_get_builtin_type_info(THEFT_BUILTIN_int)
        },
        .seed = seed,
        .trials = 100
    };
}

void set_up_theft_run_config_int_int(struct theft_run_config *config) {
    theft_seed seed = theft_seed_of_time();

    *config = (struct theft_run_config){
        .type_info = {
            theft_get_builtin_type_info(THEFT_BUILTIN_int),
            theft_get_builtin_type_info(THEFT_BUILTIN_int)
        },
        .seed = seed,
        .trials = 100
    };
}

void set_up_theft_run_config_u8_int_int(struct theft_run_config *config) {
    theft_seed seed = theft_seed_of_time();

    *config = (struct theft_run_config){
        .type_info = {
            theft_get_builtin_type_info(THEFT_BUILTIN_uint8_t),
            theft_get_builtin_type_info(THEFT_BUILTIN_int),
            theft_get_builtin_type_info(THEFT_BUILTIN_int)
        },
        .seed = seed,
        .trials = 100
    };
}

u8 mock_bus_reader(pca9685_s *driver, u8 address) {
    (void)driver;

    mock_driver.address = address;
    return mock_driver.value;
}

u8 mock_bus_writer(pca9685_s *driver, u8 address, u8 data_in) {
    (void)driver;

    mock_driver.address = address;
    mock_driver.value = data_in;

    return 0;
}

