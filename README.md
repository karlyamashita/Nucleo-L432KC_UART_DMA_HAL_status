# Nucleo-L432KC_UART_DMA_HAL_status

This Project was designed on a Nucleo-L432KC but the code can be port to any STM32 device/development board.\
Check out the [YouTube Tutorial](https://youtube.com/) :film_strip:\
Be sure to subscribe and like the video. Any comments will be appreciated :grinning:

## This project demonstrates how to 
+ Create a UART ring buffer data structure for both Receiving and Transmitting
+ Set up DMA for UART Rx and Tx
+ Use **HAL_UARTEx_ReceiveToIdle_DMA** and **HAL_UARTEx_RxEventCallback**
+ How to check **HAL** return status


### HAL drivers
I've seen a lot of post on ST's forums saying how unreliable the HAL driver are. Now I can't vouch for all of the HAL drivers as I have not used them all extensively
But I can definately say that the **HAL_UART** and **HAL_CAN** driver works. I have found majority of people are using the drivers incorrectly. 

The HAL_UART driver checks for different errors and either returns HAL_OK or HAL_BUSY. 
I see people writing code in **stm32l4xx_it.c**. There is no need to write code in this function. The interrupt calls the appropriate HAL driver, which then calls a weak callbacks.
You just override the weak callback somewhere else in your code and write your own code in that function. 

For this project I'm using the DMA. But this also applies for using interrupts like the following most used functions
+ HAL_UART_Transmit_IT
+ HAL_UART_Receive_IT
+ HAL_UARTEx_ReceiveToIdle_IT

### What usually happens when transmitting 
+ The user has multiple data packets to send and by calling **HAL_UART_Transmit_DMA** he has the 1st transmit still being processed.
+ Then the user calls **HAL_UART_Transmit_DMA** again with the 2nd packet, but because HAL driver is still transmitting the 1st packet, HAL status returns HAL_BUSY.
+ Because the user doesn't check the HAL status, he doesn't know that the 2nd packet was not sent and is lost.
+ Until HAL status returns HAL_OK, then the user could have 10's of messages that are lost.
+ The YouTube video will show how much data is lost.

### What usually happens when receiving
+ The user calls **HAL_UARTEx_ReceiveToIdle_DMA** to enable interrupts.
+ But if HAL returns HAL_BUSY then the interrupts are NOT enabled, period!
+ So the user will no longer going to receive an interrupt no matter how much data is being transmitted to the STM32 device.

### Why I consider HAL_UARTEx_ReceiveToIdle_DMA a better choice over HAL_UART_Receive_DMA
+ When receiving packets of data, there will be an idle time before the next packet is received. The DMA will detect this idle time and interrupt. In **HAL_UARTEx_RxEventCallback** you can increment your buffer pointer.
  - There is no need to copy data from the original array to another array in hopes that new received data doesn't overwrite your original array before you can process the data.
  - Because a ring buffer is used, the data is already saved to it by the DMA. You just increment the ring buffer pointer so new data is saved to the next index location while your new data is still intact for parsing. Simple right? 
+ When using **HAL_UART_Receive_DMA** you have to know the size of data being received. If the data packet is of different lengths, this could lead to the DMA buffer being out of sync. 

### ST's examples
+ If HAL status is not HAL_OK, they call **Error_Handler()**. This function does nothing but loop forever. That's a piss poor job of showing how to deal with an error. 
+ ST should have better explained that the HAL driver is **busy** and to try again to either receive or transmit.

