/*
 * Wifi.c
 *
 *  Created on: Apr 24, 2019
 *      Author: gonmigue
 */

#include "Wifi.h"
#define __NVIC_PRIORITY_SHIFT   4

//Wifi Init
void WifiConf_InitBuffers(Wifi_Obj *Wifi_Obj, uint32_t size) {
	Wifi_Obj->wifi_mode = CONF_MODE;
	Wifi_Obj->Wifi_Buffer_Tx_pt = &Wifi_Obj->Wifi_Buffer_Tx;
	Wifi_Obj->Wifi_Buffer_Rx_pt = &Wifi_Obj->Wifi_Buffer_Rx;
	Wifi_Obj->Buffer_debug_console_pt = &Wifi_Obj->Buffer_debug_console;
	buffer_init(Wifi_Obj->Buffer_debug_console_pt, size);
	buffer_init(Wifi_Obj->Wifi_Buffer_Tx_pt, size);
	buffer_init(Wifi_Obj->Wifi_Buffer_Rx_pt, size);
}

void WifiConf_Init(Wifi_Obj *Wifi_Obj, bufferType *bf, uint8_t uart_channel) {
	UART0_Init_USB();
	UART3_Init();
	WifiRouter_Reset(Wifi_Obj);
	WifiConf_CipMux(Wifi_Obj, bf, uart_channel);
	WifiConf_Cipserver(Wifi_Obj, bf, uart_channel);
	WifiConf_CIFSR(Wifi_Obj, bf, uart_channel);
}

void NVIC_SetPriority(int iInterruptID, unsigned char ucPriority) {
	volatile unsigned char *ptrPriority =
			&NVIC_IP_REG(NVIC_BASE_PTR,iInterruptID) ;
			ptrPriority += iInterruptID;
			*ptrPriority = (ucPriority << __NVIC_PRIORITY_SHIFT);
		}

//Wifi Request router functions
void WifiRouter_Reset(Wifi_Obj *Wifi_Obj) {
	Wifi_Obj->router_state = ROUTER_IDLE;
	Wifi_Obj->router_field_counter = 0;
	Wifi_Obj->router_counter = 0;
	Wifi_Obj->request_pending = 0;
}

void WifiRouter_ParseByte(Wifi_Obj *Wifi_Obj, char byte) {
	Wifi_Obj->router_request[Wifi_Obj->router_counter++] = byte;
	switch (Wifi_Obj->router_state) {
	case (ROUTER_IDLE):
		if (byte == '+') {
			Wifi_Obj->router_state = ROUTER_IPD;
			Wifi_Obj->router_field_counter = 0;
		}
		break;
	case (ROUTER_IPD):
		if (byte == ',' || Wifi_Obj->router_field_counter == 3) {
			Wifi_Obj->router_field_counter = 0;
			if(WifiConf_ValidateIPD(Wifi_Obj->router_ipd)){
				Wifi_Obj->router_state = ROUTER_UNKNOWN_N0;
			} else {
				Wifi_Obj->router_state = ROUTER_IDLE;
			}
		} else {
			Wifi_Obj->router_ipd[Wifi_Obj->router_field_counter++] = byte;
		}
		break;
	case (ROUTER_UNKNOWN_N0):
		if (byte == ',') {
			Wifi_Obj->router_state = ROUTER_UNKNOWN_N1;
		}
		break;
	case (ROUTER_UNKNOWN_N1):
		if (byte == ':') {
			Wifi_Obj->router_state = ROUTER_HTTPTYPE;
		}
		break;
	case (ROUTER_HTTPTYPE):
		if (byte == '/') {
			Wifi_Obj->router_state = ROUTER_URI;
		}
		break;
	case (ROUTER_URI):
		if (byte == ' ') {
			Wifi_Obj->router_state = ROUTER_IDLE;
			Wifi_Obj->router_field_counter = 0;
			Wifi_Obj->request_pending = 1;
		} else {
			Wifi_Obj->uri[Wifi_Obj->router_field_counter++] = byte;
		}
		break;
	}
}

//Wifi Configuration functions
void WifiConf_ParseByte(Wifi_Obj *Wifi_Obj, char byte) {
	Wifi_Obj->conf_debug_parsedbytes[Wifi_Obj->conf_counter++] = byte;
	switch (Wifi_Obj->conf_state) {
	case (CONF_WAIT_PLUS):
		if (byte == '+') {
			Wifi_Obj->conf_state = CONF_CHECK_CIFSR_IP;
		}
		break;
	case (CONF_CHECK_CIFSR_IP):
		if (byte == ':') {
			Wifi_Obj->conf_state = CONF_CHECK_IP_TYPE;
			Wifi_Obj->conf_field_counter = 0;
		} else {
			Wifi_Obj->conf_cifsr[Wifi_Obj->conf_field_counter++] = byte;
		}
		break;
	case (CONF_CHECK_IP_TYPE):
		if (byte == ',') {
			Wifi_Obj->conf_field_counter = 0;
			if (WifiConf_CheckIPType(Wifi_Obj->conf_ip_type) == STAIP) {
				Wifi_Obj->conf_state = CONF_PARSE_IP;
			} else {
				Wifi_Obj->conf_state = CONF_WAIT_PLUS;
			}
		} else {
			Wifi_Obj->conf_ip_type[Wifi_Obj->conf_field_counter++] = byte;
		}
		break;
	case (CONF_PARSE_IP):
		if (byte == '\r') {
			Wifi_Obj->conf_state = CONF_IDLE;
			Wifi_Obj->wifi_mode = SERVER_MODE;
			Wifi_Obj->conf_field_counter = 0;
		} else {
			Wifi_Obj->conf_static_ip_addr[Wifi_Obj->conf_field_counter++] =
					byte;
		}
		break;
	}
}

void WifiConf_CipMux(Wifi_Obj *Wifi_Obj, bufferType *bf, uint8_t uart_channel) {
	UART_SendString(bf, cip_mux_str, uart_channel);
	WifiConf_Wait(NOPS);
}

void WifiConf_Cipserver(Wifi_Obj *Wifi_Obj, bufferType *bf,
		uint8_t uart_channel) {
	UART_SendString(bf, cip_server_str, uart_channel);
	//WifiConf_Wait(NOPS);
}

void WifiConf_CIFSR(Wifi_Obj *Wifi_Obj, bufferType *bf, uint8_t uart_channel) {
	UART_SendString(bf, cifsr_str, uart_channel);
	Wifi_Obj->conf_state = CONF_WAIT_PLUS;
	//WifiConf_Wait(NOPS);
}

void WifiConf_Wait(uint32_t x) {
	while (wifi_pt->buffer_is_empty == 0);
}

int WifiConf_CheckIPType(char *str) {
	if (_strcomp(str, "APIP", 4)) {
		return APIP;
	} else if (_strcomp(str, "APMAC", 5)) {
		return APMAC;
	} else if (_strcomp(str, "STAIP", 5)) {
		return STAIP;
	} else if (_strcomp(str, "STAMAC", 6)) {
		return STAMAC;
	}
	return 0;
}

int WifiConf_ValidateIPD(char *str){
	if(_strcomp(str, "IPD", 3)){
		return 1;
	}
	return 0;
}

//UART Interface
void UART_SendString(bufferType *bf, char *str, int uart_channel) {
	int32_t i;
	for (i = 0; i < _strlen(str); i++) {
		buffer_push(bf, str[i]);
	}
	if (uart_channel == UART0) {
		UART0_C2 |= 0x80; //Turn on TX interrupt		
	} else if (uart_channel == UART3) {
		UART3_C2 |= 0x80; //Turn on TX interrupt
	}
}

void UART3_Init(void) {
	SIM_SCGC4 |= (1 << 13);	//CLK UART3
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; /*Enable the PORTC clock*/
	PORTC_PCR16 |= PORT_PCR_MUX(3);
	PORTC_PCR17 |= PORT_PCR_MUX(3);
	UART3_BDL = 137;		//clock=640*32768, baud rate 9600
	UART3_C2 |= (1 << 5);		//reciver interrupt enable for RDRF
	UART3_C2 |= (1 << 2);		//RE reciver enable
	UART3_C2 |= (1 << 3);		//TE Transmiter enable
	NVIC_ISER(1) |= (1 << (37%32));
	NVIC_ICPR(1) |= (1 << (37%32));
	NVIC_SetPriority(37, 1);
}

void UART0_Init_USB(void) {
	SIM_SCGC4 |= (1 << 10);	//CLK UART0
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; /*Enable the PORTB clock*/
	PORTB_PCR16 |= PORT_PCR_MUX(3);
	PORTB_PCR17 |= PORT_PCR_MUX(3);
	UART0_BDL = 137;		//clock=640*32768, baud rate 9600
	UART0_C2 |= (1 << 5);		//reciver interrupt enable for RDRF
	UART0_C2 |= (1 << 2);		//RE reciver enable
	UART0_C2 |= (1 << 3);		//TE Transmiter enable
	NVIC_ISER(0) |= (1 << (31%32));
	NVIC_ICPR(0) |= (1 << (32%32));
	NVIC_SetPriority(31, 1);
}
