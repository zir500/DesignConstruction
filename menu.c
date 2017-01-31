#include "STM32F4xx.h"
#include "LCD.h"
#include "menu.h"
#include "SWT.h"
#include "CommonFunctions.h"
#include "LED.h"
#include <string.h>

//define menu IDs
#define MENU_ID_OPEN 0 
#define MENU_ID_VOLTAGE 1 
#define MENU_ID_CURRENT 2 
#define MENU_ID_RESISTANCE 3
#define MENU_ID_VOLTAGE_MANUAL_RANGE 4 
#define MENU_ID_VOLTAGE_AUTO_RANGE 5

void scrollText(char message[], int messageLength){
	char lastCharacter = message[messageLength];
	for(int i=messageLength; i > 0; i--){
		message[i] = message[i-1];
	}
	message[0] = lastCharacter;
}

int openVoltageMenu(){
	LCD_Clear();
	LCD_GotoXY(0,0);
	LCD_PutS("Select options");
	
	char message[] = "  1.Manual Range    2.Auotmatic Range ";
	int messageLength = strlen(message)-1;
	int buttonPressed = -1;
	
	LED_Out(3);
	
	while((buttonPressed != 0) && (buttonPressed != 1) && (buttonPressed != 8) ){
		//If message wont fit on the screen then scroll it
		if(messageLength > 16){
			scrollText(message, messageLength);
		}
			LCD_GotoXY(0,1);
			LCD_PutS(message);
			buttonPressed = DelayForButton(100);	
	}
	
	int selectedMenu = 0; 
	switch(buttonPressed) {
		case 0: 
			selectedMenu = MENU_ID_VOLTAGE_MANUAL_RANGE;
			break;
		case 1:
			selectedMenu = MENU_ID_VOLTAGE_AUTO_RANGE;
		case 8:
				selectedMenu = MENU_ID_OPEN;
		break;
	}
	
	return selectedMenu;
}

int openMenu(){
	char message[] = "   1.Voltage   2.Current   3.Resistance";
	int messageLength = strlen(message)-1;
	int buttonPressed = -1;
	
	LED_Out(7);

	
	//Top Line
	LCD_GotoXY(0,0);
	LCD_PutS("Select Function");
	
	//Second Line & Wait for a button to be pressed
	while((buttonPressed != 0) && (buttonPressed != 1) && (buttonPressed != 2) && (buttonPressed != 8) ){ 
		
		//If message wont fit on the screen then scroll it
		if(messageLength > 16){
			scrollText(message, messageLength);
		}
			LCD_GotoXY(0,1);
			LCD_PutS(message);
			buttonPressed = DelayForButton(100);	
	}
	
	int selectedMenu = MENU_ID_VOLTAGE;
	switch (buttonPressed){
		case 0:
			selectedMenu = MENU_ID_VOLTAGE;
			break;
		
		case 1:
			selectedMenu = MENU_ID_CURRENT;
			break;
				
		case 2:
			selectedMenu =  MENU_ID_RESISTANCE;
			break;
		
		case 8:
			selectedMenu = MENU_ID_OPEN; //GO TO measuerment menu
			break;
	}
	return selectedMenu;
}

void menu(){
	int selectedMenuID = MENU_ID_OPEN;

	while(1){

		switch (selectedMenuID) {
			case MENU_ID_OPEN:
				selectedMenuID = openMenu();
			
			case MENU_ID_VOLTAGE:
				//go to Voltage menu
				LED_Out(1);
				selectedMenuID = openVoltageMenu();
			case MENU_ID_CURRENT:
				//got to Current menu 
				LED_Out(2);
				break; 
			case MENU_ID_RESISTANCE:
				//got to resistance menu
				LED_Out(4);
				break;
			case MENU_ID_VOLTAGE_MANUAL_RANGE:
				//manual range for voltage
				break;
			case MENU_ID_VOLTAGE_AUTO_RANGE:
				//auto range for voltage
				break;
		}
	}
}
