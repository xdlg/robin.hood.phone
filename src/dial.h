#pragma once

#include <stdbool.h>
#include <stdint.h>

#define INVALID_DIGIT 0xFF

// Initializes the dial
void dial_init();

// Monitors the dial's activity (must be called at regular intervals)
void dial_monitor_activity();

// Returns true if dialling in progress
bool dial_is_dialling();

uint8_t dial_last_digit();
