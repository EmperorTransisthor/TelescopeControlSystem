/*-------------------------------------------------------------------------
					Technika Mikroprocesorowa 2 - laboratorium
					Projekt systemu sterowania teleskopem
					autor: Michał Bogoń
					wersja: 24.01.2021r.
----------------------------------------------------------------------------*/
					
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "globals.h"
#include "klaw.h"
#include "stepper.h"
#include "functions.h"
#include "pit.h"



uint8_t step_asc_en = 1;
uint8_t step_dec_en = 0;
uint8_t step_asc_acc = 0;
uint8_t ascensionAxisDirection = 0;
uint8_t declinationAxisDirection = 0;

uint8_t reading_flag = 0;
uint8_t debounce = 10;
uint8_t debounce_flag = 0;
uint8_t debouncingFinished = 0;
uint8_t columns[3] = { (uint8_t)(COL1), (uint8_t)(COL2), (uint8_t)(COL3) };


int main (void)
{
	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	
	LCD1602_Init();		 // Inicjalizacja wyświetlacza LCD
	LCD1602_Backlight(TRUE);
	LCD1602_Print("---");				// Ekran kontrolny - nie zniknie, jeśli dalsza część programu nie działa
	DELAY(10)
	LCD1602_SetCursor(0,0);
	LCD1602_Print("PIT init");
	DELAY(10)
	
	// PIT init
	PIT_Init();
	LCD1602_Print("Klaw init");
	DELAY(10)
	Klaw_Init();
	
	LCD1602_Print("Stepper init");
	DELAY(10)
	
	
	while(1)
	{
		LCD1602_ClearAll();
		LCD1602_SetCursor(0,0);
		sprintf(display, "%s", hemisphere());
		LCD1602_Print(display);
		LCD1602_SetCursor(2,0);
		LCD1602_Print(mode_Asc());
		LCD1602_SetCursor(0,1);
		LCD1602_Print(mode_Dec());
		DELAY(1000)
	}
}
