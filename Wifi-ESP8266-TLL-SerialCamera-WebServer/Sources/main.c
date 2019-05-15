/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

//20 tx
//21 rx
#include "derivative.h" /* include peripheral declarations */

#include "RGB.h"
#include "Wifi.h"
#include "Camera.h"

#define BACKSPACE 	0x08
#define OVER_SAMPLE 16

Camera camera;
Camera *camera_pt;

void Button_SW2_Init(void);
void Wifi_Init_Wrapper(void);
void Wifi_Main(void);
void Camera_Init_Wrapper(void);
void Camera_Main(void);

//OKay
int main(void)
{
	RGB_init();
	Button_SW2_Init();
	Wifi_Init_Wrapper();
	Camera_Init_Wrapper();
	while (1)
	{
		Camera_Main();
		Wifi_Main();
	}
	return 0;
}

void Camera_Main(void)
{
	if (camera_pt->take_snapshot == 1)
	{
		camera_pt->take_snapshot = 0;
		Camera_Cmd_Snapshot(camera_pt, camera_pt->Camera_Buffer_Tx_pt);
		wifi_pt->send_trigger = 1;
	}
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
		Wifi_Http_Send_Request_PostJson(wifi_pt, wifi_pt->Wifi_Buffer_Tx_pt, "192.168.43.86", "80", "/test-payload", "image", camera_pt->image_buffer, camera_pt->image_start_pointer, camera_pt->image_end_pointer);
		RGB(0, 1, 1);
	}
}

void Camera_Init_Wrapper(void)
{
	camera_pt = &camera;
	Camera_Init(camera_pt);
	Camera_UART_Init(camera_pt, CAMERA_UART1, CAMERA_BAUD_38400);
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

void UART1_Status_IRQHandler(void)
{
	//WRITE
	uint8_t not_empty;
	not_empty = !buffer_isempty(camera_pt->Camera_Buffer_Tx_pt);
	if (((UART1_S1 & 0x80) >> 7) && (not_empty))
	{
		UART1_D = buffer_pop(camera_pt->Camera_Buffer_Tx_pt);
		if (buffer_isempty(camera_pt->Camera_Buffer_Tx_pt))
		{
			camera_pt->buffer_is_empty = 1;
			UART1_C2 &= ~(0x80);
		}
	}
	//READ
	if ((UART1_S1 & 0x20) >> 5)
	{
		char uart_recive;
		uart_recive = UART1_D;
		Camera_Parse_Byte(camera_pt, uart_recive);
	}
}

void PORTC_IRQHandler()
{
	PORTC_PCR6 &= ~(0<<24);
	camera_pt->take_snapshot = 1;
}

void Button_SW2_Init(void)
{
	SIM_SCGC5 |= (1 << 11);
	PORTC_PCR6 |= (1<<8);
	PORTC_PCR6 |= (8<<16);
	NVIC_ICPR(1) |= (1<<(61%32));		//Clean flag of LPTM in the interrupt vector
	NVIC_ISER(1) |= (1<<(61%32)); 		//Activate the LPTM interrupt
}
