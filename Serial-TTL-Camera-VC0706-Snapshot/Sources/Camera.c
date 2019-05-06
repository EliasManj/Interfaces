/*
 * Camera.c
 *
 *  Created on: May 5, 2019
 *      Author: Miguel Elias
 */

#include "Camera.h"

//Camera parser functions
void Camera_Parse_Byte(Camera *camera, char byte)
{
	switch (camera->camera_state)
	{
	case (IDLE):
		break;
	case (GET_VERSION):
		Camera_Parse_GetVersion(camera, byte);
		break;
	}
}

void Camera_Parse_GetVersion(Camera *camera, char byte)
{
	if (byte == 'V')
	{
		camera->camera_field_counter = 0;
	}
	if (camera->camera_field_counter >= _strlen(camera->cam_version))
	{
		camera->camera_state = IDLE;
	}
	camera->cam_version[camera->camera_field_counter++] = byte;
}

void Camera_Parse_StopCFBuf(Camera *camera, char byte){
	
}

//Camera functions

void Camera_Init(Camera *camera)
{
	camera->Wifi_Buffer_Rx_pt = &camera->Wifi_Buffer_Rx;
	camera->Buffer_debug_console_pt = &camera->Buffer_debug_console;
	camera->Wifi_Buffer_Tx_pt = &camera->Wifi_Buffer_Tx;
}

void Camera_SetUART(Camera *camera, bufferType *bf, int uart_channel)
{
	camera->uart_channel = uart_channel;
	camera->buffer = &bf;
	buffer_init(*camera->buffer, 100);
	if (uart_channel == UART0)
	{
		Camera_UART0_Init_USB();
	}
	else if (uart_channel == UART1)
	{
	}
}

void Camera_GetVersion(Camera *camera)
{
	camera->camera_state = GET_VERSION;
	Camera_UART_SendString_Enable_Tx(camera, "V");
}

void Camera_StopCFBuf(Camera *camera)
{
	camera->camera_state = STOP_CFBUF;
	Camera_UART_SendString_Enable_Tx(camera, "6");
}

void Camera_Snapshot(Camera *camera){
	camera->camera_state = RECIVE_IMAGE;
	Camera_UART_SendString_Enable_Tx(camera, "V4V2");
}

//Camera UART functions

void Camera_WaitEmptyBuffer(Camera *camera)
{
	while (camera->buffer_is_empty == 0)
		;
}

void Camera_UART_SendString(Camera *camera, char *str)
{
	int32_t i;
	for (i = 0; i < _strlen(str); i++)
	{
		buffer_push(*camera->buffer, str[i]);
	}
	camera->buffer_is_empty = 0;
}

void Camera_UART_SendString_Enable_Tx(Camera *camera, char *str)
{
	int32_t i;
	for (i = 0; i < _strlen(str); i++)
	{
		buffer_push(*camera->buffer, str[i]);
	}
	camera->buffer_is_empty = 0;
	if (camera->uart_channel == UART0)
	{
		UART0_C2 |= 0x80; //Turn on TX interrupt		
	}
	else if (camera->uart_channel == UART1)
	{
		//UART3_C2 |= 0x80; //Turn on TX interrupt
	}
}

void Camera_NVIC_SetPriority(int iInterruptID, unsigned char ucPriority)
{
	volatile unsigned char *ptrPriority = &NVIC_IP_REG(NVIC_BASE_PTR,iInterruptID) ;
	ptrPriority += iInterruptID;
	*ptrPriority = (ucPriority << __NVIC_PRIORITY_SHIFT);
}

void Camera_UART0_Init_USB(void)
{
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
	Camera_NVIC_SetPriority(31, 1);
}
