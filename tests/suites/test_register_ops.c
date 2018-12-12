#include "tests.h"

/* Test setup begin */

static pca9685_s mock_driver;
static struct theft_run_config run_config, run_config2;

static void setup_cb(void *data) {
    (void)data;

    set_up_mock_driver(&mock_driver);
    set_up_theft_run_config(&run_config);
    set_up_theft_run_config2(&run_config2);
}

/* Test setup ends here; tests begin */

TEST expect_register_for_channel_to_be_correct(void) {
    // Spot checks until I get time to make this property-based
    uint8_t channel;

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

    uint8_t mock_channel = *(uint8_t *)arg1;
    uint8_t mock_register = channel_to_register_base(mock_channel);

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

    uint8_t mock_address = *(uint8_t *)arg1;
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

TEST expect_prescale_calculations_to_be_correct(void) {
    // Quick spot checks for now... bounds checking below
    int prescale_v = calculate_prescale_from_frequency(200);
    ASSERT_EQ(prescale_v, 30);

    prescale_v = calculate_prescale_from_frequency(50);
    ASSERT_EQ(prescale_v, 121);

    prescale_v = calculate_prescale_from_frequency(25);
    ASSERT_EQ(prescale_v, 243);

    PASS();
}

static enum theft_trial_res
property_prescale_values_are_within_bounds(struct theft *t, void *arg1) {
    (void)t;
    int mock_frequency = *(int *)arg1;
    int prescale_v = calculate_prescale_from_frequency(mock_frequency);

    ASSERT(prescale_v >= 3 && prescale_v <= 255);

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

SUITE(test_register_ops) {
    SET_SETUP(setup_cb, NULL);

    RUN_TEST(expect_register_for_channel_to_be_correct);
    RUN_TEST(expect_converted_channels_to_be_in_bounds);
    RUN_TEST(expect_register_writes_to_succeed);
    RUN_TEST(expect_prescale_calculations_to_be_correct);
    RUN_TEST(expect_prescale_values_to_be_in_bounds);
}
