/*
			Functions header file
*/

#include "globals.h"

char* hemisphere(void);					// if ID = 0 returns South, if ID = 1 returns North
char* mode_Asc(void);					// checks Ascension   axis stepper state
char* mode_Dec(void);					// checks Declination axis stepper state

void SysTick_Handler(void);
void readColumns(void);
void readRows(void);
