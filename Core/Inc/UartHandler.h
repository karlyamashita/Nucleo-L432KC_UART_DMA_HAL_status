/*
 * UartHandler.h
 *
 *  Created on: Oct 9, 2023
 *      Author: codek
 */

#ifndef INC_UARTHANDLER_H_
#define INC_UARTHANDLER_H_


#define QUEUE_SIZE 108 // increase if msgPtr.iCnt_OverFlow > 0
#define DATA_SIZE 64 // the maximum length of packet you'll expect.

// This should be at least twice as big as the longest data size you'll ever expect.
// That way half callback doesn't interrupt in the middle of a data reception
#define DMA_FULL_CALLBACK_SIZE (DATA_SIZE * 2)

typedef struct
{
	uint8_t data[DATA_SIZE];
	uint32_t size;
}UartData_t;

typedef struct
{
	UART_HandleTypeDef *huart; // pointer to huart instance
	struct
	{
		UartData_t queue[QUEUE_SIZE];
		UartData_t *msgToParse;
		RING_BUFF_STRUCT ptr;
		uint32_t queueSize; // just a helper so that you don't have to remember the define name. Assign size when creating variable.

		bool errorFlag;
	}rx;
	struct
	{
		UartData_t queue[QUEUE_SIZE];
		UartData_t *msgToSend;
		RING_BUFF_STRUCT ptr;
		uint32_t queueSize; // just a helper so that you don't have to remember the define name. Assign size when creating variable.
	}tx;
}UartBuffer_t;



void UART_CheckEnErrorFlag(UartBuffer_t *msg);
void UART_EnableUART_DMA(UartBuffer_t *msg);

void UART_Tx_AddData(UartBuffer_t *msg, uint8_t *data, uint32_t size);
void UART_Send(UartBuffer_t *msg);



#endif /* INC_UARTHANDLER_H_ */
