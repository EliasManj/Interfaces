#include "Message_Parser.h"

void debug_val(int debug_value)
{
	if (debug_value >= 52)
	{
		int a;
		a = 1;
	}
}

void debug_msg_count(int val)
{
	if (sms_msg_count == val)
	{
		int a;
		a = 1;
	}
}

uint8_t SMS_Obj_Init(SMS_Obj *sms, uint8_t primary)
{
	sms->counter = 0;
	if (primary)
	{
		sms->current_state = IDLE;
	}
	else
	{
		sms->current_state = WAIT_PLUS;
	}
	sms->field_counter = 0;
	sms->quote_counter = 0;
	return 0;
}

uint8_t parse_SMS_byte(SMS_Obj *sms, uint8_t byte, uint32_t debug_value)
{
	sms->entire_string[sms->counter++] = byte;
	switch (sms->current_state)
	{
	case IDLE:
		if (byte == '+')
		{
			sms->current_state = TEXT_MODE;
			sms->field_counter = 0;
		}
		break;
	case TEXT_MODE:
		sms->at_cmgf_eq_1[sms->field_counter] = byte;
		if (sms->field_counter == 13)
		{
			if (validate_At_cmgf_eq_1(sms->at_cmgf_eq_1))
			{
				sms->current_state = READ_ALL;
			}
			else
			{
				sms->current_state = IDLE;
			}
			sms->field_counter = 0;
		}
		sms->field_counter++;
		break;
	case READ_ALL:
		sms->at_cmgf_read_all[sms->field_counter] = byte;
		if (sms->field_counter == 12)
		{
			if (validate_At_cmgf_read_all(sms->at_cmgf_read_all))
			{
				sms->current_state = WAIT_PLUS;
			}
			else
			{
				sms->current_state = IDLE;
			}
			sms->field_counter = 0;
		}
		sms->field_counter++;
		break;
	case WAIT_PLUS:
		if (byte == '+')
		{
			sms->current_state = CMGL;
		}
		break;
	case CMGL:
		if (byte == ',')
		{
			debug_msg_count(2);
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
		else if (byte == '\n')
		{
			sms->current_state = IDLE;
			sms->field_counter = 0;
		}
		else if (sms->field_counter < 6)
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
			sms_msg_count++;
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
	if (string_compare(byte, (uint8_t *) "CMGL:", 4) || string_compare(byte, (uint8_t *) "\"CMGL:", 5))
	{
		return 1;
	}
	return 0;
}

uint8_t validate_At_cmgf_eq_1(uint8_t byte[])
{
	if (string_compare(byte, (uint8_t *) "CMGF=1\r\r\nOK\r\nA", 0XFF) == 1)
	{
		return 1;
	}
	return 0;
}

uint8_t validate_At_cmgf_read_all(uint8_t byte[])
{
	if (string_compare(byte, (uint8_t *) "AT+CMGL=\"ALL\"", 0XFF) == 1)
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

uint8_t is_digit_or_equals(uint8_t byte)
{
	return byte == 0x30 || byte == 0x31 || byte == 0x32 || byte == 0x33 || byte == 0x34 || byte == 0x35 || byte == 0x36 || byte == 0x37 || byte == 0x38 || byte == 0x39 || byte == '=';
}

uint8_t string_compare(volatile uint8_t *array1, volatile uint8_t *array2, uint16_t until)
{
	uint32_t i;
	uint8_t response = 0;
	i = 0;
	while (array1[i] == array2[i] && response == 0)
	{
		if (array1[i] == '\0' || array2[i] == '\0' || until == i)
		{
			response = 1;
		}
		i++;
	}
	return response;
}

uint8_t SMS_Class_Init()
{
	sms_msg_count = 0;
}
