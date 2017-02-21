#ifndef __MENU_H
#define __MENU_H

#define  MODE_VOLTAGE 1
#define  MODE_CURRENT 2
#define  MODE_RESISTANCE 3

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
}MenuIds;


typedef enum {
	RANGE_ID_RANGE_10 = 3,	 //10 V, 1 A, 10 kohms 
	RANGE_ID_RANGE_1 = 2,	//10 V, 1 A, 10 kohms
	RANGE_ID_RANGE_100m = 1,
	RANGE_ID_RANGE_10m = 0,
} RangeIds;


typedef struct{
	MenuIds nextMenu;
	RangeIds selectedRange;
}RangeMenuSettings;


void menu(void);
int printAndWait(char firstLineString[], char secondLineString[], int buttons[], int size);
MenuIds measurementMenu(int isAutoRangeOn, RangeIds range);


#endif //__MENU_H
