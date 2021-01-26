/*
 * @file stepper.c
 * @author Michal Bogon
 * @date 03'rd Dec 2020
 * @brief File containing the main function.
 * @ver 1.0
 */
 
#include "MKL05Z4.h"                    //Device header
#include "stepper.h"
#include "globals.h"

static const IRQn_Type myPORTA_IRQn = PORTA_IRQn;

void Stepper_Init(void)
{
	// setting pins as outputs 
	SIM->SCGC5 					|= SIM_SCGC5_PORTA_MASK; 

	PORTA->PCR[DIR_ASC] 				|= PORT_PCR_MUX(1);
	PORTA->PCR[DIR_DEC]			 	|= PORT_PCR_MUX(1);
	PORTA->PCR[M0_ASC]  				|= PORT_PCR_MUX(1);
	PORTA->PCR[M0_DEC] 				|= PORT_PCR_MUX(1);
	PORTA->PCR[M1_ASC]  				|= PORT_PCR_MUX(1);
	PORTA->PCR[M1_DEC] 				|= PORT_PCR_MUX(1);
	PORTA->PCR[ASC_EN]				|= PORT_PCR_MUX(1);
	
	
	PTA->PDDR |= (1<<DIR_ASC) | (1<<DIR_DEC) | (1<<M0_ASC) | (1<<M0_DEC) | (1<<M1_ASC) | (1<<M1_DEC) | (1<<ASC_EN);
	PTA->PSOR |= (1<<DIR_ASC) | (1<<DIR_DEC) | (1<<M0_ASC) | (1<<M0_DEC) | (1<<M1_ASC) | (1<<M1_DEC) | (1<<ASC_EN);
	
	
	PORTB->PCR[STEPPER_ASC] 	|= PORT_PCR_MUX(1);
	PORTB->PCR[STEPPER_DEC]  	|= PORT_PCR_MUX(1);
	PORTB->PCR[DEC_EN]		|= PORT_PCR_MUX(1);
	
	PTB->PDDR |= (1<<STEPPER_ASC) | (1<<STEPPER_DEC) | (1<<DEC_EN);
	PTB->PSOR |= (1<<STEPPER_ASC) | (1<<STEPPER_DEC);
	PTB->PCOR |= (1<<DEC_EN);
	
	Step_Dec_Microstepping_2();
	
	// temporary setting
	PTA->PDOR |= (1<<DIR_ASC);
	
	NVIC_SetPriority(myPORTA_IRQn, 3);
	
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);
}

// ASCENTION AXIS STEPPER FUNCTIONS

void Step_Asc_start(void)
{
	PTA->PDOR |= (1<<ASC_EN);
	step_asc_en = 1;
}

void Step_Asc_stop(void)
{
	PTA->PCOR |= (1<<ASC_EN);
	step_asc_en = 0;
}

void Step_Asc_modeChng(void)
{
	PTA->PTOR |= (1<<ASC_EN);
	step_asc_en ^= 1;
}

void Step_Asc_Microstepping_16(void)
{
	// High states on both M0 and M1 for 1/16 microstepping
	PTA->PDOR |= (1<<M0_ASC);
	PTA->PDOR |= (1<<M1_ASC);
	step_asc_acc = 0;
}

void Step_Asc_Microstepping_2(void)
{
	// High state on M0 and low on M1 for 1/2 microstepping
	PTA->PDOR |= (1<<M0_ASC);
	PTA->PCOR |= (1<<M1_ASC);
	step_asc_acc = 1;
}

void Step_Asc_DirChng(void)
{
	PTA->PTOR |= (1<<DIR_ASC); 
	ascensionAxisDirection ^= 1;
}

// DECLINATION AXIS STEPPER FUNCTIONS

void Step_Dec_start(void)
{
	PTB->PSOR |= (1<<DEC_EN);
	step_dec_en = 1;
}

void Step_Dec_stop(void)
{
	PTB->PCOR |= (1<<DEC_EN); 
	step_dec_en = 0;
}

void Step_Dec_Microstepping_2(void)
{
	// High state 
	PTA->PDOR |= (1<<M0_DEC);
	PTA->PCOR |= (1<<M1_DEC);
}

void Step_Dec_DirChng(void)
{
	PTA->PTOR |= (1<<DIR_DEC); 
	declinationAxisDirection ^= 1;
}

void Step_Dec_DirHigh(void)
{
	PTA->PDOR |= (1<<DIR_DEC); 
	declinationAxisDirection = 1;
}

void Step_Dec_DirLow(void)
{
	PTA->PCOR |= (1<<DIR_DEC); 
	declinationAxisDirection = 0;
}
