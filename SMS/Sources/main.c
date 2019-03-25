/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "Buffer.h"

#define NEW_LINE 	0x0A //\n
#define CARR_RETURN 0x0D //\r
#define SUBSTITUTE  0x1A //\r
#define NOPS		1e6

//PTC16 RX
//PTC17 TX

//Button
void Push_Btn_SW2(void);

void UART_Init(void);
void UART_Send_Byte(uint8_t data);
void UART_Send_String(volatile char *array);
void SMS_Send(volatile char *number, volatile char *text);
void SMS_Read_Unread();
void UART_IRS_READ();
void nops(uint32_t x);
void UART_Recive_OK_Line(uint8_t response[]);

bufferType buffer_tx;
bufferType *buffer_tx_pt;
bufferType buffer_rx;
bufferType *buffer_rx_pt;
uint8_t uart_recive;

//Rx
uint8_t carr_return_recived = 0;
uint8_t reciving_line = 0;
uint8_t current_end_line = CARR_RETURN;
uint8_t response[2];

int main(void)
{
	Push_Btn_SW2();
	buffer_tx_pt = &buffer_tx;
	buffer_rx_pt = &buffer_rx;
	buffer_init(buffer_tx_pt, 200);
	buffer_init(buffer_rx_pt, 200);
	UART_Init();
	return 0;
}

void UART_Send_String(volatile char *array)
{
	uint32_t i;
	uint32_t tx_buffer_empty;
	i = 0;
	if (buffer_is_empty(buffer_tx_pt))
	{
		tx_buffer_empty = 1;
	}
	while (array[i] != '\0')
	{
		buffer_push(buffer_tx_pt, array[i]);
		i++;
	}
	if (tx_buffer_empty == 1)
	{
		tx_buffer_empty = 0;
		UART3_C2 |= 0x80; //Turn on TX interrupt
	}
}

void UART_Send_Byte(uint8_t data)
{
	if (buffer_is_empty(buffer_tx_pt))
	{
		buffer_push(buffer_tx_pt, data);
		UART3_C2 |= 0x80; //Turn on TX interrupt
	}
	else
	{
		buffer_push(buffer_tx_pt, data);
	}
}

void UART_Recive_OK_Line(uint8_t response[])
{
	current_end_line = CARR_RETURN;
	buffer_clear(buffer_rx_pt);
	carr_return_recived = 0;
	reciving_line = 1;
	while (carr_return_recived == 0)
	{

	}
	reciving_line = 0;
	response[0] = buffer_rx_pt->data[0];
	response[1] = buffer_rx_pt->data[1];
}

void UART_Recive_SMS_Line(uint8_t response[])
{
	current_end_line = '>';
	buffer_clear(buffer_rx_pt);
	carr_return_recived = 0;
	reciving_line = 1;
	while (carr_return_recived == 0)
	{

	}
	reciving_line = 0;
	response[0] = buffer_rx_pt->data[0];
	response[1] = buffer_rx_pt->data[1];
}

uint8_t OK_Response(uint8_t response[])
{
	if (response[0] == 'o' || response[0] == 'O')
	{
		if (response[1] == 'k' || response[1] == 'K')
		{
			return 1;
		}
	}
	return 0;
}

uint8_t SMS_Input_Response(uint8_t response[])
{
	if (response[0] == '>')
	{
		return 1;
	}
	return 0;
}

void UART_Init(void)
{
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
	UART3_C2 |= 0x80; //Turn on TX interrupt
}

void UART3_Status_IRQHandler(void)
{
	//WRITE
	if ((UART3_S1 & 0x80) >> 7)
	{
		if (!buffer_is_empty(buffer_tx_pt))
		{
			UART3_D = buffer_pop(buffer_tx_pt);
		}
		if (buffer_is_empty(buffer_tx_pt))
		{
			UART3_C2 &= ~(0x80);
		}
	}
	if (reciving_line == 1)
	{
		UART_IRS_READ();
	}
}

void UART_IRS_READ()
{
	//Read
	if ((UART3_S1 & 0x20) >> 5)
	{
		uart_recive = UART3_D;
		if (uart_recive == current_end_line)
		{
			carr_return_recived = 1;
		}
		else
		{
			buffer_push(buffer_rx_pt, uart_recive);
		}

	}
}

void SMS_Send(volatile char *number, volatile char *text)
{
	do
	{
		UART_Send_String("AT\r\n");
		UART_Recive_OK_Line(response);
	} while (!OK_Response(response));
	if (OK_Response(response))
	{
		UART_Send_String("AT+CMGF=1\r\n");
		UART_Recive_OK_Line(response);
		if (OK_Response(response))
		{
			UART_Send_String("AT+CMGS=\"");
			UART_Send_String(number);
			UART_Send_String("\"\r\n");
			UART_Recive_SMS_Line(response);
			UART_Send_String(text);
			UART_Send_Byte(SUBSTITUTE);
		}
	}
}

void SMS_Read_Unread()
{
	UART_Send_String("AT+CMGL=\"REC UNREAD\"\r\n");
}

void nops(uint32_t x)
{
	int i;
	for (i = 0; i < x; i++)
	{
		__asm("nop");
	}
}

void Push_Btn_SW2(void) {
	SIM_SCGC5 |= (1 << 11);
	PORTC_PCR6 |= (1<<8);
	PORTC_PCR6 |= (8<<16);
	NVIC_ICPR(1) |= (1<<(61%32));		//Clean flag of LPTM in the interrupt vector
	NVIC_ISER(1) |= (1<<(61%32)); 		//Activate the LPTM interrupt
}

void PORTC_IRQHandler() {
	PORTC_PCR6 &= ~(0<<24);
	SMS_Send("3324975397", "This is a test message lol");
}
