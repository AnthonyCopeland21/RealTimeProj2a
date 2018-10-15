#ifndef RUN_H
#define RUN_H

#include "startup.h"

#define MOV (0x20)
#define RECIPE_END (0x00)
#define WAIT (0x40)
#define LOOP (0x80)
#define END_LOOP (0xA0)
#define Tony_Grad_Command (0x60)
#define Josh_Grad_Command (0xC0)

void master_loop(void);

// each of these will change the register
int pause_recipe(int left_right);
int cont_recipe(int left_right);
int move_right_one(int left_right);
int move_left_one(int left_right);

#endif //  RUN_H