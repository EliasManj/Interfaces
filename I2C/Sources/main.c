#include "derivative.h" /* include peripheral declarations */

#define setSDA(x) 			GPIOB_PDOR ^= (-((x&0x1U)) ^ GPIOB_PDOR ) & (1 << 2);
#define setSCL(x) 			GPIOB_PDOR ^= (-((x&0x1U)) ^ GPIOB_PDOR ) & (1 << 3);
#define setTest(x)			GPIOB_PDOR ^= (-((x&0x1U)) ^ GPIOB_PDOR ) & (1 << 10);
#define toggleTest()		GPIOB_PTOR |= (1 << 10);
#define readSDA() 			((GPIOB_PDIR & 0x04U)>>2)
#define readSCL() 			((GPIOB_PDIR & 0x06U)>>3)
#define PREESCALE			2

#define T3 1*PREESCALE 				//14 us	
#define T7 1*PREESCALE 				//14 us	
#define T6 1*PREESCALE 				//14 us
#define T2 1*PREESCALE 				//14 us
#define T10 1*PREESCALE				//14 us
#define T11 1*PREESCALE				//14 us
#define CONTROL_BYTE 0xAAU

//RTC
#define DS1307_ADDRESS_W 	0xD0
#define DS1307_ADDRESS_R 	0xD1
#define SECONDS_ADD	 		0x00
#define MINUTES_ADD	 		0x01
#define HOURS_ADD	 		0x02
#define DAYS_ADD	 		0x03
#define DATE_ADD	 		0x04
#define MONTH_ADD	 		0x05
#define YEAR_ADD	 		0x06
#define MISC_ADD	 		0x07

uint8_t vI2C_Acknowledge();
void I2C_Init();
void nops(uint32_t x);
void I2C_Start();
void I2C_Stop();
uint8_t I2C_SendByte(uint8_t data);
uint8_t I2C_ReciveByte();

uint8_t SDA; //PTB2
uint8_t SCL; //PTB3

int main()
{
	uint8_t ACK0;
	uint8_t ACK1;
	uint8_t ACK2;
	uint8_t ACK3;
	uint8_t ACK4;
	uint8_t ACK5;
	uint8_t ACK6;
	uint8_t byte;
	I2C_Init();
	while (1)
	{
		setTest(0);
		nops(500);
		I2C_Start();
		//Write
		ACK0 = I2C_SendByte(DS1307_ADDRESS_W);
		ACK1 = I2C_SendByte(DAYS_ADD);
		ACK2 = I2C_SendByte(0xAA);
		//Read
		ACK3 = I2C_SendByte(DS1307_ADDRESS_W);
		ACK4 = I2C_SendByte(DAYS_ADD);
		ACK5 = I2C_SendByte(DS1307_ADDRESS_R);
		byte = I2C_ReciveByte();
		nops(500);
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

void I2C_Stop()
{
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
//	toggleTest();
	//Wait T6
	nops(T6);
	setSCL(0);
	toggleTest()
	;
}

uint8_t I2C_SendByte(uint8_t data)
{
	//Now start sending addres from MSB first
	uint8_t temp;
	uint8_t count = 7;
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
	//Wait T3
	nops(T3);
	setSCL(1);
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
	//Wait T2
	nops(T2);
	setSCL(0);
	return temp;
}

uint8_t I2C_ReciveByte()
{
	uint8_t count = 8;
	uint8_t data = 0;
	//Wait for start sequence
	GPIOB_PDDR &= ~(1 << 2);
	while (count != 0)
	{
		nops(T6);
		setSCL(1);
		nops(T3);
		if (readSDA() == 1)
		{
			data |= 1;
		}
		toggleTest();
		nops(T2);
		nops(T2);
		setSCL(0);
		data <<= 1;
		count--;
	}
	return data;
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
	setTest(1);
	GPIOB_PDDR |= (1 << 2);
	setSDA(0);
	return temp;
}
