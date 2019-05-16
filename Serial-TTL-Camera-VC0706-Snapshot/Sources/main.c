/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "Camera.h"
#include "Button.h"

Camera camera;
Camera *camera_pt;

void Camera_Init_Wrapper(void);

int main(void)
{

	Button_SW2_Init();
	RGB_init();
	RGB(0, 0, 1);
	Camera_Init_Wrapper();
	while (1)
	{
		if (camera_pt->take_snapshot == 1)
		{
			camera_pt->take_snapshot = 0;
			Camera_Cmd_Snapshot(camera_pt, camera_pt->Wifi_Buffer_Tx_pt);
			Camera_Cmd_SendSerial(camera_pt, camera_pt->Buffer_debug_console_pt);
		}
	}

	return 0;
}

void Camera_Init_Wrapper(void)
{
	camera_pt = &camera;
	Camera_Init(camera_pt);
	Camera_UART_Init(camera_pt, CAMERA_UART3, CAMERA_BAUD_38400);
	Camera_SetupSerialCom_Init(camera_pt, CAMERA_UART0, CAMERA_BAUD_38400);
}

void UART0_Status_IRQHandler(void)
{
	//WRITE
	if (((UART0_S1 & 0x80) >> 7) && (!buffer_isempty(camera_pt->Buffer_debug_console_pt)))
	{
		UART0_D = buffer_pop(camera_pt->Buffer_debug_console_pt);
		if (buffer_isempty(camera_pt->Buffer_debug_console_pt))
		{
			camera_pt->buffer_is_empty = 1;
			UART0_C2 &= ~(0x80);
		}
	}
	//READ
	if ((UART0_S1 & 0x20) >> 5)
	{
		char uart_recive;
		uart_recive = UART0_D;
		Camera_Parse_Byte(camera_pt, uart_recive);
	}
}

void UART3_Status_IRQHandler(void)
{
	//WRITE
	uint8_t not_empty;
	not_empty = !buffer_isempty(camera_pt->Wifi_Buffer_Tx_pt);
	if (((UART3_S1 & 0x80) >> 7) && (not_empty))
	{
		UART3_D = buffer_pop(camera_pt->Wifi_Buffer_Tx_pt);
		if (buffer_isempty(camera_pt->Wifi_Buffer_Tx_pt))
		{
			camera_pt->buffer_is_empty = 1;
			UART3_C2 &= ~(0x80);
		}
	}
	//READ
	if ((UART3_S1 & 0x20) >> 5)
	{
		char uart_recive;
		uart_recive = UART3_D;
		Camera_Parse_Byte(camera_pt, uart_recive);
	}
}

void UART1_Status_IRQHandler(void)
{
	//WRITE
	uint8_t not_empty;
	not_empty = !buffer_isempty(camera_pt->Wifi_Buffer_Tx_pt);
	if (((UART1_S1 & 0x80) >> 7) && (not_empty))
	{
		UART1_D = buffer_pop(camera_pt->Wifi_Buffer_Tx_pt);
		if (buffer_isempty(camera_pt->Wifi_Buffer_Tx_pt))
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
