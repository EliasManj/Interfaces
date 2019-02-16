/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

int SDA, SCL;
char dato;

void I2C_ByteWrite(unsigned short direccion, unsigned char dato);
void vI2C_Start(void);
unsigned char vI2C_AckInput(void);

void vI2C_Init(void)
{
	//definir pin para SCL, salida, 0
	//definir pin para SAD, salida, 0

}

void vI2C_Start(void)
{
	SDA = 0;
	SCL = 1;
	//Esperar t7 -- unos "nops"
	SDA = 1;
	//Esperar t6
	SCL = 0;
	SDA = 0;
}

void vI2C_SendByte(char dato)
{
	unsigned char cont = 8;
	do
	{	//Sale el mas significativo
		if (dato & 0x80)
		{
			SDA = 1;
		}
		else
		{
			SDA = 0;
		}
		SCL = 1;
		//esperar
		SCL = 0;
		//esperar
		dato <<= 1;
	} while (cont--);
	SDA = 0;
}

void I2C_ByteWrite(unsigned short direccion, unsigned char dato)
{
	vI2C_Start();
	vI2C_SendByte(0xA0);
	if (vI2C_AckInput() == 0)
	{
		vI2C_SendByte(direccion << 8);
		if (vI2C_AckInput() == 0)
		{
			vI2C_SendByte(direccion);
			if (vI2C_AckInput() == 0)
			{
				vI2C_SendByte(dato);
				if (vI2C_AckInput() == 0)
				{
					vI2C_Stop();
				}
			}
		}
	}
}

unsigned char vI2C_AckInput(void)
{
	unsigned char temp;
	//sda entrada
	//Scl = 1
	if (SDA == 1)
		temp = 1;
	else
		temp = 0;
	SCL = 0;
	//sda salida
	return temp;
}

unsigned char vI2C_ReciveByte(void)
{
	unsigned char count = 8;
	unsigned char temp;

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

void vI2_ACK_OUT(unsigned char ack_bit)
{
	//SDA output
	if (ack_bit == 1)
	{
		SDA = 1;
	}
	else
	{
		SDA = 0;
	}
	SCL = 1;
	//Nops
	SCL = 0;
	SDA = 0;
}
