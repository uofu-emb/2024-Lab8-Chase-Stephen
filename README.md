## Current Workflow Status

![example workflow](https://github.com/uofu-emb/2024-Lab8-Chase-Stephen.git/actions/workflows/main.yml/badge.svg)

# Lab 8 CAN
Activities:
Wire up CAN transceiver to your boards. In the example code RX/TX is on GPIO 4/5 - any GPIO is valid.
On one board, create a function that sends a message periodically using can2040_transmit
On another board, register a callback with can_callback_config to receive and log the messages.
Verify that the two are communicating.

![Application Screenshot](images/lab8_1.png)

Screenshot 1.
We can see 

Observe the electrical behavior of the bus line and the microcontroller lines.

# Part 2
This activity will observe bus contention from a babbling node. "In a Controller Area Network a babbling node is a node continuously (and usually erroneously) transmitting CAN frames with identical - often high - priority."

Combine the send and receive code together, so each microcontroller does both.
On one board, broadcast messages with a high priority continuously.
On the other board, broadcast a message with a lower priority periodically.
Use oscilloscope to observe the access pattern.
Observe how often the lower priority message is actually transmitted and received.




Introduce a short busy wait delay into the babbling node, slowly increase the duration until you observe the other messages consistently received.
Document your observations in your repository.