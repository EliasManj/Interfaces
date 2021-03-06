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

//Other
#define NEW_LINE 	'\n'
#define CARR_RETURN '\r'

//IP TYPES
#define APIP									0
#define APMAC									1
#define STAIP									2
#define STAMAC									3

//UART Channels
#define UART0									0
#define UART3									3
#define NOPS									1e3

//Modes
#define CONF_MODE								0
#define SERVER_MODE								1

//Conf states
#define CONF_IDLE								0
#define CONF_CIPMUX								1
#define CONF_CIPSERVER							2
#define CONF_CIFSR								3
#define CONF_WAIT_PLUS							4
#define CONF_CHECK_CIFSR_IP						5
#define CONF_CHECK_IP_TYPE						6
#define CONF_STAIP								7
#define CONF_PARSE_IP							8

//Router states
#define ROUTER_IDLE 				0
#define ROUTER_IPD 					1
#define ROUTER_UNKNOWN_N0 			2
#define ROUTER_UNKNOWN_N1 			3
#define ROUTER_HTTPTYPE 			4
#define ROUTER_URI 					5

//String literals
//AT Commands
static char *cip_mux_str = {"AT+CIPMUX=1\r\n"};
static char *cip_server_str = {"AT+CIPSERVER=1,80\r\n"};
static char *cifsr_str = {"AT+CIFSR\r\n"};
//IP types

struct Wifi_Struct {
	/*Uart buffers Wifi buffers go to UART3, debug buffer goes to usb UART0 */
	bufferType Wifi_Buffer_Tx;
	bufferType *Wifi_Buffer_Tx_pt;
	bufferType Wifi_Buffer_Rx;
	bufferType *Wifi_Buffer_Rx_pt;
	bufferType Buffer_debug_console;
	bufferType *Buffer_debug_console_pt;
	uint8_t buffer_is_empty;
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
};

typedef struct Wifi_Struct Wifi_Obj;

Wifi_Obj wifi;
Wifi_Obj *wifi_pt;

//UART functions
void Wifi_UART3_Init(void);
void Wifi_UART0_Init_USB(void);
void Wifi_UART_SendString_Enable_Tx(bufferType *bf, char *str, int uart_channel);

//Configure helper functions
void WifiConf_Init(Wifi_Obj *Wifi_Obj, bufferType *bf, uint8_t uart_channel);
void WifiConf_InitBuffers(Wifi_Obj *Wifi_Obj, uint32_t size);
void WifiConf_CipMux(Wifi_Obj *Wifi_Obj, bufferType *bf, uint8_t uart_channel);
void WifiConf_Cipserver(Wifi_Obj *Wifi_Obj, bufferType *bf, uint8_t uart_channel);
void WifiConf_CIFSR(Wifi_Obj *Wifi_Obj, bufferType *bf, uint8_t uart_channel);
void WifiConf_Wait(uint32_t x);
void WifiConf_ParseByte(Wifi_Obj *Wifi_Obj, char byte);
int WifiConf_CheckIPType(char *str);
//Router functions
void WifiRouter_Reset(Wifi_Obj *Wifi_Obj);
void WifiRouter_ParseByte(Wifi_Obj *Wifi_Obj, char byte);
int WifiConf_ValidateIPD(char *str);


#endif /* WIFI_H_ */
