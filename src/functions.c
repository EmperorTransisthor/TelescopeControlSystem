
#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "functions.h"
#include "globals.h"

char* hemisphere()
{
	if (ascensionAxisDirection == 0)
		return "S";
	else if (ascensionAxisDirection == 1)
		return "N";
	else 
	{
		ascensionAxisDirection = (uint8_t)(ascensionAxisDirection) % 2;
		return hemisphere();
	}
}

char* mode_Asc()
{
	if (step_asc_en)
	{
		if (!step_asc_acc)
			return "Following Asc";
		else
			if (ascensionAxisDirection)
				return "Rewinding Asc>";
			else 
				return "Rewinding Asc<";
	}
	else
		return "Stop Asc";
		
}

char* mode_Dec()
{
	if (step_dec_en)
	{
		if (declinationAxisDirection)
			return "Rewinding Dec >>";
		else 
			return "Rewinding Dec <<";
	}
	else
			return "Stop Dec";
}

void readColumns()
{
	reading_flag = 1;
	
	// Setting rows as outputs
	PTB->PDDR |= (1<<ROW1) | (1<<ROW2);
	PTB->PSOR |= (1<<ROW1) | (1<<ROW2);
	
	// Setting columns as inputs
	PTB->PDDR &= ~(uint32_t)(1<<COL1);
	PTB->PDDR &= ~(uint32_t)(1<<COL2);
	PTB->PDDR &= ~(uint32_t)(1<<COL3);
	
}

void readRows()
{
	PTB->PDDR |= (1<<COL1) | (1<<COL2) | (1<<COL3);
	PTB->PCOR |= (1<<COL1) | (1<<COL2) | (1<<COL3);
	
	PTB->PDDR &= ~(1<<ROW1);
	PTB->PDDR &= ~(1<<ROW2);
	
	reading_flag = 0;
}

