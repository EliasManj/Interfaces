/*
 * cmd_parser.h
 *
 *  Created on: Mar 29, 2019
 *      Author: Miguel Elias
 */

#ifndef CMD_PARSER_H_
#define CMD_PARSER_H_

#include "derivative.h"

#define CMDLEN 		20
#define NONE		0
#define LED_R 		1
#define LED_G		2
#define LED_B		3

struct RequestStruct {
	volatile uint8_t size;
	volatile uint8_t n_items;
	volatile uint8_t is_full;
	volatile char data[CMDLEN];
};

typedef struct RequestStruct Request;

void cmd_init(Request *cmd, uint8_t size);
void cmd_add(Request *cmd, char byte);	
void cmd_clear(Request *cmd);
uint16_t cmd_parse(Request *cmd);

#endif /* CMD_PARSER_H_ */
