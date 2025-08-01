if (NOT TARGET pico_divider)
    message("Skipping pico_divider_test as pico_divider is unavailable on this platform")
    return()
endif()
PROJECT(pico_divider_test)

if (PICO_ON_DEVICE)
    add_executable(pico_divider_test
            pico_divider_test.c
            )

    target_link_libraries(pico_divider_test pico_stdlib)

    if (PICO_RP2040)
        pico_set_divider_implementation(pico_divider_test hardware_explicit) # want to compare against compiler impl
    endif()

    pico_add_extra_outputs(pico_divider_test)

    target_compile_definitions(pico_divider_test PRIVATE
#            PICO_DIVIDER_DISABLE_INTERRUPTS=1
#            TURBO
    )

    # this is a separate test as hardware_explicit above causes it not to be tested at all!
    add_library(pico_divider_nesting_test_core INTERFACE)
    target_sources(pico_divider_nesting_test_core INTERFACE
            pico_divider_nesting_test.c
    )
    target_link_libraries(pico_divider_nesting_test_core INTERFACE pico_stdlib hardware_dma)

    add_executable(pico_divider_nesting_test_with_dirty_check)
    target_link_libraries(pico_divider_nesting_test_with_dirty_check pico_divider_nesting_test_core)
    if (PICO_RP2040)
        pico_set_divider_implementation(pico_divider_nesting_test_with_dirty_check hardware)
    endif()
    pico_add_extra_outputs(pico_divider_nesting_test_with_dirty_check)

    add_executable(pico_divider_nesting_test_with_disable_irq)
    target_link_libraries(pico_divider_nesting_test_with_disable_irq pico_divider_nesting_test_core)
    target_compile_definitions(pico_divider_nesting_test_with_disable_irq PRIVATE
            PICO_DIVIDER_DISABLE_INTERRUPTS=1)
    if (PICO_RP2040)
        pico_set_divider_implementation(pico_divider_nesting_test_with_disable_irq hardware)
    endif()
    pico_add_extra_outputs(pico_divider_nesting_test_with_disable_irq)

endif()