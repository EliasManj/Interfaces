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

#define NOPS		1e3
#define NEW_LINE 	0x0A
#define CARR_RETURN '\r'
#define BACKSPACE 	0x08
#define OVER_SAMPLE 16
#define CORE_CLOCK          48000000    // Core clock speed
uint8_t rx_status;
uint8_t request_status;
uint8_t state;
uint8_t uart_recive;

bufferType Buffer_rx;
bufferType *rx_bf;
bufferType Buffer_tx;
bufferType *tx_bf;
bufferType Buffer_tx_console;
bufferType *tx_bf_console;

Request request;
Request *request_pt;

//OKay


void uart_init_pta(void);
void uart_init_pte(void);
void uart_bluetooth(void);
void Wifi_Init();
void Get_IP(bufferType *bf);
void UART3_Init(void);
void uart_init_usb(void);
void WaitOK();
void CheckOkay();

int main(void) {

	rx_bf = &Buffer_rx;
	tx_bf = &Buffer_tx;
	tx_bf_console = &Buffer_tx_console;
	request_pt = &request;
	buffer_init(rx_bf, 50);
	buffer_init(tx_bf_console, 50);
	buffer_init(tx_bf, 50);
	RGB_init();
	uart_init_usb();
	UART3_Init();
	Wifi_Init(tx_bf);
	Get_IP(tx_bf);
	while (1) {
		if (request_status == 1) {
			request_status = 0;
			switch (request_pt->led_state) {
			case (NONE):
				RGB(0, 0, 0);
				break;
			case (RED):
				RGB(1, 0, 0);
				break;
			case (GREEN):
				RGB(0, 1, 0);
				break;
			case (BLUE):
				RGB(0, 0, 1);
				break;
			}
		} else if (request_status != 0) {
			buffer_push(tx_bf_console, request_status);
			UART0_C2 |= 0x80;	//Turn on TX interrupt
		}
	}
	return 0;
}

void uart_bluetooth(void) {
	SIM_SCGC4 |= (1 << 11);	//CLK UART1
	SIM_SCGC5 |= (1 << 13);	//CLOCK for PORTE
	MCG_C1 |= (1 << 6);
	PORTE_PCR1 = (3<<8);	//Port control for UART_0
	PORTE_PCR0 = (3<<8);	//Port control for UART_0
	// Set the baud rate divisor
	//UART1_BDH = 0x0D;		//clock=640*32768, baud rate 9600
	UART1_BDL = 0x0D;		//clock=640*32768, baud rate 9600
	UART1_C2 |= (1 << 5);		//reciver interrupt enable for RDRF
	UART1_C2 |= (1 << 2);		//RE reciver enable
	UART1_C2 |= (1 << 3);		//TE Transmiter enable
	NVIC_ISER(0) |= (1 << 13);
	NVIC_ICPR(0) |= (1 << 13);
}

void UART0_Status_IRQHandler(void) {
	//WRITE
	if (((UART0_S1 & 0x80) >> 7) && (!buffer_isempty(tx_bf_console))) {
		UART0_D = buffer_pop(tx_bf_console);
		if (buffer_isempty(tx_bf_console)) {
			UART0_C2 &= ~(0x80);
		}
	}
	/*
	 //READ
	 if ((UART0_S1 & 0x20) >> 5 && !(buffer_isfull(tx_bf_console))) {
	 uart_recive = UART0_D;
	 buffer_push(tx_bf_console, uart_recive);
	 if (uart_recive != CARR_RETURN) {

	 } else {
	 rx_status = 1;
	 }
	 UART0_C2 |= 0x80;	//Turn on TX interrupt
	 }*/
}

void UART3_ERR_IRQHandler() {

}

void UART3_Status_IRQHandler(void) {
	//WRITE
	if (((UART3_S1 & 0x80) >> 7) && (!buffer_isempty(tx_bf))) {
		UART3_D = buffer_pop(tx_bf);
		if (buffer_isempty(tx_bf)) {
			UART3_C2 &= ~(0x80);
		}
	}
	//READ
	if (request_pt->waiting_for_okay || request_pt->waiting_for_request) {
		if ((UART3_S1 & 0x20) >> 5 && !(buffer_isfull(rx_bf))) {
			uart_recive = UART3_D;
			buffer_push(rx_bf, uart_recive);
			request_status = parse_request_byte(request_pt, uart_recive);
			if (uart_recive != CARR_RETURN) {

			} else {
				buffer_push(rx_bf, NEW_LINE);
				rx_status = 1;
			}
			UART3_C2 |= 0x80;	//Turn on TX interrupt
		}
	}
}

void Wifi_Init(bufferType *bf) {
	buffer_push(bf, 'A');
	buffer_push(bf, 'T');
	buffer_push(bf, '+');
	buffer_push(bf, 'C');
	buffer_push(bf, 'I');
	buffer_push(bf, 'P');
	buffer_push(bf, 'S');
	buffer_push(bf, 'E');
	buffer_push(bf, 'R');
	buffer_push(bf, 'V');
	buffer_push(bf, 'E');
	buffer_push(bf, 'R');
	buffer_push(bf, '=');
	buffer_push(bf, '1');
	buffer_push(bf, ',');
	buffer_push(bf, '8');
	buffer_push(bf, '0');
	buffer_push(bf, '\r');
	buffer_push(bf, '\n');
	WaitOK();
}

void Get_IP(bufferType *bf) {
	buffer_push(bf, 'A');
	buffer_push(bf, 'T');
	buffer_push(bf, '+');
	buffer_push(bf, 'C');
	buffer_push(bf, 'I');
	buffer_push(bf, 'F');
	buffer_push(bf, 'S');
	buffer_push(bf, 'R');
	buffer_push(bf, '\r');
	buffer_push(bf, '\n');
	WaitOK();
}

void UART3_Init(void) {
	SIM_SCGC4 |= (1 << 13);	//CLK UART3
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; /*Enable the PORTC clock*/
	PORTC_PCR16 |= PORT_PCR_MUX(3);
	PORTC_PCR17 |= PORT_PCR_MUX(3);
	UART3_BDL = 137;		//clock=640*32768, baud rate 9600
	UART3_C2 |= (1 << 5);		//reciver interrupt enable for RDRF
	UART3_C2 |= (1 << 2);		//RE reciver enable
	UART3_C2 |= (1 << 3);		//TE Transmiter enable
	NVIC_ISER(1) |= (1 << (37%32));
	NVIC_ICPR(1) |= (1 << (37%32));
}

void uart_init_usb(void) {
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

void nops10(uint32_t x) {
	int i;
	int j;
	for (j = 0; j < 10; j++) {
		for (i = 0; i < x; i++) {
			__asm("nop");
		}
	}
}

void Enable_UART3() {

}
