/*
 * cmd_parser.c
 *
 *  Created on: Mar 29, 2019
 *      Author: Miguel Elias
 */

/*
 *  Commands:
 *  LED:R
 *  LED:G
 *  LED:B
 * */

#include "cmd_parser.h"

void cmd_init(Request *cmd, uint8_t size)
{
	cmd->size = size;
}

uint16_t cmd_parse(Request *cmd)
{
	if (cmd->data[0] == 'L' && cmd->data[1] == 'E' && cmd->data[2] == 'D' && cmd->data[3] == ':')
	{
		if (cmd->data[4] == 'R')
		{
			return LED_R;
		}
		else if (cmd->data[4] == 'G')
		{
			return LED_G;
		}
		else if (cmd->data[4] == 'B')
		{
			return LED_B;
		}
		else
		{
			return NONE;
		}
	}
	else
	{
		return 0;
	}
}

void cmd_add(Request *cmd, char byte)
{
	if (cmd->is_full == 0)
	{
		cmd->data[cmd->n_items++] = byte;
	}
	if (cmd->n_items >= (cmd->size - 1))
	{
		cmd->is_full = 1;
	}
}

void cmd_clear(Request *cmd)
{
	uint8_t i = 0;
	while (i < cmd->size)
	{
		cmd->data[i++] = 0;
	}
	cmd->n_items = 0;
	cmd->is_full = 0;
}
