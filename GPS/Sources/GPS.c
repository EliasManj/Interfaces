/*
 * GPS.c
 *
 *  Created on: Feb 21, 2019
 *      Author: Miguel Elias
 */

#include "GPS.h"

uint16_t comas_index[11] =
{ 6, 13, 15, 23, 25, 34, 36, 42, 48, 55, 61 };

uint8_t parse_gps_byte_commas(GPS_Struct *gps, uint8_t byte)
{
	gps->gps_string[gps->counter++] = byte;
	switch (gps->current_state)
	{
	case IDLE:
		if (byte == '$')
		{
			gps->counter = 1;
			gps->current_state = GPGGA;
		}
		break;
	case GPGGA:
		if (byte == ',')
		{
			gps->field_counter = 0;
			if (validate_GPGGA(gps->GPGGA_String))
			{
				gps->current_state = IDLE;
			}
			else
			{
				gps->current_state = TIME_STAMP;
			}
		}
		else
		{
			gps->GPGGA_String[gps->field_counter++] = byte;
		}
		break;
	case TIME_STAMP:
		if (byte == ',')
		{
			gps->field_counter = 0;
			gps->current_state = LATITUDE;
		}
		else
		{
			gps->time_stamp[gps->field_counter++] = byte;
		}
		break;
	case LATITUDE:
		if (byte == ',')
		{
			gps->field_counter = 0;
			gps->current_state = NORTH_SOUTH;
		}
		else
		{
			gps->latitude[gps->field_counter++] = byte;
		}
		break;
	case NORTH_SOUTH:
		if (byte == ',')
		{
			gps->field_counter = 0;
			gps->current_state = LONGUITUDE;
		}
		else
		{
			if (byte == 'N' || byte == 'S')
			{
				gps->north_south = byte;
			}
			else
			{
				gps->current_state = IDLE;
			}
		}
		break;
	case LONGUITUDE:
		if (byte == ',')
		{
			gps->field_counter = 0;
			gps->current_state = EAST_WEST;
		}
		else
		{
			gps->longuitude[gps->field_counter++] = byte;
		}
		break;
	case EAST_WEST:
		if (byte == ',')
		{
			gps->field_counter = 0;
			gps->current_state = ALTITUDE;
		}
		else
		{
			if (byte == 'E' || byte == 'W')
			{
				gps->east_west = byte;
			}
			else
			{
				gps->current_state = IDLE;
			}
		}
		break;
	case ALTITUDE:
		if (byte == ',')
		{
			gps->field_counter = 0;
			gps->current_state = IDLE;
			set_DMS_data(gps);
		}
		else
		{
			gps->altitude[gps->field_counter++] = byte;
		}
		break;
	}
	return 0;
}

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
		set_DMS_data(gps);
		gps->counter = 0;
	}
	return 0;
}

void set_DMS_data(GPS_Struct *gps)
{
	//Get latitude
	gps->latitude_DMS_Degree = (10 * (gps->latitude[0] - 0x30) + (gps->latitude[1] - 0x30));
	gps->latitude_DMS_Minutes = (10 * (gps->latitude[2] - 0x30) + (gps->latitude[3] - 0x30));
	uint32_t val = (1000 * (gps->latitude[5] - 0x30) + 100 * (gps->latitude[6] - 0x30) + 10 * (gps->latitude[7] - 0x30) + (gps->latitude[8] - 0x30));
	gps->latitude_DMS_Seconds = (val * 60) / 9999;
	//Get longitude
	gps->longitude_DMS_Degree = (100 * (gps->longuitude[0] - 0x30) + 10 * (gps->longuitude[1] - 0x30) + (gps->longuitude[2] - 0x30));
	gps->longitude_DMS_Minutes = (10 * (gps->longuitude[3] - 0x30) + (gps->longuitude[4] - 0x30));
	val = (1000 * (gps->latitude[6] - 0x30) + 100 * (gps->longuitude[7] - 0x30) + 10 * (gps->longuitude[8] - 0x30) + (gps->longuitude[9] - 0x30));
	gps->longitude_DMS_Seconds = (val * 60) / 9999;
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
