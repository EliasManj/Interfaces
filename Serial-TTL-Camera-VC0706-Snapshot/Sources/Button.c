/*
 * Button.c
 *
 *  Created on: May 11, 2019
 *      Author: Miguel Elias
 */
#include "Button.h"

void Button_SW2_Init(void)
{
	SIM_SCGC5 |= (1 << 11);
	PORTC_PCR6 |= (1<<8);
	PORTC_PCR6 |= (8<<16);
	NVIC_ICPR(1) |= (1<<(61%32));		//Clean flag of LPTM in the interrupt vector
	NVIC_ISER(1) |= (1<<(61%32)); 		//Activate the LPTM interrupt
}
