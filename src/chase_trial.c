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
QueueHandle_t msgQueue;

// CAN bus callback function to handle received messages
static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg) {
    // Send received message to the queue
    xQueueSendToBack(msgQueue, msg, portMAX_DELAY);
}

// CAN bus interrupt handler
static void PIOx_IRQHandler(void) {
    can2040_pio_irq_handler(&cbus);
}

// Function to transmit a CAN message
static void transmit(void) {
    struct can2040_msg msg;
    msg.id = 0x123;
    msg.dlc = 1;
    msg.data[0] = 11;
    
    if (can2040_transmit(&cbus, &msg) >= 0) {
        printf("Message transmitted with ID: %u, Data: %u\n", msg.id, msg.data[0]);
    } else {
        printf("Message transmission failed\n");
    }
}

// Function to set up the CAN bus
void canbus_setup(void) {
    uint32_t pio_num = 0;
    uint32_t sys_clock = 125000000;
    uint32_t bitrate = 500000;
    uint32_t gpio_rx = 4, gpio_tx = 5;

    // Setup CAN bus with configuration parameters
    can2040_setup(&cbus, pio_num);
    can2040_callback_config(&cbus, can2040_cb);

    // Enable interrupts
    irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
    irq_set_priority(PIO0_IRQ_0, PICO_DEFAULT_IRQ_PRIORITY - 1);
    irq_set_enabled(PIO0_IRQ_0, 1);

    // Start CAN bus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
}

// Task to repeatedly transmit messages
void transmit_task(void *args) {
    while (1) {
        transmit();
        //vTaskDelay(pdMS_TO_TICKS(5000)); // Delay between transmissions
        vTaskDelay(pdMS_TO_TICKS(500)); // Delay between transmissions
    }
}

// Task to receive and print CAN messages
void receive_task(void *args) {
    struct can2040_msg received_msg;
    while (1) {
        if (xQueueReceive(msgQueue, &received_msg, portMAX_DELAY) == pdPASS) {
            printf("Received message with ID: %u, Data: %u\n", 
                   received_msg.id, received_msg.data[0]);
        }
    }
}

int main(void) {
    stdio_init_all();
    hard_assert(cyw43_arch_init() == PICO_OK);
    sleep_ms(5000); // Wait for terminal to connect
    canbus_setup();

    // Create the message queue for received CAN messages
    msgQueue = xQueueCreate(10, sizeof(struct can2040_msg));
    if (msgQueue == NULL) {
        printf("Failed to create message queue\n");
        return 1;
    }

    // Create Tasks for transmit and receive
    xTaskCreate(transmit_task, "TransmitTask", TEST_RUNNER_STACK_SIZE, NULL, TEST_RUNNER_PRIORITY, NULL);
    xTaskCreate(receive_task, "ReceiveTask", TEST_RUNNER_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3UL, NULL);

    printf("Starting scheduler\n");
    vTaskStartScheduler(); // Start FreeRTOS scheduler

    return 0; // Should never reach here
}
