/*
 * cmd_parser.h
 *
 *  Created on: Mar 29, 2019
 *      Author: Miguel Elias
 */

#ifndef CMD_PARSER_H_
#define CMD_PARSER_H_

#include "derivative.h"
#include "buffer.h"

#define PATHLEN 	20
#define NONE		0
#define LED_R 		1
#define LED_G		2
#define LED_B		3

//STATES REQUEST
#define IDLE								0
#define WAITFORCOLON						1
#define WAITFORSLASH_OR_COMMA				2
#define PARSING_IP							3
#define PARSING_URI							4

//STATES CONFIG
#define IDLE_CONF						0
#define CIPMUX							1
#define OK0								2
#define CIPSERVER						3
#define OK1								4
#define CIFSR							5
#define WAITPLUS						6
#define PARSINGCIFSR					7
#define STATIC_IP						8

//LEDSTATES
#define NONE							0
#define RED								1
#define GREEN							2
#define BLUE							3

//CIFSR address
#define NO_ADR							0 
#define APIP							1 
#define APMAC							2 
#define STAIP							3 
#define STAMAC							4 

struct RequestStruct {
	volatile uint8_t path[PATHLEN];
	volatile uint8_t request_string[100];
	volatile uint8_t IPD[3];
	volatile uint8_t CIFSR_Type[6];
	volatile uint8_t static_ip_addr[20];
	volatile uint32_t counter;
	volatile uint32_t field_counter;
	volatile uint32_t current_state;
	volatile uint32_t max_wait;
	volatile uint32_t led_state;
	//COnf signals
	volatile uint32_t current_state_conf;
	volatile uint8_t conf_string[100];
	volatile uint8_t configurating;
	volatile uint8_t AT_CIPMUX[9];
	volatile uint8_t AT_CIPSERVER[12];
	volatile uint8_t OK[2];
	volatile uint8_t ok_counter;
	volatile uint8_t ok_signal;
	volatile uint8_t waiting_for_okay;
	volatile uint8_t waiting_for_request;
};

typedef struct RequestStruct Request;

void request_parser_init(Request *request);
uint8_t parse_request_byte(Request *request, uint8_t byte);
uint8_t parse_request(Request *request);
uint8_t validate_ipd(uint8_t byte[]);
uint8_t is_digit_or_equals(uint8_t byte);
uint8_t string_compare(volatile uint8_t *array1, volatile uint8_t *array2,
		uint16_t until);
void ConfigureCIP_MUX(bufferType *bf, Request *request);
void WaitOK(Request *request);

#endif /* CMD_PARSER_H_ */
