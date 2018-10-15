#include "startup.h"

// Purpose: 	start of program and intialization. Expect user input
// Input: 		none
// Output: 		PASS/FAIL returned.
//				UART output
void start(void) {
	//intiialization and setup functions
	System_Clock_Init();
	LED_Init();
	UART2_Init();
	timer_startup();
	output_setup();

	// master_loop contains a loop that will run infinitely
	// master control loop that is not blocking
	// run once every 100ms, use timer to spin at end of loop
	master_loop();
}

// Purpose: 	setup of TIM2 for PWM
// Input: 		none
// Output: 		none
void timer_startup(void) {
	// TIM2 should be set to UP mode, 20ms to reset
	// CCR values will drive the high and low of the PWM signal
	// CCR1 for servo 1 and CCR2 for servo 2
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; 	// ensure RCC->APB1ENR1 set to right timer
	
	TIM2->PSC = 0; 						    					// load prescaler into TIM2->PSC register. it will be																		
																					// 80MHz divided by prescaler plus 1
	TIM2->EGR |= TIM_EGR_UG;                // create an update event using the TIM2->EGR register
	TIM2->CCER  &= ~0xFFFF;          				//Disables the input enable by clearing register
	TIM2->CCMR1 &= ~0xFFFF;									//Clear CCMR1 register
	TIM2->CCMR1 &= ~TIM_CCMR1_IC1F; 				//Set filter to 1st setting on page 909
	TIM2->CCMR1 |= TIM_CCMR1_CC1S_0; 				//Set CC1 channel as input
	TIM2->CCMR1 &= ~TIM_CCMR1_IC1PSC;
	TIM2->CCER  |= TIM_CCER_CC1E;    				//Re-enables input enable
	TIM2->CR1   |= TIM_CR1_CEN;      				//Starts Counter
}

// Purpose: 	Setup GPIOA for output
// Input: 		None
// Output: 		None
void output_setup(void) {
	// we need to setup 2 output GPIOs for the 2 signals to the 2 servos
	
	RCC->AHB2ENR   |=   RCC_AHB2ENR_GPIOAEN; // Enable GPIOA
	
	GPIOA->MODER 	 &=   ~3; 				         // clear out bits 0 and 1 for PA0
	GPIOA->MODER   &=   ~(0xFF << (2*0));    // Set desired values to tie PA0
	GPIOA->MODER   |=   0x22 << (2*0);       // as an alternate function tied to TIM2_CH1 and TIM2_CH2	
	GPIOA->AFR[0]  &=   ~(0x0000000F);
	GPIOA->AFR[0]  |=   0x00000011;
	GPIOA->PUPDR   |=   0x22 << (2*0);
}




// there will be 2 interupts here
// 1st: left servo interupt, at a CCR
// 2nd: right servo interupt, at a diff CCR
// each interupt should check to see if the recipe has started or not
// this can be done by checking the status register

