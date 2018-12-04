#include "tests.h"

/**
 * This suite tests the PCA9685 driver's initial START state.
 *
 * All slave addresses are one 8-bit byte. The first bit
 * is fixed (1), and the last bit is the logic bit.
 * If it is set to 1, it selects a read, and a 0 selects a
 * write. Thus, an address base + 1 also means "disabled",
 * when referring to a bus address, and 0 means completely
 * off for LEDs.
 */

TEST leds_are_off(void) {
	uint8_t mock_led = LED_ON;
	create_led_driver(&mock_led);

	ASSERT_EQ_FMT(LED_OFF, mock_led, "%X");

	PASS();
}

TEST all_call_bus_is_enabled(void) {
	uint8_t mock_all_call_bus = BUS_DISABLED;
	create_all_call_bus(&mock_all_call_bus);

	ASSERT_EQ_FMT(BUS_ENABLED, mock_all_call_bus, "%X");

	PASS();
}

TEST subcall_bus_one_is_disabled(void) {
	uint8_t mock_subadr1 = BUS_ENABLED;

	create_subcall_bus(&mock_subadr1, SUBADR1);
	ASSERT_EQ_FMT(SUBADR1_DEFAULTS, mock_subadr1, "%X");

	PASS();
}

TEST subcall_bus_two_is_disabled(void) {
	uint8_t mock_subadr2 = BUS_ENABLED;

	create_subcall_bus(&mock_subadr2, SUBADR2);
	ASSERT_EQ_FMT(SUBADR2_DEFAULTS, mock_subadr2, "%X");

	PASS();
}

TEST subcall_bus_three_is_disabled(void) {
	uint8_t mock_subadr3 = BUS_ENABLED;

	create_subcall_bus(&mock_subadr3, SUBADR3);
	ASSERT_EQ_FMT(SUBADR3_DEFAULTS, mock_subadr3, "%X");

	PASS();
}

TEST mode_register_one_defaults(void) {
	uint8_t mock_mode1_defaults = ZERO;

	create_mode_register(&mock_mode1_defaults, MODE1);
	ASSERT_EQ_FMT(MODE1_DEFAULTS, mock_mode1_defaults, "%X");

	PASS();
}

TEST mode_register_two_defaults(void) {
	uint8_t mock_mode2_defaults = ZERO;

	create_mode_register(&mock_mode2_defaults, MODE2);
	ASSERT_EQ_FMT(MODE2_DEFAULTS, mock_mode2_defaults, "%X");

	PASS();
}

SUITE(test_start_state) {
	RUN_TEST(leds_are_off);
	RUN_TEST(all_call_bus_is_enabled);
	RUN_TEST(subcall_bus_one_is_disabled);
	RUN_TEST(subcall_bus_two_is_disabled);
	RUN_TEST(subcall_bus_three_is_disabled);
	RUN_TEST(mode_register_one_defaults);
	RUN_TEST(mode_register_two_defaults);
}
