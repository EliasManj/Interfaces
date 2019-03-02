#include "derivative.h" 

void LPTM_init(void);
void RGB_init(void);
void shiftLEDs(void);
void ADC_init(void);
void Ports_init(void);
void shift(void);
void Ports_init(void);

uint8_t LEDs[3];
uint8_t timerStateReached;
uint8_t i;
uint8_t count = 0;

//PTB2 PTB3 PTB10 PTB11

#define step_1000  ((1<<2))	
#define step_1100  ((1<<2)|(1<<3))
#define step_0100  ((1<<3))
#define step_0110  ((1<<3)|(1<<10))
#define step_0010  ((1<<10))
#define step_0011  ((1<<10)|(1<<11))
#define step_0001  ((1<<11))
#define step_1001  ((1<<2)|(1<<11))

#define US104 0x3FFFU
#define PREESCALE 1

uint16_t secuencia[8] =
{ step_1000, step_1100, step_0100, step_0110, step_0010, step_0011, step_0001,
		step_1001 };

uint16_t secuencia_r[8] =
{ step_1001, step_0001, step_0011, step_0010, step_0110, step_0100, step_1100,
		step_1000 };
uint8_t dir = 0;

int main(void)
{
	LEDs[0] = 0;
	LEDs[1] = 1;
	LEDs[2] = 1;
	timerStateReached = 0;
	i = 0;
	RGB_init();
	Push_Btn_SW2();
	Ports_init();
	LPTM_init();
	return 0;
}

void Ports_init(void)
{
	SIM_SCGC5 |= (1 << 10);
	PORTB_PCR2 =(1<<8); 		//PTB2 GPIO
	PORTB_PCR3 =(1<<8); 		//PTB3 GPIO
	PORTB_PCR10 =(1<<8); 		//PTB10 GPIO
	PORTB_PCR11 =(1<<8); 		//PTB11 GPIO
	GPIOB_PDDR |= (1 << 10) | (1 << 11) | (1 << 2) | (1 << 3); //Set GPIOB as output
}

void Push_Btn_SW2(void)
{
	SIM_SCGC5 |= (1 << 11);
	PORTC_PCR6 |= (1<<8);
	PORTC_PCR6 |= (8<<16);
	NVIC_ICPR(1) |= (1<<(61%32));		//Clean flag of LPTM in the interrupt vector
	NVIC_ISER(1) |= (1<<(61%32)); 		//Activate the LPTM interrupt
}

void LPTM_init(void)
{
	SIM_SCGC6 |= (1 << 24);
	SIM_SCGC6 |= (1 << 25);
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
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
	FTM1_SC &= ~(FTM_SC_CLKS_MASK);
	FTM1_SC |= FTM_SC_TOIE_MASK;
	FTM1_SC |= FTM_SC_CLKS(1);
}

void RGB_init(void)
{
	//Activate SIM_SCGC5 (system control gating register 5) for port B and D
	SIM_SCGC5 |= (1 << 10);		//Port B
	SIM_SCGC5 |= (1 << 13);		//Port D
	PORTB_PCR21 = (1<<8);		//Set PTB21 as GPIO
	PORTB_PCR22 = (1<<8);		//set PTB22 as GPIO
	PORTE_PCR26 = (1<<8);		//Set PTE26 as GPIO
	GPIOB_PDDR = (1 << 21);		//Set PTB21 as output
	GPIOB_PDDR |= (1 << 22);	//set PTB22 as output
	GPIOE_PDDR = (1 << 26);		//Set PTE26 as output
}

void FTM1_IRQHandler()
{
	FTM1_SC &= ~FTM_SC_TOF_MASK;
	FTM1_FMS &= ~FTM_FMS_FAULTF_MASK;
	FTM1_EXTTRIG &= ~FTM_EXTTRIG_TRIGF_MASK;
	timerStateReached = 1;
	if (count++ == PREESCALE)
	{
		count = 0;
		shift();
		shiftLEDs();
	}
}

void PORTC_IRQHandler()
{
	PORTC_PCR6 &= ~(0<<24);
	if(dir == 0)
	{
		dir = 1;
	}
	else
	{
		dir = 0;
	}
}

void shift(void)
{
	if (dir == 0)
	{
		GPIOB_PDOR = ((secuencia_r[i++]) & 0x00000FFF);
	}
	else
	{
		GPIOB_PDOR = ((secuencia[i++]) & 0x00000FFF);
	}
	if (i == 8)
	{
		i = 0;
	}
}

void shiftLEDs(void)
{
	uint8_t tmp0;
	GPIOB_PDOR |= (LEDs[0] << 21);
	GPIOB_PDOR |= (LEDs[1] << 22);
	GPIOE_PDOR = (LEDs[2] << 26);
	tmp0 = LEDs[0];
	LEDs[0] = LEDs[1];
	LEDs[1] = LEDs[2];
	LEDs[2] = tmp0;
}

