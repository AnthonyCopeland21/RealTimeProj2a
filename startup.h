#ifndef STARTUP_H
#define STARTUP_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "capture_data.h"
#include "LED.h"
#include "stm32l476xx.h"
#include "SysClock.h"
#include "UART.h"

// define constants PASS and FAIL for returning from functions
#define PASS (1)
#define FAIL (0)

int post(void);
int start(void);
void timer_startup(void);
void input_setup(void);

#endif // STARTUP_H
