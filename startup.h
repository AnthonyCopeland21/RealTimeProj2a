#ifndef STARTUP_H
#define STARTUP_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "LED.h"
#include "run.h"
#include "stm32l476xx.h"
#include "SysClock.h"
#include "UART.h"

// define constants PASS and FAIL for returning from functions
#define PASS (1)
#define FAIL (0)

// recipe status is global for run to use
// LSB: Right Recipe
// XX00 - right recipe has not been started
// XX00 (0x0) - never started
// XX01 (0x1) - should NEVER occur, recipe is not able to run if it's not started
// XX10 (0x2) - started and paused
// XX11 (0x3) - started and running

// MSB: Left Recipe
// 00XX - left recipe has not been started
// 00XX (0x0) - never started
// 01XX (0x4) - should NEVER occur, recipe is not able to run if it's not started
// 10XX (0x8) - started and paused
// 11XX (0xC) - started and running

// This status register is key.
// You can only continue a recipe after it's been paused
// and a recipe can only be started if it's stopped
static int recipe_status = 0x0;

void start(void);
void timer_startup(void);
void output_setup(void);

#endif // STARTUP_H
