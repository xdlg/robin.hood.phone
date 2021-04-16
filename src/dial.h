#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    DIAL_STATE_WAITING,
    DIAL_STATE_DIALLING,
    DIAL_STATE_FINISHED
} dial_state_t;

// Initializes the dial.
void dial_init();

// Returns the dial's state (must be called at regular intervals).
dial_state_t dial_state();

// Returns the dialled number as null-terminated string (only valid if state is DIAL_STATE_FINISHED).
char* dial_number();
