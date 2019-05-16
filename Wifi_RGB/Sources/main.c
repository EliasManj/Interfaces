/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

//20 tx
//21 rx
#include "derivative.h" /* include peripheral declarations */

#include "RGB.h"
#include "Wifi.h"

#define BACKSPACE 	0x08
#define OVER_SAMPLE 16

void Button_SW2_Init(void);
void Wifi_Init_Wrapper(void);
void Wifi_Main(void);

//OKay
int main(void)
{
	RGB_init();
	Button_SW2_Init();
	Wifi_Init_Wrapper();
	while (1)
	{
		Wifi_Main();
	}
	return 0;
}

void Wifi_Main(void)
{
	if (wifi_pt->request_pending == 1)
	{
		Wifi_Router_Route(wifi_pt);
	}
	else if (wifi_pt->send_trigger == 1)
	{
		wifi_pt->send_trigger = 0;
		RGB(0, 0, 1);
		switch (wifi_pt->led_state)
		{
		case (RED):
			Wifi_Http_Send_Request_Post_Json(wifi_pt, wifi_pt->Wifi_Buffer_Tx_pt, "192.168.43.86", "80", "/test-payload", "led-state", "red", 0, _strlen("red") - 1);
			break;
		case (BLUE):
			Wifi_Http_Send_Request_Post_Json(wifi_pt, wifi_pt->Wifi_Buffer_Tx_pt, "192.168.43.86", "80", "/test-payload", "led-state", "blue", 0, _strlen("blue") - 1);
			break;
		case (GREEN):
			Wifi_Http_Send_Request_Post_Json(wifi_pt, wifi_pt->Wifi_Buffer_Tx_pt, "192.168.43.86", "80", "/test-payload", "led-state", "green", 0, _strlen("green") - 1);
			break;
		}
	}
}

void Wifi_Init_Wrapper(void)
{
	wifi_pt = &wifi;
	Wifi_InitBuffers(wifi_pt, 10000);
	Wifi_Init(wifi_pt, wifi_pt->Wifi_Buffer_Tx_pt, WIFI_UART3);
}

void UART0_Status_IRQHandler(void)
{
	//WRITE
	if (((UART0_S1 & 0x80) >> 7) && (!buffer_isempty(wifi_pt->Buffer_debug_console_pt)))
	{
		uint8_t send;
		send = buffer_pop(wifi_pt->Buffer_debug_console_pt);
		UART0_D = send;
		if (buffer_isempty(wifi_pt->Buffer_debug_console_pt))
		{
			wifi_pt->buffer_is_empty = 1;
			UART0_C2 &= ~(0x80);
		}
	}
	if ((UART0_S1 & 0x20) >> 5)
	{
		char uart_recive;
		uart_recive = UART0_D;
		Wifi_Parse_Byte(wifi_pt, uart_recive);
	}
}

void UART3_Status_IRQHandler(void)
{
	//WRITE
	if (((UART3_S1 & 0x80) >> 7) && (!buffer_isempty(wifi_pt->Wifi_Buffer_Tx_pt)))
	{
		UART3_D = buffer_pop(wifi_pt->Wifi_Buffer_Tx_pt);
		if (buffer_isempty(wifi_pt->Wifi_Buffer_Tx_pt))
		{
			wifi_pt->buffer_is_empty = 1;
			UART3_C2 &= ~(0x80);
		}
	}
	//READ
	if ((UART3_S1 & 0x20) >> 5)
	{
		char uart_recive;
		uart_recive = UART3_D;
		Wifi_Parse_Byte(wifi_pt, uart_recive);
	}
}

void PORTC_IRQHandler()
{
	PORTC_PCR6 &= ~(0<<24);
	wifi_pt->send_trigger = 1;
}

void Button_SW2_Init(void)
{
	SIM_SCGC5 |= (1 << 11);
	PORTC_PCR6 |= (1<<8);
	PORTC_PCR6 |= (8<<16);
	NVIC_ICPR(1) |= (1<<(61%32));		//Clean flag of LPTM in the interrupt vector
	NVIC_ISER(1) |= (1<<(61%32)); 		//Activate the LPTM interrupt
}
