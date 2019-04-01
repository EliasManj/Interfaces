/*
 * Message_Parser.h
 *
 *  Created on: Mar 25, 2019
 *      Author: Miguel Elias
 */

#ifndef MESSAGE_PARSER_H_
#define MESSAGE_PARSER_H_

#include "derivative.h" /* include peripheral declarations */

//Status
#define DONE_PARSING

int sms_msg_count;

//FSM
#define IDLE 					0
#define TEXT_MODE 				1
#define READ_ALL				2
#define WAIT_PLUS				3
#define CMGL 					4
#define REC_READ 				5
#define CELL_NUMBER 			6
#define SOMETHING	 			7
#define DATE	 				8
#define MESSAGE	 				9

struct SMS_Struct
{
	uint16_t counter;
	uint16_t current_state;
	uint16_t field_counter;
	uint16_t quote_counter;
	//Parsed data
	uint8_t entire_string[200];
	uint8_t at_cmgf_eq_1[13];
	uint8_t at_cmgf_read_all[12];
	uint8_t cmgl_string[6];
	uint8_t cell_number[12];
	uint8_t message[100];
	uint32_t message_counter;
	uint8_t *message_pt;
	uint8_t date[22];
};

typedef struct SMS_Struct SMS_Obj;

struct SMS_Inbox_Struct
{
	uint8_t counter;
	SMS_Obj *sms_array[5];
};

typedef struct SMS_Inbox_Struct SMS_Inbox;

//Sms obj
uint8_t parse_SMS_byte(SMS_Obj *sms, uint8_t byte, uint32_t debug_value);
uint8_t SMS_Obj_Init(SMS_Obj *sms, uint8_t primary);
uint8_t SMS_Class_Init();
uint8_t validate_CMGL(uint8_t byte[]);
uint8_t validate_Date(uint8_t byte[]);
uint8_t validate_At_cmgf_eq_1(uint8_t byte[]);
uint8_t validate_At_cmgf_read_all(uint8_t byte[]);
uint8_t string_compare(volatile uint8_t *array1, volatile uint8_t *array2, uint16_t until);
uint8_t is_digit_or_equals(uint8_t byte);
uint8_t SMS_Obj_Init(SMS_Obj *sms, uint8_t primary);
void allocate_msg(SMS_Obj *sms);

//DEbug
void debug_val(int a, int b, int sms);
void debug_msg_count(int val);

//inbox
uint8_t add_to_inbox(SMS_Inbox *inbox, SMS_Obj *sms);

#endif /* MESSAGE_PARSER_H_ */
