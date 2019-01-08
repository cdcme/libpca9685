#include "tests.h"

#include <math.h>

/* Test setup begin */

static pca9685_s mock_driver;
static struct theft_run_config run_config, run_config2, run_config3, run_config4;

static void setup_cb(void *data) {
    (void)data;

    set_up_mock_driver(&mock_driver);
    set_up_theft_run_config_u8(&run_config);
    set_up_theft_run_config_u8_int_int(&run_config2);
    set_up_theft_run_config_int(&run_config3);
    set_up_theft_run_config_int_int(&run_config4);
}

/* Test setup ends here; tests begin */

TEST expect_register_for_channel_to_be_correct(void) {
    // Quick spot checks... bounds checking below
    u8 channel;

    channel = channel_to_register_base(0);
    ASSERT_EQ(channel, 0x06);

    channel = channel_to_register_base(1);
    ASSERT_EQ(channel, 0x0A);

    channel = channel_to_register_base(9);
    ASSERT_EQ(channel, 0x2A);

    channel = channel_to_register_base(15);
    ASSERT_EQ(channel, 0x42);

    channel = channel_to_register_base(16);
    ASSERT_EQ(channel, 0x42);

    PASS();
}

static enum theft_trial_res
property_converted_channels_exist(struct theft *t, void *arg1){
    (void)t;

    u8 mock_channel = *(u8 *)arg1;
    u8 mock_register = channel_to_register_base(mock_channel);

    // Min/max LED register address
    return (mock_register >= 0x06 && mock_register <= 0x45) ? THEFT_TRIAL_PASS : THEFT_TRIAL_FAIL;
}

TEST expect_converted_channels_to_be_in_bounds(void) {
    enum theft_run_res res;

    run_config.name = __func__;
    run_config.prop1 = property_converted_channels_exist;

    res = theft_run(&run_config);

    ASSERT_EQ(res, THEFT_RUN_PASS);

    PASS();
}

static enum theft_trial_res
property_can_set_led_byte_data(struct theft *t, void *arg1, void *arg2, void *arg3){
    (void)t;

    u8 mock_address = *(u8 *)arg1;
    int mock_on_bits = *(int *)arg2;
    int mock_off_bits = *(int *)arg3;

    set_led_bytes(&mock_driver, mock_address, mock_on_bits, mock_off_bits);

    ASSERT_STR_EQ(mock_driver.command, "i2c_write");
    ASSERT_STR_EQ(mock_driver.status, "ok");

    return THEFT_TRIAL_PASS;
}

TEST expect_register_writes_to_succeed(void) {
    enum theft_run_res res;

    run_config2.name = __func__;
    run_config2.prop3 = property_can_set_led_byte_data;

    res = theft_run(&run_config2);

    ASSERT_EQ(res, THEFT_RUN_PASS);

    PASS();
}

TEST expect_prescale_calculations_to_be_accurate(void) {
    // Quick spot checks... bounds checking below
    int prescale_v = calculate_prescale_from_frequency(FREQ_MAX);
    ASSERT_EQ(prescale_v, PRESCALE_MIN);

    prescale_v = calculate_prescale_from_frequency(200);
    ASSERT_EQ(prescale_v, 30);

    prescale_v = calculate_prescale_from_frequency(50);
    ASSERT_EQ(prescale_v, 121);

    prescale_v = calculate_prescale_from_frequency(25);
    ASSERT_EQ(prescale_v, 243);

    prescale_v = calculate_prescale_from_frequency(FREQ_MIN);
    ASSERT(prescale_v <= PRESCALE_MAX);

    PASS();
}

static enum theft_trial_res
property_prescale_values_are_within_bounds(struct theft *t, void *arg1) {
    (void)t;
    int mock_frequency = *(int *)arg1;
    int prescale_v = calculate_prescale_from_frequency(mock_frequency);

    ASSERT(prescale_v >= PRESCALE_MIN && prescale_v <= PRESCALE_MAX);

    return THEFT_TRIAL_PASS;
}

TEST expect_prescale_values_to_be_in_bounds(void) {
    enum theft_run_res res;

    run_config.name = __func__;
    run_config.prop1 = property_prescale_values_are_within_bounds;

    res = theft_run(&run_config);

    ASSERT_EQ(res, THEFT_RUN_PASS);

    PASS();
}

TEST expect_delay_calculations_to_be_accurate(void) {
    int delay = calculate_delay_from_percentage(0);
    ASSERT_EQ(delay, 41);

    delay = calculate_delay_from_percentage(100);
    ASSERT_EQ(delay, LED_MAX_STEPS);

    PASS();
}

static enum theft_trial_res
property_delay_calculations_are_in_bounds(struct theft *t, void *arg1) {
    (void)t;
    int delay_p = *(int *)arg1;
    int delay_from_p = calculate_delay_from_percentage(delay_p);

    ASSERT(delay_from_p >= 1 && delay_from_p <= LED_MAX_STEPS);

    return THEFT_TRIAL_PASS;
}

TEST expect_delay_calculations_to_be_in_bounds(void) {
    enum theft_run_res res;

    run_config3.name = __func__;
    run_config3.prop1 = property_delay_calculations_are_in_bounds;

    res = theft_run(&run_config3);

    ASSERT_EQ(res, THEFT_RUN_PASS);

    PASS();
}

TEST expect_on_time_calculations_to_be_accurate(void) {
    int on_time = calculate_on_time_from_percentage(0);
    ASSERT_EQ(on_time, 0);

    on_time = calculate_on_time_from_percentage(100);
    ASSERT_EQ(on_time, LED_MAX_STEPS);

    PASS();
}

static enum theft_trial_res
property_on_time_calculations_are_in_bounds(struct theft *t, void *arg1) {
    (void)t;
    int on_time_p = *(int *)arg1;
    int on_time_from_p = calculate_on_time_from_percentage(on_time_p);

    ASSERT(on_time_from_p >= 0 && on_time_from_p <= LED_MAX_STEPS);

    return THEFT_TRIAL_PASS;
}

TEST expect_on_time_calculations_to_be_in_bounds(void) {
    enum theft_run_res res;

    run_config3.name = __func__;
    run_config3.prop1 = property_on_time_calculations_are_in_bounds;

    res = theft_run(&run_config3);

    ASSERT_EQ(res, THEFT_RUN_PASS);

    PASS();
}

TEST expect_off_steps_calculations_to_be_accurate(void) {
    int off_steps = calculate_off_steps_from_delay_and_on_time(1, 0);
    ASSERT_EQ(off_steps, 0);

    off_steps = calculate_off_steps_from_delay_and_on_time(1, 25);
    ASSERT_EQ(off_steps, 25);

    off_steps = calculate_off_steps_from_delay_and_on_time(25, 25);
    ASSERT_EQ(off_steps, 49);

    off_steps = calculate_off_steps_from_delay_and_on_time(25, 1);
    ASSERT_EQ(off_steps, 25);

    off_steps = calculate_off_steps_from_delay_and_on_time(LED_MAX_STEPS, LED_MAX_STEPS);
    ASSERT_EQ(off_steps, LED_MAX_BITS);

    PASS();
}

SUITE(test_register_ops) {
    SET_SETUP(setup_cb, NULL);

    RUN_TEST(expect_register_for_channel_to_be_correct);
    RUN_TEST(expect_converted_channels_to_be_in_bounds);
    RUN_TEST(expect_register_writes_to_succeed);
    RUN_TEST(expect_prescale_calculations_to_be_accurate);
    RUN_TEST(expect_prescale_values_to_be_in_bounds);
    RUN_TEST(expect_delay_calculations_to_be_accurate);
    RUN_TEST(expect_delay_calculations_to_be_in_bounds);
    RUN_TEST(expect_on_time_calculations_to_be_accurate);
    RUN_TEST(expect_on_time_calculations_to_be_in_bounds);
    RUN_TEST(expect_off_steps_calculations_to_be_accurate);
}
