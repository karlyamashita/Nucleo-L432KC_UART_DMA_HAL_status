/*
 * UartHandler.c
 *
 *  Created on: Oct 9, 2023
 *      Author: Karl Yamashita
 *
 *      Copyrigth 2023 Karl Yamashita
 */


#include "main.h"


// if error flag then try to enable interrupt again
void UART_CheckEnErrorFlag(UartBuffer_t *msg)
{
	if(msg->rx.errorFlag)
	{
		msg->rx.errorFlag = false;
		UART_EnableUART_DMA(msg);
	}
}

// enable UART DMA interrupt
void UART_EnableUART_DMA(UartBuffer_t *msg)
{
	if(HAL_UARTEx_ReceiveToIdle_DMA(msg->huart, msg->rx.queue[msg->rx.ptr.index_IN].data, DMA_FULL_CALLBACK_SIZE) != HAL_OK)
	{
		msg->rx.errorFlag = true;
	}
}


// add data to tx buffer
void UART_Tx_AddData(UartBuffer_t *msg, uint8_t *data, uint32_t size)
{
	msg->tx.msgToSend = &msg->tx.queue[msg->tx.ptr.index_IN];

	memset(&msg->tx.msgToSend->data, 0 , sizeof(msg->tx.msgToSend->data));
	strncpy((char*)msg->tx.msgToSend->data, (char*)data, size);
	msg->tx.msgToSend->size = size;

	RingBuff_Ptr_Input(&msg->tx.ptr, msg->tx.queueSize);
}

// transmit any available data in tx buffer
void UART_Send(UartBuffer_t *msg)
{
	if(msg->tx.ptr.cnt_Handle)
	{
		msg->tx.msgToSend = &msg->tx.queue[msg->tx.ptr.index_OUT];

		if(HAL_UART_Transmit_DMA(msg->huart, msg->tx.msgToSend->data, msg->tx.msgToSend->size) == HAL_OK)
		{
			RingBuff_Ptr_Output(&msg->tx.ptr, msg->tx.queueSize); // only increment pointer to next message when status is HAL_OK
		}
	}
}
