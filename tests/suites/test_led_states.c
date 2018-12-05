#include "tests.h"

/**
 * This suite tests LED states for the PCA9685.
 */

static uint8_t mock_led;

static void setup_cb(void *data) {
	(void)data;
	create_led_driver(&mock_led);
}

static void teardown_cb(void *data) {
	(void)data;
}

// Turn an LED completely on
TEST led_full_on(void) {
	turn_on_led(1);

	ASSERT_EQ_FMT(LED_ON, mock_led, "%X");

	PASS();
}

// Turn an LED completely off
TEST led_full_off(void) {
	turn_on_led(1);
	turn_off_led(1);

	ASSERT_EQ_FMT(LED_OFF, mock_led, "%X");

	PASS();
}

static enum theft_trial_res
prop_char_fails_cause_shrink(struct theft *t, void *arg1) {
	(void)t;
	char *test_str = arg1;

	return strlen(test_str) ? THEFT_TRIAL_FAIL : THEFT_TRIAL_PASS;
}


TEST char_fail_shrinkage(void) {
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config cfg = {
			.name = __func__,
			.prop1 = prop_char_fails_cause_shrink,
			.type_info = {
					theft_get_builtin_type_info(THEFT_BUILTIN_char_ARRAY),
			},
			.bloom_bits = 20,
			.seed = seed,
			.trials = 1,
	};

			ASSERT_EQm("should fail until full contraction",
			           THEFT_RUN_FAIL, theft_run(&cfg));
			PASS();
}


SUITE(test_led_states) {
	SET_SETUP(setup_cb, NULL);
	SET_TEARDOWN(teardown_cb, NULL);

	RUN_TEST(led_full_on);
	RUN_TEST(led_full_off);
	RUN_TEST(char_fail_shrinkage);
}
