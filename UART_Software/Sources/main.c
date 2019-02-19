/*
 * main.c
 *
 *  Created on: Jan 26, 2019
 *      Author: Miguel Elias
 */
#include "derivative.h" /* include peripheral declarations */
#include "Buffer.h"
#include "Test_Signal.h"

#define IS_INPUT_CAPTURE() ((FTM0_CnSC(0)&(FTM_CnSC_ELSB_MASK))>>FTM_CnSC_ELSB_SHIFT)
#define GET_VALUE() ((GPIOC_PDIR & 0x02U)>>1)
#define US150 0xB00U //Original was CFC
#define US104 0x901U

void FTM_Output_Tx_Config();
void UART_IO_Config();
void FTM0_Input_Capture_Init();
void FTM_Config_Clk();
void UART_Send_Byte(uint8_t data);
void UART_Send_Line(volatile char *word);

uint8_t counter;
uint8_t rx_data;
uint8_t rx_count = 0xFFU;
uint8_t tx_count = 0xFFU;
uint8_t tx_busy = 0;

uint8_t TX_data;
uint8_t TX_finished;
bufferType buffer =
{ 0, 0, BUFLEN,
{ } };
bufferType* buffer_pt;

int main()
{
	buffer_pt = &buffer;
	Test_Signal_Init();
	FTM_Config_Clk();
	UART_IO_Config();
	return 0;
}

void UART_IO_Config()
{
	FTM_Config_Clk();
	FTM_Output_Tx_Config();
	FTM0_Input_Capture_Init();
}

void FTM_Config_Clk() //Configures the clock options for TFM0 and FTM1
{
	SIM_SCGC6 |= (1 << 24);
	SIM_SCGC6 |= (1 << 25);
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
}

void UART_Send_Byte(uint8_t data)
{
	buffer_push(buffer_pt, data);
	if (buffer_has_one(buffer_pt) && tx_busy == 0)
	{
		FTM1_SC &= ~(FTM_SC_CLKS_MASK);
		FTM1_SC |= FTM_SC_TOIE_MASK;
		FTM1_SC |= FTM_SC_CLKS(1);
		tx_busy = 1;
	}
}

void UART_Send_Line(volatile char *array)
{
	uint32_t i = 0;
	while (array[i] != '\0')
	{
		UART_Send_Byte(array[i]);
		i++;
	}
	UART_Send_Byte('\r');
	UART_Send_Byte('\n');
}

void FTM_Output_Tx_Config()
{
	//Configure FTM1 -> Not gonna use output compare pin so PTC5
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTC_PCR5 |= (0x1U << 8);
	GPIOC_PDDR |= (1 << 5); //Set PTC5 as output
	FTM1_SC &= ~(FTM_SC_CLKS_MASK);
	FTM1_MOD = US104;
	FTM1_MODE |= FTM_MODE_FTMEN_MASK | FTM_MODE_WPDIS_MASK;
	FTM1_SYNC = 0x8U;
	FTM1_COMBINE = 0x20202020U;
	NVIC_ICPR(1) |= (1 << (43 % 32)); //Clean flag of LPTM in the interrupt vector
	NVIC_ISER(1) |= (1 << (43 % 32));	//Activate the LPTM interrupt
	GPIOC_PDOR |= (1 << 5);
}

void FTM0_Input_Capture_Init()
{
	//Configure a FTM0 on input capture PTC1
	PORTC_PCR1 = (0x4U << 8);
	FTM0_SC &= ~(FTM_SC_CLKS_MASK);
	FTM0_MODE = FTM_MODE_FTMEN_MASK | FTM_MODE_WPDIS_MASK;
	FTM0_SC &= ~FTM_SC_TOIE_MASK;
	FTM0_MOD = US150;
	FTM0_SYNC = 0x8U;
	FTM0_COMBINE = 0x20202020U;
	FTM0_CnSC(0) = FTM_CnSC_CHIE_MASK | FTM_CnSC_ELSB_MASK;
	NVIC_ICPR(1) |= (1 << (42 % 32)); //Clean flag of LPTM in the interrupt vector
	NVIC_ISER(1) |= (1 << (42 % 32));	//Activate the LPTM interrupt
	FTM0_SC |= FTM_SC_CLKS(1);
}

void FTM0_Input_Capture()
{
	PORTC_PCR1 = (0x4U << 8);
	FTM0_SC &= ~(FTM_SC_CLKS_MASK);
	FTM0_SC &= ~FTM_SC_TOIE_MASK;
	FTM0_MOD = US150;
	FTM0_CnSC(0) = FTM_CnSC_CHIE_MASK | FTM_CnSC_ELSB_MASK;
	FTM0_SC |= FTM_SC_CLKS(1);
	FTM0_CNT = 0;
}

void FTM1_IRQHandler() //Handler for TX
{
	//Clear interrupt
	FTM1_SC &= ~FTM_SC_TOF_MASK;
	FTM1_FMS &= ~FTM_FMS_FAULTF_MASK;
	FTM1_EXTTRIG &= ~FTM_EXTTRIG_TRIGF_MASK;
	if (tx_count > 9)
	{
		//Wait for FTM to stalilize
		tx_count = 0;
		if (!buffer_is_empty(buffer_pt))
		{
			TX_data = buffer_pop(buffer_pt);
		}
	}
	else if (tx_count == 0)
	{
		//Send start bit 0
		GPIOC_PCOR |= (1 << 5); //Set to 0
		tx_count++;
	}
	else if (tx_count < 9)
	{
		if ((TX_data & 0x01U) == 0x01U)
		{
			GPIOC_PDOR |= (1 << 5);
		}
		else
		{
			GPIOC_PDOR = ~(1 << 5);
		}
		TX_data >>= 1;
		tx_count++;
	}
	else if (tx_count == 9)
	{
		tx_count = 0xF;
		TX_finished = 1;
		GPIOC_PDOR |= (1 << 5);
		if (buffer_is_empty(buffer_pt))
		{
			//FTM1_SC &= ~(FTM_SC_CLKS_MASK);
			FTM1_SC &= ~FTM_SC_TOIE_MASK;
			//FTM1_SC |= FTM_SC_CLKS(1);
			tx_busy = 0;
		}
	}
}

void FTM0_IRQHandler() //Handler for RX
{
	//Clear interrupt
	FTM0_SC &= ~FTM_SC_TOF_MASK;
	FTM0_FMS &= ~FTM_FMS_FAULTF_MASK;
	FTM0_EXTTRIG &= ~FTM_EXTTRIG_TRIGF_MASK;
	FTM0_CnSC(0) &= ~FTM_CnSC_CHF_MASK;
	if ((rx_count > 10) | (rx_count == 0))
	{
		FTM0_SC |= FTM_SC_TOIE_MASK;
		FTM0_CnSC(0) = 0x0U;
		PORTC_PCR1 = (1 << 8);
		FTM0_CNT = 0;
		rx_count = 10;
		return;
	}
	else
	{
		if (rx_count == 10)
		{
			FTM0_SC &= ~(FTM_SC_CLKS_MASK);
			FTM0_MOD = US104;
			FTM0_SC |= FTM_SC_CLKS(1);
		}
		if (rx_count > 2)
		{
			rx_data >>= 1;
			Test_Signal_Toggle();
			if (GET_VALUE() != 0)
			{
				rx_data |= 0x80U;
			}
		}
		else if (rx_count == 2)
		{
			if (GET_VALUE() == 0) //Frame Error
			{
				PORTC_PCR1 |= (0x4U << 8);
			}
		}
		else if (rx_count == 1)
		{
			UART_Send_Byte(rx_data);
			rx_data = 0;
			//Go back to input capture mode
			FTM0_Input_Capture();
		}
		rx_count--;
	}

}
