#include "serial.h"
#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "LED.h"
#include "LCD/lcd_driver.h"
#include <stdlib.h>
#include <string.h>

struct packetNode *packetListHead = 0;
struct packetNode *packetListCurrent = 0;

void send_packet(char* packet, char length){
	printf("%s\n", packet);

}

static void _configUSART2(uint32_t BAUD){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;	/* Enable USART2 Clock */

	GPIOA->MODER |= GPIO_MODER_MODER3_1;		//Setup PA3 as Input (USART2 Rx Pin)
	GPIOA->MODER |=  GPIO_MODER_MODER2_1;		/* Setup TX and  pin for Alternate Function */
	
	GPIOA->AFR[0] |= (7 << (4*2));		/* Setup RX as the Alternate Function */
	GPIOA->AFR[0] |= (7 << (4*3));		/* Setup TX as the Alternate Function */
	
	
  USART2->CR1 |= USART_CR1_UE;	/* Enable USART */
	
	USART2->CR1 &= ~USART_CR1_M;  //M bit - word length
	 
	USART2->CR2 &= ~USART_CR2_STOP;  //set to 1 stop bit

	USART2->BRR = SystemCoreClock/(BAUD*4);  //the actual baud rate is given by this formula

	USART2->CR1 |= USART_CR1_TE;	/* Enable Tx */
	
	USART2->CR1 |= USART_CR1_RE;	/* Enable RX */
	
	/* Enable interrupt */
	
	USART2-> CR1 |= USART_CR1_RXNEIE; 
	
	NVIC_EnableIRQ(USART2_IRQn);  /* Enable interrupt */
}

void serial_init(void) {
	_configUSART2(9600);
}


void USART2_IRQHandler(void) {
	
	char receivedPacket[MAX_PACKET_SIZE] = '\0';
	int length = 0;
	
	receive(receivedPacket, MAX_PACKET_SIZE, &length);  

	struct packetNode *thisNode = (struct packetNode*)malloc(sizeof(struct packetNode));
	strncpy(thisNode->packetContents, receivedPacket, MAX_PACKET_SIZE);
	thisNode->packetLength = length;
	thisNode->next = 0;
	
	if(packetListHead == 0){
		packetListHead = thisNode;
	} else {
		packetListHead->next = thisNode;
	}
	
//	lcd_clear_display();
//	lcd_write_string(receivedPacket, 1, 0);

}

/*Blocking function which waits for a packet to be fully received before returning. 
Parameters:
	receivedPacket - buffer which will be filled with the packet received which will be \0 terminated.
	bufferLength - The size of the receivedPacket buffer
	packetLengt - POinter to an int which will be filled with the total size of the packet (including the \0 character)
*/
void receive(char receivedPacket[], int bufferLength, int* packetLength){
	char* receivedString;

		//Disable the interrupt so we can receive the entire packet
		USART2-> CR1 &= ~USART_CR1_RXNEIE; 

		char lengthOfMsg = (char)USART2->DR;
		//First byte will be the length of the string.
		receivedString = (char*)malloc(sizeof(char)*lengthOfMsg);
		
		if(receivedString != NULL){
			//Now that we have received the length of the message proceed to read the rest of the message
			int numChars = 0;
			char recieved = '\0';
			
			while (numChars < lengthOfMsg){
				if(USART2->SR & USART_SR_RXNE){
					recieved = (char)USART2->DR;
					receivedString[numChars] = recieved;
					numChars++;
				}
			}
			//Entire message received, append a string temination for convinience.
			receivedString[numChars] = '\0'; 

			//Send the acknowledgement for this packet
			send_packet("ack", 3);
			
			// Copy the packet data into the output buffer & set the length output.
			strncpy(receivedPacket, receivedString, bufferLength);
			*packetLength = numChars;
			
			//Reenable the interrupt
			USART2-> CR1 |= USART_CR1_RXNEIE; 

			free(receivedString);
		}
}


