/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

#define NEW_LINE 	0x0A
#define CARR_RETURN 0x0D
#define BACKSPACE 	0x08

#include "buffer.h"

uint8_t uart_recive;
uint8_t rx_status;

bufferType Buffer_tx;
bufferType *tx_bf;

//Buffers to Bluetooth
bufferType buffer_master;
bufferType *master_device;


//Parsed data
uint16_t device;
uint16_t led;

void uart_init_bluetooth(void);
void uart_init_usb(void);
void send_via_bluetooth();

int main(void)
{
	tx_bf = &Buffer_tx;
	master_device = &buffer_master;
	buffer_init(tx_bf, 100);
	buffer_init(master_device, 100);
	uart_init_usb();
	uart_init_bluetooth();
	while (1)
	{
		if (rx_status == 1)
		{
			rx_status = 0;
			send_via_bluetooth();
		}
	}
	return 0;
}

void send_via_bluetooth()
{
	//Activate uart3 so that it starts sending data from command buffer
	UART3_C2 |= 0x80; //Turn on TX interrupt
}

void uart_init_usb(void)
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

void uart_init_bluetooth(void)
{
	SIM_SCGC4 |= (1 << 13);	//CLK UART3
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; /*Enable the PORTC clock*/
	PORTC_PCR16 |= PORT_PCR_MUX(3);
	PORTC_PCR17 |= PORT_PCR_MUX(3);
	UART3_BDL = 137;		//clock=640*32768, baud rate 9600
	UART3_C2 |= (1 << 5);		//reciver interrupt enable for RDRF
	//UART3_C2 |= (1 << 2);		//RE reciver enable
	UART3_C2 |= (1 << 3);		//TE Transmiter enable
	NVIC_ISER(1) |= (1 << (37%32));
	NVIC_ICPR(1) |= (1 << (37%32));
}

void UART0_Status_IRQHandler(void)
{
	//WRITE
	if (((UART0_S1 & 0x80) >> 7) && (!buffer_isempty(tx_bf)))
	{
		UART0_D = buffer_pop(tx_bf);
		if (buffer_isempty(tx_bf))
		{
			UART0_C2 &= ~(0x80);
		}
	}
	//READ
	if ((UART0_S1 & 0x20) >> 5)
	{
		uart_recive = UART0_D;
		buffer_push(tx_bf, uart_recive);
		buffer_push(master_device, uart_recive);
		if (uart_recive != CARR_RETURN)
		{

		}
		else
		{
			buffer_push(tx_bf, NEW_LINE);
			rx_status = 1;
		}
		UART0_C2 |= 0x80;	//Turn on TX interrupt
	}
}

void UART3_Status_IRQHandler(void)
{
	//WRITE
	if (((UART3_S1 & 0x80) >> 7) && (!buffer_isempty(master_device)))
	{
		UART3_D = buffer_pop(master_device);
		if (buffer_isempty(master_device))
		{
			UART3_C2 &= ~(0x80);
		}
	}
}
