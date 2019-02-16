/*
 * main.c
 *
 *  Created on: Feb 12, 2019
 *      Author: Miguel Elias
 */

#include "derivative.h" /* include peripheral declarations */

#define setSDA(x) 			GPIOB_PDOR ^= (-((x&0x1U)) ^ GPIOB_PDOR ) & (1 << 2);
#define setSCL(x) 			GPIOB_PDOR ^= (-((x&0x1U)) ^ GPIOB_PDOR ) & (1 << 3);
#define setTest(x)			GPIOB_PDOR ^= (-((x&0x1U)) ^ GPIOB_PDOR ) & (1 << 10);
#define toggleTest()		GPIOB_PTOR |= (1 << 10);
#define readSDA() 			((GPIOB_PDIR & 0x04U)>>2)
#define readSCL() 			((GPIOB_PDIR & 0x06U)>>3)
#define PREESCALE			8

#define T3 20*PREESCALE 				//14 us	
#define T7 10*PREESCALE 				//14 us	
#define T6 10*PREESCALE 				//14 us
#define T2 20*PREESCALE 				//14 us
#define T10 20*PREESCALE				//14 us
#define T11 20*PREESCALE				//14 us
#define CONTROL_BYTE 0xAAU

uint8_t vI2C_Acknowledge();
void I2C_Init();
void nops(uint32_t x);
void I2C_Start();
void I2C_Stop();
uint8_t I2C_SendByte(uint8_t data);

uint8_t SDA; //PTB2
uint8_t SCL; //PTB3
uint8_t ACK; 


int main()
{
	I2C_Init();
	while(1){
		nops(500);
		I2C_Start();
		ACK = I2C_SendByte(CONTROL_BYTE);
		I2C_SendByte(0xF0);
		I2C_SendByte(0xCA);
		I2C_Stop();
	}
	return 0;
}

void I2C_Init()
{
	//Definir PTB2 como SDA y PTB3 como SCL
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB_PCR2 |= (0x1U << 8);
	PORTB_PCR3 |= (0x1U << 8);
	PORTB_PCR10 |= (0x1U << 8);
	//Poner primero como output SDA
	GPIOB_PDDR |= (1 << 2);
	GPIOB_PDDR |= (1 << 3);
	GPIOB_PDDR |= (1 << 10);
	setSDA(1);
	setSCL(1);
	setTest(1);
	nops(1000);
}

void nops(uint32_t x)
{
	int i;
	for (i = 0; i < x; i++)
	{
		__asm("nop");
	}
}

void I2C_Stop(){
	setSCL(1);
	nops(T10);
	setSDA(1);
}

void I2C_Start()
{
	//Initial values
	setSDA(1);
	setSCL(1);
	//Wait T7 start
	nops(T7);
	setSDA(0);
	toggleTest()
	;
	//Wait T6
	nops(T6);
	setSCL(0);
	toggleTest()
	;
}

uint8_t I2C_SendByte(uint8_t data)
{
	//Now start sending addres from MSB first
	uint8_t count = 8;
	do
	{
		if (data & 0x80)
		{
			setSDA(1);
		}
		else
		{
			setSDA(0);
		}
		//Wait T3
		nops(T3);
		setSCL(1);
		//Wait T2
		nops(T2);
		setSCL(0);
		//Wait T11
		nops(T11);
		data <<= 1;
	} while (count-- != 0);
	return vI2C_Acknowledge();
}

uint8_t I2C_ReciveByte(){
	uint8_t count = 8;
	uint8_t data = 0;
	//Wait for start sequence
	while(readSDA()==1);
	do
		{
			nops(T6);
			nops(T3);	
			while(readSCL()==0);
			if (readSDA()==1)
			{
				data |= 1;
			}
			//Wait T2
			nops(T2);
			setSCL(0);
			while(readSCL()==1);
			data <<= 1;
		} while (count-- != 0);
	return data;
}

uint8_t vI2C_ReciveByte_RTC(void)
{
	uint8_t count = 8;
	uint8_t temp;

	//Leemos cuando el reloj este en 1
	//Poner gpio de sda en input
	do
	{
		SCL = 1;
		//esperar nops
		temp <<= 1;
		if (SDA == 1)
		{
			temp++;
		}
	} while (--count);
	//SDA output

	return temp;
}

uint8_t vI2C_Acknowledge()
{
	uint8_t temp;
	//Change SDA to input and read the ack flag
	GPIOB_PDDR &= ~(1 << 2);
	if (readSDA() == 1)
	{
		temp = 1;
	}
	else
	{
		temp = 0;
	}
	GPIOB_PDDR |= (1 << 2);
	return temp;
}
