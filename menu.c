#include "STM32F4xx.h"
#include "LCD/lcd_driver.h"
#include "SWT.h"
#include "CommonFunctions.h"
#include "LED.h"
#include <string.h>
#include "menu.h"
#include <stdlib.h>
#include "ADC.h"
#include "StateMenu.h"


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
	
	selectMode(0xF);
	
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
			break;
	}
	
	return selectedMenu;
}

RangeMenuSettings openManualCurrent() {
	
	MULTIMETER_MODE = MODE_CURRENT;
	int buttonArray[4] = {0,1,2,8};
	int size = 4;
	
	selectMode(0xF);
	
	int buttonPressed = printAndWait("Current Manual", "1.1A  2.100mA 3.10mA ", buttonArray, size);
	RangeMenuSettings selectedSettings;
	selectedSettings.nextMenu = MENU_ID_MEASUREMENT;
	selectedSettings.selectedRange = RANGE_ID_RANGE_10;
	
	switch(buttonPressed) {

		case 1:
			//manual range of 1 A
			selectedSettings.selectedRange = RANGE_ID_RANGE_1;
			break;
		case 2:
			//manual range of 100 mA
			selectedSettings.selectedRange = RANGE_ID_RANGE_100m;
			break;
		case 3:
			//manual range of 10 mA
			selectedSettings.selectedRange = RANGE_ID_RANGE_10m;
			break; 
		case 8:
			selectedSettings.nextMenu = MENU_ID_CURRENT;
			break;
	}
	
	return selectedSettings;
	
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
			break;
	}
	
	return selectedMenu;
}

RangeMenuSettings openManualResistance() {
	
	MULTIMETER_MODE = MODE_RESISTANCE;
	int buttonArray[5] = {0,1,2,3,8};
	int size = 5;
	
	selectMode(0xF);
	
	int buttonPressed = printAndWait("Resistance Manual", "1.1kOhm  2.10kOhm 3.100kOhm 4.1MOhm ", buttonArray, size);
	RangeMenuSettings selectedSettings;
	selectedSettings.nextMenu = MENU_ID_MEASUREMENT;
	selectedSettings.selectedRange = RANGE_ID_RANGE_10;
	
	switch(buttonPressed) {
		case 0: 
			//manual range of 1kOhm
			selectedSettings.selectedRange = RANGE_ID_RANGE_1;
			break;
		case 1:
			//manual range of 10kOhm
			selectedSettings.selectedRange = RANGE_ID_RANGE_10;
			break;
		case 2:
			//manual range of 100kOhm
			selectedSettings.selectedRange = RANGE_ID_RANGE_100m;
			break;
		case 3:
			//manual range of 1MOhm
			selectedSettings.selectedRange = RANGE_ID_RANGE_10m;
			break; 
		case 8:
			selectedSettings.nextMenu = MENU_ID_RESISTANCE;
			break;
	}
	
	return selectedSettings;
	
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

	int autoRange = 0; 
	while(1){
		
		waitForRelease();
		lcd_clear_display();
		
		switch (selectedMenuID) {
			
			case MENU_ID_MEASUREMENT:
				LED_Out(0);
				selectedMenuID = measurementMenu(autoRange, selectedSettings.selectedRange);
			//	selectedMenuID = MENU_ID_VOLTAGE_MANUAL_RANGE;
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
			  autoRange = 0; 
				break;
			
			case MENU_ID_VOLTAGE_AUTO_RANGE:
				//auto range for Current
				//auto ranging stuff 
				LED_Out(0);
				selectedMenuID = MENU_ID_MEASUREMENT;
				autoRange = 1; 
				break;
			
			case MENU_ID_CURRENT:
				//got to Current menu
				LED_Out(3);
				selectedMenuID = openCurrentMenu();
				break; 
			
			case MENU_ID_CURRENT_MANUAL_RANGE:
				//manual range for CURRENT
				//go to select range menu
			  LED_Out(7);
				selectedSettings = openManualCurrent();
				selectedMenuID = selectedSettings.nextMenu;
				autoRange = 0; 
				break;
			
			case MENU_ID_CURRENT_AUTO_RANGE:
				//auto range for CURRENT
				//auto ranging stuff 
				LED_Out(0);
				selectedMenuID = MENU_ID_MEASUREMENT;
				autoRange = 1; 
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
				selectedSettings = openManualResistance();
				selectedMenuID = selectedSettings.nextMenu;
				autoRange = 0; 
				break;
			
			case MENU_ID_RESISTANCE_AUTO_RANGE:
				//auto range for resistance
				//auto ranging stuff 
				LED_Out(0);
				selectedMenuID = MENU_ID_MEASUREMENT;
				autoRange = 1; 
				break;
			
			default:
				selectedMenuID = MENU_ID_MEASUREMENT;
				autoRange = 1; 
				break;
		}	
	}
}

RangeIds autoRanging(RangeIds currentRange) {
	
	int value = read_ADC1();
		
	if (value >= SAMPLES_DEPTH && currentRange != RANGE_ID_RANGE_10) {
		currentRange--;
	} else if (value <= 10 && currentRange != RANGE_ID_RANGE_10m) {
		currentRange++;
	}
	return currentRange;
}

MenuIds measurementMenu(int isAutoRangeOn, RangeIds range) {
	
	int buttonArray = 8;
	int size = 1;
	MenuIds selectedMenu = MENU_ID_VOLTAGE_MANUAL_RANGE; 
	
	char* measurement = "";
	
	const double maximInputVoltage = 3.01; // The voltage which represents a maximum reading in the range
	
	while(DelayForButton(300, &buttonArray, size) !=  8){

		unsigned int value; // Raw value measured from ADC (0-4096)
		float actualValue; // Actual value being represented by the input (eg 100 mA)
		
		char* units; // The Units of the current value to be displayed.
		char* rangeString;// The range (and units) of this measurement.
		char modeString = 'M';// Indicated which mode the multimeter is in (Auto-range or manual).
		
		unsigned int rangeMode = 0x3;	//range Control signal.
		unsigned int typeMode = 0x3<<2; //Measurement type control signal (Eg. Are we measuring current?)
		
		if (isAutoRangeOn == 1) {
			range = autoRanging(range);
			modeString = 'A';
		}
		
		state currentState = stateLookUp[MULTIMETER_MODE][range];
		measurement = currentState.measurementString;
		typeMode = currentState.typeMode;
		rangeMode = currentState.rangeMode;
		rangeString = currentState.rangeString;
		units = currentState.unitString;
		
		selectMode(typeMode | rangeMode);
		value = read_ADC1();
		actualValue = retSignedValue(value, currentState.scalingFactor/maximInputVoltage);
		
		display_Measure(measurement, modeString, rangeString, units, actualValue);	
		
		//Set which menu to return to if the menu button is pressed.
		switch(MULTIMETER_MODE){
			
			case MODE_VOLTAGE:
			  selectedMenu = MENU_ID_VOLTAGE_MANUAL_RANGE;
				break;
			
			case MODE_CURRENT:
				selectedMenu = MENU_ID_CURRENT_MANUAL_RANGE;
				break;
			
			case MODE_RESISTANCE:
			 selectedMenu = MENU_ID_RESISTANCE_MANUAL_RANGE;
				break;
			
			default:  // Default to voltage Mode
				MULTIMETER_MODE = MODE_VOLTAGE;
			  selectedMenu = MENU_ID_RESISTANCE_MANUAL_RANGE;
				break;
		}		
	}
	
	return selectedMenu;
}

//Scales the adc reading to produce a meaningful measurement value.
float retSignedValue(int readValue, float rangeValue) { 
	return (readValue * (3.3/4096.0) * rangeValue) -10; 
}
