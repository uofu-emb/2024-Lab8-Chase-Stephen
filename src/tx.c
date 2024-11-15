#include </home/stephen/Desktop/5785/2024-Lab8-Chase-Stephen/lib/can/src/can2040.h>
#include <hardware/regs/intctrl.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>



static struct can2040 cbus;


static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{

}

static void PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}


void canbus_setup(void)
{
    
    uint32_t pio_num = 0;
    uint32_t sys_clock = 125000000, bitrate = 500000;
    uint32_t gpio_rx = 4, gpio_tx = 5;

    // Setup canbus
    can2040_setup(&cbus, pio_num);
    can2040_callback_config(&cbus, can2040_cb);

    // Enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
    irq_set_priority(PIO0_IRQ_0, PICO_DEFAULT_IRQ_PRIORITY - 1);
    irq_set_enabled(PIO0_IRQ_0, 1);

    // Start canbus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
    

}





int main(void)
{
    stdio_init_all();
    hard_assert(cyw43_arch_init() == PICO_OK);
    sleep_ms(5000);
    canbus_setup();


    struct can2040_msg tx_message;
    tx_message.id = 0x123;
    tx_message.dlc = 3;
    tx_message.data[0] = 12;
    tx_message.data[1] = 2;
    tx_message.data[2] = 1;

    for (int i = 0; i < 100; i++){
        if (can2040_transmit(&cbus, &tx_message) >= 0) {
            printf("Message transmitted with ID: %u, Data: %u\n", tx_message.id, tx_message.data[0]);
        }
        sleep_ms(1000);
    }

	return 0;
}