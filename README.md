## Threadx Demo
- Using stmcubeide version: 1.15.1 and an STM32f446RE
- RTOS code is in Threadx-Demo/Core/Src/app_threadx.c
- Using 3 threads with normal priority
- Thread0 loads a message into a queue
- When the message is loaded a thread 0 specific count and sum increments by 1
- Thread1 and 2 take the message and print it over uart
- When the message is recieved the threads increment a gloabl variable by 1
- Thread1 and 2 print the count and sum over uart
- Thread0 prints every piece of data

## To add:
- In thread0 add a random number instead of a count into the queue
- Thread0 should print everything
