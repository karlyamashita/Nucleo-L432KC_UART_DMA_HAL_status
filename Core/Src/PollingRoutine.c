/*
 * PollingRoutine.c
 *
 *  Created on: Oct 8, 2023
 *      Author: codek
 */

#include "main.h"



extern UART_HandleTypeDef huart2;
extern TimerCallbackStruct timerCallback;

const char fw_version[] = "FW Version 1.0.12\r\n";


// init rx/tx buffer and assign queue size
UartBuffer_t uart2 =
{
	.huart = &huart2,
	.rx.queueSize = QUEUE_SIZE,
	.tx.queueSize = QUEUE_SIZE
};



// called before main while loop
void PollingInit(void)
{
	TimerCallbackRegisterOnly(&timerCallback, SendBunchOfData);
	TimerCallbackTimerStart(&timerCallback, SendBunchOfData, 500, TIMER_REPEAT); // Send bunch of data every 100ms

	UART_EnableUART_DMA(&uart2);
}

// called from inside main while loop
void PollingRoutine(void)
{
	TimerCallbackCheck(&timerCallback);

	UART_Parse(&uart2);
	UART_CheckEnErrorFlag(&uart2);

	UART_Send(&uart2);
}



// parse data and send back out
void UART_Parse(UartBuffer_t *msg)
{
	char str[32] = {0};

	if(msg->rx.ptr.cnt_Handle)
	{
		msg->rx.msgToParse = &msg->rx.queue[msg->rx.ptr.index_OUT];

		if(strncmp((char*)msg->rx.msgToParse->data, "set toggle led", strlen("set toggle led")) == 0)
		{
			BlinkGreenLED();
		}
		else if(strncmp((char*)msg->rx.msgToParse->data, "get fw Version", strlen("get fw Version")) == 0)
		{
			strcpy(str, fw_version);
			UART_Tx_AddData(&uart2, (uint8_t*)str, strlen(str));
		}
		else if(strncmp((char*)msg->rx.msgToParse->data, "start counter", strlen("start counter")) == 0)
		{
			TimerCallbackTimerStart(&timerCallback, SendBunchOfData, 500, TIMER_REPEAT); // Send bunch of data every 100ms
		}
		else if(strncmp((char*)msg->rx.msgToParse->data, "stop counter", strlen("stop counter")) == 0)
		{
			TimerCallbackDisable(&timerCallback, SendBunchOfData);
		}
		else
		{
			UART_Tx_AddData(&uart2, msg->rx.msgToParse->data, msg->rx.msgToParse->size);
		}
		RingBuff_Ptr_Output(&msg->rx.ptr, msg->rx.queueSize);
	}
}

// the uart DMA Idle callback. Increment buffer pointer and enable UART DMA interrupt again.
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart == &huart2)
	{
		uart2.rx.queue[uart2.rx.ptr.index_IN].size = Size;
		RingBuff_Ptr_Input(&uart2.rx.ptr, uart2.rx.queueSize);
		UART_EnableUART_DMA(&uart2);
	}
}

// Add a bunch of strings to tx buffer
void SendBunchOfData(void)
{
	char str[32] = {0};
	int i;

	for(i = 0; i < 100; i++) // counter 0-99
	{
		snprintf(str, 32, "Counter %d\r\n", i);
		UART_Tx_AddData(&uart2, (uint8_t*)str, strlen((char*)str));
	}
}

// blink led
void BlinkGreenLED(void)
{
	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
}

