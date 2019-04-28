/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

//20 tx
//21 rx
#include "derivative.h" /* include peripheral declarations */

#include "RGB.h"
#include "Wifi.h"

#define NOPS		1e3
#define BACKSPACE 	0x08
#define OVER_SAMPLE 16
#define CORE_CLOCK          48000000    // Core clock speed
uint8_t rx_status;
uint8_t request_status;
uint8_t state;
uint8_t uart_recive;

//OKay
int main(void) {
	uint8_t led_state;
	RGB_init();
	wifi_pt = &wifi;
	WifiConf_InitBuffers(wifi_pt, 200);
	WifiConf_Init(wifi_pt, wifi_pt->Buffer_debug_console_pt, UART0);
	while (1) {
		if (wifi_pt->request_pending == 1) {
			wifi_pt->request_pending = 0;
			led_state = _parse_uri(wifi_pt->uri);
			switch (led_state) {
			case (RED):
				RGB(1, 0, 0);
				break;
			case (GREEN):
				RGB(0, 1, 0);
				break;
			case (BLUE):
				RGB(0, 0, 1);
				break;
			}
		}
	}
	return 0;
}

void UART0_Status_IRQHandler(void) {
	//WRITE
	if (((UART0_S1 & 0x80) >> 7)
			&& (!buffer_isempty(wifi_pt->Buffer_debug_console_pt))) {
		UART0_D = buffer_pop(wifi_pt->Buffer_debug_console_pt);
		if (buffer_isempty(wifi_pt->Buffer_debug_console_pt)) {
			wifi_pt->buffer_is_empty = 1;
			UART0_C2 &= ~(0x80);
		}
	}
	if ((UART0_S1 & 0x20) >> 5) {
		char uart_recive;
		uart_recive = UART0_D;
		switch (wifi_pt->wifi_mode) {
		case (CONF_MODE):
			WifiConf_ParseByte(wifi_pt, uart_recive);
			break;
		case (SERVER_MODE):
			WifiRouter_ParseByte(wifi_pt, uart_recive);
			break;
		}
		//request_status = parse_request_byte(request_pt, uart_recive);
	}
}

void UART3_Status_IRQHandler(void) {
	//WRITE
	if (((UART3_S1 & 0x80) >> 7)
			&& (!buffer_isempty(wifi_pt->Wifi_Buffer_Tx_pt))) {
		UART3_D = buffer_pop(wifi_pt->Wifi_Buffer_Tx_pt);
		if (buffer_isempty(wifi_pt->Wifi_Buffer_Tx_pt)) {
			wifi_pt->buffer_is_empty = 1;
			UART3_C2 &= ~(0x80);
		}
	}
	//READ
	if ((UART3_S1 & 0x20) >> 5) {
		char uart_recive;
		uart_recive = UART3_D;
		switch (wifi_pt->wifi_mode) {
		case (CONF_MODE):
			WifiConf_ParseByte(wifi_pt, uart_recive);
			break;
		case (SERVER_MODE):
			break;
		}
		//request_status = parse_request_byte(request_pt, uart_recive);
	}
}
