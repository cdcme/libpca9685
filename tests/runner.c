#include "runner.h"

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	struct greatest_report_t report;
	(void)argc;
	(void)argv;

	GREATEST_INIT();
	greatest_set_verbosity(3);

	RUN_SUITE(test_start_state);
	RUN_SUITE(test_led_states);

	GREATEST_PRINT_REPORT();

	greatest_get_report(&report);

	if (report.failed > 0) {
		return 1;
	}

	return 0;
}
