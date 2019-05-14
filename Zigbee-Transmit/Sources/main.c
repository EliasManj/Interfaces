/*************
 **** CODE FOR PAN *****
 *************/

#include "derivative.h" /* include peripheral declarations */
#define setCS(x) 			GPIOD_PDOR ^= (-((x&0x1U)) ^ GPIOD_PDOR ) & (1<<0);
#define CONFIG_SIZE 18
//#define TRANSFER_SIZE

void SPI_config(void);
void SPI_send(int dato);
void SPI_send16(int dato);
void SPI_send24(int dato);
void ZigBee_send(char dato, char address);
void nops(uint32_t x);

void SPI_rx(void);
/*
uint8_t masterTxData[TRANSFER_SIZE] = {0x10,
		0x03,0xC0,0x30,0x01,0xC0,0x50,0x80,0xC0,0XD0,0x90,0xC0,0XF0,0x80,0xC1,0x10,0x10, //24
		0xC4,0x10,0x21,0x75,0x80,((0x3f<<1)+1),0x60,((0x3E<<1)+1),0x40,((0x3E<<1)+1),((0x3E<<1)+1),0x32, //36
		0xC0,0x10,0x03,0xC0,0x70,0x00,((0x36<<1)+1),0x04,((0x36<<1)+1),0x00}; //46

uint8_t masterTxDataR[TRANSFER_SIZE] = {(0x2A<<1),(0x18<<1),(0x2E<<1),0xC0,0x00,0xC0,0x20,0xC0,0x40,0xC0,0xC0
		,0xC0,0xE0,0xC1,0x00};
 */
int setup_data [CONFIG_SIZE] = {(((0x2A<<1)+1)<<8)+0x07,(((0x18<<1)+1)<<8)+0x98,(((0x2E<<1)+1)<<8)+0x95
};

int data_r [2] = {(((0x2A<<1))<<8)+0x07,(((0x18<<1))<<8)+0x98};



uint8_t rx_data[16]; 
uint8_t rx_count = 0;

void SPI_config(void){

	//Clock settings
	SIM_SCGC5 |= (1<<12); // Port D Clock Enabled
	SIM_SCGC6 |= (1<<12); // SPI Clock Enabled

	//Port Settings
	PORTD_PCR0 |= (1<<8); // ChipSelect
	PORTD_PCR1 |= (1<<9); // ALT2 SPI_CLK
	PORTD_PCR2 |= (1<<9); // ALT2 SPI_SOutput
	PORTD_PCR3 |= (1<<9); // ALT2 SPI_SInput

	GPIOD_PDDR |= 1; // set SDA as output
	setCS(1);

	SPI0_MCR =0;
	// Master mode | active low CS | Receive enable | CSPort = 1 inactive | TX 'dis' | flush RX FIFO
	SPI0_MCR |= (1<<31)+(1<<25)+(1<<24)+(1<<16)+(1<<13)+(1<<10); 

	// Clock and prescaler config
	SPI0_CTAR0 = 0;
	SPI0_CTAR0 |= SPI_CTAR_FMSZ(7)| SPI_CTAR_PDT(3) | SPI_CTAR_DT(1) | SPI_CTAR_BR(1) | (1<<12) | (1<<3) | 1; // 16 bits data / delay 

}

void nops(uint32_t x)
{
	int i;
	for (i = 0; i < x; i++)
	{
		__asm("nop");
	}
}

void SPI_send(int dato){
	setCS(0);
	SPI0_MCR &= ~(SPI_MCR_HALT_MASK | SPI_MCR_FRZ_MASK); // allow transfers | Do not halt on transfer
	SPI0_SR &= ~SPI_SR_EOQF_MASK; // se declara que el ultimo bit de TX no ha sido enviado 
	SPI0_PUSHR |= dato;
	SPI0_SR |= SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK; // clear flags 
	SPI0_MCR |= (1<<11); // clear TXFIFO 
}

void SPI_send16(int dato){

	SPI_send(dato>>8);
	nops(660);
	SPI_send(dato);
	nops(660);
	setCS(1);
}

void SPI_send24(int dato){

	SPI_send(dato>>16);
	nops(650);
	SPI_send(dato>>8);
	nops(660);
	SPI_send(dato);
	nops(660);
	setCS(1);
}

void SPI_rx(void){
	char bit;
	//bit = SPI0_SR && (1<<17);
	bit = SPI0_SR && (0x0F<<7);

	while(bit){
		rx_data[rx_count++] |= SPI0_POPR; // put data on array 
		SPI0_SR |= ~(1<<17);
		SPI0_MCR |= (1<<10);
		bit = 0;
	}
}

void ZigBee_send(char dato, char address){  // PAGINA 112 para info

	SPI_send24(((((address)<<1)+1)<<12)+ 0x01 +(1<<23)); // Header Length
	SPI_send24(((((address+1)<<1)+1)<<12)+ 0x01 +(1<<23)); // data payload
	SPI_send24(((((address+2)<<1)+1)<<12)+ 0xA5 +(1<<23)); // Header 
	SPI_send24(((((address+3)<<1)+1)<<12)+ dato +(1<<23)); // Data

	SPI_send16((((0x1B<<1)+1)<<8)+0x01); // Write 1 in TXTRIG to start transmission 

	SPI_send16((((0x31<<1))<<8)+0x01); // INTSTAT = 0x01 intento de transmicion  | interrupts are clear by reading the regiseter
	// RX CODE
	SPI_send16((((0x24<<1))<<8)+0x00); // TXNSTAT = 0x00 TX succed
	// RX CODE
}

int main(void)
{
	SPI_config();

	SPI_send16((((0x2A<<1)+1)<<8)+0x07);
	SPI_send16((((0x18<<1)+1)<<8)+0x98);
	SPI_send16((((0x2E<<1)+1)<<8)+0x95);

	SPI_send24((((0x200<<1)+1)<<12)+ 0x03 +(1<<23));
	SPI_send24((((0x201<<1)+1)<<12)+ 0x01 +(1<<23));
	SPI_send24((((0x202<<1)+1)<<12)+ 0x80 +(1<<23));
	SPI_send24((((0x206<<1)+1)<<12)+ 0x90 +(1<<23));
	SPI_send24((((0x207<<1)+1)<<12)+ 0x80 +(1<<23));
	SPI_send24((((0x208<<1)+1)<<12)+ 0x10 +(1<<23));
	SPI_send24((((0x220<<1)+1)<<12)+ 0x21 +(1<<23));

	/*** PAN Coordinator  Non Beacon  ***/
	SPI_send16((((0x00<<1)+1)<<8)+0x09); // PAN Coordinator | promiscous mode
	SPI_send16((((0x11<<1)+1)<<8)+0x0C); // Unslotted CSMA-CA | Collision avoidance disabled | 5 CSMA-CA attempts to access channel
	SPI_send16((((0x10<<1)+1)<<8)+0xFF); // No Superframe or Beacon transmit by Coordinator

	// 
	SPI_send16((((0x3A<<1)+1)<<8)+0x80); // 
	SPI_send16((((0x60<<1)+1)<<8)+0x60); // Recommended THold
	SPI_send16((((0x3E<<1)+1)<<8)+0x40); // 
	SPI_send16((((0x32<<1)+1)<<8)+0xF6); // Interrupt on TX and RX FIFO on last node F7

	SPI_send24((((0x200<<1)+1)<<12)+ 0x03 +(1<<23)); // Channel 11
	SPI_send24((((0x203<<1)+1)<<12)+ 0x00 +(1<<23)); // Transmission power 

	//////////////////////////////LOOKOUT FOR DEBUGGING////////////////////////////
	SPI_send16((((0x36<<1)+1)<<8)+0x04); //Hold RF State machine on Hold | Reset start 
	SPI_send16((((0x36<<1)+1)<<8)+0x00); //Reset RF State machine | Reset finish

	for(;;) {
		
		ZigBee_send(0x0A,0x00);
	}
	return 0;
}
