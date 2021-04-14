#pragma once

#include <stdbool.h>
#include <stdint.h>

#define INVALID_TIME_SINCE_ACTIVITY 0xFFFF
#define INVALID_DIGIT 0xFF

// Initializes the dial
void dial_init();

// Monitors the dial's activity (must be called at regular intervals)
void dial_monitor_activity();

// Returns the elapsed time in milliseconds since a dialling activity has been detected.
// If no activity has ever been detected, the returned value is meaningless.
uint32_t dial_time_since_activity_ms();

// Returns true if dialling in progress
bool dial_is_dialling();

uint8_t dial_last_digit();
