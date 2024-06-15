# ThreadX Demo
- Using STM32CubeIDE version 1.15.1 and an STM32F446RE
- RTOS code is in `ThreadX-Demo/Core/Src/app_ThreadX.c`
- Using 3 threads with normal priority:

The point of this demo is to see if threadx is able to run on th EVT platform and to have something to compare the behavior to when we add this to EVT-Core.

## Thread 0:
 1. After intializing all the variables there is a tx_thread_sleep that is used stop the thread from adding to the queue so thread 1 and 2 stay in sync. 
 2. Generates a random number and, using tx_queue_send, adds it to a queue.
 3. Waits to be able to take the semaphore(When the code is first flashed the semaphore starts in the released state so thread 0 should always get it first).
 4. If the random number is successfully added to the queue a thread 0 specific count increments by 1 and the random number is added to a thread 0 sum.
 5. Releases the semaphore.
 6. tx_thread_sleep(100) makes the thread sleep for 100 ticks or 1 second.
 7. Every time the count is incremented 20 times it calculates the average of all thread data and then prints the average along with all other stats in the RTOS.
 
 ## Thread 1/2:
 1. After thread0 releases the semaphore for the first time thread 1 and thread 2 are created.
 2. Inside thread 1 tx_queue_receive waits forever until it has received a message from the queue.
 3. Wait until the semaphore is released.
 4. If tx_queue_receive has received the random number, thread 1 increments a global and thread 1 count and adds the random number to a global and thread 1 sum.
 5. Print out the random number it received, a thread 1 count, and a thread 1 sum over UART.
 6. After thread 1 is done printing, it releases the semaphore 
 7. Thread 0 goes through the message-sending process again.
 8. After thread0 releases the semaphore a second time thread 2 goes through steps 2-6 with its variables.
 
 ## Global and Thread variables
 - The global and thread variables are used to show how threads communicate using context switching.
 
 ## STM32CubeIDE
 1. Open the .ioc file.
 2. In the middlware category download the X-CUBE-AZURE-F4 software package.
	- Scroll down to the package and install version 1.1.0.
	- Open the drop down menu and find RTOS ThreadX.
	- Open the RTOS ThreadX drown and then another Threadx dropdown menu and check of the 'Core' option. 
	- Click ok then in the middleware and software packages category of cubemx find X-XUBE-AZURE-F4 and enable it by checking the RTOS ThreadX box.
	- Under configuration in the AzureRTOS Application tab change memory pool size to be 65536. This ensures we have enough allocated memory space.
 3. In the system core category.
	- open RCC and change HSE and LSE to be Crystal/Ceramic Resonator.
	- Open SYS and change the timebase source to be TIM6.