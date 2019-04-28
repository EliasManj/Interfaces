/*
 * helper_functions.h
 *
 *  Created on: Apr 24, 2019
 *      Author: gonmigue
 */

#ifndef HELPER_FUNCTIONS_H_
#define HELPER_FUNCTIONS_H_

#include "derivative.h" /* include peripheral declarations */

#define RED 		0
#define GREEN 		1
#define BLUE	 	2

uint8_t _parse_uri(char *uri);
uint8_t _strcomp(volatile char *array1, volatile char *array2, uint16_t length);
uint8_t _strlen(char *str);

#endif /* HELPER_FUNCTIONS_H_ */
