if (NOT TARGET hardware_pwm)
    message("Skipping hardware_pwm_test as hardware_pwm is unavailable on this platform")
    return()
endif()
add_executable(hardware_pwm_test hardware_pwm_test.c)

target_link_libraries(hardware_pwm_test PRIVATE pico_test hardware_pwm)
pico_add_extra_outputs(hardware_pwm_test)