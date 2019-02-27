/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "Buffer.h"
#include "GPS.h"

#define NEW_LINE 	0x0A
#define CARR_RETURN 0x0D
#define BACKSPACE 	0x08

//UART Buffer
uint8_t uart_recive;

//GPS 
GPS_Struct GPS_Object;
GPS_Struct *GPS_pt;

void UART_Init(void);
void UART_Write();

int main(void)
{
	GPS_pt = &GPS_Object;
	UART_Init();
	while (1)
	{
	}
	return 0;
}

void UART_Init(void)
{
	SIM_SCGC4 |= (1 << 13);	//CLK UART3
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; /*Enable the PORTB clock*/
	PORTC_PCR16 |= PORT_PCR_MUX(3);
	PORTC_PCR17 |= PORT_PCR_MUX(3);
	UART3_BDL = 137;		//clock=640*32768, baud rate 9600
	UART3_C2 |= (1 << 5);		//reciver interrupt enable for RDRF
	UART3_C2 |= (1 << 2);		//RE reciver enable
	UART3_C2 |= (1 << 3);		//TE Transmiter enable
	NVIC_ISER(1) |= (1 << (37%32));
	NVIC_ICPR(1) |= (1 << (37%32));
}

void UART3_Status_IRQHandler(void)
{
	if ((UART3_S1 & 0x20) >> 5 ){
		uart_recive = UART3_D;
		parse_gps_byte(GPS_pt, uart_recive);
	}
}

void UART3_ERR_IRQHandler(){
	
}

void UART_Read()
{
	//READ
	if ((UART0_S1 & 0x20) >> 5 && !(buffer_is_full(rx_bf)))
	{
		uart_recive = UART0_D;
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
