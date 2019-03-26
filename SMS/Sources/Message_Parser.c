#include "Message_Parser.h"

uint8_t SMS_Obj_Init(SMS_Obj *sms)
{
	sms->counter = 0;
	sms->current_state = 0;
	sms->field_counter = 0;
	sms->quote_counter = 0;
	return 0;
}

uint8_t parse_SMS_byte(SMS_Obj *sms, uint8_t byte)
{
	switch (sms->current_state)
	{
	case IDLE:
		if (byte == '+')
		{
			sms->current_state = CMGL;
		}
		break;
	case CMGL:
		if (byte == ',')
		{
			sms->field_counter = 0;
			if (validate_CMGL(sms->cmgl_string))
			{
				sms->current_state = REC_READ;
			}
			else
			{
				sms->current_state = IDLE;
			}
		}
		else if (is_digit(byte) || byte == '\n')
		{
			sms->current_state = IDLE;
		}
		else
		{
			sms->cmgl_string[sms->field_counter++] = byte;
		}
		break;
	case REC_READ:
		if (byte == ',')
		{
			sms->current_state = CELL_NUMBER;
		}
		break;
	case CELL_NUMBER:
		if (byte == ',')
		{
			sms->field_counter = 0;
			sms->current_state = SOMETHING;
		}
		else
		{
			sms->cell_number[sms->field_counter++] = byte;
		}
		break;
	case SOMETHING:
		if (byte == ',')
		{
			sms->current_state = DATE;
		}
		break;
	case DATE:
		if (byte == '\r')
		{
			sms->field_counter = 0;
			if (validate_Date(sms->date))
			{
				sms->current_state = MESSAGE;
			}
			else
			{
				sms->current_state = IDLE;
			}
		}
		else
		{
			sms->date[sms->field_counter++] = byte;
		}
		break;
	case MESSAGE:
		if (byte == '\r')
		{
			sms->field_counter = 0;
			sms->current_state = IDLE;
			return 1;
		}
		else
		{
			sms->message[sms->field_counter++] = byte;
		}
		break;
	}
	return 0;
}

uint8_t validate_Date(uint8_t byte[])
{
	if (byte[0] == '\"' && byte[3] == '/' && byte[6] == '/' && byte[9] == ',' && byte[12] == ':' && byte[15] == ':' && byte[18] == '-')
	{
		return 1;
	}
	return 0;
}

uint8_t validate_CMGL(uint8_t byte[])
{
	if (byte[0] == 'C' && byte[1] == 'M' && byte[2] == 'G' && byte[3] == 'L' && byte[4] == ':')
	{
		return 1;
	}
	return 0;
}

uint8_t add_to_inbox(SMS_Inbox *inbox, SMS_Obj *sms)
{
	if (inbox->counter++ >= 5)
	{
		//Full
		return 1;
	}
	else
	{
		inbox->sms_array[inbox->counter++] = sms;
		return 0;
	}
}

uint8_t is_digit(uint8_t byte)
{
	return byte == 0x30 || byte == 0x31 || byte == 0x32 || byte == 0x33 || byte == 0x34 || byte == 0x35 || byte == 0x36 || byte == 0x37 || byte == 0x38 || byte == 0x39;
}
