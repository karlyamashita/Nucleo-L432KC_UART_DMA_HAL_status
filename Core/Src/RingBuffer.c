/*
 * RingBuffer.c
 *
 *  Created on: Sep 18, 2019
 *      Author: Karl
 *
 *
*/

#include "main.h"
#include "ringBuffer.h"


void RingBuff_Ptr_Reset(RING_BUFF_STRUCT *ptr) {
	ptr->index_IN = 0;
	ptr->index_OUT = 0;

	ptr->cnt_Handle = 0;
	ptr->cnt_OverFlow = 0;
}

/*
 * Description: Call this after data has been save to array.
 *
 */
void RingBuff_Ptr_Input(RING_BUFF_STRUCT *ptr, uint32_t bufferSize) {
	ptr->index_IN++;
	if (ptr->index_IN >= bufferSize)
		ptr->index_IN = 0;

	ptr->cnt_Handle++;
	if (ptr->index_IN == ptr->index_OUT) {
		ptr->cnt_OverFlow++;
		if (ptr->cnt_OverFlow > RING_BUFF_OVERFLOW_SIZE)
			ptr->cnt_OverFlow = 0;
		if (ptr->index_IN == 0) {
			ptr->index_OUT = bufferSize - 1;
		} else {
			ptr->index_OUT = ptr->index_IN - 1;
		}
		ptr->cnt_Handle = 1;
	}
}

/*
 * Description: Call this after data has been parsed
 *
 */
void RingBuff_Ptr_Output(RING_BUFF_STRUCT *ptr, uint32_t bufferSize) {
	if (ptr->cnt_Handle) {
		ptr->index_OUT++;
		if (ptr->index_OUT >= bufferSize)
			ptr->index_OUT = 0;
		ptr->cnt_Handle--;
	}
}
