/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "Camera.h"
#include "Button.h"

Camera camera;
Camera *camera_pt;

uint8_t take_snapshot = 0;

int main(void)
{
	camera_pt = &camera;
	Camera_Init(camera_pt);
	Button_SW2_Init();
	RGB_init();
	RGB(0,0,1);
	Camera_SetUART(camera_pt, camera_pt->Wifi_Buffer_Tx_pt, UART3, BAUD_38400);

	while (1)
	{
		if (take_snapshot == 1)
		{
			Camera_Snapshot(camera_pt, camera_pt->Wifi_Buffer_Tx_pt);
		}
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

void PORTC_IRQHandler()
{
	PORTC_PCR6 &= ~(0<<24);
	take_snapshot = 1;
}
