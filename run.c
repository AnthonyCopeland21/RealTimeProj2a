#include "run.h"

unsigned char recipe_servo1[] = {MOV | 3, MOV | 5, RECIPE_END};
unsigned char recipe_servo2[] = {WAIT | 10, MOV | 2, RECIPE_END};
int left_servo_position = 0;
int right_servo_position = 0;

// Purpose:		Loop that waits for command and runs recipes
// Input:			None
// Output: 		None
void master_loop(void) {
	int i = 0;
	char rxByte = 0;
	char user_input[10];
	uint8_t buffer[BufferSize];
	uint8_t response_buffer[BufferSize];
	//do stuff
	while (1) {
		rxByte = USART_Read(USART2);
		sprintf((char *)response_buffer, "%c", rxByte);
		USART_Write(USART2, response_buffer, BufferSize);
		if (rxByte != '\0')	{
			user_input[i] = rxByte;
			i++;
		}
		if (rxByte == '\r') {
			// user_input[0] is going to be the first command
			// user_input[1] is going to be the second command
			
			// left servo
			if (user_input[0] == 'p' || user_input[0] == 'P') {
				if ((recipe_status & 0x3) == 0x3) {
					pause_recipe(0);
				}
				else {
					// error, recipe either never started
					// or the recipe is already paused
				}
			}
			else if (user_input[0] == 'c' || user_input[0] == 'C') {
				if ((recipe_status & 0x3) == 0x2) {
					cont_recipe(0);
				}
				else {
					// error, recipe either never started
					// or the recipe is already paused
				}
			}
			
			
			// right servo
			if (user_input[1] == 'p' || user_input[1] == 'P') {
				if ((recipe_status & 0xC) == 0xC) {
					pause_recipe(1);
				}
				else {
					// error, recipe either never started
					// or the recipe is already paused
				}
			}
			else if (user_input[1] == 'c' || user_input[1] == 'C') {
				if ((recipe_status & 0xC) == 0x8) {
					cont_recipe(1);
				}
				else {
					// error, recipe either never started
					// or the recipe is already paused
				}
			}
		}
		
		//next commands in recipe
		
		// spin for 100ms
		while(/*SysTick flag not set*/){
			//wait for flag
		}
	}
}

// Purpose:		Pause specified recipe
// Input:			(int) left_right is an integer where 0 is the left servo and 1 is right servo
// Output: 		None
int pause_recipe(int left_right) {
	if (left_right == 0){
		// set left status to paused
		recipe_status &= ~0x4;
	}
	else {
		// set right status to paused
		recipe_status &= ~0x1;
	}
}

// Purpose:		Continues a specified paused recipe 
// Input:			(int) left_right is an integer where 0 is the left servo and 1 is right servo
// Output: 		None
int cont_recipe(int left_right) {
	if(left_right == 0) {
		// set left status to running
		recipe_status |= 0x4;
	}
	else {
		// set left status to running
		recipe_status |= 0x1;
	}
}

// Purpose:		Moves specified servo 1 value to the right
// Input:			None
// Output: 		None
int move_right_one(int left_right) {
	if (left_right == 0) {
		if (left_servo_position < 5) {
			left_servo_position++;
			TIM2->CCR1 += 320;
			//change duty cycle to move left servo 1 to the right
		}
		else {
			// ERROR: CANNOT MOVE ANY MORE TO THE RIGHT
			// use LEDs for errors
		}	
	}
	else {
		if(right_servo_position < 5) {
			right_servo_position++;
			TIM2->CCR2 += 320;
			//change duty cycle to move rigth servo 1 to the right
		}
		else {
			// ERROR: CANNOT MOVE ANY MORE TO THE RIGHT
			// use LEDs for errors
		}
	}
}

// Purpose:		Moves specified servo 1 value to the left
// Input:			None
// Output: 		None
int move_left_one(int left_right) {
	if (left_right == 0) {
		if (left_servo_position > 0) {
			left_servo_position--;
			TIM2->CCR1 -= 320;
			//change duty cycle to move left servo 1 to the left
		}
		else {
			// ERROR: CANNOT MOVE ANY MORE TO THE LEFT
			// use LEDs for errors
		}
	}
	else {
		if(right_servo_position > 0) {
			right_servo_position--;
			TIM2->CCR2 -= 320;
			//change duty cycle to move rigth servo 1 to the left
		}
		else {
			// ERROR: CANNOT MOVE ANY MORE TO THE LEFT
			// use LEDs for errors
		}
	}	
}