#ifndef RUN_H
#define RUN_H

#include "startup.h"

void master_loop(void);

// each of these will change the register
int pause_recipe(int left_right);
int cont_recipe(int left_right);
int move_right_one(int left_right);
int move_left_one(int left_right);

#endif //  RUN_H