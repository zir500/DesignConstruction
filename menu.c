#include "STM32F4xx.h"
#include "LCD.h"
#include "menu.h"
#include <string.h>
#include <stdlib.h>

void scrollText(char message[], int messageLength){
	char lastCharacter = message[messageLength];
	for(int i=messageLength; i > 0; i--){
		message[i] = message[i-1];
	}
	message[0] = lastCharacter;
}



void openMenu(){
	LCD_GotoXY(0,0);
	LCD_PutS("Select Function");
	
	char message[] = "   1.Voltage   2.Current   3.Resistance";
	int messageLength = strlen(message)-1;
	
	while(1){ //No button pressed
		
		//If message wont fit on the screen then scroll it
		if(messageLength > 16){
			scrollText(message, messageLength);
		}
			LCD_GotoXY(0,1);
			LCD_PutS(message);
			Delay(100);
	}
	
}



