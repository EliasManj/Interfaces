/*
 * Checks.h
 *
 *  Created on: Apr 22, 2019
 *      Author: gonmigue
 */

#ifndef CHECKS_H_
#define CHECKS_H_

#include "derivative.h"



uint8_t Check_Address_Type(volatile uint8_t byte[]) {
	if (byte[0] == 'A' && byte[1] == 'P' && byte[2] == 'I' && byte[3] == 'P')
		return APIP;
	if (byte[0] == 'A' && byte[1] == 'P' && byte[2] == 'M' && byte[3] == 'A'
			&& byte[4] == 'C')
		return APMAC;
	if (byte[0] == 'S' && byte[1] == 'T' && byte[2] == 'A' && byte[3] == 'I'
			&& byte[4] == 'P')
		return STAIP;
	if (byte[0] == 'S' && byte[1] == 'T' && byte[2] == 'A' && byte[3] == 'M'
			&& byte[4] == 'A' && byte[5] == 'C')
		return STAMAC;
	return 0;
}

uint8_t Check_CIPMUX(volatile uint8_t byte[]){
	return string_compare(byte, (uint8_t *)"AT+CIPMUX=1", 11);
}

uint8_t string_compare(volatile uint8_t *array1, volatile uint8_t *array2, uint16_t until)
{
	uint32_t i;
	uint8_t response = 0;
	i = 0;
	while (array1[i] == array2[i] && response == 0)
	{
		if (array1[i] == '\0' || array2[i] == '\0' || until == i)
		{
			response = 1;
		}
		i++;
	}
	return response;
}

#endif /* CHECKS_H_ */
