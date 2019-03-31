/*
 * RGB.c
 *
 *  Created on: Mar 30, 2019
 *      Author: Miguel Elias
 */

#include "RGB.h"

	void RGB_init(void)
{
	//Activate SIM_SCGC5 (system control gating register 5) for port B and D
	SIM_SCGC5 |= (1 << 10);		//Port B
	SIM_SCGC5 |= (1 << 12);		//Port D
	PORTB_PCR18 = (1 << 8);		//Set PTB18 as GPIO
	PORTB_PCR19 = (1 << 8);		//set PTB19 as GPIO
	PORTD_PCR1 = (1 << 8);		//Set PTD1 as GPIO
	GPIOB_PDDR = (1 << 18);		//Set PTB18 as output
	GPIOB_PDDR |= (1 << 19);	//set PTB19 as output
	GPIOD_PDDR = (1 << 1);		//Set PTD1 as output
}

void RGB(int r, int g, int b)
{
	r ^= 1;
	g ^= 1;
	b ^= 1;
	GPIOB_PDOR ^= (-(r) ^ GPIOB_PDOR) & (1 << 18);
	GPIOB_PDOR ^= (-(g) ^ GPIOB_PDOR) & (1 << 19);
	GPIOD_PDOR ^= (-(b) ^ GPIOD_PDOR) & (1 << 1);
}
