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

struct CommandStruct {
	volatile uint8_t size;
	volatile uint8_t n_items;
	volatile uint8_t is_full;
	volatile char data[CMDLEN];
};

typedef struct CommandStruct Command;

void cmd_init(Command *cmd, uint8_t size);
void cmd_add(Command *cmd, char byte);	
void cmd_clear(Command *cmd);
uint16_t cmd_parse(Command *cmd);

#endif /* CMD_PARSER_H_ */
