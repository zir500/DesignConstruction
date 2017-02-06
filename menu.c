#include "STM32F4xx.h"
#include "LCD/lcd_driver.h"
#include "SWT.h"
#include "CommonFunctions.h"
#include "LED.h"
#include <string.h>
#include "menu.h"



int MULTIMETER_MODE = MODE_VOLTAGE;


//define menu IDs
#define MENU_EXIT -1
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
	
	outputSignalON(4U);
	MULTIMETER_MODE = MODE_VOLTAGE;
	
	lcd_clear_display();
	lcd_write_string("Select options", 0, 0);

	
	char message[] = "  1.Manual Range    2.Auotmatic Range ";
	int messageLength = strlen(message)-1;
	int buttonPressed = -1;
	
	LED_Out(3);
	
	while((buttonPressed != 0) && (buttonPressed != 1) && (buttonPressed != 8) ){
		//If message wont fit on the screen then scroll it
		if(messageLength > 16){
			scrollText(message, messageLength);
		}

		lcd_write_string(message, 1, 0);
		buttonPressed = DelayForButton(100);	
	}
	
	int selectedMenu = 0; 
	switch(buttonPressed) {
		case 0: 
			selectedMenu = MENU_ID_VOLTAGE_MANUAL_RANGE;
			break;
		case 1:
			selectedMenu = MENU_ID_VOLTAGE_AUTO_RANGE;
			break;
		case 8:
			selectedMenu = MENU_ID_OPEN;
			outputSignalOFF(4U);
			break;
	}
	
	return selectedMenu;
}


int openMenu(){
	char message[] = "   1.Voltage   2.Current   3.Resistance";
	int messageLength = strlen(message)-1;
	int buttonPressed = -1;
	
	LED_Out(7);
	
	lcd_clear_display();
	//Top Line
	lcd_write_string("Select Function", 0, 0);
	
	//Second Line & Wait for a button to be pressed
	while((buttonPressed != 0) && (buttonPressed != 1) && (buttonPressed != 2) && (buttonPressed != 8) ){ 
		
		//If message wont fit on the screen then scroll it
		if(messageLength > 16){
			scrollText(message, messageLength);
		}
	
			lcd_write_string(message, 1, 0);
			buttonPressed = DelayForButton(400);	
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
			selectedMenu = MENU_EXIT; //GO TO measuerment menu
			break;
	}
	return selectedMenu;
}


void menu(){
	int selectedMenuID = MENU_ID_OPEN;

	while(1){
		waitForRelease();
		switch (selectedMenuID) {
			case MENU_EXIT:
				return;
				break;
			case MENU_ID_OPEN:
				selectedMenuID = openMenu();
				break;
			case MENU_ID_VOLTAGE:
				//go to Voltage menu
				LED_Out(1);
				selectedMenuID = openVoltageMenu();
				break;
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
