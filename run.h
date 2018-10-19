#ifndef RUN_H
#define RUN_H

#include "startup.h"

#define MOV (0x20)						// working
#define RECIPE_END (0x00)			// working
#define WAIT (0x40)
#define LOOP (0x80)
#define END_LOOP (0xA0)
#define Tony_Grad_Command (0x60)
#define Josh_Grad_Command (0xC0)

#define SPACING (300)
#define LEFT_START (500)
#define RIGHT_START (750)

void master_loop(void);

// each of these will change the register
void parse_command(int left_right, int command);
void pause_recipe(int left_right);
void cont_recipe(int left_right);
void move_to(int left_right, int position);
void wait(int left_right, int time);
void move_right_one(int left_right);
void move_left_one(int left_right);
void start_recipe(int left_right);


#endif //  RUN_H
