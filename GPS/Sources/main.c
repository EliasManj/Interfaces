/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "Buffer.h"

#define NEW_LINE 	0x0A
#define CARR_RETURN 0x0D
#define BACKSPACE 	0x08

//UART Buffer
uint8_t uart_recive;
bufferType Buffer_rx =
{ 0, 0, BUFLEN,
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
bufferType *rx_bf;

//GPS Parser States

uint16_t current_state;
uint16_t gps_counter;

void UART_Init(void);
void UART_Write();

int main(void)
{
	rx_bf = &Buffer_rx;
	UART_Init();
	while (1)
	{
	}
	return 0;
}

void UART_Init(void)
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
}

void UART0_Status_IRQHandler(void)
{

}

void UART_Read()
{
	//READ
	if ((UART0_S1 & 0x20) >> 5 && !(buffer_is_full(rx_bf)))
	{
		uart_recive = UART0_D;
		parse_gps_byte(uart_recive);
		buffer_push(rx_bf, uart_recive);
	}
}

void UART_Write()
{
	//WRITE
	if (((UART0_S1 & 0x80) >> 7) && (!buffer_is_empty(rx_bf)))
	{
		UART0_D = buffer_pop(rx_bf);
		if (buffer_is_empty(rx_bf))
		{
			UART0_C2 &= ~(0x80);
		}
	}
}
