/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

//20 tx
//21 rx

#include "derivative.h" /* include peripheral declarations */
#include "buffer.h"
#include "cmd_parser.h"
#include "RGB.h"

#define NEW_LINE 	0x0A
#define CARR_RETURN '\r'
#define BACKSPACE 	0x08
#define OVER_SAMPLE 16
#define CORE_CLOCK          48000000    // Core clock speed
uint8_t rx_status;
uint8_t state;
uint8_t uart_recive;

bufferType Buffer_rx;
bufferType *rx_bf;

Request request;
Request *request_pt;

void uart_init_pta(void);
void uart_init_pte(void);
void uart_bluetooth(void);

int main(void)
{
	rx_bf = &Buffer_rx;
	request_pt = &request;
	buffer_init(rx_bf, 50);
	cmd_init(request_pt, 10);
	RGB_init();
	uart_init_pte();
	while (1)
	{
		if (rx_status == 1)
		{
			rx_status = 0;
			state = cmd_parse(request_pt);
			switch (state)
			{
			case NONE:
				RGB(0, 0, 0);
				break;
			case LED_R:
				RGB(1, 0, 0);
				break;
			case LED_G:
				RGB(0, 1, 0);
				break;
			case LED_B:
				RGB(0, 0, 1);
				break;
			}
			cmd_clear(request_pt);
		}
	}
	return 0;
}

void uart_init_pta(void)
{
	SIM_SCGC4 |= (1 << 10);	//CLK UART0
	SIM_SCGC5 |= (1 << 9);	//CLOCK for PORTA
	SIM_SOPT2 |= (1 << 26);	//Enable UART0 clock with MCGFLLCLK clock or MCGPLLCLK/2 clock
	PORTA_PCR1 = (1<<9);	//Port control for UART_0
	PORTA_PCR2 = (1<<9);	//Port control for UART_0
	UART0_BDL = 137;		//clock=640*32768, baud rate 9600
	UART0_C2 |= (1 << 5);		//reciver interrupt enable for RDRF
	UART0_C2 |= (1 << 2);		//RE reciver enable
	UART0_C2 |= (1 << 3);		//TE Transmiter enable
	NVIC_ISER |= (1 << 12);
	NVIC_ICPR |= (1 << 12);
}

void uart_init_pte(void)
{
	SIM_SCGC4 |= (1 << 10);	//CLK UART0
	SIM_SCGC5 |= (1 << 13);	//CLOCK for PORTE
	SIM_SOPT2 |= (1 << 26);	//Enable UART0 clock with MCGFLLCLK clock or MCGPLLCLK/2 clock
	PORTE_PCR20 = (4<<8);	//Port control for UART_0
	PORTE_PCR21 = (4<<8);	//Port control for UART_0a	
	UART0_BDL = 137;		//clock=640*32768, baud rate 9600
	UART0_C2 |= (1 << 5);		//reciver interrupt enable for RDRF
	UART0_C2 |= (1 << 2);		//RE reciver enable
	UART0_C2 |= (1 << 3);		//TE Transmiter enable
	NVIC_ISER |= (1 << 12);
	NVIC_ICPR |= (1 << 12);
}

void uart_bluetooth(void)
{
	SIM_SCGC4 |= (1 << 11);	//CLK UART1
	SIM_SCGC5 |= (1 << 13);	//CLOCK for PORTE
	MCG_C1 |= (1<<6);
	PORTE_PCR1 = (3<<8);	//Port control for UART_0
	PORTE_PCR0 = (3<<8);	//Port control for UART_0
	// Set the baud rate divisor
	//UART1_BDH = 0x0D;		//clock=640*32768, baud rate 9600
	UART1_BDL = 0x0D;		//clock=640*32768, baud rate 9600
	UART1_C2 |= (1 << 5);		//reciver interrupt enable for RDRF
	UART1_C2 |= (1 << 2);		//RE reciver enable
	UART1_C2 |= (1 << 3);		//TE Transmiter enable
	NVIC_ISER |= (1 << 13);
	NVIC_ICPR |= (1 << 13);
}

void UART0_IRQHandler(void)
{
	//WRITE
	if (((UART0_S1 & 0x80) >> 7) && (!buffer_isempty(rx_bf)))
	{
		UART0_D = buffer_pop(rx_bf);
		if (buffer_isempty(rx_bf))
		{
			UART0_C2 &= ~(0x80);
		}
	}
	//READ
	if ((UART0_S1 & 0x20) >> 5 && !(buffer_isfull(rx_bf)))
	{
		uart_recive = UART0_D;
		cmd_add(request_pt, uart_recive);
		buffer_push(rx_bf, uart_recive);
		if (uart_recive != CARR_RETURN)
		{

		}
		else
		{
			rx_status = 1;
			//buffer_push(rx_bf, NEW_LINE);
			//buffer_push(rx_bf, '\r');
		}
		UART0_C2 |= 0x80;	//Turn on TX interrupt
	}
}

void UART1_IRQHandler(void)
{
	//WRITE
	if (((UART1_S1 & 0x80) >> 7) && (!buffer_isempty(rx_bf)))
	{
		UART1_D = buffer_pop(rx_bf);
		if (buffer_isempty(rx_bf))
		{
			UART1_C2 &= ~(0x80);
		}
	}
	//READ
	if ((UART1_S1 & 0x20) >> 5 && !(buffer_isfull(rx_bf)))
	{
		uart_recive = UART1_D;
		buffer_push(rx_bf, uart_recive);
		cmd_add(request_pt, uart_recive);
		if (uart_recive != CARR_RETURN)
		{

		}
		else
		{
			buffer_push(rx_bf, NEW_LINE);
			rx_status = 1;
		}
		UART1_C2 |= 0x80;	//Turn on TX interrupt
	}
}
