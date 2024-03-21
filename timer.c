#include "MKL46Z4.h"
#include "timer.h"

//	SYSTICK
uint32_t SysTick_Configuration (uint32_t ticks){
	SysTick->LOAD  = ticks -1;                                  /* set reload register */
	NVIC_SetPriority(SysTick_IRQn, (1<<4) -1);  /* set Priority for SystickInterrupt */
	SysTick->VAL   = 0;                                          /* Load theSysTickCounter Value */
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk|
	SysTick_CTRL_TICKINT_Msk |
	SysTick_CTRL_ENABLE_Msk;                 /* Enable SysTickIRQ and SysTickTimer */
	return (0);                                                  /* Function successful */
}
int32_t volatile msTicks = 0; // Interval counter in ms
int32_t volatile msTicksInDelay = 0;
void init_SysTick_interrupt(void){
	SysTick->LOAD = SystemCoreClock/ 1000; //configured the SysTickto count in 1ms
	/* Select Core Clock & Enable SysTick& Enable Interrupt */
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;
}
unsigned long long tinhthoigian = 0;	//	Increasing second
long long SysTick_Handler(void) { // SysTickinterrupt Handler
	msTicks++; // Increment counter 
	msTicksInDelay++;
	if(msTicks == 1000){
		tinhthoigian++;
		msTicks = 0;
	}
	return tinhthoigian;
}
void Delay (uint32_t TICK) { 
	msTicksInDelay = 0;
	while (msTicksInDelay < TICK); // Wait TICK ms 
	msTicksInDelay = 0; // Reset counter 
}
