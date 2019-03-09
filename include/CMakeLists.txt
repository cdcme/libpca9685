target_sources(pca9685
    PUBLIC
        pca9685.h
    PRIVATE
        registers.h
)

target_include_directories(pca9685 PUBLIC ${CMAKE_CURRENT_LIST_DIR})
