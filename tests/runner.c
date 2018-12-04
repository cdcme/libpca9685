#include "runner.h"

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	(void)argc;
	(void)argv;

	GREATEST_INIT();
	greatest_set_verbosity(3);

	RUN_SUITE(test_start_state);
	RUN_SUITE(test_led_states);

	GREATEST_PRINT_REPORT();
	return greatest_all_passed();
}
