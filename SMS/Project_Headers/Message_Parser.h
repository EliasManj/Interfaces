/*
 * Message_Parser.h
 *
 *  Created on: Mar 25, 2019
 *      Author: Miguel Elias
 */

#ifndef MESSAGE_PARSER_H_
#define MESSAGE_PARSER_H_

#include "derivative.h" /* include peripheral declarations */

//FSM
#define IDLE 				0
#define CMGL 				1
#define REC_READ 			2
#define CELL_NUMBER 		3
#define SOMETHING	 		4
#define DATE	 			5
#define MESSAGE	 			6

struct SMS_Struct{
	uint16_t counter;
	uint16_t current_state;
	uint16_t field_counter;
	uint16_t quote_counter;
	//Parsed data
	uint8_t cmgl_string[8];
	uint8_t cell_number[12];
	uint8_t message[100];
	uint8_t date[22];
};

typedef struct SMS_Struct SMS_Obj;

struct SMS_Inbox_Struct{
	uint8_t counter;
	SMS_Obj *sms_array[5];
};

typedef struct SMS_Inbox_Struct SMS_Inbox;

//Sms obj
uint8_t parse_SMS_byte(SMS_Obj *sms, uint8_t byte);
uint8_t SMS_Obj_Init(SMS_Obj *sms);
uint8_t validate_CMGL(uint8_t byte[]);
uint8_t validate_Date(uint8_t byte[]);
uint8_t is_digit(uint8_t byte);

//inbox
uint8_t add_to_inbox(SMS_Inbox *inbox, SMS_Obj *sms);

#endif /* MESSAGE_PARSER_H_ */
