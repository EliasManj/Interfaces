/*
 * Wifi_Uart_Interface.c
 *
 *  Created on: May 14, 2019
 *      Author: Miguel Elias
 */

#include "Wifi_Uart_Interface.h"

//UART Interface
void Wifi_UART_SendString_Enable_Tx(Wifi_Obj *Wifi_Obj, bufferType *bf, char *str)
{
	int32_t i;
	for (i = 0; i < _strlen(str); i++)
	{
		buffer_push(bf, str[i]);
	}
	wifi_pt->buffer_is_empty = 0;
	Wifi_Enable_Tx(Wifi_Obj);
}

void Wifi_UART_SubString(Wifi_Obj *Wifi_Obj, bufferType *bf, char *str, int start, int end)
{
	int32_t i;
	for (i = start; i <= end; i++)
	{
		buffer_push(bf, str[i]);
	}
	wifi_pt->buffer_is_empty = 0;
	Wifi_Enable_Tx(Wifi_Obj);
}

void Wifi_UART_SendString(bufferType *bf, char *str)
{
	int32_t i;
	for (i = 0; i < _strlen(str); i++)
	{
		buffer_push(bf, str[i]);
	}
	wifi_pt->buffer_is_empty = 0;
}

void Wifi_UART_SendString_UntilEmpty(bufferType *bf, char *str)
{
	int32_t i;
	i = 0;
	char byte;
	byte = str[i];
	while (byte != ' ' || byte != 0)
	{
		if (byte == 0 || byte == ' ')
		{
			break;
		}
		else
		{
			buffer_push(bf, byte);
		}
		i++;
		byte = str[i];
	}
	wifi_pt->buffer_is_empty = 0;
}

void Wifi_Enable_Tx(Wifi_Obj *Wifi_Obj)
{
	if (Wifi_Obj->uart_channel == WIFI_UART0)
	{
		UART0_C2 |= 0x80; //Turn on TX interrupt		
	}
	else if (Wifi_Obj->uart_channel == WIFI_UART3)
	{
		UART3_C2 |= 0x80; //Turn on TX interrupt
	}
}

void Wifi_UART_Init_3(void)
{
	SIM_SCGC4 |= (1 << 13);	//CLK UART3
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; /*Enable the PORTC clock*/
	PORTC_PCR16 |= PORT_PCR_MUX(3);
	PORTC_PCR17 |= PORT_PCR_MUX(3);
	UART3_BDL = WIFI_BAUD9600;		//clock=640*32768, baud rate 9600
	UART3_C2 |= (1 << 5);		//reciver interrupt enable for RDRF
	UART3_C2 |= (1 << 3);		//TE Transmiter enable
	NVIC_ISER(1) |= (1 << (37%32));
	NVIC_ICPR(1) |= (1 << (37%32));
	Wifi_NVIC_SetPriority(37, 1);
}

void Wifi_UART_Init_0(void)
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
	Wifi_NVIC_SetPriority(31, 1);
}

void Wifi_NVIC_SetPriority(int iInterruptID, unsigned char ucPriority)
{
	volatile unsigned char *ptrPriority = &NVIC_IP_REG(NVIC_BASE_PTR,iInterruptID) ;
	ptrPriority += iInterruptID;
	*ptrPriority = (ucPriority << __NVIC_PRIORITY_SHIFT);
}

void Wifi_UART_Init(Wifi_Obj *Wifi_Obj, int uart_channel)
{
	Wifi_Obj->uart_channel = uart_channel;
	if (uart_channel == WIFI_UART0)
	{
		Wifi_UART_Init_0();
	}
	else if (Wifi_Obj->uart_channel == WIFI_UART3)
	{
		Wifi_UART_Init_3();
	}
}
