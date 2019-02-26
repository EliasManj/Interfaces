/*
 * GPS.c
 *
 *  Created on: Feb 21, 2019
 *      Author: Miguel Elias
 */

#include "GPS.h"

uint16_t comas_index[11] =
{ 6, 13, 15, 23, 25, 34, 36, 42, 48, 55, 61 };

uint8_t parse_gps_byte(GPS_Struct *gps, uint8_t byte)
{
	//Validate $ start byte
	if (gps->counter == 0)
	{
		if (byte != '$')
		{
			return 1;
		}
	}
	else
	{
		//GPGGA
		if (gps->counter >= $GPGGA_START && gps->counter <= $GPGGA_END)
		{
			gps->GPGGA_String[gps->field_counter++] = byte;
			//Validate GPGGA, if its not GPGGA -> drop it
			if (gps->counter == $GPGGA_END)
			{
				gps->field_counter = 0;
				if (validate_GPGGA(gps->GPGGA_String))
				{
					gps->counter = 0;
					return 1;
				}
			}
		}
		else if (gps->counter >= GPGGA_TIME_STAMP_START && gps->counter <= GPGGA_TIME_STAMP_END)
		{
			gps->time_stamp[gps->field_counter++] = byte;
			if (gps->counter == GPGGA_TIME_STAMP_END)
			{
				gps->field_counter = 0;
			}
		}
		else if (gps->counter >= GPGGA_LATITUDE_START && gps->counter <= GPGGA_LATITUDE_END)
		{
			gps->latitude[gps->field_counter++] = byte;
			if (gps->counter == GPGGA_LATITUDE_END)
			{
				gps->field_counter = 0;
			}
		}
		else if (gps->counter >= GPGGA_LONGUITUDE_START && gps->counter <= GPGGA_LONGUITUDE_END)
		{
			gps->longuitude[gps->field_counter++] = byte;
			if (gps->counter == GPGGA_LONGUITUDE_END)
			{
				gps->field_counter = 0;
			}
		}
		else if (gps->counter >= GPGGA_ALTITUDE_START && gps->counter <= GPGGA_ALTITUDE_END)
		{
			gps->altitude[gps->field_counter++] = byte;
			if (gps->counter == GPGGA_ALTITUDE_END)
			{
				gps->field_counter = 0;
			}
		}
	}
	gps->gps_string[gps->counter++] = byte;
	if (gps->counter >= GPGGA_GPS_STR_LEN)
	{
		gps->counter = 0;
	}
	return 0;
}

void get_DMS_data(GPS_Struct *gps){
	uint16_t latitude_DMS_Degree = gps->latitude;
}

uint8_t validate_GPGGA(uint8_t *MNEA)
{
	if (MNEA[0] == 'G' && MNEA[1] == 'P' && MNEA[2] == 'G' && MNEA[3] == 'G' && MNEA[4] == 'A')
	{
		return 0;
	}
	return 1;
}

void Debug_Byte(uint8_t uart_recive, uint8_t byte)
{
	if (uart_recive == byte)
	{
		__asm("nop");
	}
}
