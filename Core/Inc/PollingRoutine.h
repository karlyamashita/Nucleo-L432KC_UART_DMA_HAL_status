/*
 * PollingRoutine.h
 *
 *  Created on: Oct 8, 2023
 *      Author: codek
 */

#ifndef INC_POLLINGROUTINE_H_
#define INC_POLLINGROUTINE_H_


#include "main.h"



void PollingInit(void);
void PollingRoutine(void);


void UART_Parse(UartBuffer_t *msg);

void SendBunchOfData(void);

void BlinkGreenLED(void);


#endif /* INC_POLLINGROUTINE_H_ */
