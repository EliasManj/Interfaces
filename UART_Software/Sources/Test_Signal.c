/*
 * Test_Signal.c
 *
 *  Created on: Feb 7, 2019
 *      Author: Miguel Elias
 */

#include "Test_Signal.h"
#include "derivative.h" /* include peripheral declarations */

uint32_t n;

//Use PTB2

void Test_Signal_Init()
{
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	PORTB_PCR2 |= (1<<8);
	GPIOB_PDDR |= (1<<2);
	
	PORTB_PCR21 = (1 << 8);		//Set PTB21 as GPIO
	GPIOB_PDDR |= (1 << 21);	//Set PTB21 as output
}

void Test_Signal_Toggle()
{
	GPIOB_PDOR ^= (1<<2);
	GPIOB_PDOR ^= (1<<21); //Put PTB21 as HIGH
}

void Test_Count(uint32_t val){
	if(n++ == val){
		n = 0;
	}
}
