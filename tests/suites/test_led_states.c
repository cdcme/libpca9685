#include "tests.h"

/**
 * This suite tests LED states for the PCA9685.
 */

static uint8_t mock_led;

static void setup_cb(void *data) {
	create_led_driver(&mock_led);
}

static void teardown_cb(void *data) {
	
}

// Turn an LED completely on
TEST led_full_on(void) {
	turn_on_led(1);

	ASSERT_EQ_FMT(1, mock_led, "%X");

	PASS();
}

// Turn an LED completely off
TEST led_full_off(void) {
	turn_on_led(1);
	turn_off_led(1);

	ASSERT_EQ_FMT(0, mock_led, "%X");

	PASS();
}

SUITE(test_led_states) {
	SET_SETUP(setup_cb, NULL);
	SET_TEARDOWN(teardown_cb, NULL);

	RUN_TEST(led_full_on);
	RUN_TEST(led_full_off);
}
