#include "MKL05Z4.h"
#include "pit.h"
#include "globals.h"


static const IRQn_Type myPIT_IRQn = PIT_IRQn;


void PIT_IRQHandler()
{
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		PTB->PTOR |= (1 << STEPPER_ASC) | (1 << STEPPER_DEC);
		
	}
	
	
	else if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
		if (!debounce_flag)
			debouncingFinished = 1;
		else if (debounce_flag)
		{
			debounce--;
			debounce_flag = 0;
		}
		
		// If none of the buttons is pressed, reset debounce value
		if ((( PTB->PDIR & (1 << ROW1) ) == 0) ||		// Checking, if any of the rows is active
				(( PTB->PDIR & (1 << ROW2) ) == 0))			// If isn't, reset
		{
			DEBOUNCE_RESET
		}
	}
		
	//clear pending IRQ
	NVIC_ClearPendingIRQ(myPIT_IRQn);
	
	//PTB->PTOR |=(1<<10); // debug indicator
}




void PIT_Init(void)
{
	// Enable clock to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	
	// Enable module, freeze timers in debug mode
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;				// Włączenie modułu PIT
	
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(PIT_3_32sec);		// Załadowanie wartości startowej
	PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;		// Odblokowanie przerwania i wystartowanie licznika
	
	PIT->CHANNEL[1].LDVAL = PIT_LDVAL_TSV(PIT_1ms);		// Załadowanie wartości startowej
	PIT->CHANNEL[1].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;		// Odblokowanie przerwania i wystartowanie licznika
	
	NVIC_SetPriority(myPIT_IRQn, 2);
	
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn);	
}
