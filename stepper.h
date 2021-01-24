/*
*			Stepper library
*/

#include <stdint.h>
#include "globals.h"

#ifndef _STEPPER_H
#define _STEPPER_H

#define M0_16										1							//	High state for 1/16 microstepping for M0 driver resolution selector input
#define M0_2										1							//	High state for 1/2  microstepping for M0 driver resolution selector input
#define M1_16										1							//	High state for 1/16 microstepping for M1 driver resolution selector input
#define M1_2										0							//	Low  state for 1/2  microstepping for M1 driver resolution selector input

void Stepper_Init(void);											// Initialization of PORTA and specifying outputs
void Step_Asc_start(void);										// Function enabling  Declination axis stepper
void Step_Asc_stop(void);											// Function disabling Declination axis stepper
void Step_Asc_modeChng(void);
void Step_Asc_Microstepping_16(void);					// Function setting 1/16 microstepping for Ascension axis stepper
void Step_Asc_Microstepping_2(void);					// Function setting 1/2  microstepping for Ascension axis stepper
void Step_Asc_DirChng(void);									// Function changing direction of Ascention 	axis stepper

void Step_Dec_start(void);										// Function enabling  Declination axis stepper
void Step_Dec_stop(void);											// Function disabling Declination axis stepper						
void Step_Dec_Microstepping_2(void);
void Step_Dec_DirChng(void);									// Function changing direction of Declination axis stepper
void Step_Dec_DirHigh(void);									// Function changing direction of Declination axis stepper
void Step_Dec_DirLow(void);										// Function changing direction of Declination axis stepper

#endif
