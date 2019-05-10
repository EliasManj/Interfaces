/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "Camera.h"

Camera camera;
Camera *camera_pt;

int main(void)
{
	camera_pt = &camera;
	Camera_Init(camera_pt);
	Camera_SetUART(camera_pt, camera_pt->Wifi_Buffer_Tx_pt, UART3);
	//Camera_GetVersion(camera_pt);
	Camera_Snapshot(camera_pt);
	while (1)
	{

	}

	return 0;
}

void UART0_Status_IRQHandler(void)
{
	//WRITE
	if (((UART0_S1 & 0x80) >> 7) && (!buffer_isempty(*camera_pt->buffer)))
	{
		UART0_D = buffer_pop(*camera_pt->buffer);
	}
	//READ
	if ((UART0_S1 & 0x20) >> 5)
	{
		char uart_recive;
		uart_recive = UART0_D;
		Camera_Parse_Byte(camera_pt, uart_recive);
	}
	if (buffer_isempty(*camera_pt->buffer))
	{
		camera_pt->buffer_is_empty = 1;
		UART0_C2 &= ~(0x80);
	}
}

void UART3_Status_IRQHandler(void)
{
	//WRITE
	if (((UART3_S1 & 0x80) >> 7) && (!buffer_isempty(*camera_pt->buffer)))
	{
		UART3_D = buffer_pop(*camera_pt->buffer);
	}
	//READ
	if ((UART3_S1 & 0x20) >> 5)
	{
		char uart_recive;
		uart_recive = UART3_D;
		Camera_Parse_Byte(camera_pt, uart_recive);
	}
	if (buffer_isempty(*camera_pt->buffer))
	{
		camera_pt->buffer_is_empty = 1;
		UART3_C2 &= ~(0x80);
	}
}
