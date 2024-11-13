#include </home/stephen/Desktop/5785/2024-Lab8-Chase-Stephen/lib/can/src/can2040.h>
#include <hardware/regs/intctrl.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#define TEST_RUNNER_PRIORITY (tskIDLE_PRIORITY + 2UL)
#define TEST_RUNNER_STACK_SIZE configMINIMAL_STACK_SIZE

static struct can2040 cbus;
int x = 1;

static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    printf("2");

}

static void PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}

static void transmit(void)
{
    struct can2040_msg msg;
    msg.id = 1;
    msg.dlc = 1;
    msg.data[0] = 12;
    x = can2040_transmit(&cbus, &msg);
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

// int main(void)
// {
//     stdio_init_all();
//     sleep_ms(5000);
//     canbus_setup();
//     transmit();
//     printf("1");
//     sleep_ms(5000);
//     return 0;
// }

void runner_thread (__unused void *args)
{
    //queue = xQueueCreate(100, sizeof(struct request_msg));
    while(1){
        printf("transmit\n");
        transmit();
        printf("x: %d\n", x);
        vTaskDelay(5000);
    }
}

int main(void)
{
    stdio_init_all();
    hard_assert(cyw43_arch_init() == PICO_OK);
    sleep_ms(5000);
    canbus_setup();
    printf("Launching runner\n");
    xTaskCreate(runner_thread, "TestRunner",
                TEST_RUNNER_STACK_SIZE, NULL, TEST_RUNNER_PRIORITY, NULL);
    vTaskStartScheduler();
	return 0;
}




// #include <can2040.h>
// #include <hardware/regs/intctrl.h>
// #include <stdio.h>
// #include <pico/stdlib.h>
// #include <FreeRTOS.h>
// #include <task.h>
// #include <queue.h>

// static struct can2040 cbus;

// QueueHandle_t msgs;
// static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
// {
//     xQueueSendToBack(msgs, msg, portMAX_DELAY);
// }

// static void PIOx_IRQHandler(void)
// {
//     can2040_pio_irq_handler(&cbus);
// }

// void canbus_setup(void)
// {
//     uint32_t pio_num = 0;
//     uint32_t sys_clock = 125000000, bitrate = 500000;
//     uint32_t gpio_rx = 4, gpio_tx = 5;

//     // Setup canbus
//     can2040_setup(&cbus, pio_num);
//     can2040_callback_config(&cbus, can2040_cb);

//     // Enable irqs
//     irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
//     irq_set_priority(PIO0_IRQ_0, PICO_DEFAULT_IRQ_PRIORITY - 1);
//     irq_set_enabled(PIO0_IRQ_0, 1);

//     // Start canbus
//     can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
// }

// void main_task(__unused void *params)
// {
//     struct can2040_msg data;
//     xQueueReceive(msgs, &data, portMAX_DELAY);
//     printf("Got message\n");
// }

// int main(void)
// {
//     stdio_init_all();
//     msgs = xQueueCreate(100, sizeof(struct can2040_msg));
//     canbus_setup();
//     TaskHandle_t task;
//     xTaskCreate(main_task, "MainThread",
//                 configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1UL, &task);
//     vTaskStartScheduler();
//     return 0;
// }