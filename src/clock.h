#pragma once

#include <stdint.h>

// Initializes the clock
void clock_init();

// Returns the current time in milliseconds since clock initialization
uint32_t clock_now_ms();
