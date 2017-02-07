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
#define MENU_ID_VOLTAGE_MANUAL_RANGE 2 
#define MENU_ID_VOLTAGE_AUTO_RANGE 3

#define MENU_ID_CURRENT 4
#define MENU_ID_CURRENT_MANUAL_RANGE 5 
#define MENU_ID_CURRENT_AUTO_RANGE 6

#define MENU_ID_RESISTANCE 7
#define MENU_ID_RESISTANCE_MANUAL_RANGE 8 
#define MENU_ID_RESISTANCE_AUTO_RANGE 9

#define SCROLL_RATE 400

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
		buttonPressed = DelayForButton(SCROLL_RATE);	
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
			//outputSignalOFF(4U);
			break;
	}
	
	return selectedMenu;
}

int openCurrentMenu(){
	
	outputSignalON(4U);
	MULTIMETER_MODE = MODE_CURRENT;
	
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
		buttonPressed = DelayForButton(SCROLL_RATE);	
	}
	
	int selectedMenu = 0; 
	switch(buttonPressed) {
		case 0: 
			selectedMenu = MENU_ID_CURRENT_MANUAL_RANGE;
			break;
		case 1:
			selectedMenu = MENU_ID_CURRENT_AUTO_RANGE;
			break;
		case 8:
			selectedMenu = MENU_ID_OPEN;
			outputSignalOFF(4U);
			break;
	}
	
	return selectedMenu;
}

int openResistanceMenu(){
	
	outputSignalON(4U);
	MULTIMETER_MODE = MODE_RESISTANCE;
	
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
		buttonPressed = DelayForButton(SCROLL_RATE);	
	}
	
	int selectedMenu = 0; 
	switch(buttonPressed) {
		case 0: 
			selectedMenu = MENU_ID_RESISTANCE_MANUAL_RANGE;
			break;
		case 1:
			selectedMenu = MENU_ID_RESISTANCE_AUTO_RANGE;
			break;
		case 8:
			selectedMenu = MENU_ID_OPEN;
			//outputSignalOFF(4U);
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
	lcd_write_string("Select Function", 0, 0);
	
	//Second Line & Wait for a button to be pressed
	while((buttonPressed != 0) && (buttonPressed != 1) && (buttonPressed != 2) && (buttonPressed != 8) ){ 
		
		//If message wont fit on the screen then scroll it
		if(messageLength > 16){
			scrollText(message, messageLength);
		}
	
			lcd_write_string(message, 1, 0);
			buttonPressed = DelayForButton(SCROLL_RATE);	
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
		lcd_clear_display();
		switch (selectedMenuID) {
			
			case MENU_EXIT:
				return;
			
			case MENU_ID_OPEN:
				selectedMenuID = openMenu();
				break;
			
			case MENU_ID_VOLTAGE:
				//go to Voltage menu
				LED_Out(1);
				selectedMenuID = openVoltageMenu();
				break;
			
			case MENU_ID_VOLTAGE_MANUAL_RANGE:
				//manual range for Current
				//go to select range menu
				break;
			
			case MENU_ID_VOLTAGE_AUTO_RANGE:
				//auto range for Current
				//auto ranging stuff 
				selectedMenuID = MENU_EXIT;
				break;
			
			case MENU_ID_CURRENT:
				//got to Current menu 
				LED_Out(1);
				selectedMenuID = openCurrentMenu();
				break; 
			
			case MENU_ID_CURRENT_MANUAL_RANGE:
				//manual range for CURRENT
				//go to select range menu
				break;
			
			case MENU_ID_CURRENT_AUTO_RANGE:
				//auto range for CURRENT
				//auto ranging stuff 
				selectedMenuID = MENU_EXIT;
				break;
			
			case MENU_ID_RESISTANCE:
				//got to resistance menu
				LED_Out(1);
				selectedMenuID = openResistanceMenu();
				break;
			
			case MENU_ID_RESISTANCE_MANUAL_RANGE:
				//manual range for resistance
				//go to select range menu
				break;
			
			case MENU_ID_RESISTANCE_AUTO_RANGE:
				//auto range for resistance
				//auto ranging stuff 
				selectedMenuID = MENU_EXIT;
				break;
			
			default:
				selectedMenuID = MENU_EXIT;
				break;
		}	
	}
}
