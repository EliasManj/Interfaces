/*
 * helper_functions.c
 *
 *  Created on: Apr 24, 2019
 *      Author: gonmigue
 */

#include "helper_functions.h"

uint8_t _strlen(char *str) {
	uint8_t i, length;
	for (i = 0; str[i] != '\0'; i++) {
		length++;
	}
	return length;
}

uint8_t _strcomp(volatile char *array1, volatile char *array2, uint16_t length) {
	uint32_t i;
	uint8_t response = 0;
	i = 0;
	while (array1[i] == array2[i] && response == 0) {
		if (array1[i] == '\0' || array2[i] == '\0' || i == (length - 1)) {
			response = 1;
		}
		i++;
	}
	return response;
}

uint8_t _parse_uri(char *uri) {
	if (_strcomp(uri, "LED-R", 5)) {
		return RED;
	} else if (_strcomp(uri, "LED-B", 5)) {
		return BLUE;
	} else if (_strcomp(uri, "LED-G", 5)) {
		return GREEN;
	}
}
