/*
 * GPS.h
 *
 *  Created on: Feb 21, 2019
 *      Author: Miguel Elias
 */

#ifndef GPS_H_
#define GPS_H_

#include "derivative.h" /* include peripheral declarations */

//GPGGA sentence parameters
#define GPGGA_GPS_STR_LEN							66
#define $GPGGA_START								1	
#define $GPGGA_END									5	
#define GPGGA_TIME_STAMP_START						7	
#define GPGGA_TIME_STAMP_END						16	
#define GPGGA_LATITUDE_START 						18
#define GPGGA_LATITUDE_END 							28
#define GPGGA_LONGUITUDE_START 						30
#define GPGGA_LONGUITUDE_END 						41
#define GPGGA_ALTITUDE_START						52
#define GPGGA_ALTITUDE_END							59

//Parser
uint16_t current_state;

struct GPS_Obj
{
	//$GPGGA,004108.000,2039.8052,N,10323.1903,W,1,05,1.8,1542.9,M,0.0,M,,*4B
	//Control
	uint16_t counter;
	uint8_t field_counter;
	uint8_t gps_string[GPGGA_GPS_STR_LEN];
	//GPGGA Data
	uint8_t GPGGA_String[$GPGGA_END - $GPGGA_START + 1];
	uint8_t time_stamp[GPGGA_TIME_STAMP_END - GPGGA_TIME_STAMP_START + 1];
	uint8_t latitude[GPGGA_LATITUDE_END - GPGGA_LATITUDE_START + 1];
	uint8_t longuitude[GPGGA_LONGUITUDE_END - GPGGA_LONGUITUDE_START + 1];
	uint8_t altitude[GPGGA_ALTITUDE_END - GPGGA_ALTITUDE_START + 1];
	//Parsed data
	uint32_t latitude_DMS_Degree;
	uint32_t latitude_DMS_Minutes;
	uint32_t latitude_DMS_Seconds;
	uint32_t longitude_DMS_Degree;
	uint32_t longitude_DMS_Minutes;
	uint32_t longitude_DMS_Seconds;
};

typedef struct GPS_Obj GPS_Struct;

uint8_t parse_gps_byte(GPS_Struct *gps, uint8_t byte);
uint8_t validate_GPGGA(uint8_t string[]);
void set_DMS_data(GPS_Struct *gps);
void Compare_Byte(uint8_t uart_recive, uint8_t byte);

#endif /* GPS_H_ */
