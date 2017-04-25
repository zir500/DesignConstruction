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
#include "serial.h"
#include <stdio.h>
#include <string.h>
#include "DDS.h"
#include <math.h>

extern char RECIEVE_BUFFER[RECIEVE_BUFFER_SIZE];


int MULTIMETER_MODE = MODE_VOLTAGE;


#define SCROLL_RATE 400

#define PI 3.141592653

const float maximInputVoltage = 3.0; // The input voltage which represents a maximum reading.
float maximumValue = 0.0;
float minimumValue = 10.0;


void scrollText(char message[], int messageLength){

	char lastCharacter = message[messageLength];
	for(int i=messageLength; i > 0; i--){
		message[i] = message[i-1];
	}
	message[0] = lastCharacter;
}

MenuIds openVoltageMenu(){
	
	MULTIMETER_MODE = MODE_VOLTAGE;
	int buttonArray[3] = {0,1,7};
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
		case 7:
			selectedMenu = MENU_ID_OPEN;
			break;
	}
	
	return selectedMenu;
}

RangeMenuSettings openManualVoltage() {

	MULTIMETER_MODE = MODE_VOLTAGE;
	int buttonArray[6] = {0,1,2,3,4,7};
	int size = 6;
	
	selectMode(0xF, 0xF);
	
	int buttonPressed = printAndWait("Voltage Manual", " 1.10V  2.1V  3.100mV 4.10mV 5.1mV ", buttonArray, size);
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
		case 4:
			//manual range of 1mV
			selectedSettings.selectedRange = RANGE_ID_RANGE_1mV;
			break;
		case 7:
			selectedSettings.nextMenu = MENU_ID_VOLTAGE;
			break;
	}
	
	return selectedSettings;
}

MenuIds openCurrentMenu(){
	
	MULTIMETER_MODE = MODE_CURRENT;
	
	int buttonArray[3] = {0,1,7};
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
		
		case 7:
			selectedMenu = MENU_ID_OPEN;
			break;
	}
	
	return selectedMenu;
}

RangeMenuSettings openManualCurrent() {
	
	MULTIMETER_MODE = MODE_CURRENT;
	int buttonArray[4] = {0,1,2,7};
	int size = 4;
	
	selectMode(0xF, 0xF);
	
	int buttonPressed = printAndWait("Current Manual", "1.1A  2.100mA 3.10mA ", buttonArray, size);
	RangeMenuSettings selectedSettings;
	selectedSettings.nextMenu = MENU_ID_MEASUREMENT;
	selectedSettings.selectedRange = RANGE_ID_RANGE_1;
	
	switch(buttonPressed) {

		case 0:
			//manual range of 1 A
			selectedSettings.selectedRange = RANGE_ID_RANGE_1;
			break;
		case 1:
			//manual range of 100 mA
			selectedSettings.selectedRange = RANGE_ID_RANGE_100m;
			break;
		case 2:
			//manual range of 10 mA
			selectedSettings.selectedRange = RANGE_ID_RANGE_10m;
			break; 
		case 7:
			selectedSettings.nextMenu = MENU_ID_CURRENT;
			break;
	}
	
	return selectedSettings;
	
}

MenuIds openResistanceMenu(){
	
	MULTIMETER_MODE = MODE_RESISTANCE;
	
	int buttonArray[4] = {0,1,2,7};
	int size = 4;
	int buttonPressed = printAndWait("Select options", " 1.Manual Range  2.Auotmatic Range  3.Continuity Test  ", buttonArray, size);
	MenuIds selectedMenu = MENU_ID_MEASUREMENT; 
	
	switch(buttonPressed) {
		case 0: 
			selectedMenu = MENU_ID_RESISTANCE_MANUAL_RANGE;
			break;
		
		case 1:
			selectedMenu = MENU_ID_RESISTANCE_AUTO_RANGE;
			break;
		
		case 2: 
			selectedMenu = MENU_ID_RESISTANCE_CONTINUITY;
			break;
		
		case 7:
			selectedMenu = MENU_ID_OPEN;
			break;
	}
	
	return selectedMenu;
}

RangeMenuSettings openManualResistance() {
	
	MULTIMETER_MODE = MODE_RESISTANCE;
	int buttonArray[5] = {0,1,2,3,7};
	int size = 5;
	
	selectMode(0xF, 0xF);
	
	int buttonPressed = printAndWait("Resistance Manual", "1.1MOhm 2.100kOhm 3.10kOhm 4.1kOhm ", buttonArray, size);

	RangeMenuSettings selectedSettings;
	selectedSettings.nextMenu = MENU_ID_MEASUREMENT;
	selectedSettings.selectedRange = RANGE_ID_RANGE_10;
	
	switch(buttonPressed) {
		case 0: 
			//manual range of 1MOhm
			selectedSettings.selectedRange = RANGE_ID_RANGE_10;
			break;
		case 1:
			//manual range of 100kOhm
			selectedSettings.selectedRange = RANGE_ID_RANGE_1;
			break;
		case 2:
			//manual range of 10kOhm
			selectedSettings.selectedRange = RANGE_ID_RANGE_100m;
			break;
		case 3:
			//manual range of 1kOhm
		  selectedSettings.selectedRange = RANGE_ID_RANGE_10m;
			break; 
		case 7:
			selectedSettings.nextMenu = MENU_ID_RESISTANCE;
			break;
	}
	
	return selectedSettings;
	
}

MenuIds openMenu(){

	int buttonArray[6] = {0,1,2,3,4,5};
	int size = 6;
	int buttonPressed = printAndWait("Select Function", " 1.Voltage  2.Current  3.Resistance 4.Capacitance 5.Computer Mode  6.Signal Generator  ", buttonArray, size);
	
	MenuIds selectedMenu = MENU_ID_VOLTAGE; 
	switch (buttonPressed){
		default:
		case 0:
			selectedMenu = MENU_ID_VOLTAGE;
			break;
		
		case 1:
			selectedMenu = MENU_ID_CURRENT;
			break;
				
		case 2:
			selectedMenu =  MENU_ID_RESISTANCE;
			break;
		
		case 3:
			selectedMenu = MENU_ID_CAPACITANCE;
			break;
		
		case 4:
			selectedMenu = MENU_ID_COMPUTER_LINK;;
			break;
		
		case 5:
			selectedMenu = MENU_ID_SIGNAL_GENERATOR;
			break;
	}
	return selectedMenu;
}


MenuIds openMaxMenu(){
	
	int button = 7;
	int size = 1;
		
	char stringValue[16];
	sprintf(stringValue, "%.4f", maximumValue);
	
	char stringMeasure[16];
	
	MenuIds selectedMenu = MENU_ID_MEASUREMENT; 
		switch(MULTIMETER_MODE) {
			case MODE_VOLTAGE: 
				strncpy(stringMeasure, "Maximum Voltage", 15); 
				break;
			
			case MODE_CURRENT:
				strncpy(stringMeasure, "Maximum Current", 15); 
				break;
			
			case MODE_RESISTANCE:
				strncpy(stringMeasure, "Maximum Resist.", 15); 
				break;
		}
		
	int buttonPressed = printAndWait(stringMeasure, stringValue, &button, size);
	
	return selectedMenu;
}

MenuIds openMinMenu(){
	
	int button = 7;
	int size = 1;
		
	char stringValue[16];
	sprintf(stringValue, "%.4f", minimumValue);
	
	char stringMeasure[16];
	
	MenuIds selectedMenu = MENU_ID_MEASUREMENT; 
		switch(MULTIMETER_MODE) {
			case MODE_VOLTAGE: 
				strncpy(stringMeasure, "Minimum Voltage", 15); 
				break;
			
			case MODE_CURRENT:
				strncpy(stringMeasure, "Minimum Current", 15); 
				break;
			
			case MODE_RESISTANCE:
				strncpy(stringMeasure, "Minimum Resist.", 15); 
				break;
		}
	
	int buttonPressed = printAndWait(stringMeasure, stringValue, &button, size);
	
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
			char truncatedMsg[17];
			strncpy(truncatedMsg, message, 16);
			truncatedMsg[16] = '\0';
			lcd_write_string(truncatedMsg, 1, 0);
			
		} else {
			lcd_write_string(message, 1, 0);
		}
		buttonPressed = DelayForButton(SCROLL_RATE, buttons, size);	
	}

	free(message);
	return buttonPressed; 
}

void menu(){
	MenuIds selectedMenuID = MENU_ID_OPEN;
	RangeMenuSettings selectedSettings;
	selectedSettings.selectedRange = RANGE_ID_RANGE_1;

	int autoRange = 0; 
	int isContinuity = 0;
	while(1){
		
		waitForRelease();
		lcd_clear_display();
		//back button always on
		LED_On(7);

		switch (selectedMenuID) {
			
			case MENU_ID_MEASUREMENT:
				LED_Out(96);
			
				selectedMenuID = measurementMenu(autoRange, selectedSettings.selectedRange, isContinuity);
				//reset
				isContinuity = 0;
				break; 
			
			case MENU_ID_OPEN:
				LED_Out(63);
				selectedMenuID = openMenu();
				break;
			
			case MENU_ID_VOLTAGE:  //go to Voltage menu
				LED_Out(3);
				selectedMenuID = openVoltageMenu();
				break;
			
			case MENU_ID_VOLTAGE_MANUAL_RANGE:
				//manual range for Current
				//go to select range menu
				LED_Out(31);
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

				//to avoid in gettting the error state (idx = o of StateMenu struct)
				selectedSettings.selectedRange = RANGE_ID_RANGE_1;
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
			
			case MENU_ID_RESISTANCE_CONTINUITY:
				selectedMenuID = MENU_ID_MEASUREMENT;
				autoRange = 1; 		
			  isContinuity = 1;
				break;
			
			case MENU_ID_MIN:
				LED_Out(0);
				selectedMenuID = openMinMenu();
				break;
			
			case MENU_ID_MAX:
				LED_Out(0);
				selectedMenuID = openMaxMenu();
				break;
			
			case MENU_ID_COMPUTER_LINK:
				//LED_Out(128);
				computerLinkMenu();
				break;
			
			case MENU_ID_CAPACITANCE:
			
				selectedMenuID = capacitanceMenu();
				break;
			
			case MENU_ID_SIGNAL_GENERATOR:
				LED_Out(128);
				selectedMenuID = signalGeneratorMenu();
				break;
			
			default:
				selectedMenuID = MENU_ID_MEASUREMENT;
				autoRange = 1; 
				break;
		}	
	}	
}

// Work in progress 
MenuIds computerLinkMenu(){
	while(1){
		if (strcmp(RECIEVE_BUFFER, "voltage mode") == 0) {
			//go to voltage mode
			lcd_clear_display();
			Delay(100); //NEED THIS, otherwise nothing is displayed
			lcd_write_string("Voltage Mode",0,0);
				memset(RECIEVE_BUFFER, 0, RECIEVE_BUFFER_SIZE);
			MULTIMETER_MODE = MODE_VOLTAGE;
			
		} else if (strcmp(RECIEVE_BUFFER, "resistance mode") == 0) {
			//go to resistance mode 
			lcd_clear_display();
			Delay(100); //NEED THIS, otherwise nothing is displayed
			lcd_write_string("Resistance Mode",0,0);
			memset(RECIEVE_BUFFER, 0, RECIEVE_BUFFER_SIZE);
		}	
			printf("\n%04d\n", read_ADC1());
	}
}
	
MenuIds signalGeneratorMenu(){
	long frequency = 1000;
	
	long max_frequency = 60000000; 
	long min_frequency = 1;
	char printedString[16];
	
	//turn on enable switch 
	GPIOE_SignalON(3);
	
	while(1){
		int buttonArray[4] = {0,1,2,7};
		int size = 4;
		
		sprintf(printedString, "Fq %lu            ", frequency);
		int buttonPressed = printAndWait(printedString, " 1.+  2.-  3.Sine/Square  ", buttonArray, size);
		
		
		
		switch(buttonPressed) {
			case 0: 
				frequency += 1000;
				if (frequency > max_frequency) {
					frequency = max_frequency;
				}
				setFrequency(frequency);
				break;
				
			case 1:
				frequency -= 1000;
				if (frequency < min_frequency) {
					frequency = min_frequency;
				}
				setFrequency(frequency);
				break;
			
			case 2: 
				//Swap Sin/square wave bit
				GPIOB->ODR ^= 0x8000;
				break;
			
			case 7:
				return MENU_ID_OPEN;
		}
	}
}

MenuIds capacitanceMenu() {
	
	int buttonArray[1] = {7};	// What button to listen for.
	int size = 1;					//Number of buttons to listen for.

	int buttonPressed = -1;
	
	while( buttonPressed != 7 ){

		buttonPressed = DelayForButton(100, buttonArray, size);
		
		long freq;
		long maxFreq = 30000;
		
		//set to sinWave
		GPIOB_SignalOFF(15);
		
		uint32_t delay;
		
		unsigned int value; // Raw value measured from ADC (0-4096)
		int actualValue = 0;
	
		int storedActualValue = 0;
		long storedFreq;
		int minValueRead = 5000;
		
		//set to resonance mode
		GPIOC_SignalON(7);
		
		//set voltage mode (2) and lowest range (4)
		selectMode(2,0);
		
		//set voltage AC mode
		GPIOC_SignalON(13);
		lcd_write_string("Measuring freq: ", 0, 0);

	  char frqToWrite[16];
	
		int incStepSize = 10;

		for (freq = 1; freq <= maxFreq; freq+=incStepSize) {
			setFrequency(freq);
			
			//calculate and set delay
			delay = (uint32_t) ( (1.0/freq) / 1000.0);
			Delay(250);
			
			value = read_ADC1();
			actualValue = retSignedValue(value, 0.002*1000);
			printf("%d,", value);
			
			if(freq == 200){
				int j=1;
			} else if (freq == 87) {
				int j=1;
			} else if (freq == 400) {
				int j=1;
			}
			
			if ( (freq > 1000) && (freq < 5000) ) {
				incStepSize = 100;
			} else if (freq > 5000) {
				incStepSize = 1000;
			} 
			

			if (value < minValueRead) {
				minValueRead = value;
				storedFreq = freq;
				storedActualValue = actualValue;
			}
			sprintf(frqToWrite, "%ld-%u", freq, value);
			lcd_write_string(frqToWrite, 1,0);
			
		}
		
		lcd_clear_display();
		double capacitance;
		
		capacitance = (1 / ( pow( ( 2 * PI * storedFreq), 2) * 0.033 ) ); 		
		display_Measure("Capacitance", ' ', "", "F", capacitance);			
		
		Delay(10000);

		//set voltage DC mode
		GPIOC_SignalOFF(13);
		
		//set resonance off
		GPIOC_SignalOFF(7);
	
	} 
	
	return MENU_ID_OPEN;
}

//isContinuity acts like a Bool type 
RangeIds autoRanging(RangeIds currentRange, int isContinuity) {
	
	RangeIds upperLimit;
	RangeIds lowerLimit;
	
	switch(MULTIMETER_MODE) {
		
		default:
		case MODE_VOLTAGE:
			upperLimit = RANGE_ID_RANGE_10;
			lowerLimit = RANGE_ID_RANGE_1mV;
			break;
		case MODE_CURRENT:
			upperLimit = RANGE_ID_RANGE_1;
			lowerLimit = RANGE_ID_RANGE_10m;
			break;
		case MODE_RESISTANCE:
			upperLimit = RANGE_ID_RANGE_10;

			if ( isContinuity == 0 ) {
				lowerLimit = RANGE_ID_RANGE_10m;
			} else {
				lowerLimit = RANGE_ID_RANGE_1mV;
			}
			break; 
	}

	int value = read_ADC1();
	
	if ( (MULTIMETER_MODE == MODE_VOLTAGE) || (MULTIMETER_MODE == MODE_CURRENT) ) {
		
		// (2048 + 410, 2048 - 410) represents the acceptable band
		// if it is outside this, change to a more precise range
		if ( (value < 2048 + 410) && (value > 2048 - 410) && currentRange != lowerLimit) {
			currentRange++;
			buzzerOn(200); //short beep
		} else if ( (value > 4096 - 10) || (value < 10) ) {
			if (currentRange != upperLimit) {
				currentRange--;
				buzzerOn(200); //short beep
			} else {
				//TODO write in the report that this will be a continous beep, as the autorange is called continously
				buzzerOn(1000); //long beep 
			}
		}
	} else {
	
			if (value >= SAMPLES_DEPTH) {
				if ( currentRange != upperLimit ) {
					currentRange--;
					buzzerOn(200); //short beep
				} else {
					//TODO write in the report that this will be a continous beep, as the autorange is called continously
					buzzerOn(1000); //long beep
				}
			} else if (value <= 410 && currentRange != lowerLimit) { //410 represents the next range available
				currentRange++;
				buzzerOn(200); //short beep
			}
	}
	return currentRange;
}

MenuIds measurementMenu(int isAutoRangeOn, RangeIds range, int isContinuity) {
	
	int buttonArray[3] = {5,6,7};	// What button to listen for.
	int size = 3;					//Number of buttons to listen for.
	MenuIds selectedMenu = MENU_ID_VOLTAGE_MANUAL_RANGE; 
	
	char* measurement = "";
	
	int buttonPressed = -1;
	
	maximumValue = 0.0;
	minimumValue = 10.0;
	
	unsigned int value; // Raw value measured from ADC (0-4096)
	float actualValue; // Actual value being represented by the input (eg 100 mA)
	
	while( buttonPressed != 7 ){

		//TODO take the variable init out of the loop

		char* units; // The Units of the current value to be displayed.
		char* rangeString;// The range (and units) of this measurement.
		char modeString = 'M';// Indicated which mode the multimeter is in (Auto-range or manual).
		
		unsigned int rangeMode ;	//range Control signal.
		unsigned int typeMode ; //Measurement type control signal (Eg. Are we measuring current?)
		
		buttonPressed = DelayForButton(300, buttonArray, size);
		
		if (buttonPressed == 6) { 
			return MENU_ID_MAX; //go to the maximum menu
		} else if (buttonPressed == 5) {
			return MENU_ID_MIN; //go to the minimum menu
		} else {
		
			if (isAutoRangeOn == 1) {
				range = autoRanging(range, isContinuity);
				modeString = 'A';
			}
			
			state currentState = stateLookUp[MULTIMETER_MODE][range];
			measurement = currentState.measurementString;
			typeMode = currentState.typeMode;
			rangeMode = currentState.rangeMode;
			rangeString = currentState.rangeString;
			units = currentState.unitString;
			
			selectMode(typeMode , rangeMode);
			value = read_ADC1();
			actualValue = retSignedValue(value, currentState.scalingFactor);
			
			if (actualValue > maximumValue) {
				maximumValue = actualValue;
			}
			
			if (actualValue < minimumValue) {
				minimumValue = actualValue;
			}
			
			//turn buzzer on for manual ranges
			if ( ( (value > 4096 - 10) || (value < 10) ) && (MULTIMETER_MODE != MODE_RESISTANCE) ) {
				buzzerOn(500);
			}				
			
			//turn buzzer on for continuit test
			if ( isContinuity == 1 ) {
				if ( ( actualValue < 50 ) && (rangeMode == RANGE_ID_RANGE_1mV) ) {
					buzzerOn(500);
				}
			}
			
			display_Measure(measurement, modeString, rangeString, units, actualValue);	
			
			//Set which menu to return to if the menu button is pressed.
			switch(MULTIMETER_MODE){
				
				default:
				case MODE_VOLTAGE:
					selectedMenu = MENU_ID_VOLTAGE_MANUAL_RANGE;
					break;
				
				case MODE_CURRENT:
					selectedMenu = MENU_ID_CURRENT_MANUAL_RANGE;
					break;
				
				case MODE_RESISTANCE:
					selectedMenu = MENU_ID_RESISTANCE_MANUAL_RANGE;
					break;
			}		
		}
	}
	return selectedMenu;
}

const float ADC_VREF = 3.0;

//Scales the adc reading to produce a meaningful measurement value.
float retSignedValue(int readValue, float scalingFactor) { 
	float retValue;
	//float maxUsableDivisions = (4096.0f/ADC_VREF) * maximInputVoltage;
	
	if ( (MULTIMETER_MODE == MODE_VOLTAGE) || (MULTIMETER_MODE == MODE_CURRENT) ) {
		retValue = (readValue * (ADC_VREF/4096.0f) * scalingFactor/maximInputVoltage) - (scalingFactor/2.0f);
	} else {
		retValue = (readValue * (ADC_VREF/4096.0f) * scalingFactor/maximInputVoltage);
	} 	
	return retValue;
}
