/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

void UART_Init(void);
void UART_Send_Byte(void);

unsigned char rx_data;
unsigned char dato;
unsigned char IC;
unsigned char IC_value;
unsigned char tb;
unsigned char cont;
unsigned char valor_referencia;
unsigned char RxD;
unsigned char rx_count;



void UART_Rec_Init(void)
{
	//configurar input capture
	//timer on

}

void interrupt_INPUT_CAPTURE(void)
{
	//apagar bandera
	if (IC)
	{	//cambiar funcion a output compare
		valor_referencia = IC_value + tb + tb / 2;
		cont = 10;
	}
	else
	{
		if (cont != 2)
		{
			dato>>=1;
			if(RxD != 0){
				dato |= 0x80;
			}
			rx_count--;
		}
		if(rx_count==2){
			if(RxD==0){
				
			}//frame error
			else rx_count--;
		}
		if(rx_count==1){
			//data_ready = 1;
			//config ic flanco de bajada
		}
	}
}

