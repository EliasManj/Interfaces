/*
 * Wifi.h
 *
 *  Created on: Apr 24, 2019
 *      Author: gonmigue
 */

#ifndef WIFI_H_
#define WIFI_H_

#include "derivative.h" /* include peripheral declarations */
#include "buffer.h"
#include "helper_functions.h"
#include "RGB.h"
#include "Camera.h"

//Other
#define WIFI_NEW_LINE 	'\n'
#define WIFI_CARR_RETURN '\r'

#define WIFI_BAUD9600		137
#define WIFI_BAUD38400		34

//IP TYPES
#define WIFI_APIP									0
#define WIFI_APMAC									1
#define WIFI_STAIP									2
#define WIFI_STAMAC									3

//UART Channels
#define WIFI_UART0									0
#define WIFI_UART3									3
#define WIFI_NOPS									1e6

//Modes
#define WIFI_CONF_MODE								0
#define WIFI_SERVER_MODE								1
#define WIFI_RECIVE_POST_REQUEST					    2

//Conf states
#define WIFI_CONF_IDLE								0
#define WIFI_CONF_CIPMUX								1
#define WIFI_CONF_CIPSERVER							2
#define WIFI_CONF_CIFSR								3
#define WIFI_CONF_WAIT_PLUS							4
#define WIFI_CONF_CHECK_CIFSR_IP						5
#define WIFI_CONF_CHECK_IP_TYPE						6
#define WIFI_CONF_STAIP								7
#define WIFI_CONF_PARSE_IP							8

//Router states
#define WIFI_ROUTER_IDLE 				0
#define WIFI_ROUTER_IPD 					1
#define WIFI_ROUTER_UNKNOWN_N0 			2
#define WIFI_ROUTER_UNKNOWN_N1 			3
#define WIFI_ROUTER_HTTPTYPE 			4
#define WIFI_ROUTER_URI 					5

//String literals
//AT Commands
static char *cip_mux_str =
{ "AT+CIPMUX=1\r\n" };
static char *cip_server_str =
{ "AT+CIPSERVER=1,80\r\n" };
static char *cifsr_str =
{ "AT+CIFSR\r\n" };
static char *cip_send =
{ "AT+CIPSEND=1," };
static char *cip_start =
{ "AT+CIPSTART=1,\"TCP\"," };
static char *get_request =
{ "GET / HTTP/1.1\r\nHost: " };
//IP types

struct Wifi_Struct
{
	/*Uart buffers Wifi buffers go to UART3, debug buffer goes to usb UART0 */
	bufferType Wifi_Buffer_Tx;
	bufferType *Wifi_Buffer_Tx_pt;
	bufferType Wifi_Buffer_Rx;
	bufferType *Wifi_Buffer_Rx_pt;
	bufferType Buffer_debug_console;
	bufferType *Buffer_debug_console_pt;
	uint8_t buffer_is_empty;
	uint8_t uart_channel;
	//Configure mode
	uint8_t wifi_mode;
	uint8_t conf_state;
	char conf_static_ip_addr[17];
	char conf_cifsr[5];
	char conf_ip_type[6];
	uint8_t conf_debug_parsedbytes[100];
	uint32_t conf_counter;
	uint32_t conf_field_counter;
	//Server request mode
	uint32_t router_counter;
	uint32_t router_field_counter;
	uint8_t router_state;
	uint8_t request_pending;
	char router_ipd[3];
	char uri[10];
	char router_request[100];
	//Send Mode
	uint8_t send_trigger;
	char send_cip_start_str[38];
};

typedef struct Wifi_Struct Wifi_Obj;

Wifi_Obj wifi;
Wifi_Obj *wifi_pt;

//UART init functions
void Wifi_UART_Init(Wifi_Obj *Wifi_Obj, int uart_channel);
void Wifi_UART_Init_3(void);
void Wifi_UART_Init_0(void);

//UART functions
void Wifi_UART_SendString_Enable_Tx(Wifi_Obj *Wifi_Obj, bufferType *bf, char *str);
void Wifi_UART_SendString(bufferType *bf, char *str);
void Wifi_UART_SendString_UntilEmpty(bufferType *bf, char *str);
void Wifi_UART_SubString(Wifi_Obj *Wifi_Obj, bufferType *bf, char *str, int start, int end);
void Wifi_UART_WaitEmptyBuffer();
void Wifi_Enable_Tx(Wifi_Obj *Wifi_Obj);

//Wifi functions
void Wifi_Init(Wifi_Obj *Wifi_Obj, bufferType *bf, uint8_t uart_channel);
void Wifi_InitBuffers(Wifi_Obj *Wifi_Obj, uint32_t size);
//Wifi conf commands
void Wifi_Conf_CipMux(Wifi_Obj *Wifi_Obj, bufferType *bf);
void Wifi_Conf_Cipserver(Wifi_Obj *Wifi_Obj, bufferType *bf);
void Wifi_Conf_CIFSR(Wifi_Obj *Wifi_Obj, bufferType *bf);

//Parser
void Wifi_Conf_ParseByte(Wifi_Obj *Wifi_Obj, char byte);
int Wifi_Conf_CheckIPType(char *str);

//Router functions
void Wifi_Router_Reset(Wifi_Obj *Wifi_Obj);
void Wifi_Router_ParseByte(Wifi_Obj *Wifi_Obj, char byte);
void Wifi_Router_Route(Wifi_Obj *Wifi_Obj);
int Wifi_Router_ValidateIPD(char *str);

//Send HTTP request functions
void Wifi_Http_Send_Cmd_StartConnection(Wifi_Obj *Wifi_Obj, bufferType *bf, char *ip, char *port);
void Wifi_Http_Send_Request_ContentLength(bufferType *bf, char *keyword, char *content, int content_length);
void Wifi_Http_Send_Request_JSONPostRequestSize(Wifi_Obj *Wifi_Obj, bufferType *bf, char *ip, char *uri, char *keyword, char *content, int content_length, int headers_length);
int Calculate_Content_Length(char *keyword, char *content, int content_start_index, int content_end_index);
int Calculate_Headers_Length_Post(char *ip, char *uri, int content_length);

//Http commands
void Wifi_Http_Send_Request_Get(Wifi_Obj *Wifi_Obj, bufferType *bf, char *ip, char *port, char *uri);
void Wifi_Http_Send_Request_PostJson(Wifi_Obj *Wifi_Obj, bufferType *bf, char *ip, char *port, char *uri, char * keyword, char *content, int content_start_index, int content_end_index);
void Wifi_Http_Send_Request_TestAPR(bufferType *bf, char *ip, char *port, char *request, int uart_channel);

void Wifi_NVIC_SetPriority(int iInterruptID, unsigned char ucPriority);

#endif /* WIFI_H_ */
