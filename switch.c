#include "MKL46Z4.h" 
#include "switch.h"
#include "stdbool.h"
#include "lcd.h"

#define PORTC_D_IRQ_NBR (IRQn_Type) 31

bool active_flag = 0;

void PORTC_PORTD_IRQHandler(void);

void led_init(void)
{
    // Enable clock to PORTD + PORTE
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

    // Set the Mux control of PTD5 to 001, or GPIO
    PORTD->PCR[5] = PORT_PCR_MUX(1);
    // Set PTD5 as an output
    PTD->PDDR |= (1 << 5);
		PTD->PSOR = (1<<5); //Turn off green led

    // Set the Mux control
    PORTE->PCR[29] = PORT_PCR_MUX(1);
    // Set output
    PTE->PDDR |= (1 << 29);
		PTD->PCOR = (1<<29); //Turn on red led
}

extern uint16_t step_count;
void sw3_cb(void)
{
	if((PTC -> PDIR & (1<<12)) == 0)
	{
		active_flag = 0;
		step_count = 0;
		SegLCD_DisplayDecimal(0);
	}
}

void sw1_cb(void)
{
		if((PTC -> PDIR & (1<<3)) == 0)
		{
			active_flag = !active_flag;
			PTE->PTOR = (1 << 29);
		}
		PTD->PSOR = (1<<5); //Turn off green led
}

void sw1_init(void)
{
	SIM->SCGC5 |= (1<<11);
	PORTC->PCR[3] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //Pin C3 GPIO - Pullup
	PTC->PDDR &= ~((uint32_t)(1u<<3)); //input
	PORTC->PCR[3] &= ~0xF0000;
	PORTC->PCR[3] |= 0xA0000; //falling edge
	NVIC_ClearPendingIRQ(PORTC_D_IRQ_NBR); //clear irq
	NVIC_EnableIRQ(PORTC_D_IRQ_NBR); //enable irq
}

void PORTC_PORTD_IRQHandler(void)
{
	uint32_t i = 0;
	for (i=0; i<1000; i++);
	sw1_cb();
	sw3_cb();
	PORTC->PCR[3] |= PORT_PCR_ISF_MASK; //clear int flag
	PORTC->PCR[12] |= PORT_PCR_ISF_MASK; //clear int flag
}
void sw3_init(void)
{
	SIM->SCGC5 |= (1<<11);
	PORTC->PCR[12] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //Pin C3 GPIO - Pullup
	PTC->PDDR &= ~((uint32_t)(1u<<12)); //input
	PORTC->PCR[12] &= ~0xF0000;
	PORTC->PCR[12] |= 0xA0000; //falling edge
	NVIC_ClearPendingIRQ(PORTC_D_IRQ_NBR); //clear irq
	NVIC_EnableIRQ(PORTC_D_IRQ_NBR); //enable irq
}

