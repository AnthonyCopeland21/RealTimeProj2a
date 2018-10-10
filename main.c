#include "startup.h"

// Purpose: 	Kickoff the program
// Input: 		none
// Output: 		PASS/FAIL returned
int main(void){
	start();
	return PASS;
}


/**************************************
Pseudocode

Where is the recipe stored? .txt file? Opcode hardcoded into array?

Program starts up. Waits for command from user
User enters command based on USER COMMANDS
Parse user command, determine action.
do action
if error occurs, light error LED

Control pair of servo motors using custom interpreted control language
Custom commands will be needed
6 servo positions, 0->5
PWM control signals of 20ms periods, duty cycle affects position
Servos require 5V source
DEMO VALID PWM SIGNAL BEFORE CONNECTING TO SERVO.
Wait at bottom of control loop for total of 100ms

Non-blocking infinite loop that executes one loop every 100ms
loop checks for keyboard input
command input
process state changes for servo 1 and 2 when necessary

Command(Recipe) Format
3 bits opcode 5 bits param
0xXXXYYYYY
	XXX		- opcode
	YYYYY - param
001		mov, param 0..5
010		wait, param 0..31
100		loop, param 0..31
101		end loop
000		end

LED Display: Create error codes that are sent to LED_Light function
Recipe Runs - green led
Recipe Paused - all off
Recipe Error - red on
Recipe Nested Loop Error - both on

User Commands
Pause - P or p
Continue - C or c
Right 1 - R or r
Left 1 - L or l
No operation - N or n
Begin Recipe - B or b

Example User Commands:
bb
pP
cc


**************************************/