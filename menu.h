#ifndef __MENU_H
#define __MENU_H

#define  MODE_VOLTAGE 1
#define  MODE_CURRENT 2
#define  MODE_RESISTANCE 3

void menu(void);
int printAndWait(char firstLineString[], char secondLineString[], int buttons[], int size);

#endif //__MENU_H
