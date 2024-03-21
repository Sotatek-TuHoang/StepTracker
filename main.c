#include "MKL46Z4.h"                    // Device header
#include "lcd.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

#include "mma8451q.h"
#include "switch.h"
#include "timer.h"

float x = 0, y = 0, z = 0;
float x_acceleration = 0;
float previous_x_acceleration = 0;
mma8451q_data_t data;

void DisplayCurrentStep(void);

void initial(){

	SegLCD_Init();
	init_SysTick_interrupt();
	
	mma8451q_init();
	mma8451q_calib();
	
	led_init();
	sw1_init();
	sw3_init();
}

void SystemClockConfiguration(void)
{
		/*System clock = 41.94 MHz*/
    /* OUTDIV4 = 2: Bus clock = system clock / 3*/
    SIM->CLKDIV1 = (uint32_t)0x00020000UL; /* Update system prescalers */
    
    /* Switch to FEI Mode */
    MCG->C1 = (uint8_t)0x06U; 															//  MCG->C1: CLKS=0,FRDIV=0,IREFS=1,IRCLKEN=1,IREFSTEN=0  => 00000110b ~ 0x06 => Select using source clock in FLL bacause using MCGOOUTCLK

    MCG->C2 &= (uint8_t)0x00;																// MCG_C2: LOCRE0=0,FCFTRIM=0,RANGE0=0,HGO0=0,EREFS0=0,LP=0,IRCS=0

    MCG->C4 = (uint8_t)(MCG->C4 & 0x20U);										// MCG->C4: DMX32=0, DRST_DRS=1 => output frequency is 40 - 50 MHz
   
    OSC0->CR = (uint8_t)0x00U;															// OSC0->CR:ERCLKEN=1, EREFSTEN=0, SC2P=0,SC4P=0,SC8P=0,SC16P=0

    MCG->C5 = (uint8_t)0x00U; 															// MCG->C5: x=0,PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=0
    
    MCG->C6 = (uint8_t)0x00U;																// MCG->C6: LOLIE0=0,PLLS=0,CME0=0,VDIV0=0
    
    while((MCG->S & MCG_S_IREFST_MASK) == 0x00U)
    {
        /* Check that the source of the FLL reference clock is the internal reference clock. */
    }
    
    while((MCG->S & 0x0CU) != 0x00U)
    {
        /* Wait until output of the FLL is selected */
    }
}

int main(){
	SystemClockConfiguration();
	initial();
	SegLCD_DisplayDecimal(0);
	while(1){
		DisplayCurrentStep();
		Delay(100);
	}
	return 0;
}

uint16_t step_count = 0;
extern bool active_flag;
void DisplayCurrentStep(void){
		
	if (active_flag)
	{
		PTD->PTOR = (1<<5);
		mma8451_get_data(&data);
		x = data.x_out;
		y = data.y_out;
		z = data.z_out;
		x_acceleration = x * SENSORS_GRAVITY_EARTH;
		Delay(100);
	
    if (previous_x_acceleration > 5 && x_acceleration < 0)
    {
        step_count++;
    }
    previous_x_acceleration = x_acceleration;

		SegLCD_DisplayDecimal(step_count);
	}
}
	

