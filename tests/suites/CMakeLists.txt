target_sources(test_runner
    PRIVATE
        test_start_state.c
        test_led_states.c
    PUBLIC
        suites.h
        tests.h
)

target_include_directories(test_runner PUBLIC ${CMAKE_CURRENT_LIST_DIR})
