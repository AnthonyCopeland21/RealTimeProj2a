#include "run.h"

//OUR SERVO IS GROUP3 SERVO 2015


unsigned char recipe_servo1[] = {MOV | 3, MOV | 5, MOV | 1, MOV | 0, MOV | 3, RECIPE_END};
unsigned char recipe_servo2[] = {MOV | 5, MOV | 3, MOV | 1, MOV | 2, MOV | 5, MOV | 2, MOV | 0, MOV | 5, MOV | 0, RECIPE_END};
int left_servo_position = 0;
int right_servo_position = 0;
int left_servo_count = 0;
int right_servo_count = 0;
int left_servo_wait = 0;
int right_servo_wait = 0;
	
	
	
// Purpose:		Loop that waits for command and runs recipes
// Input:			None
// Output: 		None
void master_loop(void) {
	int i = 0;

	char rxByte = 0;
	char user_input[10];
	//uint8_t buffer[BufferSize];
	uint8_t response_buffer[BufferSize];
	USART_Write(USART2, (uint8_t *)"Hello, Welcome to Servo Fun!\n\r", 30);
	// infinite loop for 100ms breaks
	while (1) {
		TIM5->CR1 |= TIM_CR1_CEN;
		TIM5->SR &= ~TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF | TIM_SR_UIF;
		while (rxByte != '\r' && (TIM5->SR & TIM_SR_CC1IF) != TIM_SR_CC1IF){
			if ((USART2->ISR & USART_ISR_RXNE)){
				rxByte = ((uint8_t)(USART2->RDR & 0xFF));
				sprintf((char *)response_buffer, "%c", rxByte);
				USART_Write(USART2, response_buffer, 1);
				if (rxByte != 0) {
					user_input[i] = rxByte;
					i++;
				}
				if (rxByte == '\r') {
					USART_Write(USART2, (uint8_t *)"\n", 1);
					i = 0;
				}
			}
		}
		if (rxByte == '\r') {			
			// LEFT SERVO
			if (user_input[0] == 'p' || user_input[0] == 'P') {
				if ((recipe_status & 0xC) == 0xC) {
					pause_recipe(0);
				}
				else {
					// error, recipe either never started
					// or the recipe is already paused
				}
			}
			else if (user_input[0] == 'c' || user_input[0] == 'C') {
				if ((recipe_status & 0xC) == 0x8) {
					cont_recipe(0);
				}
				else {
					// error, recipe either never started
					// or the recipe is already paused
				}
			}
			else if (user_input[0] == 'b' || user_input[0] == 'B') {
				if ((recipe_status & 0xC) == 0x0) {
					start_recipe(0);
				}
			}
			else if (user_input[0] == 'r' || user_input[0] == 'R') {
				if ((recipe_status & 0xC) == 0x8) {
					move_right_one(0);
				}
			}
			else if (user_input[0] == 'l' || user_input[0] == 'L') {
				if ((recipe_status & 0xC) == 0x8) {
					move_left_one(0);
				}
			}
				
			// RIGHT SERVO
			if (user_input[1] == 'p' || user_input[1] == 'P') {
				if ((recipe_status & 0x3) == 0x3) {
					pause_recipe(1);
				}
				else {
					// error, recipe either never started
					// or the recipe is already paused
				}
			}
			else if (user_input[1] == 'c' || user_input[1] == 'C') {
				if ((recipe_status & 0x3) == 0x2) {
					cont_recipe(1);
				}
				else {
					// error, recipe either never started
					// or the recipe is already paused
				}
			}
			else if (user_input[1] == 'b' || user_input[1] == 'B') {
				if ((recipe_status & 0x3) == 0x0) {
					start_recipe(1);
				}
			}
			else if (user_input[1] == 'r' || user_input[1] == 'R') {
				if ((recipe_status & 0x3) == 0x2) {
					move_right_one(1);
				}
			}
			else if (user_input[1] == 'l' || user_input[1] == 'L') {
				if ((recipe_status & 0x3) == 0x2) {
					move_left_one(1);
				}
			}
		}

		
		//next commands in recipe
		//recipe_servo1 and recipe_servo2
		if((recipe_status & 0xC) == 0xC) {
			parse_command(0, recipe_servo1[left_servo_count]);
			left_servo_count++;
		}
		if((recipe_status & 0x3) == 0x3) {
			parse_command(1, recipe_servo2[right_servo_count]);
			right_servo_count++;
		}
		TIM5->SR &= ~TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF | TIM_SR_UIF;
		while((TIM5->SR & TIM_SR_CC1IF) != TIM_SR_CC1IF){
			// wait for flag to be set, that means 100ms has passed
		}
		
		//clear rxByte
		rxByte = 0;
	}
}

// Purpose: 	Parse command opcode
// Input:			left_right, 0 for left servo 1 for right servo
// 						command, opcode for command
// Output:		None
void parse_command(int left_right, int command) {
	if ((command & MOV) == MOV){
		move_to(left_right, (command&0x1F));
	}
	else if (command == 0x00){
		if (left_right == 0) {
			recipe_status &= ~0xC;
			left_servo_count = 0;
		}
		else {
			recipe_status &= ~0x3;
			right_servo_count = 0;
		}
	}
	else if ((command & WAIT) == WAIT){
		wait(left_right, (command&0x1F));
	}
	else if ((command & LOOP) == LOOP){
		move_to(left_right, (command&0x1F));
	}
	else if ((command & END_LOOP) == END_LOOP){
		move_to(left_right, (command&0x1F));
	}
}

// Purpose:		Start recipe
// Input:			left_right, 0 for left servo 1 for right servo
// Output:		None
void start_recipe(int left_right) {
	if (left_right == 0){
		recipe_status |= 0xC;
	}
	else {
		recipe_status |= 0x3;
	}
}

// Purpose:		Pause specified recipe
// Input:			(int) left_right is an integer where 0 is the left servo and 1 is right servo
// Output: 		None
void pause_recipe(int left_right) {
	if (left_right == 0){
		// set left status to paused
		recipe_status &= 0xB;
	}
	else {
		// set right status to paused
		recipe_status &= 0xE;
	}
}

// Purpose:		Continues a specified paused recipe 
// Input:			(int) left_right is an integer where 0 is the left servo and 1 is right servo
// Output: 		None
void cont_recipe(int left_right) {
	if(left_right == 0) {
		// set left status to running
		recipe_status |= 0xC;
	}
	else {
		// set left status to running
		recipe_status |= 0x3;
	}
}

// Purpose: 	Wait given amount of time
// Input:			left_right, 0 for left, 1 for right
//						time, amount of seconds to wait
// Output:		None
void wait(int left_right, int time) {
	if(left_right == 0) {
		if (time >= 0 && time <= 31) {
			// set left servo to wait
		}
		else {
			// error
		}
	}
	else {
		if (time >= 0 && time <= 31) {
			// set right servo to wait
		}
		else {
			// error
		}
	}
}



// Purpose:		Moves left or right servo to specified position
// Input:			left_right, 0 for left, 1 for right
//						position, 0 thru 5 position
// Output:		None
void move_to(int left_right, int position) {
	if(left_right == 0) {
		if (position <= 5 && position >= 0) {
			TIM2->CCR1 = LEFT_START + (position * SPACING);
			left_servo_position = position;
		}
		else {
			//error
		}
	}
	else {
		if (position <= 5 && position >= 0) {
			TIM2->CCR2 = RIGHT_START + position * SPACING;
			right_servo_position = position;
		}
		else {
			//error
		}
	}
}

// Purpose:		Moves specified servo 1 value to the left
// Input:			None
// Output: 		None
void move_left_one(int left_right) {
	if (left_right == 0) {
		if (left_servo_position < 5) {
			left_servo_position++;
			TIM2->CCR1 += SPACING;
		}
		else {
			// ERROR: CANNOT MOVE ANY MORE TO THE RIGHT
		}	
	}
	else {
		if(right_servo_position < 5) {
			right_servo_position++;
			TIM2->CCR2 += SPACING;
			//change duty cycle to move rigth servo 1 to the right
		}
		else {
			// ERROR: CANNOT MOVE ANY MORE TO THE RIGHT
			// use LEDs for errors
		}
	}
}

// Purpose:		Moves specified servo 1 value to the right
// Input:			None
// Output: 		None
void move_right_one(int left_right) {
	if (left_right == 0) {
		if (left_servo_position > 0) {
			left_servo_position--;
			TIM2->CCR1 -= SPACING;
		}
		else {
			// ERROR: CANNOT MOVE ANY MORE TO THE LEFT
			// use LEDs for errors
		}
	}
	else {
		if (right_servo_position > 0) {
			right_servo_position--;
			TIM2->CCR2 -= SPACING;
			//change duty cycle to move rigth servo 1 to the left
		}
		else {
			// ERROR: CANNOT MOVE ANY MORE TO THE LEFT
			// use LEDs for errors
		}
	}	
}




		/* THIS IS ALL TEST CODE THAT WILL SHOW THE SERVOS MOVING PROPERLY
		// spin until 100ms flag is set
		while((TIM5->SR & TIM_SR_CC1IF) != TIM_SR_CC1IF){
			// wait for flag to be set, that means 100ms has passed
		}
		TIM5->SR &= ~TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF | TIM_SR_UIF;
		move_to(0,2);
		move_to(1,4);
		
		while((TIM5->SR & TIM_SR_CC1IF) != TIM_SR_CC1IF){
			// wait for flag to be set, that means 100ms has passed
		}
		
		
		
		TIM5->SR &= ~TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF | TIM_SR_UIF;
		while((TIM5->SR & TIM_SR_CC1IF) != TIM_SR_CC1IF){
		}
		TIM5->SR &= ~TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF | TIM_SR_UIF;
		move_to(0,4);
		move_to(1,2);
		
		while((TIM5->SR & TIM_SR_CC1IF) != TIM_SR_CC1IF){
			// wait for flag to be set, that means 100ms has passed
		}
		TIM5->SR &= ~TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF | TIM_SR_UIF;
		// set duty cycle to 0 for both servos? stop the humming
		*/
