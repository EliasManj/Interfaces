/*
 * GPS.c
 *
 *  Created on: Feb 21, 2019
 *      Author: Miguel Elias
 */

#include "GPS.h"

uint8_t parse_gps_byte(GPS_Struct *gps, uint8_t byte)
{
	//Validate $ start byte
	uint8_t status;
	if (gps->counter == 0)
	{
		if (byte != '$')
		{
			return 1;
		}
	}
	//Validate comas
	uint16_t i;
	for(i = 0; i<16; i++){
		if(gps->counter == gps->comas_index[i]){
			if(byte != ','){
				return 1;
			}
		}
	}
	gps->gps_string[gps->counter++] = byte;
	return 0;
}
