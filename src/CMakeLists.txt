# This is the main binary. List your C files here.
# This is the main binary. List your C files here.
add_executable(rx
    rx.c
    )

add_executable(tx
    tx.c
    )

add_executable(chase_trial
    chase_trial.c
    )

add_executable(txrx
    txrx.c
    )

pico_set_program_name(rx "rx")
pico_set_program_version(rx "0.1")

pico_set_program_name(tx "tx")
pico_set_program_version(tx "0.1")

pico_set_program_name(chase_trial "chase_trial")
pico_set_program_version(tx "0.1")

pico_set_program_name(txrx "txrx")
pico_set_program_version(tx "0.1")

# Modify the below lines to enable/disable output over UART/USB.
# These are mutually exclusive, only set one.
pico_enable_stdio_uart(rx 0)
pico_enable_stdio_usb(rx 1)

pico_enable_stdio_uart(tx 0)
pico_enable_stdio_usb(tx 1)

pico_enable_stdio_uart(chase_trial 0)
pico_enable_stdio_usb(chase_trial 1)

pico_enable_stdio_uart(txrx 0)
pico_enable_stdio_usb(txrx 1)


# Add the libraries to the build
target_link_libraries(rx PRIVATE
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
  can2040
)

target_link_libraries(tx PRIVATE
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
  can2040
)

target_link_libraries(chase_trial PRIVATE
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
  can2040
)

target_link_libraries(txrx PRIVATE
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
  can2040
)
# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(rx PRIVATE
        pico_cyw43_arch_none
    )
    target_link_libraries(tx PRIVATE
        pico_cyw43_arch_none
    )
    target_link_libraries(chase_trial PRIVATE
        pico_cyw43_arch_none
    )
    target_link_libraries(txrx PRIVATE
        pico_cyw43_arch_none
    )
endif()
# Another pico macro which generates the uf2 file in addition to the elf
pico_add_extra_outputs(rx)
pico_add_extra_outputs(tx)
pico_add_extra_outputs(chase_trial)
pico_add_extra_outputs(txrx)


add_custom_target(flash1 picotool load -f $<TARGET_FILE:rx> DEPENDS rx)
add_custom_target(flash2 picotool load -f $<TARGET_FILE:tx> DEPENDS tx)
add_custom_target(flash3 picotool load -f $<TARGET_FILE:chase_trial> DEPENDS chase_trial)
add_custom_target(flash4 picotool load -f $<TARGET_FILE:txrx> DEPENDS txrx)
