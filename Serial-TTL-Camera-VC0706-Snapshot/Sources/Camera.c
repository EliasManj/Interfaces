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
	case (CAMERA_IDLE):
		break;
	case (CAMERA_GET_VERSION):
		Camera_Parse_GetVersion(camera, byte);
		break;
	case (CAMERA_STOP_CFBUF):
		Camera_Parse_StopCFBuf(camera, byte);
		break;
	case (CAMERA_RECIVE_IMAGE):
		Camera_Parse_GetSnapshot(camera, byte);
		break;
	case (CAMERA_RESUME):
		Camera_Parse_Resume(camera, byte);
		break;
	case (CAMERA_GET_FBUF_LEN):
		Camera_Parse_GetFBufLen(camera, byte);
		break;
	}
}

void Camera_Parse_GetFBufLen(Camera *camera, char byte)
{
	if (camera->parsing_fbuf_len == 1)
	{
		camera->buffer_len[camera->camera_field_counter++] = byte;
	}
	if (byte == 0x04)
	{
		camera->parsing_fbuf_len = 1;
	}
	if (camera->camera_field_counter >= 4)
	{
		Camera_ResetProperties(camera);
	}
}

void Camera_Parse_GetSnapshot(Camera *camera, char byte)
{
	camera->image_buffer[camera->camera_field_counter++] = byte;
	if (camera->incoming_jpg_marker)
	{
		if (byte == 0xD8)
		{
			camera->image_buffering = 1;
			camera->image_start_pointer = camera->camera_field_counter - 1;
		}
		if (byte == 0xD9 && camera->image_buffering)
		{
			camera->image_end_pointer = camera->camera_field_counter - 1;
			Camera_ResetProperties(camera);
		}
		camera->incoming_jpg_marker = 0;
	}
	if (byte == 0xFF)
	{
		camera->incoming_jpg_marker = 1;
	}
}

void Camera_Parse_GetVersion(Camera *camera, char byte)
{
	if (byte == 'V')
	{
		camera->camera_field_counter = 0;
	}
	if (byte == '.')
	{
		camera->camera_delta_counter = 0;
	}
	if (camera->camera_field_counter >= CAMERA_VER_SIZE || camera->camera_delta_counter == 2)
	{
		Camera_ResetProperties(camera);
	}
	else
	{
		camera->cam_version[camera->camera_field_counter++] = byte;
		camera->camera_delta_counter++;
	}
}

void Camera_Parse_StopCFBuf(Camera *camera, char byte)
{
	if (byte == 0x36)
	{
		camera->camera_delta_counter = 0;
	}
	if (camera->camera_delta_counter == 2)
	{
		Camera_ResetProperties(camera);
	}
	camera->camera_delta_counter++;
}

void Camera_Parse_Resume(Camera *camera, char byte)
{
	if (byte == 0x36)
	{
		camera->camera_delta_counter = 0;
	}
	if (camera->camera_delta_counter == 2)
	{
		Camera_ResetProperties(camera);
	}
	camera->camera_delta_counter++;
}

//Camera functions

void Camera_Init(Camera *camera)
{
	camera->Wifi_Buffer_Rx_pt = &camera->Wifi_Buffer_Rx;
	camera->Buffer_debug_console_pt = &camera->Buffer_debug_console;
	camera->Wifi_Buffer_Tx_pt = &camera->Wifi_Buffer_Tx;
	buffer_init(camera->Wifi_Buffer_Tx_pt, 100);
	buffer_init(camera->Buffer_debug_console_pt, 100);
	camera->camera_field_counter = 0;
	camera->take_snapshot = 0;
	camera->camera_delta_counter = 0xFF;
}

void Camera_ResetProperties(Camera *camera)
{
	camera->parsing_fbuf_len = 0;
	camera->camera_field_counter = 0;
	camera->camera_delta_counter = 0xFF;
	camera->camera_state = CAMERA_IDLE;
	camera->image_buffering = 0;
}

void Camera_UART_Init(Camera *camera, int uart_channel, int baud_rate)
{
	camera->uart_channel = uart_channel;
	if (uart_channel == CAMERA_UART0)
	{
		Camera_UART_Init_0(baud_rate);
	}
	else if (uart_channel == CAMERA_UART1)
	{
		Camera_UART_Init_1(baud_rate);
	}
	else if (uart_channel == CAMERA_UART3)
	{
		Camera_UART_Init_3(baud_rate);
	}
}

void Camera_Cmd_GetVersion(Camera *camera, bufferType *bf)
{
	camera->camera_state = CAMERA_GET_VERSION;
	Camera_UART_SendByte(bf, 0x56);
	Camera_UART_SendByte(bf, 0x00);
	Camera_UART_SendByte(bf, 0x11);
	Camera_UART_SendByte(bf, 0x00);
	Camera_UART_Enable_Rx(camera);
	Camera_UART_Enable_Tx(camera);
	Camera_WaitForState(camera, CAMERA_IDLE);
}

void Camera_Cmd_ResumeFrame(Camera *camera, bufferType *bf)
{
	camera->camera_state = CAMERA_RESUME;
	Camera_UART_SendByte(bf, 0x56);
	Camera_UART_SendByte(bf, 0x00);
	Camera_UART_SendByte(bf, 0x36);
	Camera_UART_SendByte(bf, 0x01);
	Camera_UART_SendByte(bf, 0x03);
	Camera_UART_Enable_Rx(camera);
	Camera_UART_Enable_Tx(camera);
	Camera_WaitForState(camera, CAMERA_IDLE);
}

void Camera_StopCFBuf(Camera *camera, bufferType *bf)
{
	camera->camera_state = CAMERA_STOP_CFBUF;
	Camera_UART_SendByte(bf, 0x56);
	Camera_UART_SendByte(bf, 0x00);
	Camera_UART_SendByte(bf, 0x36);
	Camera_UART_SendByte(bf, 0x01);
	Camera_UART_SendByte(bf, 0x00);
	Camera_UART_Enable_Rx(camera);
	Camera_UART_Enable_Tx(camera);
	Camera_WaitForState(camera, CAMERA_IDLE);
}

void Camera_Cmd_Snapshot(Camera *camera, bufferType *bf)
{
	RGB(1, 1, 0);
	Camera_StopCFBuf(camera, bf);
	Camera_Cmd_GetFBufLen(camera, bf);
	Camera_Cmd_GetReadFBuf(camera, bf);
	delay(100000);
	Camera_Cmd_ResumeFrame(camera, bf);
	RGB(0, 1, 0);
}

void Camera_Cmd_GetReadFBuf(Camera *camera, bufferType *bf)
{
	camera->camera_state = CAMERA_RECIVE_IMAGE;
	Camera_UART_SendByte(bf, 0x56);
	Camera_UART_SendByte(bf, 0x00);
	Camera_UART_SendByte(bf, 0x32); //Command byte
	Camera_UART_SendByte(bf, 0x0C);
	Camera_UART_SendByte(bf, 0x00); //Buf type
	Camera_UART_SendByte(bf, 0x0A); //Control mode
	Camera_UART_SendByte(bf, 0x00); //Start addres start (4 bytes)
	Camera_UART_SendByte(bf, 0x00);
	Camera_UART_SendByte(bf, 0x00);
	Camera_UART_SendByte(bf, 0x00); //Start addres end
	Camera_UART_SendByte(bf, camera->buffer_len[0]); //data length start (4 bytes)
	Camera_UART_SendByte(bf, camera->buffer_len[1]);
	Camera_UART_SendByte(bf, camera->buffer_len[2]);
	Camera_UART_SendByte(bf, camera->buffer_len[3]); //date length end
	Camera_UART_SendByte(bf, 0x0B);
	Camera_UART_SendByte(bf, 0xB8);
	Camera_UART_Enable_Rx(camera);
	Camera_UART_Enable_Tx(camera);
	Camera_WaitForState(camera, CAMERA_IDLE);
}

void Camera_Cmd_GetFBufLen(Camera *camera, bufferType *bf)
{
	camera->camera_state = CAMERA_GET_FBUF_LEN;
	Camera_UART_SendByte(bf, 0x56);
	Camera_UART_SendByte(bf, 0x00);
	Camera_UART_SendByte(bf, 0x34);
	Camera_UART_SendByte(bf, 0x01);
	Camera_UART_SendByte(bf, 0x00);
	Camera_UART_Enable_Rx(camera);
	Camera_UART_Enable_Tx(camera);
	Camera_WaitForState(camera, CAMERA_IDLE);
}

void Camera_WaitForState(Camera *camera, int state)
{
	while (camera->camera_state != state)
		;
}


//Camera UART functions

void Camera_UART_WaitEmptyBuffer(Camera *camera)
{
	while (camera->buffer_is_empty == 0)
		;
}

void Camera_UART_SendString(Camera *camera, bufferType *bf, char *str)
{
	int32_t i;
	for (i = 0; i < _strlen(str); i++)
	{
		buffer_push(bf, str[i]);
	}
	camera->buffer_is_empty = 0;
}

void Camera_UART_Enable_Rx(Camera *camera)
{
	if (camera->uart_channel == CAMERA_UART0)
	{
		UART0_C2 |= (1 << 2);		//RE reciver enable	
	}
	else if (camera->uart_channel == CAMERA_UART1)
	{
		UART1_C2 |= (1 << 2);		//RE reciver enable
	}
	else if (camera->uart_channel == CAMERA_UART3)
	{
		UART3_C2 |= (1 << 2);		//RE reciver enable
	}
}

void Camera_UART_SendString_Enable_Tx(Camera *camera, bufferType *bf, char *str)
{
	int32_t i;
	for (i = 0; i < _strlen(str); i++)
	{
		buffer_push(bf, str[i]);
	}
	camera->buffer_is_empty = 0;
	if (camera->uart_channel == CAMERA_UART0)
	{
		UART0_C2 |= 0x80; //Turn on TX interrupt		
	}
	else if (camera->uart_channel == CAMERA_UART1)
	{
		UART1_C2 |= 0x80; //Turn on TX interrupt
	}
	else if (camera->uart_channel == CAMERA_UART3)
	{
		UART3_C2 |= 0x80; //Turn on TX interrupt
	}
}

void Camera_UART_SendByte(bufferType *bf, char byte)
{
	buffer_push(bf, byte);
}

void Camera_UART_Enable_Tx(Camera *camera)
{
	if (camera->uart_channel == CAMERA_UART0)
	{
		UART0_C2 |= 0x80; //Turn on TX interrupt		
	}
	else if (camera->uart_channel == CAMERA_UART1)
	{
		UART1_C2 |= 0x80; //Turn on TX interrupt
	}
	else if (camera->uart_channel == CAMERA_UART3)
	{
		UART3_C2 |= 0x80; //Turn on TX interrupt
	}
}

void Camera_UART_NVIC_SetPriority(int iInterruptID, unsigned char ucPriority)
{
	volatile unsigned char *ptrPriority = &NVIC_IP_REG(NVIC_BASE_PTR,iInterruptID) ;
	ptrPriority += iInterruptID;
	*ptrPriority = (ucPriority << CAMERA__NVIC_PRIORITY_SHIFT);
}

void Camera_UART_Init_0(int baud_rate)
{
	SIM_SCGC4 |= (1 << 10);	//CLK UART0
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; /*Enable the PORTB clock*/
	PORTB_PCR16 |= PORT_PCR_MUX(3);
	PORTB_PCR17 |= PORT_PCR_MUX(3);
	switch (baud_rate)
	{
	case (CAMERA_BAUD_9600):
		UART0_BDL = CAMERA_BAUD_9600_BDL;
		UART0_BDH = CAMERA_BAUD_9600_BDH;
		break;
	case (CAMERA_BAUD_14400):
		UART0_BDL = CAMERA_BAUD_14400_BDL;
		UART0_BDH = CAMERA_BAUD_14400_BDH;
		break;
	case (CAMERA_BAUD_38400):
		UART0_BDL = CAMERA_BAUD_38400_BDL;
		UART0_BDH = CAMERA_BAUD_38400_BDH;
		break;
	}
	UART0_C2 |= (1 << 5);		//reciver interrupt enable for RDRF
	UART0_C2 |= (1 << 2);		//RE reciver enable
	UART0_C2 |= (1 << 3);		//TE Transmiter enable
	NVIC_ISER(0) |= (1 << (31%32));
	NVIC_ICPR(0) |= (1 << (31%32));
	Camera_UART_NVIC_SetPriority(31, 1);
}

void Camera_UART_Init_3(int baud_rate)
{
	SIM_SCGC4 |= (1 << 13);	//CLK UART3
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; /*Enable the PORTC clock*/
	PORTC_PCR16 |= PORT_PCR_MUX(3);
	PORTC_PCR17 |= PORT_PCR_MUX(3);
	switch (baud_rate)
	{
	case (CAMERA_BAUD_9600):
		UART3_BDL = CAMERA_BAUD_9600_BDL;
		break;
	case (CAMERA_BAUD_14400):
		UART3_BDL = CAMERA_BAUD_14400_BDL;
		break;
	case (CAMERA_BAUD_38400):
		UART3_BDL = CAMERA_BAUD_38400_BDL;
		break;
	}
	UART3_C2 |= (1 << 5);		//reciver interrupt enable for RDRF
	UART3_C2 |= (1 << 3);		//TE Transmiter enable
	NVIC_ISER(1) |= (1 << (37%32));
	NVIC_ICPR(1) |= (1 << (37%32));
	Camera_UART_NVIC_SetPriority(37, 1);
}

void Camera_UART_Init_1(int baud_rate)
{
	SIM_SCGC4 |= (1 << 11);	//CLK UART1
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; 	/*Enable the PORTC clock*/
	PORTC_PCR4 |= PORT_PCR_MUX(3);
	PORTC_PCR3 |= PORT_PCR_MUX(3);
	switch (baud_rate)
	{
	case (CAMERA_BAUD_9600):
		UART1_BDL = CAMERA_BAUD_9600_BDL;
		break;
	case (CAMERA_BAUD_14400):
		UART1_BDL = CAMERA_BAUD_14400_BDL;
		break;
	case (CAMERA_BAUD_38400):
		UART1_BDL = CAMERA_BAUD_38400_BDL;
		break;
	}
	UART1_C2 |= (1 << 5);		//reciver interrupt enable for RDRF
	UART1_C2 |= (1 << 3);		//TE Transmiter enable
	NVIC_ISER(1) |= (1 << (33%32));
	NVIC_ICPR(1) |= (1 << (33%32));
	Camera_UART_NVIC_SetPriority(33, 1);
}
