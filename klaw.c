#include "klaw.h"
#include "frdm_bsp.h"
#include "stepper.h"
#include "functions.h"
#include "lcd1602.h"
#include <time.h>




static const IRQn_Type myPORTB_IRQn = PORTB_IRQn;
extern uint8_t key_down;
extern uint8_t buf_key;


// This one below is working
void Klaw_Init(void)
{
		// Setting as input
    SIM->SCGC5 					|= SIM_SCGC5_PORTB_MASK; 
    PORTB->PCR[ROW1]  	|= PORT_PCR_MUX(1); 
    PORTB->PCR[ROW2]  	|= PORT_PCR_MUX(1); 
	PORTB->PCR[COL1]  	|= PORT_PCR_MUX(1);
	PORTB->PCR[COL2]  	|= PORT_PCR_MUX(1);
	PORTB->PCR[COL3]  	|= PORT_PCR_MUX(1);


    PORTB->PCR[ROW1]  |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;  // Enable pull-up resitor on Pin 1
    PORTB->PCR[ROW2]  |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

    PORTB -> PCR[ROW1]  |= PORT_PCR_IRQC(0xa);
    PORTB -> PCR[ROW2]  |= PORT_PCR_IRQC(0xa);
	
	// Setting as output
	PTB->PDDR |= (1<<COL1) | (1<<COL2) | (1<<COL3);
	PTB->PCOR |= (1<<COL1) | (1<<COL2) | (1<<COL3);
    
		
	NVIC_SetPriority(myPORTB_IRQn, 3);
		
    NVIC_ClearPendingIRQ(PORTB_IRQn);
    NVIC_EnableIRQ(PORTB_IRQn);
}




void PORTB_IRQHandler(void)
{
	//clear pending interrupts
	NVIC_ClearPendingIRQ(myPORTB_IRQn);
	
	// Rewind in south
  if((PORTB->ISFR & (1<<ROW1)))
	{
		if (debouncingFinished) 
		{
			PTB->PDOR |= (1<<COL1) | (1<<COL2) | (1<<COL3);
			for (int i = 0; i < 3; i++)
			{
				PTB->PCOR |= (1<<columns[i]);
				
				// Checking, if first column is LOW
				if ((i == 0) && (PTB->PDIR & (1 << ROW1) ) == 0)
				{
					uint8_t dirChng = 0;
					
					// Changing direction, if user set North Hemisphere
					if (ascensionAxisDirection != 0)
					{
						Step_Asc_DirChng();
						dirChng = 1;
					}
					Step_Asc_Microstepping_2();										// Changing microstepping to 1/2
					LCD1602_SetCursor(2,0);
					LCD1602_Print(mode_Asc());
					while ((PTB -> PDIR & (1 << ROW1) ) ==0);			// Waits till button is released
					
					// Changing to previous settings
					Step_Asc_Microstepping_16();
					if (dirChng)
						Step_Asc_DirChng();
					
					debouncingFinished = 0;
					DEBOUNCE_RESET
					PORTB->PCR[ROW1] |= PORT_PCR_IRQC_MASK;
					break;
				}
				
				// Checking, if second column is LOW
				else if((i == 1) && (PTB->PDIR & (1 << ROW1) ) == 0)
				{
					Step_Asc_modeChng();
					
					while ((PTB -> PDIR & (1 << ROW1) ) ==0);
					DEBOUNCE_RESET
					PORTB->PCR[ROW1] |= PORT_PCR_IRQC_MASK;
					break;
				}
				
				// Checking, if third column is LOW
				else if((i == 2) && (PTB->PDIR & (1 << ROW1) ) == 0)
				{
					uint8_t dirChng = 0;
					
					// Changing direction, if user set South Hemisphere
					if (ascensionAxisDirection != 1)
					{
						Step_Asc_DirChng();
						dirChng = 1;
					}
					Step_Asc_Microstepping_2();										// Changing microstepping to 1/2
					LCD1602_SetCursor(2,0);
					LCD1602_Print(mode_Asc());
					while ((PTB -> PDIR & (1 << ROW1) ) ==0);			// Waits till button is released
					
					// Changing to previous settings
					Step_Asc_Microstepping_16();
					if (dirChng)
						Step_Asc_DirChng();
					
					DEBOUNCE_RESET
					PORTB->PCR[ROW1] |= PORT_PCR_IRQC_MASK;
					break;
					}
				
				PTB->PDOR |= (1<<columns[i]);
				}
			PTB->PCOR |= (1<<COL1) | (1<<COL2) | (1<<COL3);
			}
		else
			debounce_flag = 1;
		PORTB->PCR[ROW1] |= PORT_PCR_IRQC_MASK;
	}

	else if((PORTB->ISFR & (1<<ROW2)))
	{
		if (debouncingFinished) 
		{
			PTB->PDOR |= (1<<COL1) | (1<<COL2) | (1<<COL3);
			for (int i = 0; i < 3; i++)
			{
				PTB->PCOR |= (1<<columns[i]);
				
				// Checking, if first column is LOW
				if ((i == 0) && ((PTB->PDIR & (1 << ROW2) ) == 0))
				{
					Step_Dec_DirHigh();
					Step_Dec_start();
					LCD1602_SetCursor(0,1);
					LCD1602_Print(mode_Dec());
					while ((PTB -> PDIR & (1 << ROW2) ) ==0);
					Step_Dec_stop();
					DEBOUNCE_RESET
					PORTB->PCR[ROW2] |= PORT_PCR_IRQC_MASK;
					break;
				}
				
				// Checking, if first column is LOW
				else if ((i == 1) && ((PTB->PDIR & (1 << ROW2) ) == 0))
				{
					Step_Asc_DirChng();
					while ((PTB -> PDIR & (1 << ROW2) ) ==0);
					DEBOUNCE_RESET
					PORTB->PCR[ROW2] |= PORT_PCR_IRQC_MASK;
					break;
				}
				
				// Checking, if first column is LOW
				else if ((i == 2) && ((PTB->PDIR & (1 << ROW2) ) == 0))
				{
					Step_Dec_DirLow();
					Step_Dec_start();
					LCD1602_SetCursor(0,1);
					LCD1602_Print(mode_Dec());
					while ((PTB -> PDIR & (1 << ROW2) ) ==0);
					Step_Dec_stop();
					DEBOUNCE_RESET
					PORTB->PCR[ROW2] |= PORT_PCR_IRQC_MASK;
					break;
				}
				PTB->PDOR |= (1<<columns[i]);
			}
			PTB->PCOR |= (1<<COL1) | (1<<COL2) | (1<<COL3);
		}
		else
			debounce_flag = 1;
		PORTB->PCR[ROW2] |= PORT_PCR_IRQC_MASK;
	}
	
	PORTB->ISFR = 0xffffffff;
}
