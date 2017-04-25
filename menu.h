#ifndef __MENU_H
#define __MENU_H

#define  MODE_VOLTAGE 0
#define  MODE_CURRENT 1
#define  MODE_RESISTANCE 2

#define SAMPLES_DEPTH 3972

typedef enum {
	MENU_ID_MEASUREMENT,
	MENU_ID_OPEN,
	MENU_ID_VOLTAGE,
	MENU_ID_VOLTAGE_MANUAL_RANGE,
	MENU_ID_VOLTAGE_AUTO_RANGE,

	MENU_ID_CURRENT,
	MENU_ID_CURRENT_MANUAL_RANGE,
	MENU_ID_CURRENT_AUTO_RANGE,

	MENU_ID_RESISTANCE,
	MENU_ID_RESISTANCE_MANUAL_RANGE,
	MENU_ID_RESISTANCE_AUTO_RANGE,
	MENU_ID_RESISTANCE_CONTINUITY,
	
	MENU_ID_CAPACITANCE,
	
	MENU_ID_MAX,
	MENU_ID_MIN,
	
	MENU_ID_COMPUTER_LINK,
	MENU_ID_SIGNAL_GENERATOR,
}MenuIds;

//TODO rename 
typedef enum {
	RANGE_ID_RANGE_10 = 0,	 
	RANGE_ID_RANGE_1 = 1,
	RANGE_ID_RANGE_100m = 2,
	RANGE_ID_RANGE_10m = 3,
	RANGE_ID_RANGE_1mV = 4,
} RangeIds;


typedef struct{
	MenuIds nextMenu;
	RangeIds selectedRange;
}RangeMenuSettings;


void menu(void);
int printAndWait(char firstLineString[], char secondLineString[], int buttons[], int size);
MenuIds measurementMenu(int isAutoRangeOn, RangeIds range, int isContinuity);
float retSignedValue(int readValue, float rangeValue);
MenuIds computerLinkMenu(void);
MenuIds signalGeneratorMenu(void);
MenuIds capacitanceMenu(void);




#endif //__MENU_H
