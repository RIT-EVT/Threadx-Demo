/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "gpio.h"
#include <stdio.h>  // For sprintf
#include <time.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEMO_STACK_SIZE 1024
#define DEMO_QUEUE_SIZE 100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
TX_THREAD thread_0;
TX_THREAD thread_1;
TX_THREAD thread_2;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
TX_QUEUE                queue_0;
TX_SEMAPHORE            counting_semaphore_0;
TX_MUTEX                mutex_0;
TX_EVENT_FLAGS_GROUP    event_flags_0;
TX_BYTE_POOL            byte_pool_0;
TX_BLOCK_POOL           block_pool_0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void    thread_0_entry(ULONG thread_input);
void    thread_1_entry(ULONG thread_input);
void    thread_2_entry(ULONG thread_input);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_Init */
  CHAR *pointer = TX_NULL;

  /* Allocate the stack for thread 0.  */
  tx_byte_allocate(byte_pool, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

  //TODO: look into parameters of tx thread create
  /* Create the main thread 0.  */
  tx_thread_create(&thread_0, "thread 0", thread_0_entry, 0,
		  pointer, DEMO_STACK_SIZE,
		  1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

  /* Allocate the stack for thread 1.  */
  tx_byte_allocate(byte_pool, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

  /* Create the main thread 1.  */
  tx_thread_create(&thread_1, "thread 1", thread_1_entry, 0,
  		  pointer, DEMO_STACK_SIZE,
  		  1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

  /* Allocate the stack for thread 2.  */
  tx_byte_allocate(byte_pool, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

  /* Create the main thread 2.  */
  tx_thread_create(&thread_2, "thread 2", thread_2_entry, 0,
		  pointer, DEMO_STACK_SIZE,
		  1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

  /* Create the message queue shared by threads 1 and 2. */
  tx_queue_create(&queue_0, "queue 0", TX_1_ULONG, pointer, DEMO_QUEUE_SIZE*sizeof(ULONG));

  /* Create the semaphore used by threads 1 and 2. */
  tx_semaphore_create(&counting_semaphore_0, "counting semaphore 0", 1);

  /* USER CODE END App_ThreadX_Init */
  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */
	HAL_UART_Transmit(&huart2, (uint8_t*)"Kernel starting.\r\n", 20, 100);
  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
ULONG shared_count = 0;//Times a random number has been sent to the queue
ULONG shared_sum = 0;//Sum of random numbers
//Shared average should match thread0 average

ULONG received_message;
ULONG queue_status;
ULONG thread0_count = 0;
ULONG thread0_sum = 0;
ULONG thread1_count = 0;
ULONG thread1_sum = 0;
ULONG thread2_count = 0;
ULONG thread2_sum = 0;

ULONG semaphore_status;

void thread_0_entry(ULONG thread_input)
{
    char buffer[300];  // Buffer to hold the formatted string
    srand(time(0));
    ULONG num;
    HAL_UART_Transmit(&huart2, (uint8_t*)"Made Thread 0.\r\n", 17, 100);
    /* This thread simply sits in while-forever-sleep loop.  */
    while (1)
    {
        /* Send message to queue 0. */
    	//TODO: Add random number instead of count to queue
    	num = rand() % 25 + 1;
    	//TODO: Thread0 count and average and compare to everything else
    	//TODO: Global count and average should match thread0 count and average
    	queue_status = tx_queue_send(&queue_0, &num, TX_WAIT_FOREVER);
        semaphore_status = tx_semaphore_get(&counting_semaphore_0, TX_WAIT_FOREVER);

    	if(queue_status == TX_SUCCESS )
    	{
            shared_count++;
            shared_sum += num;
            thread0_count++;
            thread0_sum += num;
    	}


        //TODO: Should print piece of data
        if(thread0_count % 10 == 0)
        {
            // Format the string with thread0_count and sum0 values into the buffer array
            sprintf(buffer,
            		"Shared count: %lu\r\n"
            		"Shared sum: %lu\r\n"
            		"Thread 0 count: %lu\r\n"
            		"Thread 0 sum: %lu\r\n"
            		"Thread 0 average: %lu\r\n"
            		"Thread 1 count: %lu\r\n"
            		"Thread 1 sum: %lu\r\n"
            		"Thread 1 average: %lu\r\n"
            		"Thread 2 count: %lu\r\n"
            		"Thread 2 sum: %lu\r\n"
            		"Thread 2 average: %lu\r\n"
            		"\r\n",
					shared_count, shared_sum,
					thread0_count, thread0_sum, thread0_sum / thread0_count,
					thread1_count, thread1_sum, thread1_sum / thread1_count,
					thread2_count, thread2_sum, thread2_sum / thread2_count);

            // Transmit the formatted string
            HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 1000);
        }

        semaphore_status = tx_semaphore_put(&counting_semaphore_0);

        /* Sleep for 100 ticks = 1 second.  */
        tx_thread_sleep(100);

        if ( (queue_status != TX_SUCCESS) || (semaphore_status != TX_SUCCESS) )
        {
            HAL_UART_Transmit(&huart2, (uint8_t*)"Thread 0 Error.\r\n", 17, 100);
        }
    }
}

void thread_1_entry(ULONG thread_input)
{

	HAL_UART_Transmit(&huart2, (uint8_t*)"Made Thread 1.\r\n", 17, 100);
	char message_buffer[100]; // Buffer to store the message string
    /* This thread simply sits in while-forever-sleep loop.  */
    while (1)
    {
        /* Retrieve a message from the queue. */
    	queue_status = tx_queue_receive(&queue_0, &received_message, TX_WAIT_FOREVER);
    	/* Get the semaphore with suspension. */
    	semaphore_status = tx_semaphore_get(&counting_semaphore_0, TX_WAIT_FOREVER);

    	if(queue_status == TX_SUCCESS )
    	{
            shared_count++;
            shared_sum += received_message;
            thread1_count++;
            thread1_sum += received_message;
    	}


        snprintf(message_buffer, sizeof(message_buffer),
        		"Thread 1 received message: %lu\r\n"
        		"Thread 1 count: %lu\r\n"
        		"Thread 1 sum: %lu\r\n"
        		"\r\n",
        		received_message, thread1_count, thread1_sum);

        HAL_UART_Transmit(&huart2, (uint8_t*)message_buffer, strlen(message_buffer), 100);


        /* Release the semaphore. */
        semaphore_status = tx_semaphore_put(&counting_semaphore_0);

        if ( (queue_status != TX_SUCCESS) || (semaphore_status != TX_SUCCESS) )
        {
        	HAL_UART_Transmit(&huart2, (uint8_t*)"Thread 1 Error.\r\n", 17, 100);
        }

    }
}

void thread_2_entry(ULONG thread_input)
{
	ULONG received_message;
	ULONG queue_status;

	HAL_UART_Transmit(&huart2, (uint8_t*)"Made Thread 2.\r\n", 17, 100);
	char message_buffer[100]; // Buffer to store the message string

    /* This thread simply sits in while-forever-sleep loop.  */
    while (1)
    {
        queue_status = tx_queue_receive(&queue_0, &received_message, TX_WAIT_FOREVER);
        semaphore_status = tx_semaphore_get(&counting_semaphore_0, TX_WAIT_FOREVER);

    	if(queue_status == TX_SUCCESS )
    	{
            shared_count++;
            shared_sum += received_message;
            thread2_count++;
            thread2_sum += received_message;
    	}


        snprintf(message_buffer, sizeof(message_buffer),
        		"Thread 2 received message: %lu\r\n"
        		"Thread 2 count: %lu\r\nThread 2 sum: %lu\r\n"
        		"\r\n",
        		received_message, thread2_count, thread2_sum);

        HAL_UART_Transmit(&huart2, (uint8_t*)message_buffer, strlen(message_buffer), 100);

        semaphore_status = tx_semaphore_put(&counting_semaphore_0);

        if ( (queue_status != TX_SUCCESS) || (semaphore_status != TX_SUCCESS) )
        {
        	HAL_UART_Transmit(&huart2, (uint8_t*)"Thread 2 Error.\r\n", 17, 100);
        }

    }
}
/* USER CODE END 1 */
