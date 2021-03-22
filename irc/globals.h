/*
*	Global variables definitions
*/
#include <stdint.h>
#include <time.h>
#include "MKL05Z4.h"
#include "frdm_bsp.h"

#ifndef _GLOBALS_H
#define _GLOBALS_H

//1966080u
#define SysTick_3_32_sec						 122880u			// number of clock ticks for one Systick interrupt
#define PIT_3_32sec									1966080u
#define PIT_1ms											 167776u
#define HIGH(x)								PDOR |=	(1<<x)
#define LOW(x)								PCOR |= (1<<x)		
#define OUTPUT_A(x)			PTA->(x)
#define OUTPUT_B(x)			PTB->(x)
#define DEBOUNCE_RESET				debounce = 10, debouncingFinished = 0;			// setting debounce value to 50

/*
#ifndef __CLK_TCK
#define __CLK_TCK 								 41943040u
#endif
*/

/*
#define SCL 3	
#define SDA 4
*/

// Ports


typedef enum{ COL1 = 0, COL2 = 1, COL3 = 2,		// columns
							ROW1 = 6, ROW2 = 7, BUT6, 			// rows
							BUT7, BUT8, BUT9 
} ButtonType_PORTB;


typedef enum{ DIR_ASC = 6, M0_ASC = 8, M1_ASC = 10, ASC_EN = 12,
							DIR_DEC = 7, M0_DEC = 9, M1_DEC = 11//, DEC_EN = 5
} StepperType_PORTA;

typedef enum{ STEPPER_ASC = 8, 
							STEPPER_DEC = 9, DEC_EN = 10
} StepperType_PORTB;


// Globals
extern uint8_t reading_flag;
extern uint8_t step_asc_en;													// right ascension axis stepper flag
extern uint8_t step_dec_en;													// declination axis stepper flag	
extern uint8_t step_asc_acc;													// 
extern uint8_t ascensionAxisDirection;
extern uint8_t declinationAxisDirection;

// Debouncing
extern uint8_t debounce;
extern uint8_t debounce_flag;
extern uint8_t debouncingFinished;
extern uint8_t columns[3];

#endif
