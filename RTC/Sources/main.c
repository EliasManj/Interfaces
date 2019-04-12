#include "derivative.h" /* include peripheral declarations */

unsigned char temp[100];
unsigned char time[7];

//unsigned char acc_data[6]={(0xD0),(0x6B),(0x00),(0xD0),(0x3B),(0xD1)};//IMU externa
//unsigned char acc_data[8]={(0x1D<<1),(0x01),((0x1D<<1)+1),(0x1D<<1),(0x5B),(0x03),(0x20),(0x00)};//IMU interna
//unsigned char acc_dataw[8]={(0xD0),(0x01),(0x08),(0x03),(0x07),(0x0E),(0x04),(0x12)};//IMU interna

unsigned char acc_dataw[9] =
{ (0xD0), (0x00), (0x00), (0x15), (0x06), (0x03), (0x18), (0x04), (0x18) };
unsigned char acc_datar[3] =
{ (0xD0), (0x00), (0xD1) };

unsigned char i_rx = 0;
unsigned char i_tx = 0;
unsigned char read_flag = 0;
unsigned char next_d_flag = 1;
unsigned char flag_new = 1;

void I2C0_IRQHandler(void)
{
	/*Limpiar banderas de IRQ 24(I2C0)*/
	NVICICPR0 = 1<<(24%32);
	I2C0_S=I2C_S_IICIF_MASK;

	next_d_flag=1;

	if(read_flag == 1)
	{
		temp[i_rx++]=I2C0_D;
		if(i_rx>8)
		{
			I2C0_C1=(0b1110<<4)+I2C_C1_TXAK_MASK;
			read_flag = 0;
			data_pros();
			flag_new = 1;

		}

	}

}

void I2C_Init(void)
{
	SIM_SCGC4 = (1 << 6); //prender sim I2C0
	SIM_SCGC5 = (1 << 13); //prender sim PORTE
	PORTE_PCR24 = (0b101<<8); //I2C0_SCL alt5
	PORTE_PCR25 = (0b101<<8); //I2C0_SDA alt5
	I2C0_F = (0x1E); //CLk rate, el que disque jala con la imu
	I2C0_C1 = (0b1111 << 4); //Module En, Interrupt En, Master mode, transmit
	/*Limpiar banderas de IRQ 24(I2C0)*/NVICICPR0 = 1<<(24%32);
	/*Habilitar interrupciones de IRQ 24(I2C0)*/
	NVICISER0 = 1<<(24%32);

}

void I2C_send1(unsigned char size, unsigned char data[])
{
	unsigned char i;
	i_tx = 0;
	I2C0_C1 = (0b1111 << 4); //Module En, Interrupt En, Master mode, transmit
	while (i_tx < size)
	{
		if (I2C0_S & (1 << 7) && next_d_flag == 1)
		{
			next_d_flag = 0;
			i++;
			I2C0_D = data[i_tx++];
		}
	}

}

void I2C_send2(unsigned char size, unsigned char data[])
{
	unsigned char i;
	i_tx = 0;
	I2C0_C1 = (0b1111 << 4); //Module En, Interrupt En, Master mode, transmit
	while (i_tx < size)
	{
		if (I2C0_S & (1 << 7) && next_d_flag == 1)
		{
			next_d_flag = 0;
			i++;

			if (i_tx == 2)
			{
				I2C0_C1 = (0b1111 << 4) + (0b1 << 2);
			}

			I2C0_D = data[i_tx++];

		}
	}
}

void I2C_read()
{
	for (;;)
	{
		if (I2C0_S & (1 << 2))
		{
			read_flag = 1;
			I2C0_C1 = (0b1110 << 4);
			break;
		}
	}
}
void data_pros()
{
	time[0] = ((temp[1] >> 4) & 0b00000111) * 10 + (0b00001111 & temp[1]);
	time[1] = ((temp[2] >> 4) & 0b00000111) * 10 + (0b00001111 & temp[2]);
	time[2] = 0b00001111 & temp[3];
	time[3] = 0b00000111 & temp[4];
	time[4] = ((temp[5] >> 4) & 0b00000011) * 10 + (0b00001111 & temp[5]);
	time[5] = 0b00000111 & temp[6];
	time[6] = ((temp[7] >> 4) & 0b00001111) * 10 + (0b00001111 & temp[7]);
}

int main(void)

{

	I2C_Init();
	//I2C_send1(9, acc_dataw);
	//I2C_send2(3, acc_datar);
	//I2C_read();

	for (;;)
	{

		if (flag_new == 1)
		{

			i_rx = 0;
			i_tx = 0;
			read_flag = 0;
			next_d_flag = 1;
			flag_new = 0;
			I2C_send2(3, acc_datar);
			I2C_read();

		}

	}

	return 0;
}
