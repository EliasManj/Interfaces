#include "derivative.h" /* include peripheral declarations */
#include "RGB.h"
#include "Wifi.h"

void Wifi_Init_Wrapper(void);


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
		
	}
}

void Wifi_Init_Wrapper(void)
{
	wifi_pt = &wifi;
	Wifi_InitBuffers(wifi_pt, 20000);
	Wifi_Init(wifi_pt, wifi_pt->Wifi_Buffer_Tx_pt, WIFI_UART3);
}

void UART0_Status_IRQHandler(void)
{
	//WRITE
	if (((UART0_S1 & 0x80) >> 7) && (!buffer_isempty(wifi_pt->Buffer_debug_console_pt)))
	{
		UART0_D = buffer_pop(wifi_pt->Buffer_debug_console_pt);
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
		switch (wifi_pt->wifi_mode)
		{
		case (WIFI_CONF_MODE):
			Wifi_Conf_ParseByte(wifi_pt, uart_recive);
			break;
		case (WIFI_SERVER_MODE):
			Wifi_Router_ParseByte(wifi_pt, uart_recive);
			break;
		}
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
		switch (wifi_pt->wifi_mode)
		{
		case (WIFI_CONF_MODE):
			Wifi_Conf_ParseByte(wifi_pt, uart_recive);
			break;
		case (WIFI_SERVER_MODE):
			Wifi_Router_ParseByte(wifi_pt, uart_recive);
			break;
		}
	}
}
