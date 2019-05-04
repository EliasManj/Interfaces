/*
 * helper_functions.c
 *
 *  Created on: Apr 24, 2019
 *      Author: gonmigue
 */

#include "helper_functions.h"

int _strlen(char *str)
{
	uint8_t i, length;
	for (i = 0; str[i] != '\0'; i++)
	{
		length++;
	}
	return length;
}

int _strlenInts(char *str)
{
	uint8_t i, length;
	for (i = 0; str[i] != ' '; i++)
	{
		length++;
	}
	return length;
}

uint8_t _strcomp(volatile char *array1, volatile char *array2, uint16_t length)
{
	uint32_t i;
	uint8_t response = 0;
	i = 0;
	while (array1[i] == array2[i] && response == 0)
	{
		if (array1[i] == '\0' || array2[i] == '\0' || i == (length - 1))
		{
			response = 1;
		}
		i++;
	}
	return response;
}

uint8_t _strcopy(char *array1, char *array2, int length, int from1, int from2)
{
	if (_strlen(array1) >= length + from1 || _strlen(array2) >= length + from2)
	{
		return -1;
	}
	else
	{
		int i, j, k;
		i = from1;
		j = from2;
		k = 0;
		while (k < length)
		{
			array1[i] = array2[j];
			i++;
			j++;
			k++;
		}
	}
	return 0;
}

uint8_t _parse_uri(char *uri)
{
	if (_strcomp(uri, "LED-R", 5))
	{
		return RED;
	}
	else if (_strcomp(uri, "LED-B", 5))
	{
		return BLUE;
	}
	else if (_strcomp(uri, "LED-G", 5))
	{
		return GREEN;
	}
	return 0;
}

void delay(int val){
	int i;
	for(i = 0; i < val; i++);
}

void itoa(int value, char *result, int base)
{
	// check that the base if valid
	if (base < 2 || base > 36)
	{
		*result = '\0';
		return result;
	}

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do
	{
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
	} while (value);

	// Apply negative sign
	if (tmp_value < 0)
		*ptr++ = '-';
	*ptr-- = '\0';
	while (ptr1 < ptr)
	{
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}
