/*
 * GPS.h
 *
 *  Created on: Feb 21, 2019
 *      Author: Miguel Elias
 */

#ifndef GPS_H_
#define GPS_H_

#include "derivative.h" /* include peripheral declarations */

#define START_BYTE$ 	0x00
#define GPRMC 			0x01
#define TIMESTAMP 		0x02
#define WARNING			0x03

#define COMA_CHAR 		','

//Parser
uint16_t current_state;

struct GPS_Obj
{
	//Control
	uint16_t counter;
	uint32_t comas_index[11];
	uint8_t gps_string[66];
	uint8_t GPRMC_String[6];
	//GPS data
	uint8_t time_stamp[6];
	uint8_t validity;
	uint8_t latitude[7];
	uint8_t north_south;
	uint8_t longuitude[8];
	uint8_t east_west;
	uint8_t knot_speed[5];
} GPS_Default = {
		0, {6, 13, 15, 23, 25, 34, 36, 42, 48, 55, 61}
};

typedef struct GPS_Obj GPS_Struct;

uint8_t* get_time_stamp(GPS_Struct *gps);
uint8_t parse_gps_byte(GPS_Struct *gps, uint8_t byte);

#endif /* GPS_H_ */
