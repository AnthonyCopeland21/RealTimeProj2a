#include "run.h"

int recipe_servo1[20];
int recipe_servo2[20];

// Purpose:		Loop that waits for command and runs recipes
// Input:			None
// Output: 		None
void master_loop(void) {
	int i = 0;
	char rxByte = 0;
	char user_input[5];
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
			recipe_status = 0x0000;
			// user_input will be user command. Do user command
			// if invalid, error and let user know
		}
		
		// spin for 100ms
	}
}
// Purpose:		Loop that waits for command and runs recipes
// Input:			None
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

// Purpose:		Loop that waits for command and runs recipes
// Input:			None
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

// Purpose:		Loop that waits for command and runs recipes
// Input:			None
// Output: 		None
int move_right_one(int left_right);

// Purpose:		Loop that waits for command and runs recipes
// Input:			None
// Output: 		None
int move_left_one(int left_right);

// Purpose:		Loop that waits for command and runs recipes
// Input:			None
// Output: 		None