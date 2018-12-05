#include "libpca9685.h"

#include <stdio.h>

static uint8_t *register_address;

void create_led_driver(uint8_t *address) {
	register_address = address;
	*register_address = LED_OFF;
}

void create_all_call_bus(uint8_t *address) {
	*address = BUS_ENABLED;
}

void turn_on_led(uint8_t led_number) {
	(void)led_number;
	*register_address = LED_ON;
}

void turn_off_led(uint8_t led_number) {
	(void)led_number;
	*register_address = LED_OFF;
}

void create_subcall_bus(uint8_t *bus_address, uint8_t register_address) {
	switch (register_address) {

		case SUBADR1 :
			*bus_address = SUBADR1_DEFAULTS;
			break;
		case SUBADR2:
			*bus_address = SUBADR2_DEFAULTS;
			break;
		case SUBADR3:
			*bus_address = SUBADR3_DEFAULTS;
			break;
		default :
			*bus_address = BUS_DISABLED;
	}
}

void create_mode_register(uint8_t *defaults, uint8_t register_address){
	switch (register_address) {
		case MODE1 :
			*defaults = MODE1_DEFAULTS;
			break;
		case MODE2 :
			*defaults = MODE2_DEFAULTS;
			break;
		default :
			*defaults = BUS_DISABLED;
	}
}
