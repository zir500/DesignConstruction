#include "STM32F4xx.h"
#include "LCD/lcd_driver.h"
#include "SWT.h"
#include "CommonFunctions.h"
#include "LED.h"
#include <string.h>
#include "menu.h"
#include <stdlib.h>
#include "ADC.h"


int MULTIMETER_MODE = MODE_VOLTAGE;

#define SCROLL_RATE 400

void scrollText(char message[], int messageLength){

	char lastCharacter = message[messageLength];
	for(int i=messageLength; i > 0; i--){
		message[i] = message[i-1];
	}
	message[0] = lastCharacter;
}

MenuIds openVoltageMenu(){
	
	MULTIMETER_MODE = MODE_VOLTAGE;
	int buttonArray[3] = {0,1,8};
	int size = 3;
	int buttonPressed = printAndWait("Select options", "1.Manual Range    2.Auotmatic Range ", buttonArray, size);
	MenuIds selectedMenu = MENU_ID_MEASUREMENT; 
	
	switch(buttonPressed) {
		case 0: 
			selectedMenu = MENU_ID_VOLTAGE_MANUAL_RANGE;
			break;
		case 1:
			selectedMenu = MENU_ID_VOLTAGE_AUTO_RANGE;
			break;
		case 8:
			selectedMenu = MENU_ID_OPEN;
			break;
	}
	
	return selectedMenu;
}


RangeMenuSettings openManualVoltage() {

	MULTIMETER_MODE = MODE_VOLTAGE;
	int buttonArray[5] = {0,1,2,3,8};
	int size = 5;
	
	selectMode(0x15);
	
	int buttonPressed = printAndWait("Voltage Manual", " 1.10V  2.1V  3.100mV 4.10mV ", buttonArray, size);
	RangeMenuSettings selectedSettings;
	selectedSettings.nextMenu = MENU_ID_MEASUREMENT;
	selectedSettings.selectedRange = RANGE_ID_RANGE_10;
	
	switch(buttonPressed) {
		case 0: 
			//manual range of 10 V
			selectedSettings.selectedRange = RANGE_ID_RANGE_10;
			break;
		case 1:
			//manual range of 1 V
			selectedSettings.selectedRange = RANGE_ID_RANGE_1;
			break;
		case 2:
			//manual range of 100 mV
			selectedSettings.selectedRange = RANGE_ID_RANGE_100m;
			break;
		case 3:
			//manual range of 10 mV
			selectedSettings.selectedRange = RANGE_ID_RANGE_10m;
			break; 
		case 8:
			selectedSettings.nextMenu = MENU_ID_VOLTAGE;
			//outputSignalOFF(4U);
			break;
	}
	
	return selectedSettings;
}

MenuIds openCurrentMenu(){
	
	MULTIMETER_MODE = MODE_CURRENT;
	
	int buttonArray[3] = {0,1,8};
	int size = 3;
	
	int buttonPressed = printAndWait("Select options", "1.Manual Range    2.Auotmatic Range ", buttonArray, size);
	MenuIds selectedMenu = MENU_ID_MEASUREMENT; 
	
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

MenuIds openResistanceMenu(){
	
	MULTIMETER_MODE = MODE_RESISTANCE;
	
	int buttonArray[3] = {0,1,8};
	int size = 3;
	int buttonPressed = printAndWait("Select options", " 1.Manual Range    2.Auotmatic Range ", buttonArray, size);
	MenuIds selectedMenu = MENU_ID_MEASUREMENT; 
	
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

MenuIds openMenu(){

	int buttonArray[4] = {0,1,2,8};
	int size = 4;
	int buttonPressed = printAndWait("Select Function", " 1.Voltage   2.Current   3.Resistance ", buttonArray, size);
	
	MenuIds selectedMenu = MENU_ID_VOLTAGE; 
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
			selectedMenu = MENU_ID_MEASUREMENT; //GO TO measuerment menu
			break;
	}
	return selectedMenu;
}


int printAndWait(char firstLineString[], char* secondLineString, int buttons[], int size) {
		
	lcd_write_string(firstLineString, 0, 0);

	int messageLength = strlen(secondLineString)-1;
	char* message = malloc(messageLength+1 * sizeof(char));
	strcpy(message, secondLineString);

	int buttonPressed = -1;
	
	while(buttonPressed == -1){
		//If message wont fit on the screen then scroll it
		if(messageLength > 16){
			scrollText(message, messageLength);
		}

		lcd_write_string(message, 1, 0);
		buttonPressed = DelayForButton(SCROLL_RATE, buttons, size);	
	}

	free(message);
	return buttonPressed; 
}


void menu(){
	MenuIds selectedMenuID = MENU_ID_OPEN;
	RangeMenuSettings selectedSettings;
	selectedSettings.selectedRange = RANGE_ID_RANGE_10;

	while(1){
		
		waitForRelease();
		lcd_clear_display();
		
		switch (selectedMenuID) {
			
			case MENU_ID_MEASUREMENT:
				LED_Out(0);
				measurementMenu(selectedSettings.selectedRange);
				selectedMenuID = MENU_ID_VOLTAGE_MANUAL_RANGE;
				break; 
			
			case MENU_ID_OPEN:
				LED_Out(7);
				selectedMenuID = openMenu();
				break;
			
			case MENU_ID_VOLTAGE:  //go to Voltage menu
				LED_Out(3);
				selectedMenuID = openVoltageMenu();
				break;
			
			case MENU_ID_VOLTAGE_MANUAL_RANGE:
				//manual range for Current
				//go to select range menu
				LED_Out(15);
				selectedSettings = openManualVoltage();	
				selectedMenuID = selectedSettings.nextMenu;
				break;
			
			case MENU_ID_VOLTAGE_AUTO_RANGE:
				//auto range for Current
				//auto ranging stuff 
				LED_Out(0);
				selectedMenuID = MENU_ID_MEASUREMENT;
				break;
			
			case MENU_ID_CURRENT:
				//got to Current menu
				LED_Out(3);
				selectedMenuID = openCurrentMenu();
				break; 
			
			case MENU_ID_CURRENT_MANUAL_RANGE:
				//manual range for CURRENT
				//go to select range menu
			 LED_Out(15);
				break;
			
			case MENU_ID_CURRENT_AUTO_RANGE:
				//auto range for CURRENT
				//auto ranging stuff 
				LED_Out(0);
				selectedMenuID = MENU_ID_MEASUREMENT;
				break;
			
			case MENU_ID_RESISTANCE:
				//got to resistance menu
			  LED_Out(3);
				selectedMenuID = openResistanceMenu();
				break;
			
			case MENU_ID_RESISTANCE_MANUAL_RANGE:
				//manual range for resistance
				//go to select range menu
				LED_Out(15);
				break;
			
			case MENU_ID_RESISTANCE_AUTO_RANGE:
				//auto range for resistance
				//auto ranging stuff 
				LED_Out(0);
				selectedMenuID = MENU_ID_MEASUREMENT;
				break;
			
			default:
				selectedMenuID = MENU_ID_MEASUREMENT;
				break;
		}	
	}
}

void measurementMenu(RangeIds range) {
		int buttonArray = 8;
		int size = 1;
	
	while(DelayForButton(500, &buttonArray, size) !=  8){
		
		unsigned int value = read_ADC1();
		float actualValue;
		float rangeValue;
		char* units;
		
		char* rangeString;
		char mode = 'M';
		
		unsigned int rangeMode = 0x3;
		unsigned int typeMode = 0x3<<2;
		
		switch (range) {
			
			default:
			case RANGE_ID_RANGE_10:
				rangeValue = 1.0 / SAMPLES_DEPTH;
				units = "V";
				rangeString = "10V";
				rangeMode = 0x0;
				break;
			
			case RANGE_ID_RANGE_1:
				rangeValue = 10.0 / SAMPLES_DEPTH;
				units = "V";
				rangeString = "1V";
				rangeMode = 0x1;
				break;
			
			case RANGE_ID_RANGE_100m:
				rangeValue = (100.0)/ SAMPLES_DEPTH;
				units = "mV";
				rangeString = "100mV";
				rangeMode = 0x2;
				break;
			
			case RANGE_ID_RANGE_10m:
				rangeValue = (1000.0)/ SAMPLES_DEPTH;
				units = "mV";
		    rangeString = "10mV";
				rangeMode = 0x3;
				break;
		}
		
		switch(MULTIMETER_MODE){
			
			case MODE_VOLTAGE:
				actualValue = value * rangeValue;
				display_Measure("Voltage", mode, rangeString, units, actualValue);
				typeMode = 0x0<<2;
				break;
			
			case MODE_CURRENT:
				actualValue = 3.3*value/4096.0 * 3.0;
				display_Measure("Current", mode, "1mA", "mA", actualValue);
			  typeMode = 0x1<<2;
				break;
			
			case MODE_RESISTANCE:
				actualValue = 3.3*value/(4096.0*0.000003);
				display_Measure("Resistance", mode, "1mOhm", "mOhm", actualValue);
				typeMode = 0x2<<2;
				break;
			
			default:  //default is MODE_Voltage
				MULTIMETER_MODE = MODE_VOLTAGE;
				break;
		}
		selectMode(typeMode | rangeMode);
	}
}


