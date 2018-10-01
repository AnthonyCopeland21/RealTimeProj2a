#include "startup.h"

// Purpose: 	Checks to ensure input is recieved
// Input: 		none
// Output: 		PASS/FAIL returned
int post(void) {
	// Waiting for input to be sensed or time to run out
	while (TIM2->CNT < 8000000) {
		if ((TIM2->SR & TIM_SR_CC1IF) == TIM_SR_CC1IF) {
			return PASS;
		}
	}
	TIM2->CR1   &= ~TIM_CR1_CEN;      //Stop Counter
	TIM2->CNT		 = 0;			  //Reset Count
	return FAIL;
}

// Purpose: 	start of program and intialization
// Input: 		none
// Output: 		PASS/FAIL returned.
//				UART output
int start(void) {
	//intiialization and setup functions
	System_Clock_Init();
	LED_Init();
	UART2_Init();
	timer_startup();
	input_setup();

	int user_value = 0; // = '\0';
	int i = 0;
	char rxByte = 0;
	char str[10];
	uint8_t buffer[BufferSize];
	uint8_t new_buffer[BufferSize];
	if (PASS != post()) {
		USART_Write(USART2, (uint8_t *)"Post failed. Try again? (y/n)\n\r",31);
		rxByte = USART_Read(USART2);
		sprintf((char *)buffer, "%c\n\r", rxByte);
		USART_Write(USART2, buffer, 4);
		if (rxByte == 'y' || rxByte == 'Y') {
			timer_startup();
			if (PASS != post()) {
				USART_Write(USART2, (uint8_t *)"Failed again. Goodbye!\n\r", 24);
				return FAIL;
			}
		}
		else {
			USART_Write(USART2, (uint8_t *)"Failed. Goodbye!\n\r", 18);
			return FAIL;
		}
	}
	USART_Write(USART2, (uint8_t *)"Default lower limit set to 950. Default upper limit is 1050. Change lower limit? (y/n)\n\r",88);
	rxByte = USART_Read(USART2);
	sprintf((char *)buffer, "%c\n\r", rxByte);
	USART_Write(USART2, buffer, 4);
	if (rxByte == 'y' || rxByte == 'Y') {
		USART_Write(USART2, (uint8_t *)"New lower limit: ",17);
		// Waits for user to press enter
		while(rxByte != '\r') {
			rxByte = USART_Read(USART2);
			sprintf((char *)new_buffer, "%c", rxByte);
			USART_Write(USART2, new_buffer, BufferSize);
			str[i] = rxByte;
			i++;
		}
		user_value = atoi(str);
		sprintf((char *)buffer, "\n\rNew upper limit: %d\n\r", user_value + 100);
		USART_Write(USART2, buffer, 23);
		// Clears buffer
		for(i = 0; i < strlen((char *)buffer); i++) {
			buffer[i] = '\0';
		}
		USART_Write(USART2, (uint8_t *)"\n\r", 2);
		capture_data(user_value);
	}
	else {
		capture_data(950);
	}
	return PASS;
}

// Purpose: 	setup of TIM2 for capture
// Input: 		none
// Output: 		none
void timer_startup(void) {
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; 	// ensure RCC->APB1ENR1 set to right timer
	
	TIM2->PSC = 0; 						    // load prescaler into TIM2->PSC register. it will be																		// 80MHz divided by prescaler plus 1
	TIM2->EGR |= TIM_EGR_UG;                // create an update event using the TIM2->EGR register
	TIM2->CCER  &= ~0xFFFF;          		//Disables the input enable by clearing register
	TIM2->CCMR1 &= ~0xFFFF;					//Clear CCMR1 register
	TIM2->CCMR1 &= ~TIM_CCMR1_IC1F; 		//Set filter to 1st setting on page 909
	TIM2->CCMR1 |= TIM_CCMR1_CC1S_0; 		//Set CC1 channel as input
	TIM2->CCMR1 &= ~TIM_CCMR1_IC1PSC;
	TIM2->CCER  |= TIM_CCER_CC1E;    		//Re-enables input enable
	TIM2->CR1   |= TIM_CR1_CEN;      		//Starts Counter
}

// Purpose: 	setup GPIOA for input
// Input: 		nnone
// Output: 		none
void input_setup(void) {
	RCC->AHB2ENR   |=   RCC_AHB2ENR_GPIOAEN; //Enable GPIOA
	
	GPIOA->MODER 	 &=   ~3 ;				 // clear out bits 0 and 1 for PA0
	GPIOA->MODER   &=   ~(0xF << (2*0));     //Set desired values to tie PA0
	GPIOA->MODER   |=   0x2 << (2*0);        //as an input tied to TIM2_CH1
	GPIOA->AFR[0] &= ~(0x0000000F);
	GPIOA->AFR[0] |= 0x00000001;
	GPIOA->PUPDR   |=   0x2 << (2*0);
}
