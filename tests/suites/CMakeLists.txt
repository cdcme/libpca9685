target_sources(test_runner
    PRIVATE
        suites.h
        tests.h
        test_start_state.c
        test_led_states.c
)

target_include_directories(test_runner PUBLIC ${CMAKE_CURRENT_LIST_DIR})
