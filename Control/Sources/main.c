#include "derivative.h" /* include peripheral declarations */

#define l .0096
#define K1 -5.2408
#define K2 .0096
#define K3 -.0161
#define r 5.3
#define La .00058
#define Ka .02
#define L 9107

void counter_setup(void);
void PWM1_Setup(void);
void PWM2_Setup(void);

signed long long count_dir, cnt, pulses, count_dirV = 0;
unsigned char flag, move = 0;

float x1, x2, x3, x_1, x_2, x_3, vref = 0;

void FTM0_IRQHandler(void)
{  // TX OutputCompare Clock
	(void) FTM0_SC;

	if (flag)
	{					// Accion de motor
		FTM0_C1V |= 0x000007FF; // 99% DCycle
	}
	else
	{
		FTM0_C1V = 0x00000001;  // 0% DCycle
	}

	if (move && (flag == 0))
	{

		FTM0_C2V |= 0x000007FF; // 99% DCycle
		if(count_dir==-20)
		{
			move = 0;
			FTM0_C2V = 0x00000001;  // 0% DCycle
			count_dir=0;
		}
	}
}

void PORTC_IRQHandler(void)
{

	if (PORTC_PCR16 &(1<<24))
	{	// Accion de motor
		move = 1;
		flag = ~flag;
		flag&= 0x01;
	}

	if(PORTC_PCR1&(1<<24))
	{		// counter de decoder
		if(move && flag)
		{
			count_dir++;
			//count_dir%=1600;
		}
		else
		{
			count_dir--;
		}

	}

	PORTC_PCR1 |= (1 << 24);	// ENCODER A
	PORTC_PCR8 |= (1 << 24);// ENCONDER B
	PORTC_PCR16 |= (1 << 24);// OF OFF INT
}

void counter_setup(void)
{
	SIM_SCGC5 |= (1 << 11);  // port C clock enabled
	PORTC_PCR1 |= (1<<19)+(1<<17)+(1<<8); // Interrupt on falling edge  & GPIO
	GPIOC_PDDR &= ~(1 << 1);   //Change SDA to input and read the ack flag

	SIM_SCGC5 |= (1 << 11);  // port C clock enabled
	PORTC_PCR8 |= (1<<19)+(1<<17)+(1<<8); // Interrupt on falling edge  & GPIO
	GPIOC_PDDR &= ~(1 << 8);   //Change SDA to input and read the ack flag

	SIM_SCGC5 |= (1 << 11);  // port C clock enabled
	PORTC_PCR16 |= (1<<19)+(1<<17)+(1<<16)+(1<<8); // Interrupt on falling edge  & GPIO
	GPIOC_PDDR &= ~(1 << 16);   //Change SDA to input and read the ack flag

	NVIC_ICPR(1) = (1 << (61%32));  // interrupcion 
	NVIC_ISER(1) = (1 << (61%32));
}

void PWM1_Setup(void)
{

	SIM_SCGC6 |= (1 << 24);  // Enable FTM0 Clock
	SIM_SCGC5 |= (1 << 11);  // port C clock enabled

	GPIOC_PDDR |= (1 << 3); // set PTC_3 as output
	PORTC_PCR3 |= (1<<10); // ALT4

	FTM0_SC |= 0x0000002C; //SysClock (inicia conteo) - 1 PS
	FTM0_C2SC |= 0x00000048; //hab interrupciones - PWM mode 

	FTM0_MOD |= 0x00000800;
	FTM0_C2V |= 0x00000001; // 195 contador

	NVIC_ICPR(1) = (1 << (42%32));  // interrupcion FTM0_C0
	NVIC_ISER(1) = (1 << (42%32));

}

void PWM2_Setup(void)
{ // Izq

	SIM_SCGC6 |= (1 << 24);  // Enable FTM0 Clock
	SIM_SCGC5 |= (1 << 11);  // port C clock enabled

	GPIOC_PDDR |= (1 << 2); // set PTC_3 as output
	PORTC_PCR2 |= (1<<10); // ALT4

	FTM0_SC |= 0x0000002C; //SysClock (inicia conteo) - 1 PS
	FTM0_C1SC |= 0x00000048; //hab interrupciones - PWM mode 

	FTM0_MOD |= 0x00000800;
	FTM0_C1V |= 0x00000001; // 195 contador

	NVIC_ICPR(1) = (1 << (42%32));  // interrupcion FTM0_C0
	NVIC_ISER(1) = (1 << (42%32));

}

/*
 int control_eq(int theta){
 
 
 x2 = count_dir*.225;
 
 u = l*vref - k1*x_1 - k2*x_2 - k3*x_3;
 y = x_2;
 x_1 = r/La*x_1 + Ka/L*x_3 + 1/La*u + L*x2-L*x_2;
 
 x_2 = x2;
 
 return x2;
 }
 */

int main(void)
{
	counter_setup();
	PWM1_Setup();
	PWM2_Setup();

	for (;;)
	{
	}
	return 0;
}
