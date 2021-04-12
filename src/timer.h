#pragma once

#include <stdbool.h>
#include <stdint.h>

// Start counting up to the given number of milliseconds
void timer_start(uint32_t timeout_ms);

// Returns whether the timer has timed out
bool timer_has_timed_out();
