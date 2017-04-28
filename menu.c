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
extern int bufferEmpty;
extern int numReceuives;

#define ADC_VREF 2.955f

int MULTIMETER_MODE = MODE_VOLTAGE;
int VOLTAGE_COUPLING_MODE = 0; //0 for DC coupling, 1 for AC Coupling (matches control signals)


#define SCROLL_RATE 400

#define PI 3.141592653

//const float maximInputVoltage = 2.91; // The input voltage which represents a maximum reading.
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
	int buttonPressed = printAndWait("Manual/Auto Mode?", "1.Manual Range    2.Auotmatic Range ", buttonArray, size);
	MenuIds selectedMenu = MENU_ID_MEASUREMENT; 
	
	switch(buttonPressed) {
		case 0: 
			selectedMenu = MENU_ID_VOLTAGE_MANUAL_RANGE;
			break;
		case 1:
			selectedMenu = MENU_ID_VOLTAGE_AUTO_RANGE;
			break;
		case 7:
			selectedMenu = MENU_ID_VOLTAGE_COUPLING_SELECT;;
			break;
	}
	
	return selectedMenu;
}

MenuIds openVoltageCouplingMenu(){
	
	MULTIMETER_MODE = MODE_VOLTAGE;
	int buttonArray[3] = {0,1,7};
	int size = 3;
	int buttonPressed = printAndWait("Select Coupling", "1.DC   2.AC  ", buttonArray, size);
	MenuIds selectedMenu = MENU_ID_VOLTAGE; //Default to voltage menu next (only other option is back)

	
	switch(buttonPressed) {
		case 0: 
			VOLTAGE_COUPLING_MODE = 0; //Set DC Coupling
			selectedMenu = MENU_ID_VOLTAGE;
			break;
		case 1:
			VOLTAGE_COUPLING_MODE = 1; //Set AC Coupling
			selectedMenu = MENU_ID_VOLTAGE;
			break;
		case 7:
			selectedMenu = MENU_ID_OPEN;
			break;
	}
	
	return selectedMenu;
}

RangeMenuSettings openManualVoltage() {

	MULTIMETER_MODE = MODE_VOLTAGE;
	int buttonArray[6] = {0,1,2,3,7};
	int size = 6;
	
	selectMode(0xF, 0xF);
	
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
		/*case 4:
			//manual range of 1mV
			selectedSettings.selectedRange = RANGE_ID_RANGE_1mV;
			break; */
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

MenuIds continutiyMenu(){
	int buttonArray[1] = {7};	// What button to listen for.
	int size = 1;					//Number of buttons to listen for.

	int buttonPressed = -1;
	lcd_write_string("Continuity test", 0 ,0);
	
	selectMode(0, 4);
	
	while( buttonPressed != 7 ){
		buttonPressed = DelayForButton(100, buttonArray, size);
		
		int value = read_ADC1();
		float V_in = value * (ADC_VREF/4096.0f);
		
		if (V_in < 0.0626f) {
			buzzerOn(200);
		}
	}
	return MENU_ID_RESISTANCE;
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
	
	int buttonArray[7] = {0,1,2,3,4,5,6};
	int size = 7;
	int buttonPressed = printAndWait("Select Function", " 1.Voltage 7.Signal  Generator  6.Computer Mode  5.Inductance 4.Capacitance  3.Resistance  2.Current ", buttonArray, size);
	
	MenuIds selectedMenu = MENU_ID_VOLTAGE; 
	switch (buttonPressed){
		default:
		case 0:
			selectedMenu = MENU_ID_VOLTAGE_COUPLING_SELECT;
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
			selectedMenu = MENU_ID_INDUCTANCE;
			break;
		
		case 5:
			selectedMenu = MENU_ID_COMPUTER_LINK;
			break;
		
		case 6:
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
			lcd_write_string(secondLineString, 1, 0);
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
	while(1){
		waitForRelease();
		lcd_clear_display();

		switch (selectedMenuID) {
			
			case MENU_ID_MEASUREMENT:
				LED_Out(224);
			
				selectedMenuID = measurementMenu(autoRange, selectedSettings.selectedRange);
				break; 
			
			case MENU_ID_OPEN:
				LED_Out(127);
				selectedMenuID = openMenu();
				break;
			
			case MENU_ID_VOLTAGE:  //go to Voltage menu
				LED_Out(131);
				selectedMenuID = openVoltageMenu();
				break;
			
			case MENU_ID_VOLTAGE_COUPLING_SELECT:
				LED_Out(131);
				selectedMenuID = openVoltageCouplingMenu();
				break;
			
			case MENU_ID_VOLTAGE_MANUAL_RANGE:
				//manual range for Current
				//go to select range menu
				LED_Out(143);
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
				LED_Out(131);
				selectedMenuID = openCurrentMenu();
				break; 
			
			case MENU_ID_CURRENT_MANUAL_RANGE:
				//manual range for CURRENT
				//go to select range menu
			  LED_Out(135);
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
			  LED_Out(135);
				selectedMenuID = openResistanceMenu();
				break;
			
			case MENU_ID_RESISTANCE_MANUAL_RANGE:
				//manual range for resistance
				//go to select range menu
				LED_Out(143);
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
				LED_Out(128);
				selectedMenuID = continutiyMenu();
				selectedSettings.selectedRange = RANGE_ID_RANGE_1mV; //go to continuity range
				break;
			
			case MENU_ID_MIN:
				LED_Out(128);
				selectedMenuID = openMinMenu();
				break;
			
			case MENU_ID_MAX:
				LED_Out(128);
				selectedMenuID = openMaxMenu();
				break;
			
			case MENU_ID_COMPUTER_LINK:
				LED_Out(128);
				computerLinkMenu();
				break;
			
			case MENU_ID_CAPACITANCE:
				LED_Out(128);
				selectedMenuID = capacitanceMenu();
				break;
			
			case MENU_ID_INDUCTANCE:
				LED_Out(128);
				selectedMenuID = inductanceMenu();
				break; 
			
			case MENU_ID_SIGNAL_GENERATOR:
				LED_Out(191);
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
	int i=0;
	
	while(1){
		if(bufferEmpty != 1){
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
				
			}	else if (strcmp(RECIEVE_BUFFER, "send value") == 0){
				//memset(RECIEVE_BUFFER, 0, RECIEVE_BUFFER_SIZE);
				//Delay(10);
				bufferEmpty=1;
				int value = read_ADC1();
				printf("\n%04d\n", value);
				
				
				//DEBUG
				lcd_clear_display();
				Delay(100); //NEED THIS, otherwise nothing is displayed
				lcd_write_string("Send Reading",0,0);
			}
		}
	}
}
	
MenuIds signalGeneratorMenu(){
	long frequency = 0;
	int incrFreq = 1;
	
	long max_frequency = 62500000; 
	long min_frequency = 0;
	char printedString[16];
	
	//turn on enable switch 
	GPIOE_SignalON(3);
	
	while(1){
		int buttonArray[8] = {0,1,2,3,4,5,6,7};
		int size = 8;
		
		sprintf(printedString, "Fq %lu          ", frequency);
	  int buttonPressed = printAndWait(printedString, " 1.+  2.-  3.Sine/Square  4.10k  5.100k  6.1M  7.10M  ", buttonArray, size);
		
		
		if (frequency >= 100 && frequency <1000) {
			incrFreq = 10;
		}	else if (frequency >= 1000 && frequency <10000) {
			incrFreq = 100;
		}	else if (frequency >= 10000 && frequency < 100000) {
			incrFreq = 1000;
		}	else if (frequency >= 100000 && frequency < 1000000 ) {
			incrFreq = 10000;
		}else if (frequency >= 1000000) {
			incrFreq = 100000;
		}else if (frequency < 100) {
			incrFreq = 1;
		}
		
		switch(buttonPressed) {
			case 0: 
				frequency += incrFreq;
				if (frequency > max_frequency) {
					frequency = max_frequency;
				}
				setFrequency(frequency);
				break;
				
			case 1:
				frequency -= incrFreq;
				if (frequency < min_frequency) {
					frequency = min_frequency;
				}
				setFrequency(frequency);
				break;
			
			case 2: 
				//Swap Sin/square wave bit
				GPIOB->ODR ^= 0x8000;
				Delay(10);
				break;
			
			case 3:
				frequency = 10000;
				Delay(10);
				break;
			
			case 4:
				frequency = 100000;
				Delay(10);
				break; 
			
			case 5:
				frequency = 1000000;
				Delay(10);
				break;
			
			case 6:
				frequency = 10000000;
				Delay(10);
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
	
		long storedFreq;
		int minValueRead = 5000;
		
		//set to resonance mode
		GPIOC_SignalON(7);
		
		int startRange = 0;
				
		char frqToWrite[16];
		int incStepSize = 10;
		
		//set voltage AC mode
		GPIOC_SignalON(13);
			
		for (startRange=0; startRange<=4; startRange++) {
			selectMode(2,startRange);
		
			lcd_write_string("Measuring freq: ", 0, 0);

			minValueRead = 5000;
			for (freq = 1; freq <= maxFreq; freq+=incStepSize) {
				setFrequency(freq);
				
				//calculate and set delay
				delay = (uint32_t) ( (1000.0/freq));
				Delay(250);
				
				value = read_ADC1();
				//actualValue = retSignedValue(value, 0.002*1000);
				printf("%ld,%d\n", freq, value);
				
				
				if (freq >= 100){
					int i=0;
				}
				
				if ( (freq > 1000) && (freq < 5000) ) {
					incStepSize = 100;
				} else if (freq > 5000) {
					incStepSize = 1000;
				} 

				//if peak detected stored the freq and the ADC value
				if (value < minValueRead) {
					minValueRead = value;
					storedFreq = freq;
				}
				
				if ( (value - minValueRead > 500 ) || (value > 4090) ) {
					incStepSize = 10;
					break;
				}
				
				sprintf(frqToWrite, "%ld-%u %d", freq, value, startRange);
				lcd_write_string(frqToWrite, 1,0);
			}
			
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


MenuIds inductanceMenu() {
	
	int buttonArray[1] = {7};	// What button to listen for.
	int size = 1;					//Number of buttons to listen for.

	int buttonPressed = -1;
	
	lcd_write_string("Inductance", 0,0);
	lcd_write_string("Do stuff here", 1, 0);
	
	while( buttonPressed != 7 ){
		buttonPressed = DelayForButton(100, buttonArray, size);
	}
	
	return MENU_ID_OPEN;
}

RangeIds autoRanging(RangeIds currentRange) {
	
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
			lowerLimit = RANGE_ID_RANGE_1mV;
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

MenuIds measurementMenu(int isAutoRangeOn, RangeIds range) {
	
	int buttonArray[3] = {5,6,7};	// What button to listen for.
	int size = 3;					//Number of buttons to listen for.
	MenuIds selectedMenu = MENU_ID_VOLTAGE_MANUAL_RANGE; 
	
	char* measurement = "";
	
	int buttonPressed = -1;
	
	maximumValue = 0.0;
	minimumValue = 10.0;
	
	//#define AVERAGING_SIZE 10
	//int valueHistory[AVERAGING_SIZE] = {0};
	//int averagingSamepleNum = 0;
	
	unsigned int value; // Raw value measured from ADC (0-4096)
	float actualValue; // Actual value being represented by the input (eg 100 mA)
	
	//set coupling
	//GPIOC->ODR &= ~(0x2000);
	//GPIOC->ODR |= (VOLTAGE_COUPLING_MODE << 13);
	
	//used to displayed the square root in AC
	float dummyFactor = 1;
	
	if (VOLTAGE_COUPLING_MODE == 1) {
		GPIOC_SignalON(13);
	} else {
		GPIOC_SignalOFF(13);
	}
	
	while( buttonPressed != 7 ){

		//TODO take the variable init out of the loop

		char* units; // The Units of the current value to be displayed.
		char* rangeString;// The range (and units) of this measurement.
		char modeString = 'M';// Indicated which mode the multimeter is in (Auto-range or manual).
		
		unsigned int rangeMode ;	//range Control signal.
		unsigned int typeMode ; //Measurement type control signal (Eg. Are we measuring current?)		
		float maxInputVoltage;
		buttonPressed = DelayForButton(300, buttonArray, size);
		
		if (buttonPressed == 6) { 
			return MENU_ID_MAX; //go to the maximum menu
		} else if (buttonPressed == 5) {
			return MENU_ID_MIN; //go to the minimum menu
		} else {
		
			if (isAutoRangeOn == 1) {
				range = autoRanging(range);
				modeString = 'A';
			}
			
			if (VOLTAGE_COUPLING_MODE == 1) {
				dummyFactor = 0.3535;
			} else {
				dummyFactor = 1;
			}
			
			state currentState = stateLookUp[MULTIMETER_MODE][range];
			measurement = currentState.measurementString;
			typeMode = currentState.typeMode;
			rangeMode = currentState.rangeMode;
			rangeString = currentState.rangeString;
			units = currentState.unitString;
			maxInputVoltage = currentState.maxInputVoltage;
			
			selectMode(typeMode , rangeMode);
			
			Delay(200);
			value = read_ADC1();
			
			/*
			valueHistory[averagingSamepleNum] = value;
			if(averagingSamepleNum > AVERAGING_SIZE-1){
				averagingSamepleNum++;
			} else {
				averagingSamepleNum = 0;
			}
			
			int averageValue = 0;
			for(int j=0; j<AVERAGING_SIZE; j++){
				averageValue += valueHistory[j]/AVERAGING_SIZE;
			} */
			
			printf(" %d \n", value);
			
			if (MULTIMETER_MODE == MODE_RESISTANCE) {
				
			}
			
			actualValue = retSignedValue(value, currentState.scalingFactor, maxInputVoltage) * (dummyFactor);
			
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
	/*		if ( isContinuity == 1 ) {
				if ( ( actualValue < 50 ) && (rangeMode == RANGE_ID_RANGE_1mV) ) {
					buzzerOn(500);
				}
			}*/
			
			//display meassurments on the LCD 

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


//Scales the adc reading to produce a meaningful measurement value.
float retSignedValue(int readValue, float scalingFactor, float maxInputVoltage) { 
	float retValue;
	//float maxUsableDivisions = (4096.0f/ADC_VREF) * maximInputVoltage;
	
	if ( (MULTIMETER_MODE == MODE_VOLTAGE || (MULTIMETER_MODE == MODE_CURRENT) ) ) {
		retValue = (readValue * (ADC_VREF/4096.0f) * scalingFactor/maxInputVoltage) - (scalingFactor/2.0f);
	} else {
		retValue = (readValue * (ADC_VREF/4096.0f) * scalingFactor/maxInputVoltage);
	} 	
	return retValue;
}
