/*
 * main.c
 *
 *  Created on: Jan 26, 2019
 *      Author: Miguel Elias
 */
#include "derivative.h" /* include peripheral declarations */
#include "Buffer.h"

#define IS_INPUT_CAPTURE() ((FTM0_CnSC(0)&(FTM_CnSC_ELSB_MASK))>>FTM_CnSC_ELSB_SHIFT)

void FTM_Output_Tx_Config();
void UART_IO_Config();
void FTM_Input_Capture_Init();
void FTM_Config_Clk();

uint8_t counter;
uint8_t data;
uint8_t data_ready;
uint8_t RX_count;
uint8_t TX_count;
uint8_t TX_data;
uint8_t TX_finished;
bufferType buffer =
{ 0, 0, BUFLEN,
{ } };
bufferType* buffer_pt;

int main()
{
	buffer_pt = &buffer;
	UART_IO_Config();
	return 0;
}

void UART_IO_Config()
{
	FTM_Config_Clk();
	FTM_Input_Capture_Init();
	FTM_Output_Tx_Config();
}

void FTM_Config_Clk() //Configures the clock options for TFM0 and FTM1
{
	SIM_SCGC6 |= (1 << 24);
	SIM_SCGC6 |= (1 << 25);
}

void UART_Send(uint8_t data)
{
	buffer_push(buffer_pt, data);
	//Activate FTM1 timer on putput compare
	FTM1_CnSC(0) = FTM_CnSC_MSA_MASK;
	NVIC_ICPR(1) |= (1<<(43%32));	//Clean flag of LPTM in the interrupt vector
	NVIC_ISER(1) |= (1<<(43%32));	//Activate the LPTM interrupt
	FTM1_SC &= ~(FTM_SC_CLKS_MASK);
	FTM1_SC |= FTM_SC_CLKS(1);
}

void FTM_Output_Tx_Config()
{
	//Configure FTM1 on PTB12 -> Not gonna use output compare pin so PCT5
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB_PCR12 |= (0x3U<<8);
	GPIOC_PDDR |= (1 << 5); //Set PTC5 as output
	FTM1_MODE |= FTM_MODE_FTMEN_MASK | FTM_MODE_WPDIS_MASK;
	FTM1_MOD = 0xFFFF;
}

void FTM_Input_Capture_Init()
{
	//Configure a FTM0 on input capture PTC1
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC_PCR1 |= (0x4U<<8);
	FTM0_MODE |= FTM_MODE_FTMEN_MASK | FTM_MODE_WPDIS_MASK;
	FTM0_MOD = 0xFFFF;
	FTM0_SYNC = 0x8U;
	FTM0_COMBINE |= 0x20202020U;
	FTM0_CnSC(0) |= FTM_CnSC_CHIE_MASK;
	FTM0_CnSC(0) |= FTM_CnSC_ELSB_MASK;
	NVIC_ICPR(1) |= (1<<(42%32));	//Clean flag of LPTM in the interrupt vector
	NVIC_ISER(1) |= (1<<(42%32));	//Activate the LPTM interrupt
	FTM0_SC |= FTM_SC_TOIE_MASK;
	FTM0_SC &= ~(FTM_SC_CLKS_MASK);
	FTM0_SC |= FTM_SC_CLKS(1);
}

void FTM1_IRQHandler() //Handler for TX
{
	if (TX_count++ == 0)
	{
		//Send start bit 0
		TX_data = buffer_pop(buffer_pt);
		GPIOC_PCOR |= (1 << 5);
	}
	else if (TX_count++ <= 8)
	{
		if ((TX_data & 0x01U) == 0x01U)
		{
			GPIOB_PDOR = ~(1 << 5);
		}
		else
		{
			GPIOB_PDOR |= (1 << 5);
		}
		TX_data >> 1;
	}
	else
	{
		TX_count = 0;
		TX_finished = 1;
	}
}

void FTM0_IRQHandler() //Handler for RX
{
	//Clear interrupt
	FTM0_SC &= ~FTM_SC_TOF_MASK;
	FTM0_FMS &= ~FTM_FMS_FAULTF_MASK;
	FTM0_EXTTRIG &= ~FTM_EXTTRIG_TRIGF_MASK;
	if (IS_INPUT_CAPTURE())
	{
		//Confifure output compare
		FTM0_CnSC(0) = 0x14;
		counter = 10;
		return;
	}
	else
	{
		if (RX_count != 2)
		{
			data >> 1;
			if (FTM0_C0V != 0)
			{
				data |= 0x80U;
			}
			RX_count--;
		}
		if (RX_count == 2)
		{
			if (FTM0_C0V == 0) //Frame Error
			{

			}
			else
			{
				RX_count--;
			}
		}
		if (RX_count == 1)
		{
			data_ready = 1;
		}
	}

}
